# Blink com tasks de gerenciamento e Queue

O objetivo desse exemplo é aprender a utilizar o console, com entrada de usuário de uma maneira diferente. A verificação foi feita para parecer com o que ocorre quando lemos dados da serial com o Arduino, onde utilizamos a função `Serial.available()`. Com isso, a dinâmica muda e outras funcionalidades foram adicionadas em relação ao projeto anterior.

## Informações do projeto

Este exemplo foi retirado e modificado de uma aula sobre FreeRTOS realizada no canal oficial da Digi-Key no YouTube. Durante a aula a atividade foi passada e pode ser resumida na imagem a seguir. Logo após a explicação do que ocorre em cada caso.

<div align='center'>
    <img src="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/blob/main/Images/4_queue.png?raw=true">
    <p>Exercício retirado do vídeo referenciado em <b>Fontes</b></p>
</div>

### Função `taskA()`

A função taskA() printa no console tudo que recebe da `Queue 2`. Além disso, essa função é responsável por ler todos os inputs do usuário no console. Durante os experimentos, foi possível observar que apenas uma task deve ser encarregada de trabalhar com o console. Se mais de uma tentar printar algo ou realizar outras operações, pode dar erro. 

Ela também imprime no console a repetição de tudo que é digitado pelo usuário, exceto se o usuário enviar um comando no formato `delay xxx`, pois o valor referente ao `xxx` será o novo valor de intervalo entre as piscadas, que será enviado para a `Queue 1`.

### Função `taskB()`

A função taskB() atualiza o valor do blink sempre que recebe um novo valor da `Queue 1`. Além disso, a cada 100 piscadas de LED, envia para a `Queue 2` uma string com o valor `"Blinked"`.

### Fontes

- [Video: Queue](https://youtu.be/pHJ3lxOoWeI)
- [Explicação sobre entrada de dados no console Doc oficial Espressif](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/peripherals/uart.html)
- - [Explicação sobre Queue Doc oficial Espressif](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/system/freertos.html?highlight=queue-api#queue-api)

## Projeto funcionando

<div align='center'>
    <img src="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/blob/main/Images/4_blink_queue_cp.gif?raw=true">
    <p>Intervalo de piscadas alterna conforme entrada do usuário</p>
</div>