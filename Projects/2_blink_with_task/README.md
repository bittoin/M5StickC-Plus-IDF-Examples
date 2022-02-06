# Blink Example

Este exemplo foi retirado e modificado de uma aula sobre FreeRTOS realizada no canal oficial da Digi-Key no YouTube. A explicação sobre a criação de uma task, o que cada parâmetro significa e a importância de utilizá-la nos projetos pode ser encontrada tanto no vídeo quanto na documentação oficial da Espressif. Além disso, devemos observar o pino referente ao LED Onboard da M5StickC Plus. Como pode ser visto na tabela a seguir, de acordo com o mapa de pinos oficial, o LED se encontra na **GPIO10**.

**Fontes:** 

- [Video: Getting Started with FreeRTOS](https://youtu.be/JIr7Xm_riRs)
- [Explicação sobre criação de tasks Doc oficial Espressif](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/system/freertos.html?highlight=xtaskcreatepinnedtocore#_CPPv423xTaskCreatePinnedToCore14TaskFunction_tPCKcK8uint32_tPCv11UBaseType_tPC12TaskHandle_tK10BaseType_t)

<table class="tg">
<thead>
  <tr>
    <th class="tg-8n0b"><span style="color:#000">ESP32</span></th>
    <th class="tg-kf00">GPIO10</th>
    <th class="tg-kf00">GPIO9</th>
    <th class="tg-kf00">GPIO37</th>
    <th class="tg-kf00">GPIO39</th>
    <th class="tg-kf00">GPIO2</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-nuyl">RED LED</td>
    <td class="tg-nuyl">LED Pin</td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
  </tr>
  <tr>
    <td class="tg-gp73">IR Transmitter</td>
    <td class="tg-gp73"></td>
    <td class="tg-gp73">Transmitter Pin</td>
    <td class="tg-gp73"></td>
    <td class="tg-gp73"></td>
    <td class="tg-gp73"></td>
  </tr>
  <tr>
    <td class="tg-nuyl">BUTTON A</td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl">Button Pin</td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
  </tr>
  <tr>
    <td class="tg-gp73">BUTTON B</td>
    <td class="tg-gp73"></td>
    <td class="tg-gp73"></td>
    <td class="tg-gp73"></td>
    <td class="tg-gp73">Button Pin</td>
    <td class="tg-gp73"></td>
  </tr>
  <tr>
    <td class="tg-nuyl">Buzzer</td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl"></td>
    <td class="tg-nuyl">Buzzer Pin</td>
  </tr>
</tbody>
</table>

## Projeto funcionando

<div align='center'>
    <img src="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/blob/main/Images/1_blink_cp.gif?raw=true">
    <p>Projeto Blink com task funcionando</p>
</div>