# Blink Example

O objetivo desse exemplo é fazer com que o led pisque, enquanto o monitor serial espera novos valores para o período de piscada do LED. Para isso, foram utilizadas duas tasks, uma responsável por fazer o led piscar e a outra responsável por capturar o novo valor, transformar para inteiro e alterar a variável que referencia o período em que o led vai ficar ligado/desligado.