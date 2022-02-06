# Exemplo Blink

## Informações p/ o projeto

- Fonte: [Repositório blink oficial ESP-IDF](https://github.com/espressif/esp-idf/tree/v4.4/examples/get-started/blink)

Este exemplo foi retirado e modificado do repositório oficial do ESP-IDF. O detalhe que devemos observar é o pino referente ao LED Onboard da M5StickC Plus. Como pode ser visto na tabela a seguir, de acordo com o mapa de pinos oficial, o LED se encontra na **GPIO10**.

<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-nuyl{background-color:#FBFCFE;color:#7D7D7D;text-align:center;vertical-align:middle}
.tg .tg-kf00{background-color:#3166ff;color:#343434;font-weight:bold;text-align:center;vertical-align:middle}
.tg .tg-gp73{background-color:#EDF4FC;color:#7D7D7D;text-align:center;vertical-align:middle}
.tg .tg-8n0b{background-color:#3166ff;color:#FFF;font-weight:bold;text-align:center;vertical-align:middle}
</style>
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