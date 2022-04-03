# Blink Example

Este exemplo foi criado pela necessidade de ligar o LED Onboard da M5StickC como indicador de que ela está ligada. Visto que nos projetos eu não utilizo o display, algum indicador é necessário para que saibamos quando a bateria acabou ou se o sistema está ligado por acidente. Como não precisamos de um LED aceso com a potência máxima, o PWM será utilizado para acionar o led com uma das luminosidades mínimas. Além disso, devemos observar o pino referente ao LED Onboard da M5StickC Plus. Como pode ser visto na documentação do kit de desenvolvimento M5StickC-Plus, de acordo com o mapa de pinos oficial, o LED se encontra na **GPIO10**.

### Fontes

- [Explicação sobre PWM na Doc oficial Espressif](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/peripherals/ledc.html)

## Projeto funcionando

<div align='center'>
    <img src="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/blob/main/Images/6_fade_led.gif?raw=true">
    <p>PWM realizando Fade In e Fade Out</p>
</div>