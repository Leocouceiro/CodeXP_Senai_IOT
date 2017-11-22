#include <Keypad.h>

int pinLedVermelho = 9;
int pinLedVerde = 10;

bool digitandoSenha = false;

String senhaDigitada;
String senha = "123456";
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 4, 3, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    if (digitandoSenha && key != '#') {
      senhaDigitada += key;
      Serial.println(senhaDigitada);
    }
    if (key == '*') {
      digitandoSenha = true;
      Serial.println("Iniciando digitação da senha...");
    }
    if (key == '#') {
      digitandoSenha = false;
      Serial.println("Finalizou a  digitação da senha...");
      Serial.println(senhaDigitada);
      if (senhaDigitada == senha) {
        digitalWrite(pinLedVerde,HIGH);
        digitalWrite(pinLedVermelho, LOW);
        Serial.println("Senha Correta, Alarme Desativado");
        senhaDigitada = "";
      } else {
        Serial.println("Senha Incorreta! Tente novamente!");
        digitalWrite(pinLedVermelho, HIGH);
         digitalWrite(pinLedVerde,LOW);
        senhaDigitada = "";
      }
    }

    // Serial.println(key);
  }


}
