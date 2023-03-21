### ** Modificação das imagens **

 ** QR CODE 
	O qr code do pix deve ser gerado no site: https://www.gerarpix.com.br/ 
	e o valor do produto necessariamente deve ser preenchido.
	IMPORTANTE: os qrcodes sao gerados para a chave pix da conta Asaas. 

"As telas da interface gráfica foram produzidas utilizando o Canva e exportadas em
PNG com 240x400 pixels, porém para utilização no projeto foi necessário a conversão
em imagens BMP de 32 bits, nesse processo foi utilizado um conversor online" 

conversor online:  https://online-converting.com/image/convert2bmp/


### ** Como conseguir enviar codigos para o arduino com tela LCD ** 

- Deve-se baixar o arduinoIDE, depois a biblioteca relativa ao LCD especifico 
(no caso do nosso projeto estamos usando o Driver IC: ST7793, esse dado está atras do LCD)
Link para download da biblioteca especifica. 

copie as pastas dentro de '[OPEN-SMART] 3.5inch TFT LCD Breakout ST7793\Arduino library'
para a pasta '\Documents\Arduino\libraries'


https://drive.google.com/drive/folders/1UDKI61K0t3FdZD_NjZ_A4gWGwfJk4Y3X?usp=sharing

Também lembrar de instalar a biblioteca SD, pode ser pesquisada facilmente no gerenciador de lib

### ** Importante ** 

Para conseguir enviar o código para o arduino, as portas de comunicação (Tx, Rx) devem ser desligadas,
no caso do nosso projeto, a remoção dos cabos de comunicação é feita na Wemos, visto que os cabos estão soldados no arduino.