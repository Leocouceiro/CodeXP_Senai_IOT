// CONTROLE DE ACESSO RFID COM TRAVA ELÉTRICA
// USINAINFO < https://www.usinainfo.com.br >
// FACEBOOK  < https://www.facebook.com/usinainfo.arduino >
// YOUTUBE   < https://www.youtube.com/usinainfo >
// INSTAGRAM < @usinainfo >


byte x = 0;
#include <SPI.h>                 // Inclui a biblioteca SPI.h
#include <RFID.h>                // Inclui a biblioteca RFID.h
#include <Wire.h>                // Inclui a biblioteca Wire.h
#include <LiquidCrystal.h>       // Inclui a biblioteca LiquidCrystal.h

#define SS_PIN 10                       // Define os pinos do RFID
#define RST_PIN 9                       // Define os pinos do RFID

RFID rfid(SS_PIN, RST_PIN);             //Iniciliza as configurações da biblioteca RFID

LiquidCrystal lcd(6, 7, 5, 4, 3, 2);    // Declara os pinos do display

int serNum[5];                          // Variável de leitura da tag

int cards[][5] = {                      // Declara os códigos liberados para acesso
  {163, 73, 60, 91, 141}            // Tag de Acesso 1
};

int alarm = 0;
uint8_t alarmStat = 0;
uint8_t maxError = 5;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  SPI.begin();
  rfid.init();

  lcd.clear();
  lcd.print(F(" Controle RFID"));
  lcd.setCursor (0, 1);
  lcd.print(F("   de Acesso"));
  delay (3000);
  lcd.clear();

  // Start the I2C Bus as Master
  Wire.begin(9);

}

void debugNumeroCard() {
  Serial.print(rfid.serNum[0]);
  Serial.print(" ");
  Serial.print(rfid.serNum[1]);
  Serial.print(" ");
  Serial.print(rfid.serNum[2]);
  Serial.print(" ");
  Serial.print(rfid.serNum[3]);
  Serial.print(" ");
  Serial.print(rfid.serNum[4]);
  Serial.println("");
}

bool verificarCartaoValido() {
  for (int j = 0; j < sizeof(cards); j++) {
    if (verificarCartao(j)) {
      return true;
    }
  }
  return false;
}

bool verificarCartao(int idCartao) {
  for (int i = 0; i < sizeof(rfid.serNum); i++ ) {
    if (rfid.serNum[i] != cards[idCartao][i]) {
      return false;
    }
  }
  return true;
}


void loop() {
  lcd.clear();
  lcd.print(F("Acesso LeoInfo"));
  lcd.setCursor (0, 1);
  lcd.print(F(" Aguardando COD"));

  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      //        debugNumeroCard();
      if (verificarCartaoValido()) {
        Serial.println(F("Bem-Vindo(a)!"));
        lcd.clear();
        lcd.print(F("  Bem-Vindo(a)  "));
        lcd.setCursor (0, 1);
        lcd.print(F("                "));
        x = 1;
        Wire.onRequest(requestEvent);
        //transmitir(1);
        lcd.clear();
      } else {
        alarm = alarm + 1;
        Serial.println("Acesso Negado!");
        x = 3;
        Wire.onRequest(requestEvent);

        //        transmitir(3);
        lcd.clear();
      }
//      Wire.onRequest(requestEvent);
    }
  }

  
}

void requestEvent() {
  Wire.write(x); // respond with message of 6 bytes
  // as expected by master
}

void transmitir(byte x) {

  //Wire.write("hello ");
  //  Wire.beginTransmission(9); // transmit to device #9
  //  Wire.write(x);              // sends x
  //  Wire.endTransmission();    // stop transmitting
}

