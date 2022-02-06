# Exemplo Blink

## Informações p/ o projeto

- Fonte: [Repositório blink oficial ESP-IDF](https://github.com/espressif/esp-idf/tree/v4.4/examples/get-started/blink)

Este exemplo foi retirado e modificado do repositório oficial do ESP-IDF. O detalhe que devemos observar é o pino referente ao LED Onboard da M5StickC Plus. Como pode ser visto na tabela a seguir, de acordo com o mapa de pinos oficial, o LED se encontra na **GPIO10**.

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
    <p>Projeto Blink Funcionando</p>
</div>