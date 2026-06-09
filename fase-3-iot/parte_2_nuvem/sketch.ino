/* ============================================================================
 * CardioIA - Fase 3 / Parte 2
 * Transmissao para a nuvem via MQTT + Dashboard (Fog/Cloud Computing)
 * ----------------------------------------------------------------------------
 * Este sketch e a evolucao do firmware da Parte 1 (Edge). Em vez de apenas
 * imprimir as leituras no Serial, agora publicamos cada amostra em um broker
 * MQTT publico (HiveMQ) usando tres topicos hierarquicos:
 *
 *   cardioia/<deviceId>/sinais   -> payload JSON com {ts, bpm, temp, umid}
 *   cardioia/<deviceId>/alerta   -> publicado quando algum limite e violado
 *   cardioia/<deviceId>/status   -> "online" / "offline" (LWT)
 *
 * Mantemos a mesma logica de resiliencia da Parte 1: enquanto o ESP32
 * estiver desconectado (do Wi-Fi OU do broker MQTT), as amostras seguem
 * sendo gravadas no buffer circular em RAM. Quando a conexao retorna, o
 * buffer e drenado, publicando todos os dados acumulados.
 *
 * Broker: broker.hivemq.com:1883 (publico, sem TLS / sem autenticacao).
 * Para producao real (LGPD, dados sensiveis de saude) ver Secao "Seguranca"
 * em docs/fase_3/relatorio_parte_2_nuvem.md.
 *
 * Hardware: identico ao da Parte 1.
 *   - ESP32 DevKit V1
 *   - DHT22  -> GPIO 15
 *   - Pot    -> GPIO 34
 *   - LED    -> GPIO 2
 *   - Botao  -> GPIO 4 (agora forca reconexao MQTT)
 * ============================================================================ */

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// ---------- Wi-Fi (Wokwi expoe rede aberta) ----------
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// ---------- MQTT ----------
const char* MQTT_HOST       = "broker.hivemq.com";
const uint16_t MQTT_PORT    = 1883;
const char* DEVICE_ID       = "cardioia-001";
const char* TOPIC_SINAIS    = "cardioia/cardioia-001/sinais";
const char* TOPIC_ALERTA    = "cardioia/cardioia-001/alerta";
const char* TOPIC_STATUS    = "cardioia/cardioia-001/status";

// ---------- Pinos ----------
#define PIN_DHT          15
#define PIN_POT          34
#define PIN_LED_STATUS    2
#define PIN_BTN_WIFI      4

#define DHT_TYPE       DHT22
DHT dht(PIN_DHT, DHT_TYPE);

// ---------- Configuracoes de coleta ----------
const uint32_t INTERVALO_LEITURA_MS = 2000;
const uint32_t INTERVALO_PUB_MS     = 2000;
const uint32_t INTERVALO_RECONEXAO_MS = 5000;

// ---------- Limites clinicos ----------
const float TEMP_MAX_C   = 38.0;
const float TEMP_MIN_C   = 35.0;
const int   BPM_MAX      = 120;
const int   BPM_MIN      = 50;

// ---------- Buffer circular em RAM (resiliencia offline) ----------
const uint16_t BUFFER_CAP = 100;
struct Amostra {
  uint32_t timestamp_ms;
  float    temperatura_c;
  float    umidade_pct;
  int      bpm;
  bool     alerta;
};
Amostra  buffer[BUFFER_CAP];
uint16_t buf_head = 0, buf_tail = 0, buf_count = 0;
uint32_t total_descartadas = 0;

void buffer_inserir(const Amostra& a) {
  buffer[buf_head] = a;
  buf_head = (buf_head + 1) % BUFFER_CAP;
  if (buf_count < BUFFER_CAP) {
    buf_count++;
  } else {
    buf_tail = (buf_tail + 1) % BUFFER_CAP;
    total_descartadas++;
  }
}
bool buffer_remover(Amostra& out) {
  if (buf_count == 0) return false;
  out = buffer[buf_tail];
  buf_tail = (buf_tail + 1) % BUFFER_CAP;
  buf_count--;
  return true;
}

// ---------- Clientes Wi-Fi + MQTT ----------
WiFiClient    wifi_client;
PubSubClient  mqtt(wifi_client);

uint32_t t_ultima_leitura = 0;
uint32_t t_ultima_reconexao = 0;
bool btn_estado_ant = HIGH;

// ============================================================================
// CONEXAO Wi-Fi
// ============================================================================

void conectar_wifi() {
  Serial.print(F("[WIFI] Conectando a "));
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  uint32_t inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 15000) {
    delay(250);
    Serial.print('.');
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print(F("\n[WIFI] OK - IP: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(F("\n[WIFI] Falhou - seguira tentando em background."));
  }
}

// ============================================================================
// CONEXAO MQTT (handshake: CONNECT -> CONNACK)
// ============================================================================

void conectar_mqtt() {
  if (mqtt.connected()) return;
  if (WiFi.status() != WL_CONNECTED) return;

  Serial.print(F("[MQTT] Conectando ao broker "));
  Serial.print(MQTT_HOST);
  Serial.print(F(":"));
  Serial.println(MQTT_PORT);

  // LWT: o broker publica "offline" automaticamente se perdermos a conexao
  // sem desconectar de forma limpa - util para detectar falhas no dashboard.
  bool ok = mqtt.connect(DEVICE_ID,
                          /*user*/ nullptr, /*pass*/ nullptr,
                          /*willTopic*/ TOPIC_STATUS,
                          /*willQos*/ 1,
                          /*willRetain*/ true,
                          /*willMessage*/ "offline");
  if (ok) {
    Serial.println(F("[MQTT] CONNACK recebido. Status -> online."));
    mqtt.publish(TOPIC_STATUS, "online", true);  // retained
  } else {
    Serial.print(F("[MQTT] Falha. Codigo de retorno: "));
    Serial.println(mqtt.state());
  }
}

// ============================================================================
// PUBLICACAO MQTT (payload JSON)
// ============================================================================

/* Serializa uma amostra como JSON e publica no topico de sinais.
 * QoS 1 (at-least-once) garante que o broker confirme o recebimento. */
bool publicar_amostra(const Amostra& a) {
  StaticJsonDocument<256> doc;
  doc["ts"]    = a.timestamp_ms;
  doc["temp"]  = a.temperatura_c;
  doc["umid"]  = a.umidade_pct;
  doc["bpm"]   = a.bpm;
  doc["alert"] = a.alerta;

  char payload[256];
  size_t n = serializeJson(doc, payload, sizeof(payload));
  bool ok = mqtt.publish(TOPIC_SINAIS, (uint8_t*)payload, n, /*retained*/ false);

  if (ok && a.alerta) {
    // Topico dedicado de alertas - facilita assinaturas no Node-RED
    mqtt.publish(TOPIC_ALERTA, payload, false);
  }
  return ok;
}

/* Drena o buffer publicando uma amostra de cada vez. Se uma publicacao
 * falhar (ex.: queda do broker no meio do flush), aborta e mantem as
 * amostras restantes no buffer para a proxima tentativa. */
void sincronizar_buffer() {
  if (buf_count == 0) return;
  Serial.print(F("[SYNC] Publicando "));
  Serial.print(buf_count);
  Serial.println(F(" amostras pendentes..."));

  Amostra a;
  uint16_t enviadas = 0;
  while (buf_count > 0) {
    a = buffer[buf_tail];
    if (!publicar_amostra(a)) {
      Serial.println(F("[SYNC] Publicacao falhou - parando flush."));
      break;
    }
    buf_tail = (buf_tail + 1) % BUFFER_CAP;
    buf_count--;
    enviadas++;
    digitalWrite(PIN_LED_STATUS, LOW); delay(10);
    digitalWrite(PIN_LED_STATUS, HIGH);
  }
  Serial.print(F("[SYNC] OK - "));
  Serial.print(enviadas);
  Serial.println(F(" amostras enviadas via MQTT."));
}

// ============================================================================
// LEITURA DOS SENSORES (identica a Parte 1)
// ============================================================================

Amostra ler_sensores() {
  Amostra a;
  a.timestamp_ms  = millis();
  a.temperatura_c = dht.readTemperature();
  a.umidade_pct   = dht.readHumidity();
  int adc = analogRead(PIN_POT);
  a.bpm = map(adc, 0, 4095, 40, 180);
  bool temp_invalida = isnan(a.temperatura_c);
  a.alerta = (!temp_invalida && (a.temperatura_c > TEMP_MAX_C ||
                                  a.temperatura_c < TEMP_MIN_C)) ||
             (a.bpm > BPM_MAX) || (a.bpm < BPM_MIN);
  return a;
}

// ============================================================================
// BOTAO: agora forca um ciclo de reconexao (simula queda de rede)
// ============================================================================

void atualizar_botao() {
  bool estado = digitalRead(PIN_BTN_WIFI);
  if (btn_estado_ant == HIGH && estado == LOW) {
    Serial.println(F("\n>>> Botao pressionado: forcando desconexao MQTT/Wi-Fi"));
    mqtt.disconnect();
    WiFi.disconnect(true, false);
    delay(150);
  }
  btn_estado_ant = estado;
}

// ============================================================================
// SETUP & LOOP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(F("\n=== CardioIA - Cloud Node (MQTT) iniciando ==="));

  pinMode(PIN_LED_STATUS, OUTPUT);
  pinMode(PIN_BTN_WIFI, INPUT_PULLUP);
  digitalWrite(PIN_LED_STATUS, LOW);

  dht.begin();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setKeepAlive(15);   // keepalive curto -> deteccao rapida em wearable

  conectar_wifi();
  conectar_mqtt();
}

void loop() {
  atualizar_botao();

  // ----- 1) Manutencao de conexoes -----
  bool online = (WiFi.status() == WL_CONNECTED) && mqtt.connected();
  digitalWrite(PIN_LED_STATUS, online ? HIGH : LOW);

  if (!online) {
    uint32_t agora = millis();
    if (agora - t_ultima_reconexao >= INTERVALO_RECONEXAO_MS) {
      t_ultima_reconexao = agora;
      if (WiFi.status() != WL_CONNECTED) conectar_wifi();
      if (WiFi.status() == WL_CONNECTED && !mqtt.connected()) conectar_mqtt();
    }
  }

  // mqtt.loop() trata PINGREQ/PINGRESP e callbacks
  if (mqtt.connected()) mqtt.loop();

  // ----- 2) Coleta periodica + publicacao -----
  uint32_t agora = millis();
  if (agora - t_ultima_leitura >= INTERVALO_LEITURA_MS) {
    t_ultima_leitura = agora;
    Amostra a = ler_sensores();

    if (isnan(a.temperatura_c) || isnan(a.umidade_pct)) {
      Serial.println(F("[ERRO] DHT22 retornou NaN. Pulando esta amostra."));
      return;
    }

    Serial.print(F("[EDGE] t="));
    Serial.print(a.temperatura_c, 1);
    Serial.print(F("C  u="));
    Serial.print(a.umidade_pct, 1);
    Serial.print(F("%  bpm="));
    Serial.print(a.bpm);
    if (a.alerta) Serial.print(F("  *ALERTA*"));

    if (online) {
      // Antes de publicar a nova, drena qualquer pendente do buffer
      sincronizar_buffer();
      bool ok = publicar_amostra(a);
      Serial.println(ok ? F("  -> publicado") : F("  -> FALHA, indo p/ buffer"));
      if (!ok) buffer_inserir(a);
    } else {
      buffer_inserir(a);
      Serial.print(F("  -> buffered ("));
      Serial.print(buf_count);
      Serial.print(F("/"));
      Serial.print(BUFFER_CAP);
      Serial.println(F(")"));
    }
  }

  delay(20);
}
