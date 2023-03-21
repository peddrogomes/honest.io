## Do que se trata?

- Esse é um trabalho de conclusão de curso (TCC) de Engenharia Eletrônica da UFPE apresentado por mim e orientado pelo professor João Marcelo;

- Trás uma visão disruptiva do mercado, mesclando os conceitos de mercado autônomo e marketplace físico, desenvolvendo um sistema de automação para loja, no qual pequenos vendedores podem cadastrar seus produtos e terceirizar suas vendas presenciais. 

- É proposto um sistema de vendas com produtos e instruções sendo apresentadas em uma tela LCD, gerenciada por um Arduino e conectada a internet através de uma Wemos, no qual executa todo o processo de monitoramento e transferência dos pagamentos para os respectivos vendedores, utilizando um gateway de pagamento(Asaas) via HTTPS.

## Organização das pastas:

- A pasta Honest_io contém o código da "interface gráfica" do projeto, carregado no Kit Arduino R3 + LCD TFT, bem como algumas instruções relevantes;

- A pasta gateway_de_pagamento contém o código do "gateway de pagamento" do projeto, carregado na wemos, bem como algumas instruções relevantes.

## Prototipo:
![prototipo](images/prototipo.jpeg)

## Conexões dos dois módulos (Arduino > Wemos):
- Tx > Rx
- Rx > Tx
- 3.3V > 3v3V
- GND > GND
    
