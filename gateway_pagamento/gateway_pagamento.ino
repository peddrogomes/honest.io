/**
   BasicHTTPSClient.ino

    Created on: 20.08.2018

*/


#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

#include <Arduino_JSON.h>
#include "secrets.h"

// Fingerprint for demo URL, expires on June 2, 2021, needs to be updated well before this date
//const uint8_t fingerprint[20] = {0x40, 0xaf, 0x00, 0x6b, 0xec, 0x90, 0x22, 0x41, 0x8e, 0xa3, 0xad, 0xfa, 0x1a, 0xe8, 0x25, 0x41, 0x1d, 0x1a, 0x54, 0xb3};

ESP8266WiFiMulti WiFiMulti;


void setup() {

  pinMode(D1, OUTPUT);

  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.mode(WIFI_STA);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    //Serial.flush();
    delay(1000);
  }

  
  WiFiMulti.addAP(SECRET_SSID, SECRET_PASS);

  while(WiFiMulti.run() != WL_CONNECTED){
    Serial.print(".");
  }
  // pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Wi-Fi connectado");
  // digitalWrite(LED_BUILTIN, HIGH);
}

int monitor = 1;
int monitor2 = 1;
int clean = 1;

float recebido=0;
float saldo_antigo=-1;


String conteudo = "";
char caractere;
int virgula=0;
String valor = "";
float valor_float = -1;
String key = "";
String key_type = "";

float taxa = 0.1;


void loop() {
  
if ((WiFiMulti.run() == WL_CONNECTED)) {

    // wait for WiFi connection

  // ######### Leitura Serial ###############
  

    if(monitor2==1){ // roda uma unica vez
    monitor2=0;
    String conteudo = "";
    char caractere;
    int virgula=0;
    String valor = "";
    float valor_float = -1;
    String key = "";
    }

  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    if(clean){
      clean=0;
      conteudo="";
    }
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  } 
  clean = 1; //nos garante limpar a variavel sempre que for ler os dados da proxima linha
  

  //conteudo.trim();

// msg recebida "valor,chave(key_type)"

 // key_type
      // CPF      // string      // Chave do tipo CPF

      // CNPJ      // string      // Chave do tipo CNPJ

      // EMAIL      // string      // Chave do tipo EMAIL

      // PHONE      // string      // Chave do tipo PHONE

      // EVP      // string      // Chave aleátoria


  if(conteudo!=""){

  // Serial.print("Recebi: ");
  // Serial.println(conteudo);

  virgula=conteudo.indexOf(",");

  valor = conteudo.substring(0,virgula);
  valor_float = valor.toFloat();
  key = conteudo.substring(virgula+1, conteudo.indexOf("("));
  key_type = conteudo.substring(conteudo.indexOf("(")+1, conteudo.indexOf(")"));


  }



  // ######### Requisições HTTPS #############


    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    //client->setFingerprint(fingerprint);
    // Or, if you happy to ignore the SSL certificate, then use the following line instead:
    client->setInsecure();

    HTTPClient https;

    // GET monitoramento de saldo 

//    Serial.print("[HTTPS] begin...\n");
     if (https.begin(*client, "https://www.asaas.com/api/v3/finance/balance")) {  // HTTPS

       https.addHeader("Content-Type", "application/json");
       https.addHeader("access_token", access_token);
 //      Serial.print("[HTTPS] GET...\n");
//       // start connection and send HTTP header
       int httpCode = https.GET();

//       // httpCode will be negative on error
       if (httpCode > 0) {
//         // HTTP header has been send and Server response header has been handled
//         Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

//         // file found at server
         if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
           String payload = https.getString();

// //          Serial.println(payload);
           payload = payload.substring(11,payload.length()-1);
           float saldo_novo = payload.toFloat();

          //   Serial.print("Saldo novo: ");
          //  Serial.println(saldo_novo); //remove o "{"balance":"
           if(monitor==1){ // roda uma unica vez
             monitor=0;
             saldo_antigo=saldo_novo;
           }
           if(saldo_novo==saldo_antigo){
             recebido=0;
           } else{
             recebido = saldo_novo-saldo_antigo;
           }

           Serial.print("Saldo antigo: ");
           Serial.println(saldo_antigo);

           Serial.print("A receber: ");
           Serial.println(valor_float);

           Serial.print("Recebido: ");
           Serial.println(recebido);

         }
       } else {
         Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
       }

       https.end();
     } else {
       Serial.printf("[HTTPS] Unable to connect\n");
     }

// POST 
if(recebido==valor_float){



    valor = String (valor_float*(1-taxa),2);

//    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, "https://www.asaas.com/api/v3/transfers")) {  // HTTPS

      //http.begin(client, "https://www.asaas.com/api/v3/finance/balance"); //HTTP
      
      https.addHeader("Content-Type", "application/json");
      https.addHeader("access_token", access_token);
//      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      String data = "{\"value\": "+valor+",\"pixAddressKey\": \""+ key+ "\",\"pixAddressKeyType\": \""+key_type+"\",\"description\": \"Compra Honest.io\"}";
      
      //Serial.println(data);

      int httpCode = https.POST(data);

      // httpCode will be negative on error
      if (httpCode > 0 || httpCode == 200) {
        // HTTP header has been send and Server response header has been handled
        //Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
        Serial.println("pago");

        digitalWrite(D1, HIGH); 
        
        //Serial.println("Transferencia de "+valor+" realizada para o vendedor!");
        monitor = 1; //reinicia o monitoramento de pagamento
        monitor2 = 1;

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();

//          Serial.println(payload);
          // payload = payload.substring(11,payload.length()-1);
          // float saldo = payload.toFloat();
          // Serial.println(saldo); //remove o "{"balance":"


        }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  }
  delay(1000);
  digitalWrite(D1, LOW); 
}
