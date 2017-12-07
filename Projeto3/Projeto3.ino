#include <Wire.h>
#include <UIPEthernet.h>
#include <ArduinoHttpClient.h>

#define pinLedVermelho 2
#define pinLedVerde A1
#define pinoBuzzer 9
#define pinoSensorLuz A2
#define i2c_address 0x1

bool digitandoSenha = false;
bool alarmeAtivado = false;
// float seno;
// int frequencia;

// Alterar o último valor para o id do seu kit
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 };
EthernetClient ethclient;

HttpClient client = HttpClient(ethclient, "192.168.3.186", 3000);

#define LED 5

#define FIXED_IP (IPAddress(192, 168, 3, 111))

void connectEthernet() {
#ifdef FIXED_IP
  Ethernet.begin(mac, FIXED_IP);
  Serial.println(F("IP fixo!"));
#else
  if (Ethernet.begin(mac)) {
    Serial.println(F("Conectado via DHCP"));
    Serial.print(F("IP recebido:")); Serial.println(Ethernet.localIP());
  }
#endif
}

void setup() {
  Wire.begin(i2c_address);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  // Connect via DHCP
 // connectEthernet();

  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(pinoSensorLuz, INPUT);
  pinMode(pinoBuzzer, OUTPUT); // saida do buzzer alertando entrada de intruso


  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  // Start the I2C Bus as Slave on address 9

}

void receiveEvent(int bytes) {
  int x = 1;
  x = Wire.read();    // read one character from the I2C
  connectEthernet();
  if (x != 0) {
    Serial.println(F("Recebi o valor de X"));
    Serial.println(x);
    Serial.flush();
  }

  //If value received is 0 blink LED for 200 ms
  if (x == 1) {
    digitalWrite(LED, HIGH);
    enviarSMS();

  }
  //If value received is 3 blink LED for 400 ms
  if (x == 3) {
    digitalWrite(LED, LOW);
  }
}

#define SMS_TWILIO_SID "AC36135c57a2e51019e7c00f107e9408b0"
#define SMS_TWILIO_TOKEN "db587f4175c3b6360aebf4a11e8699c3"
#define SMS_PHONE_TO "5511981368780"
#define SMS_PHONE_FROM "17312014022"
#define SMS_MESSAGE "Rickyyyyy"
#define CONTENT_TYPE "application/x-www-form-urlencoded"

#define POST_PARAMS ("sid=" SMS_TWILIO_SID "&token=" SMS_TWILIO_TOKEN "&to=" SMS_PHONE_TO "&from=" SMS_PHONE_FROM "&body=" SMS_MESSAGE)
String response;

void enviarSMS() {
  Serial.println(F(POST_PARAMS));
  client.post("/sms", F(CONTENT_TYPE), F(POST_PARAMS));

  int statusCode = client.responseStatusCode();
  Serial.print(F("Status da resposta: "));
  Serial.println(statusCode);

  String response = client.responseBody();
  Serial.print(F("Resposta do servidor: "));
  Serial.println(response);
}


void loop() {

}



//void tocarSirene() {
//  for (int x = 0; x < 180; x++) {
//    //converte graus para radiando e depois obtém o valor do seno
//    seno = (sin(x * 3.1416 / 180));
//    //gera uma frequência a partir do valor do seno
//    frequencia = 2000 + (int(seno * 1000));
//    tone(pinoBuzzer, frequencia);
//    // delay(2);
//  }
//  // Serial.println(key);
//
//}
/*
  void manterHistorico() {
    if (autorizado) {
        Serial.println("Número de entradas na casa: ");
        Serial.println(entrada);
        Serial.println("E número de saídas da casa: ");
        Serial.println(saida);
      } else {

      }

    } else {
      Serial.println("Tentativas falhas e/ou tentativas de assalto: ");
      Serial.println(tentativa);

    }
  }*/
