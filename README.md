# Exemplos de códigos com ESP-IDF p/ M5StickC Plus 

Acesse o link de cada projeto para ter acesso a mais informações sobre eles. Todos os projetos foram testados na versão **v4.4 do ESP-IDF**.

<table>
<thead>
  <tr>
    <th colspan="3"><img src="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/blob/main/Images/m5stickc-plus-iot-development-module-esp32-pico.png?raw=true" width="400" height="300"></th>
  </tr>
</thead>
<thead>
  <tr>
    <th>Projeto</th>
    <th>Descrição</th>
    <th>Acesse o projeto</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td>Blink</td>
    <td>Primeiro projeto com o objetivo de fazer o LED Onboard da M5StickC piscar dado um período de tempo. Além disso, verificar todas as configurações referentes ao sistema de build do ESP-IDF e compilar para a placa.</td>
    <td><a href="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/tree/main/Projects/1_blink">Clique Aqui</a><br></td>
  </tr>
  <tr>
    <td>Blink com Tasks</td>
    <td>Utilizando as tasks do FreeRTOS para realizar o blink do LED Onboard.</td>
    <td><a href="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/tree/main/Projects/2_blink_with_task">Clique Aqui</a></td>
  </tr>
  <tr>
    <td>Blink com Entrada do Usuário + Múltiplas Tasks</td>
    <td>Duas tasks são utilizadas. Uma task é responsável por realizar o Blink do LED e a outra task recebe novos valores do usuário através do console para atualizar o intervalo de tempo do Blink.</td>
    <td><a href="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/tree/main/Projects/3_blink_period_tasks">Clique Aqui</a></td>
  </tr>
  <tr>
    <td>Blink + Múltiplas Tasks + Queue</td>
    <td>Duas tasks são utilizadas. Cada task possui uma Queue (fila) que gerencia os comandos. A Task A responsável pelos dados de entrada e saída do console, além de enviar o valor de intervalo do Blink para Task B. A Task B atualiza valor do Blink e envia mensagem para console através da Task A&nbsp;&nbsp;a cada 100 Blinks.</td>
    <td><a href="https://github.com/bittoin/M5StickC-Plus-IDF-Examples/tree/main/Projects/4_blink_with_queue">Clique Aqui</a></td>
  </tr>
  <tr>
    <td>Utilizando MPU6886 interno do M5StickC/M5StickC Plus</td>
    <td>Configuração do projeto e das bibliotecas necessárias para realizar a leitura de dados do MPU6886 interno da M5StickC Plus.</td>
    <td><a href="">Clique Aqui</a></td>
  </tr>
</tbody>
</table>
