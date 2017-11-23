#include <RestClient.h>
#include <UIPEthernet.h>

#include <Keypad.h>
//#include <ArduinoJson.h>
#include <utility/logging.h>

//#include <Temboo.h>
// #include "TembooAccount.h"

const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 }; // o ultimo 0xED foi trocado para 0x11
// Set the static IP address to use if the DHCP fails to assign
//IPAddress ip(192, 168, 3, 15);   // ip da placa de rede do arduino
//IPAddress ipServer(192, 168, 3, 141); // ip do servidor que vai receber a temperatura
EthernetClient ethclient;


const int pinLedVermelho = 2;
const int pinLedVerde = A1;
const int pinoBuzzer = 9;
const int pinoSensorLuz = A2;

bool digitandoSenha = false;
bool alarmeAtivado = false;
float seno;
int frequencia;

int numeroSenhaTentativas = 0;
String senhaDigitada;
String senha = "123";
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

RestClient client = RestClient("192.168.3.186", 3000, ethclient);
const char* sid = "AC36135c57a2e51019e7c00f107e9408b0";
const char*  token = "db587f4175c3b6360aebf4a11e8699c3";
const char*  to = "5511952021310"; // nao entendi
const char*  from = "17312014022"; // nao entendi

String response = "";

//Setup
void setup() {
  Serial.begin(9600);
  // Connect via DHCP
  if (Ethernet.begin(mac)) {
    Serial.println("Conectado via DHCP");
    Serial.print("IP recebido:"); Serial.println(Ethernet.localIP());
  }

  String parametros = "sid=";
  parametros.concat(sid);

  parametros.concat("&token=");
  parametros.concat(token);

  parametros.concat("&to=");
  parametros.concat(to);

  parametros.concat("&from=");
  parametros.concat(from);

  parametros.concat("&body=Mensagem DO ALEMMMMM");

  Serial.println(parametros);

  int statusCode = client.post("/sms", parametros.c_str(), &response);
  Serial.print("Status da resposta: ");
  Serial.println(statusCode);
  Serial.print("Resposta do servidor: ");
  Serial.println(response);
  delay(1000);
  // */
  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(pinoSensorLuz, INPUT);

  pinMode(pinoBuzzer, OUTPUT); // saida do buzzer alertando entrada de intruso

}

void loop() {
  int leitura = analogRead(pinoSensorLuz);
  if (leitura > 40 && !alarmeAtivado) {
    tocarSirene();
    // enviarSMS caso o alarme esteja ativado;
  } else {
    noTone(pinoBuzzer);
  }

  //Serial.println(leitura);
  delay(100);

  char key = keypad.getKey();

  if (key) {
    if (digitandoSenha && key != '#') {
      senhaDigitada += key;
      Serial.println(senhaDigitada);
    }
    if (key == '*') {
      digitandoSenha = true;
      senhaDigitada = "";
      Serial.println("Iniciando digitação da senha...");

    }
    if (key == '#') {
      digitandoSenha = false;
      Serial.println("Finalizou a  digitação da senha...");
      Serial.println(senhaDigitada);
      if (senhaDigitada == senha) {
        alarmeAtivado = true;
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


