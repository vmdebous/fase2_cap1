#define LDR_PIN 34
#define RELE 16
#include "DHT.h"

#define DHTPIN 21
#define DHTTYPE DHT22
bool predicao = false;
bool first = true;
int botoes[] = {23, 4, 5};
char nutrientes[] = {'N','P','K'};
DHT dht(DHTPIN, DHTTYPE);
#include <stdlib.h>





void setup() {
  Serial.begin(115200);
  pinMode(RELE, OUTPUT);
  dht.begin();
  for (int i = 0; i < 3; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
  }
}

void loop() {
  // valor lido é de 1001, definindo a leitura para 7
  int valor = analogRead(LDR_PIN);
  valor = valor - 994;

if (!predicao){


  if (first){

    
      // ligando relê para aumentar a umidade
      Serial.println("Não há probabilidade de chuva/tempestade nas próximas 12 horas");
      float temp = dht.readTemperature();
      float hum = dht.readHumidity();

      if (isnan(temp) || isnan(hum)) {
        Serial.println("Erro ao ler o DHT22");
        return;
      }
      if (hum <= 40){
        Serial.print("Umidade em ");
        Serial.print(hum);
        Serial.println(" %");
        Serial.println("Acionando relê para alimentação da bomba d'água...");
        delay(5000);
        digitalWrite(RELE, HIGH);
        Serial.println("Relê acionado");
        delay(5000);
        Serial.println("Sistema de irrigação ativado por 5 segundos para aumentar a umidade...");
        hum = dht.readHumidity() + 10.0;

      }
      if (hum > 40){
        // desliga relê
        Serial.print("Umidade em ");
        Serial.print(hum);
        Serial.println(" %");
        Serial.println("Desligando relê para interromper a o sistema de irrigação...");
        digitalWrite(RELE, LOW);
        Serial.println("Relê desligado");
        Serial.println("Sistema de irrigação desativado");
      }
      first = false;
  }
}
else{
    Serial.println("Há probabilidade de chuva/tempestade nas próximas 12 horas");
    Serial.println("Sistema de irrigação não sera ativado");
    first = false;
}


  for (int i = 0; i < 3; i++) {
    if (digitalRead(botoes[i]) == LOW) {
      Serial.print("Botão ");
      Serial.print(nutrientes[i]);
      Serial.println(" pressionado");
      if (nutrientes[i] == 'N') {
        Serial.println("Valor do pH: ");
        Serial.println(valor);
        Serial.println("Adicionando Nitrogênio...");
        valor = valor - 2;
        if (valor < 0) {
          valor = 0;
        }
        Serial.println("Adicionado Nitrogênio...");

        Serial.println("Valor do pH: ");
        Serial.println(valor);
        delay(1500);
      } else if (nutrientes[i] == 'P') {
        Serial.println("Adicionando Fósforo...");
        // não tem efeito no pH
      } else if (nutrientes[i] == 'K') {
        Serial.println("Adicionando Potássio...");
        // não tem efeito no pH
      }

    }
  }
}




