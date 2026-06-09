# Fase 3 — Parte 2: Transmissão MQTT e Dashboard Node-RED

**Código ESP32:** [src/fase_3/parte_2_nuvem/sketch.ino](../../src/fase_3/parte_2_nuvem/sketch.ino)
**Flow Node-RED:** [src/fase_3/parte_2_nuvem/node_red_flow.json](../../src/fase_3/parte_2_nuvem/node_red_flow.json)

## 1. Arquitetura e fluxo de comunicação

A Parte 2 fecha o ciclo IoT do CardioIA: o nó Edge da Parte 1 deixa de simular o envio via `Serial.println` e passa a **publicar de fato em um broker MQTT na nuvem**, de onde uma instância Node-RED consome os dados e os apresenta em um dashboard com gráfico, gauge e indicador de alerta.

```
   +---------------+   Wi-Fi    +---------------------+   subscribe    +-------------------+
   |    ESP32      | =========> |  Broker MQTT        | =============> |  Node-RED         |
   |  (publisher)  |  TCP/1883  |  broker.hivemq.com  |                |  (dashboard)      |
   +---------------+            +---------------------+                +-------------------+
        |                              ^                                       |
        |  publish JSON                | retained "online"/"offline"           |
        |  cardioia/<id>/sinais        | LWT na queda                          v
        |  cardioia/<id>/alerta        |                                 Chart BPM
        |  cardioia/<id>/status        |                                 Gauge Temp
                                                                         Indicador Alerta
```

**Tópicos publicados pelo ESP32:**

| Tópico | Quando | Payload |
|---|---|---|
| `cardioia/cardioia-001/sinais` | a cada 2 s (sempre) | `{"ts":…, "temp":…, "umid":…, "bpm":…, "alert":…}` |
| `cardioia/cardioia-001/alerta` | apenas quando algum limite é violado | mesmo JSON |
| `cardioia/cardioia-001/status` | conexão / desconexão | `"online"` ou `"offline"` (retained, via LWT) |

A hierarquia `cardioia/<deviceId>/<assunto>` é proposital: permite que o Node-RED assine `cardioia/+/sinais` e suporte **múltiplos dispositivos** simultaneamente sem precisar reconfigurar o flow. Adicionar um novo wearable no futuro é apenas atribuir um `deviceId` diferente no firmware.

## 2. Por que MQTT (e não HTTP)

O protocolo MQTT foi escolhido pelo enunciado e é, de fato, o padrão de mercado para IoT médico. Os motivos relevantes para o CardioIA:

- **Modelo publish/subscribe** desacopla produtor e consumidor: o ESP32 não precisa saber quantos dashboards estão conectados.
- **Cabeçalho minúsculo** (~2 bytes em PUBLISH) reduz consumo de bateria do wearable em comparação a HTTP/REST.
- **Conexão TCP persistente** evita o overhead de handshake a cada amostra.
- **QoS configurável**: usamos **QoS 1 (at-least-once)** nos tópicos de sinais e alerta — adequado quando o duplicate ocasional é tolerável mas a perda não é. Para a flag `status`, usamos *retained* + LWT para que o dashboard saiba o estado real assim que se conecta.
- **Keepalive curto (15 s)** no ESP32 garante que a queda de rede seja detectada rapidamente — em wearable médico, 30+ s de silêncio é tempo demais.

## 3. Lógica do firmware (`src/fase_3/parte_2_nuvem/sketch.ino`)

O sketch é um **fork direto da Parte 1**, preservando toda a lógica de coleta e o buffer circular. As mudanças significativas:

1. **Inclusão de `WiFi.h` + `PubSubClient.h` + `ArduinoJson.h`**. A rede do Wokwi (`Wokwi-GUEST`, sem senha) é usada para conectar a internet.
2. **`mqtt.setKeepAlive(15)`** — handshake mais agressivo para detectar queda.
3. **`mqtt.connect(...)` com Last Will**: caso a TCP morra abruptamente, o broker publica `"offline"` em `cardioia/cardioia-001/status` para alertar o dashboard.
4. **`publicar_amostra()`** serializa a `Amostra` em JSON com `ArduinoJson` e chama `mqtt.publish(TOPIC_SINAIS, ...)`. Se `alerta == true`, replica em `TOPIC_ALERTA`.
5. **`sincronizar_buffer()`** drena o buffer circular publicando em sequência. Se uma publicação falhar no meio do flush, o restante permanece no buffer para a próxima rodada — **a resiliência da Parte 1 é estendida também para falha de broker**, não só falha de Wi-Fi.
6. **Botão (GPIO 4)** agora **força um ciclo de reconexão**: `mqtt.disconnect()` + `WiFi.disconnect()`. Útil para demonstrar visualmente, no dashboard, a transição online → offline → online.

O LED do GPIO 2 reflete o estado consolidado `Wi-Fi conectado AND MQTT conectado`, dando ao operador da demonstração um feedback imediato sem precisar olhar o Serial.

## 4. Dashboard Node-RED (`node_red_flow.json`)

O flow exportado tem **5 nós funcionais** organizados em dois grupos de UI:

### Grupo "Sinais Vitais"
- **`ui_chart` — Batimentos cardíacos (BPM)**: linha temporal dos últimos 5 minutos, eixo Y de 30 a 190 BPM. Recebe diretamente o campo `bpm` extraído do JSON.
- **`ui_gauge` — Temperatura corporal (°C)**: medidor analógico com 3 faixas — **verde** (35–37,5 °C, normal), **amarelo** (37,5–38 °C, febre baixa) e **vermelho** (> 38 °C, febre / risco). Limite inferior visual: 30 °C; superior: 42 °C.

### Grupo "Status Clínico"
- **`ui_template` — Indicador de alerta**: bloco HTML colorido que muda de **verde (NORMAL)** para **vermelho (ALERTA)** com o motivo clínico (taquicardia, bradicardia, febre, hipotermia). Atualiza em tempo real a cada amostra.

### Função central de avaliação
O nó `function` "Extrai bpm/temp/alerta" é o cérebro do dashboard. Ele:
1. Faz parse do JSON do ESP32 e extrai `bpm`, `temp`, `umid`, `alert`.
2. Aplica os **limiares clínicos**: BPM ∈ [50, 120], temp ∈ [35, 38].
3. Gera 3 mensagens de saída: uma para o chart, uma para o gauge, uma com HTML formatado para o indicador.

Limiares justificados clinicamente (mesmos do Edge):
- **BPM > 120**: taquicardia em repouso, indicativo de febre/desidratação/arritmia.
- **BPM < 50**: bradicardia, pode indicar bloqueio AV ou efeito medicamentoso.
- **Temp > 38 °C**: febre — confirma alerta sistêmico.
- **Temp < 35 °C**: hipotermia — risco em pacientes idosos / pós-operatório.

A assinatura `cardioia/+/sinais` (wildcard) garante que o flow suporta múltiplos `deviceId` automaticamente — o dashboard mostra o último dispositivo a publicar.

## 5. Como executar end-to-end

```bash
# 1. Subir um Node-RED local com plugin de dashboard
docker run -it -p 1880:1880 nodered/node-red:latest

# 2. Acessar http://localhost:1880 → Menu → Import → colar node_red_flow.json
# 3. Deploy. Abrir http://localhost:1880/ui

# 4. Abrir o projeto Wokwi da Parte 2 (a equipe cria um novo projeto no
#    wokwi.com colando os 4 arquivos de src/fase_3/parte_2_nuvem/) e
#    iniciar a simulação.

# 5. Validar com mosquitto_sub em paralelo:
mosquitto_sub -h broker.hivemq.com -t 'cardioia/#' -v
```

**Cenários que devem aparecer no dashboard:**

| Ação | Esperado |
|---|---|
| Wokwi iniciado, DHT22 = 36,5 °C, pot no meio (~110 BPM) | Chart subindo, gauge verde, indicador **NORMAL** verde |
| Girar pot para BPM > 120 | Indicador vira **ALERTA — Taquicardia**, vermelho |
| Aumentar temp do DHT22 para 39 °C | Gauge passa pra vermelho, indicador "ALERTA — Febre" |
| Pressionar botão GPIO 4 (força queda) | LED apaga; topic `status` recebe `"offline"` (LWT) |
| Esperar ~5 s pela reconexão | Status volta a `"online"`, buffer drena → chart preenche os 2-3 pontos faltantes |

Prints esperados (a equipe captura ao rodar):
- [imagens/wokwi_mqtt_serial.png](imagens/wokwi_mqtt_serial.png) — Monitor Serial mostrando `[MQTT] CONNACK recebido`.
- [imagens/node_red_flow.png](imagens/node_red_flow.png) — editor do Node-RED com o flow montado.
- [imagens/node_red_dashboard.png](imagens/node_red_dashboard.png) — `/ui` em estado normal.
- [imagens/node_red_alerta.png](imagens/node_red_alerta.png) — `/ui` em estado de alerta.

## 6. Segurança e considerações para produção

Para fins didáticos, esta entrega usa o **broker público `broker.hivemq.com`** sem TLS e sem autenticação. Para um sistema real que processa dados de saúde, **isso seria inaceitável** — abaixo, as decisões que mudariam:

| Aspecto | Esta entrega (acadêmica) | Produção (CardioIA real) |
|---|---|---|
| **Confidencialidade** | broker público, qualquer um pode assinar `cardioia/#` | broker dedicado com **TLS 1.3** (porta 8883), certificados X.509 |
| **Autenticação** | nenhuma | usuário/senha por dispositivo + ACL por tópico |
| **Identidade do device** | `cardioia-001` hard-coded | token único provisionado no setup (chip de segurança ATECC608A se houver) |
| **Integridade** | nenhuma | mensagens assinadas + replay protection via `ts` |
| **Conformidade legal** | n/a | LGPD: dados de saúde são **sensíveis** (art. 5º II), exigem base legal específica, consentimento informado e relatório de impacto (RIPD) |
| **Retenção** | broker apaga ao desconectar | armazenamento em time-series database (InfluxDB, TimescaleDB) com política de retenção e anonimização |

O caminho de migração é direto: trocar `MQTT_HOST` para uma instância **HiveMQ Cloud**, habilitar `wifi_client.setCACert(...)` com a CA do broker e passar credenciais no `mqtt.connect()`. O resto do firmware permanece inalterado — exatamente um dos benefícios de adotar o padrão MQTT desde o protótipo.

## 7. Por que Grafana ficou fora do escopo

O PDF marca o Grafana Cloud como **opcional**. Optamos por concentrar esforços em entregar um dashboard Node-RED completo e bem documentado, com chart + gauge + indicador, em vez de duplicar a visualização. A extensão futura é trivial: instalar o plugin `grafana-mqtt-datasource` em uma instância Grafana, assinar os mesmos tópicos e replicar os painéis. A separação de visualização do produtor IoT (graças ao MQTT) torna essa adição um trabalho de configuração, não de re-engenharia.

## 8. Conclusão

A Parte 2 demonstra, em escala reduzida, o pipeline completo de IoT médico: **captura no Edge → transporte via MQTT → visualização em Cloud → alertas automáticos**. A continuidade com a Parte 1 (mesmo buffer circular, mesmos limiares clínicos, mesma estrutura `Amostra`) reforça que o que muda entre Edge e Cloud não é a lógica clínica, e sim o **transporte** dos dados. A escolha do MQTT prepara o CardioIA para escalar de 1 dispositivo (este protótipo) para uma frota de wearables sem reescrita do firmware ou do dashboard — apenas com mudança de credencial e `deviceId`.
