# Fase 3 — Parte 1: Edge Computing com ESP32

**Projeto Wokwi:** https://wokwi.com/projects/463854286798211073
**Código versionado:** [src/fase_3/parte_1_edge/sketch.ino](../../src/fase_3/parte_1_edge/sketch.ino)

## 1. Visão geral

Esta etapa do CardioIA implementa o **nó de borda (Edge)** de um sistema vestível de monitoramento cardíaco. O firmware roda em um ESP32 simulado no Wokwi e cumpre todas as exigências do enunciado da Fase 3 — Parte 1:

- ≥ 2 sensores distintos, sendo um obrigatório de temperatura.
- Resiliência offline: continua coletando mesmo sem "conexão".
- Sincronização automática quando a "Wi-Fi" simulada volta a ficar ativa.
- Estratégia de armazenamento limitado alinhada ao modelo de wearable.
- Alertas locais sem precisar consultar a nuvem (decisão no Edge).

## 2. Hardware simulado

| Componente | Pino ESP32 | Papel |
|---|---|---|
| **DHT22** (sensor obrigatório) | GPIO 15 | Mede **temperatura** e **umidade** corporal/ambiental |
| **Potenciômetro** (2º sensor — escolha do grupo) | GPIO 34 (ADC1_CH6) | Simula um **sensor cardíaco** reportando BPM (40–180) |
| **LED** | GPIO 2 | Status: aceso = online, apagado = offline, pisca = sincronizando |
| **Push-button** | GPIO 4 (`INPUT_PULLUP`) | Alterna o estado simulado de Wi-Fi |

A escolha do potenciômetro como segundo sensor é deliberada: ele gera um **sinal analógico contínuo** que se traduz em uma curva de BPM realista no dashboard da Parte 2, e permite ao grupo "provocar" condições de taquicardia e bradicardia de forma controlada para testar os alertas clínicos.

## 3. Fluxo de funcionamento

```
        +------------------+        +-----------------+        +------------------+
        |  Sensores (DHT22 | ---->  | Buffer circular | ---->  |  Serial.println  |
        |  + Potenciômetro)|        | em RAM (FIFO)   |        |  (= "nuvem")     |
        +------------------+        +-----------------+        +------------------+
                                            ^
                                            |
                                       Wi-Fi simulado
                                       (botão GPIO 4)
```

A cada **2 segundos** o firmware lê os dois sensores e monta uma `Amostra` com `{timestamp, temperatura, umidade, bpm, alerta}`. O campo `alerta` é avaliado **localmente** com base nos limites clínicos definidos pelo grupo (febre > 38 °C, hipotermia < 35 °C, taquicardia > 120 BPM, bradicardia < 50 BPM). Isso é a essência do **Edge Computing** aplicado à saúde: a decisão de "isto é uma anomalia" não depende da nuvem nem da latência da rede.

Quando a Wi-Fi simulada está **OFFLINE**, a amostra é empilhada no buffer circular em RAM e nada é enviado. Quando o operador pressiona o botão e a flag vira **ONLINE**, o firmware drena o buffer publicando cada amostra com `Serial.println` em formato CSV — simulando o que será MQTT JSON na Parte 2. O LED pisca a cada amostra enviada, dando feedback visual da sincronização.

## 4. Estratégia de resiliência (buffer circular)

O enunciado pede uma "estratégia de armazenamento limitado alinhada ao modelo de negócio". Optamos por **buffer circular em RAM** (`Amostra buffer[100]`) com política **FIFO de descarte do mais velho** quando o buffer atinge a capacidade máxima.

**Dimensionamento:** com leitura a cada 2 s, **100 amostras = 200 s ≈ 3 min 20 s** de autonomia offline antes de começar a descartar dados antigos.

**Justificativa clínica:** para um paciente cardíaco usando o wearable em ambiente residencial ou hospitalar, 3 min cobre folgadamente cenários como troca de cômodo, banho rápido, entrada em elevador ou áreas com sombra de Wi-Fi. Casos de queda de rede mais longos passam a violar o SLA do wearable e, nesse cenário, **preservar as amostras mais recentes é clinicamente mais relevante** do que reter o histórico antigo: o médico precisa saber o que está acontecendo *agora*, não o que aconteceu há 10 min se o aparelho está há 15 min sem rede. Por isso a política FIFO descarta o tail (mais velho), não o head (mais novo). O contador `total_descartadas` é exposto no Monitor Serial após cada flush para auditoria.

## 5. Sobre o SPIFFS

O PDF do enunciado explicita que o SPIFFS é **volátil no simulador Wokwi/PlatformIO** (perde tudo ao reiniciar) e autoriza o Monitor Serial como alternativa de resiliência. Adotamos a **alternativa mais robusta** dentro dessa limitação: um buffer circular em RAM. Em hardware real, este mesmo buffer seria espelhado em SPIFFS ou em um microSD a cada N amostras para sobreviver a reboots — a estrutura `Amostra` foi propositadamente pequena (16 bytes) para caber confortavelmente em ambos.

## 6. Como executar e validar

1. Abrir https://wokwi.com/projects/463854286798211073 e clicar em **Start the simulation**.
2. Acompanhar o Monitor Serial: linhas `[EDGE] t=… u=… bpm=… | buffer=NN/100 wifi=OFF` devem aparecer a cada 2 s.
3. Pressionar o botão (GPIO 4) → LED acende e linhas `[SYNC] Iniciando envio de N amostras…` seguidas de `[CLOUD<-EDGE] ts=…,temp=…` aparecem.
4. Pressionar de novo para voltar a OFFLINE; girar o potenciômetro para BPM > 120 ou ajustar a temperatura do DHT22 para > 38 °C; observar o marcador `*ALERTA*` na linha de log.
5. Encher o buffer (deixar > 200 s offline) e voltar a online: o `[AVISO]` mostra quantas amostras antigas foram descartadas.

> **Print esperado do Monitor Serial:** ver [imagens/wokwi_edge_serial.png](imagens/wokwi_edge_serial.png) (a capturar pela equipe ao rodar a simulação).
