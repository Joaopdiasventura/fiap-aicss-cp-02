# NetGuard - AICSS CP2

## Objetivo

O NetGuard simula, em um ESP32, uma verificacao simples entre um IPv4 informado pelo Serial Monitor e um IPv4 de referencia fixo para um dominio monitorado.

O sistema valida o formato do IPv4 digitado, compara o valor com a referencia configurada e sinaliza o resultado usando LEDs. O ESP32 desta atividade nao realiza DNS, Wi-Fi, WHOIS, HTTP, HTTPS, TLS ou consultas externas.

## Hardware

- ESP32 DevKit
- 1 botao
- 1 LED azul
- 1 LED vermelho
- 3 resistores de 1 kOhm
- Jumpers

## Configuracao

Valores principais em `src/main.ino`:

```cpp
const char monitoredDomain[] = "github.com";
const char referenceIpv4[] = "140.82.112.3";
const int buttonPin = 4;
const int blueLedPin = 22;
const int redLedPin = 23;
```

- Dominio: `github.com`
- IPv4 de referencia: `140.82.112.3`
- Serial: `115200 baud`
- Fim de linha no Wokwi: `CRLF`

## Funcionamento

1. Ao iniciar, os LEDs azul e vermelho ficam apagados.
2. Ao pressionar o botao, o sistema apaga os LEDs e inicia uma nova verificacao.
3. O Serial Monitor solicita um endereco IPv4.
4. O usuario digita o IPv4 e pressiona Enter.
5. O codigo valida o formato da entrada.
6. Se a entrada for valida, o IPv4 informado e comparado com `140.82.112.3`.
7. Se for igual a referencia, somente o LED azul acende.
8. Se for diferente ou invalido, somente o LED vermelho acende.
9. O sistema mostra dominio monitorado, IP informado e resultado, depois aguarda novo pressionamento do botao.

A leitura serial e nao bloqueante e trata finais de linha `\n`, `\r` e `\r\n` sem processar duas vezes a mesma entrada. O botao usa debounce com `millis()`, sem `delay()` bloqueante.

## Circuito

Conexoes principais:

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

Se `pio` nao estiver no `PATH`, use a extensao PlatformIO do VS Code ou o executavel do PlatformIO instalado no ambiente.

## Como executar no Wokwi

1. Abra a simulacao no Wokwi.
2. Abra o Serial Monitor em `115200 baud`.
3. Pressione o botao.
4. Digite um IPv4 e pressione Enter.
5. Confira o resultado no Serial Monitor e nos LEDs.

## Experimentos

| Experimento | IP | Tipo | LED azul | LED vermelho |
| --- | --- | --- | --- | --- |
| 1 | `140.82.112.3` | Referencia | Aceso | Apagado |
| 2 | `4.228.31.150` | Publico diferente | Apagado | Aceso |
| 3 | `192.168.15.8` | Privado | Apagado | Aceso |

Saidas esperadas no Serial Monitor:

```text
Dominio monitorado: github.com
IP informado: 140.82.112.3
Resultado: IP CORRETO
```

```text
Dominio monitorado: github.com
IP informado: 4.228.31.150
Resultado: IP DIFERENTE DA REFERENCIA
```

```text
Dominio monitorado: github.com
IP informado: 192.168.15.8
Resultado: IP DIFERENTE DA REFERENCIA
```

## Teste opcional de IPv4 invalido

| Entrada | Resultado esperado | LED azul | LED vermelho |
| --- | --- | --- | --- |
| `999.999.999.999` | IPv4 invalido | Apagado | Aceso |

Tambem sao invalidos exemplos como `140.82.112`, `abc.def.ghi.jkl`, `140.82.112.3.4` e entrada vazia.

## Observacao de seguranca

Neste projeto, "IP diferente" significa divergencia em relacao a referencia fixa configurada. Isso nao significa necessariamente que ocorreu DNS spoofing ou outro ataque.

Um IP diferente pode ocorrer legitimamente por distribuicao geografica, CDN, balanceamento, alteracao de infraestrutura ou multiplos servidores. Durante o trabalho de Cognitive CyberSecurity, consultas DNS realizadas pelo grupo retornaram `4.228.31.150` para `github.com`, enquanto a referencia definida para este checkpoint permaneceu `140.82.112.3`.

O ESP32 desta atividade nao faz analise avancada. Ele apenas compara o IPv4 informado com uma referencia fixa.
