
#### Para alterações de token de API, nome da rede Wi-Fi e senha Wi-Fi, é necessário apenas modificar o arquivo secrets.h e enviar o codigo .ino para a Wemos.


### ** Importante ** 

Para conseguir enviar o codigo para Wemos é necessário desconectar os cabos de comunicação Serial (porta Tx e Rx), após o envio, os cabos podem ser recolocados. 


### ** Caso não consiga enviar o código para Wemos ou não tenha a placa instalada siga as intruções abaixo **

Para realizar alterações no codigo do gateway de pagamento (wemos) é necessario adicionar a placa da Wemos no Arduino IDE.
Processo de adição da placa: (https://blog.smartkits.com.br/esp8266-como-programar-o-nodemcu-atraves-da-arduino-ide/)

Com a IDE do Arduino aberta clique em Arquivo > Preferências, 
adicione ao campo de URLs adicionais o seguinte link e clique em “OK:” 

http://arduino.esp8266.com/stable/package_esp8266com_index.json

Este processo permitirá que a IDE encontre a biblioteca do ESP8266 no Gerenciador de Placas. Para acessar o gerenciador e instalá-lo, vá em Ferramentas > Placa > Gerenciador de Placas, pesquise por “ESP8266” e instale o pacote da ESP8266 Community. Este processo nos permitirá utilizar qualquer placa que utilize o microcontrolador ESP8266.

Com a biblioteca instalada, a IDE estará propriamente configurada para programar o ESP8266. O próximo passo será selecionar nossa placa em Ferramentas > Placa > ESP8266 Boards.