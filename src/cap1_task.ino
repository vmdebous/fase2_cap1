#define LDR_PIN 34
#define RELE 16
#include "DHT.h"

#define DHTPIN 21
#define DHTTYPE DHT22
bool first = true;

int botoes[] = {23, 4, 5};
char nutrientes[] = {'N','P','K'};
DHT dht(DHTPIN, DHTTYPE);
#include <stdio.h>
#include <stdlib.h>


//FILE *file = fopen("clima.txt", "r");


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

  if (first){



    // char ch;
    // while ((ch = fgetc(file)) != EOF) {
    //   putchar(ch);
    // }

    // fclose(file);
    // Serial.println(ch);
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("Erro ao ler o DHT22");
      return;
    }

    if (hum <= 40){
      // ligando relê para aumentar a umidade
      Serial.print("Umidade em ");
      Serial.print(hum - 20);
      Serial.print(" %");
      Serial.print("\n");
      Serial.print("Acionando relê para alimentação da bomba d'água...\n");




      delay(5000);
      digitalWrite(RELE, HIGH);
      Serial.print("Relê acionado\n");
    }

    delay(5000);
    hum = dht.readHumidity() + 10.0;

    if (hum > 40){
      // desliga relê
      Serial.print("Umidade em ");
      Serial.print(hum);
      Serial.print(" %");
      Serial.print("\n");
      Serial.print("Desligando relê para alimentação da bomba d'água...\n");


      digitalWrite(RELE, LOW);
      Serial.print("Relê desligado\n");
    first = false;
  }
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
      } else if (nutrientes[i] == 'K') {
        Serial.println("Adicionando Potássio...");
      }

    }
  }
}




