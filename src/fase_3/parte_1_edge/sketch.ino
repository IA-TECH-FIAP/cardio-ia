/* ============================================================================
 * CardioIA - Fase 3 / Parte 1
 * Monitoramento Continuo Cardiaco - Edge Computing com ESP32
 * ----------------------------------------------------------------------------
 * Objetivo: simular um wearable cardiaco que:
 *   1) Le periodicamente sensores (DHT22 + potenciometro simulando BPM)
 *   2) Armazena as leituras LOCALMENTE em um buffer circular em RAM
 *      (resiliencia offline - Edge Computing)
 *   3) Quando a "conexao Wi-Fi" simulada esta ativa, envia o buffer
 *      acumulado para a "nuvem" (Serial.println) e LIMPA o buffer
 *   4) Quando offline, continua coletando sem perder dados, ate o limite
 *      do buffer (descartando os MAIS ANTIGOS - politica FIFO)
 *
 * Sensores utilizados:
 *   Sensor 1 (obrigatorio): DHT22 - mede temperatura E umidade (1 sensor)
 *   Sensor 2 (escolha do grupo): Potenciometro simulando sensor cardiaco
 *                                 que reporta os BPM (batimentos por minuto)
 *
 * Observacao sobre SPIFFS:
 *   O enunciado autoriza usar o Monitor Serial como alternativa de
 *   resiliencia, pois o SPIFFS e volatil no Wokwi/simuladores. Por isso,
 *   adotamos um BUFFER CIRCULAR em RAM (array de struct) como
 *   armazenamento local de Edge, o que e tambem mais rapido e adequado
 *   a um wearable de baixa latencia. O envio para a "nuvem" e simulado
 *   pelo Serial.println.
 *
 * Hardware simulado no Wokwi:
 *   - ESP32 DevKit V1
 *   - DHT22 conectado no GPIO 15
 *   - Potenciometro (simulando sensor de batimento cardiaco - BPM)
 *     conectado no GPIO 34 (ADC1_CH6)
 *   - LED indicador de status no GPIO 2
 *     (aceso = online / pisca = enviando / apagado = offline)
 *   - Push-button no GPIO 4 para alternar Wi-Fi simulado
 *     (pressionar = alterna online/offline)
 *
 * Projeto Wokwi publicado: https://wokwi.com/projects/463854286798211073
 * ============================================================================ */

#include <Arduino.h>
#include <DHT.h>

// ---------- Definicoes de pinos ----------
#define PIN_DHT          15
#define PIN_POT          34   // ADC para simular BPM
#define PIN_LED_STATUS    2
#define PIN_BTN_WIFI      4

// ---------- Configuracoes do sensor DHT ----------
#define DHT_TYPE       DHT22
DHT dht(PIN_DHT, DHT_TYPE);

// ---------- Configuracoes de coleta ----------
const uint32_t INTERVALO_LEITURA_MS = 2000;   // le sensores a cada 2 s
const uint32_t INTERVALO_SYNC_MS    = 5000;   // tenta sincronizar a cada 5 s

// ---------- Limites clinicos (alertas locais no Edge) ----------
const float TEMP_MAX_C   = 38.0;    // febre
const float TEMP_MIN_C   = 35.0;    // hipotermia
const int   BPM_MAX      = 120;     // taquicardia
const int   BPM_MIN      = 50;      // bradicardia

// ---------- Buffer circular em RAM (resiliencia offline) ----------
// Capacidade definida pelo modelo de negocio:
//   Com leitura a cada 2 s -> 100 amostras = 200 s (~3,3 min) de autonomia
//   offline antes de comecar a descartar os dados mais antigos.
//
// Justificativa: para um wearable cardiaco, ~3 min de "buraco" de conexao
// e suficiente para o paciente trocar de comodo, entrar em elevador,
// passar por areas com baixo sinal etc. Se passar disso, e melhor
// preservar os dados RECENTES (mais relevantes clinicamente) do que os
// antigos -> politica FIFO com descarte do mais velho.
const uint16_t BUFFER_CAP = 100;

struct Amostra {
  uint32_t timestamp_ms;   // millis() no momento da leitura
  float    temperatura_c;
  float    umidade_pct;
  int      bpm;
  bool     alerta;         // true se algum limite clinico foi violado
};

Amostra  buffer[BUFFER_CAP];
uint16_t buf_head   = 0;   // proxima posicao de escrita
uint16_t buf_tail   = 0;   // proxima posicao de leitura/envio
uint16_t buf_count  = 0;   // quantas amostras validas ha no buffer
uint32_t total_descartadas = 0;

// ---------- Estado de conectividade SIMULADA ----------
bool wifi_conectado = false;     // comeca OFFLINE para evidenciar o buffer
bool btn_estado_ant = HIGH;      // pull-up: HIGH = solto

// ---------- Controle de tempo ----------
uint32_t t_ultima_leitura = 0;
uint32_t t_ultimo_sync    = 0;

// ============================================================================
// FUNCOES AUXILIARES DO BUFFER CIRCULAR
// ============================================================================

/* Insere uma nova amostra. Se o buffer estiver cheio, sobrescreve a mais
 * antiga (FIFO com descarte do mais velho). Esse comportamento e coerente
 * com um wearable medico: priorizamos dados RECENTES, que sao
 * clinicamente mais relevantes para alerta em tempo real. */
void buffer_inserir(const Amostra& a) {
  buffer[buf_head] = a;
  buf_head = (buf_head + 1) % BUFFER_CAP;

  if (buf_count < BUFFER_CAP) {
    buf_count++;
  } else {
    // Buffer cheio: o tail tambem avanca (perdemos a amostra mais antiga)
    buf_tail = (buf_tail + 1) % BUFFER_CAP;
    total_descartadas++;
  }
}

/* Remove e retorna a amostra mais antiga (FIFO). Retorna false se vazio. */
bool buffer_remover(Amostra& out) {
  if (buf_count == 0) return false;
  out = buffer[buf_tail];
  buf_tail = (buf_tail + 1) % BUFFER_CAP;
  buf_count--;
  return true;
}

// ============================================================================
// LEITURA DOS SENSORES
// ============================================================================

/* Le DHT22 + potenciometro e devolve uma Amostra preenchida.
 * O potenciometro (0-4095 no ADC do ESP32) e mapeado para a faixa
 * fisiologica plausivel de 40-180 BPM para simular o sensor cardiaco. */
Amostra ler_sensores() {
  Amostra a;
  a.timestamp_ms  = millis();
  a.temperatura_c = dht.readTemperature();
  a.umidade_pct   = dht.readHumidity();

  int adc = analogRead(PIN_POT);              // 0 .. 4095
  a.bpm = map(adc, 0, 4095, 40, 180);          // 40 .. 180 bpm

  // Verifica alertas (Edge Computing: decisao local, sem precisar da nuvem)
  bool temp_invalida = isnan(a.temperatura_c);
  a.alerta = (!temp_invalida && (a.temperatura_c > TEMP_MAX_C ||
                                  a.temperatura_c < TEMP_MIN_C)) ||
             (a.bpm > BPM_MAX) || (a.bpm < BPM_MIN);
  return a;
}

// ============================================================================
// "ENVIO PARA NUVEM" - simulado por Serial.println
// ============================================================================

/* Quando online, drena o buffer enviando uma amostra por vez em formato
 * CSV-like, simulando um payload que seria publicado via MQTT/HTTP na
 * Parte 2 do projeto. */
void sincronizar_com_nuvem() {
  if (buf_count == 0) {
    Serial.println(F("[SYNC] Sem dados pendentes."));
    return;
  }

  Serial.print(F("[SYNC] Iniciando envio de "));
  Serial.print(buf_count);
  Serial.println(F(" amostras para a nuvem..."));

  uint16_t enviadas = 0;
  Amostra a;
  while (buffer_remover(a)) {
    // Pisca o LED a cada amostra enviada (feedback visual)
    digitalWrite(PIN_LED_STATUS, LOW);
    delay(20);
    digitalWrite(PIN_LED_STATUS, HIGH);

    // Payload simulado (CSV) - na Parte 2 virara JSON via MQTT
    Serial.print(F("[CLOUD<-EDGE] ts="));
    Serial.print(a.timestamp_ms);
    Serial.print(F(",temp="));
    Serial.print(a.temperatura_c, 2);
    Serial.print(F(",umid="));
    Serial.print(a.umidade_pct, 2);
    Serial.print(F(",bpm="));
    Serial.print(a.bpm);
    Serial.print(F(",alerta="));
    Serial.println(a.alerta ? F("1") : F("0"));
    enviadas++;
  }

  Serial.print(F("[SYNC] OK - "));
  Serial.print(enviadas);
  Serial.println(F(" amostras enviadas e removidas do buffer local."));

  if (total_descartadas > 0) {
    Serial.print(F("[AVISO] Durante o offline, "));
    Serial.print(total_descartadas);
    Serial.println(F(" amostras antigas foram descartadas (buffer cheio)."));
  }
}

// ============================================================================
// CONTROLE DE CONECTIVIDADE (Wi-Fi simulado por botao)
// ============================================================================

void atualizar_botao_wifi() {
  bool estado = digitalRead(PIN_BTN_WIFI);
  // Detecta borda de descida (pressionou)
  if (btn_estado_ant == HIGH && estado == LOW) {
    wifi_conectado = !wifi_conectado;
    Serial.print(F("\n>>> Wi-Fi alternado para: "));
    Serial.println(wifi_conectado ? F("ONLINE") : F("OFFLINE"));
    delay(150); // anti-bounce simples
  }
  btn_estado_ant = estado;

  // LED reflete o estado: aceso = online, apagado = offline
  digitalWrite(PIN_LED_STATUS, wifi_conectado ? HIGH : LOW);
}

// ============================================================================
// SETUP & LOOP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("\n=== CardioIA - Edge Node iniciando ==="));

  pinMode(PIN_LED_STATUS, OUTPUT);
  pinMode(PIN_BTN_WIFI, INPUT_PULLUP);
  digitalWrite(PIN_LED_STATUS, LOW);

  dht.begin();

  Serial.print(F("Buffer circular em RAM: capacidade = "));
  Serial.print(BUFFER_CAP);
  Serial.println(F(" amostras"));
  Serial.println(F("Pressione o botao (GPIO4) para alternar Wi-Fi simulado."));
  Serial.println(F("Estado inicial: OFFLINE (coletando dados localmente).\n"));
}

void loop() {
  atualizar_botao_wifi();

  uint32_t agora = millis();

  // ----- 1) Coleta periodica -----
  if (agora - t_ultima_leitura >= INTERVALO_LEITURA_MS) {
    t_ultima_leitura = agora;

    Amostra a = ler_sensores();

    if (isnan(a.temperatura_c) || isnan(a.umidade_pct)) {
      Serial.println(F("[ERRO] Falha ao ler DHT22. Amostra descartada."));
    } else {
      buffer_inserir(a);

      // Log local (Edge) para inspecao em tempo real
      Serial.print(F("[EDGE] t="));
      Serial.print(a.temperatura_c, 1);
      Serial.print(F("C  u="));
      Serial.print(a.umidade_pct, 1);
      Serial.print(F("%  bpm="));
      Serial.print(a.bpm);
      if (a.alerta) Serial.print(F("  *ALERTA*"));
      Serial.print(F("  | buffer="));
      Serial.print(buf_count);
      Serial.print(F("/"));
      Serial.print(BUFFER_CAP);
      Serial.print(F("  wifi="));
      Serial.println(wifi_conectado ? F("ON") : F("OFF"));
    }
  }

  // ----- 2) Sincronizacao com a nuvem -----
  // So tenta sincronizar se estiver "online" e respeitando o intervalo.
  if (wifi_conectado && (agora - t_ultimo_sync >= INTERVALO_SYNC_MS)) {
    t_ultimo_sync = agora;
    sincronizar_com_nuvem();
  }

  // Pequeno yield para a CPU
  delay(50);
}
