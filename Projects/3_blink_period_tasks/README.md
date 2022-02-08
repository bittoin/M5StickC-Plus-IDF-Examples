# Blink Example

O objetivo desse exemplo é fazer com que o led pisque, enquanto o monitor serial espera novos valores para o período de piscada do LED. Para isso, foram utilizadas duas tasks, uma responsável por fazer o led piscar e a outra responsável por capturar o novo valor, transformar para inteiro e alterar a variável que referencia o período em que o led vai ficar ligado/desligado.

## Informações do projeto

Este exemplo foi retirado e modificado de uma aula sobre FreeRTOS realizada no canal oficial da Digi-Key no YouTube. Seu funcionamento consiste na execução de duas tasks: `consoleAPP()` e `toggleLed()`.

### Função `consoleApp()`

Essa função é responsável por esperar a entrada de dados do usuário. Quando o usuário insere algum valor no console, se for um número inteiro válido, será convertido de texto para uma variável inteira e atualizará o intervalo de tempo em que o LED pisca.

### Função `toggleLed()`

Essa função é responsável unicamente por ligar e desligar o LED onboard da placa, dado o intervalo de tempo inserido pelo usuário. Por padrão, intervalos começam em 100ms (1 segundo).

### Fontes

- [Video: Task Scheduling](https://youtu.be/95yUbClyf3E)
- [Explicação sobre entrada de dados no console Doc oficial Espressif](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/peripherals/uart.html)

## Projeto funcionando

<div align='center'>
    <img src="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/blob/main/Images/3_blink_input_cp.gif?raw=true">
    <p>Intervalo de piscadas alterna conforme entrada do usuário</p>
</div>