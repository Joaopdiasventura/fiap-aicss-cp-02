# NetGuard - Checkpoint 2 AICSS

Sistema para validar se um IPv4 informado no Serial Monitor corresponde ao IPv4 de referencia de um dominio monitorado.

## Ambiente

- Placa: ESP32 DevKit (`esp32dev`)
- Framework: Arduino
- Build: PlatformIO
- Simulacao: Wokwi
- Monitor serial: 115200 bps

## Configuracao central

Os valores ficam no inicio de `src/main.ino`:

```cpp
const char monitoredDomain[] = "PENDENTE_DE_PREENCHIMENTO";
const char referenceIpv4[] = "0.0.0.0";
const int buttonPin = 4;
const int blueLedPin = 22;
const int redLedPin = 23;
```

O enunciado/PDF com o dominio e IPv4 de referencia nao foi encontrado no workspace. Por isso, `monitoredDomain` e `referenceIpv4` estao claramente marcados como pendentes e devem ser preenchidos com os dados definidos no checkpoint de Cognitive CyberSecurity antes da demonstracao final.

## Componentes

- 1 ESP32 DevKit
- 1 botao
- 1 LED azul
- 1 LED vermelho
- 3 resistores de 1 kOhm
- Jumpers

## Conexoes

Diagrama textual:

```text
ESP32 GPIO 4  -> botao -> 3V3
GPIO 4        -> resistor 1 kOhm -> GND

ESP32 GPIO 22 -> resistor 1 kOhm -> anodo LED azul
catodo LED azul -> GND

ESP32 GPIO 23 -> resistor 1 kOhm -> anodo LED vermelho
catodo LED vermelho -> GND
```

## Como compilar

```bash
pio run
```

## Como executar

1. Inicie o ESP32 ou a simulacao no Wokwi.
2. Abra o Serial Monitor em 115200 bps.
3. Pressione o botao.
4. Digite um IPv4 e pressione Enter.
5. Observe o resultado no Serial Monitor e o LED correspondente.

Na inicializacao, os dois LEDs ficam apagados. Cada novo pressionamento desliga os LEDs, limpa a entrada anterior e inicia uma nova verificacao.

## Regras de resultado

- IPv4 valido igual ao IPv4 de referencia: somente LED azul acende.
- IPv4 valido diferente do IPv4 de referencia: somente LED vermelho acende.
- Entrada em formato IPv4 invalido: somente LED vermelho acende e o Serial Monitor informa entrada invalida.

O codigo valida o formato IPv4 antes da comparacao e descarta linhas vazias para evitar verificacoes com entradas residuais.

## Experimentos obrigatorios

Antes de executar os experimentos, preencha `monitoredDomain` e `referenceIpv4` com os dados corretos.

| Experimento | Entrada no Serial Monitor                | Resultado esperado                          |
| ----------- | ---------------------------------------- | ------------------------------------------- |
| 1           | IPv4 de referencia configurado           | LED azul ligado e mensagem `CORRESPONDENTE` |
| 2           | IPv4 publico diferente do configurado    | LED vermelho ligado e mensagem `DIFERENTE`  |
| 3           | IPv4 privado, por exemplo `192.168.0.10` | LED vermelho ligado e mensagem `DIFERENTE`  |

## Observacoes

- O debounce do botao e feito com `millis()`, sem bloquear o loop principal.
- A verificacao e iniciada apenas na transicao estavel do botao para pressionado, evitando multiplas verificacoes no mesmo acionamento.
- O sistema nao realiza DNS, Wi-Fi ou qualquer consulta externa; a validacao e apenas a comparacao do IPv4 digitado com o IPv4 de referencia configurado.
