#include <RestClient.h>

#include <Keypad.h>
#include <ArduinoJson.h>
#include <UIPEthernet.h>
#include <utility/logging.h>

//#include <Temboo.h>
// #include "TembooAccount.h"
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 }; // o ultimo 0xED foi trocado para 0x11
// Set the static IP address to use if the DHCP fails to assign
//IPAddress ip(192, 168, 3, 15);   // ip da placa de rede do arduino
//IPAddress ipServer(192, 168, 3, 140); // ip do servidor que vai receber a temperatura
//EthernetClient client;


int pinLedVermelho = 2;
int pinLedVerde = A1;
int pinoBuzzer = 9;

bool digitandoSenha = false;

float seno;
int frequencia;

int numeroSenhaTentativas = 0;
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
(731) 201-4022
Your new Phone Number is +17312014022
byte rowPins[ROWS] = {5, 4, 3, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // start the Ethernet connection:
  /*
    if (Ethernet.begin(mac) == 0) {
     Serial.println("Failed to configure Ethernet using DHCP");
     // try to congifure using IP address instead of DHCP:
     Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);
    Serial.println("connecting...");
    if (client.connect(ipServer, 8080)) {
     Serial.println("connected");
     // Make a HTTP request:

     client.println("POST HTTP/1.1");
     client.println("Host: 192.168.3.140");
     client.println("Content-Type: application/json");
     client.println("Content-Length: 22");
     client.println();

     client.println("{\"temp\":37,\"id\":11}");
     client.flush();
     client.println("Connection: close");
     //
    } else {
     // if you didn't get a connection to the server:
     Serial.println("connection failed");
    }
  */
  // if you get a connection, report back via serial:

  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(A0, INPUT);

  pinMode(pinoBuzzer, OUTPUT); // saida do buzzer alertando entrada de intruso

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
      Serial.printn(senhaDigitada);
      if (senhaDigitada == senha) {
        digitalWrite(pinLedVerde, HIGH);
        digitalWrite(pinLedVermelho, LOW);
        Serial.println("Senha Correta, Alarme Desativado");
        noTone(pinoBuzzer);
        numeroSenhaTentativas = 0;
        senhaDigitada = "";
      } else {
        numeroSenhaTentativas = numeroSenhaTentativas + 1;
        Serial.println("Senha Incorreta! Tente novamente!");
        digitalWrite(pinLedVermelho, HIGH);
        digitalWrite(pinLedVerde, LOW);
        senhaDigitada = "";
      }
    }
  }
  
  if (numeroSenhaTentativas >= 3) {
    tocarSirene();
  }
  /*
    if (!client.connected()) {
     Serial.println();
     Serial.println("disconnecting.");
     client.stop();
     // client.flush;
     // do nothing forevermore:
     while (true);

    }
  */
}



void tocarSirene() {
   for (int x = 0; x < 180; x++) {
      //converte graus para radiando e depois obtém o valor do seno
      seno = (sin(x * 3.1416 / 180));
      //gera uma frequência a partir do valor do seno
      frequencia = 2000 + (int(seno * 1000));
      tone(pinoBuzzer, frequencia);
     // delay(2);
    }
    // Serial.println(key);
}


