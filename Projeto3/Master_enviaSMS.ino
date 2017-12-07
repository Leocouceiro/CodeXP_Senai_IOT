#include <Wire.h>


#include <ArduinoHttpClient.h>

#include <UIPEthernet.h>
#include <utility/logging.h>
/*
  const int pinLedVermelho = 2;
  const int pinLedVerde = A1;
  const int pinoBuzzer = 9;
  const int pinoSensorLuz = A2;

  bool digitandoSenha = false;
  bool alarmeAtivado = false;
  float seno;
  int frequencia;
*/
// Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // possivel problem

// Alterar o último valor para o id do seu kit
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 };
EthernetClient ethclient;

HttpClient client = HttpClient(ethclient, "192.168.3.186", 3000);

#define SMS_TWILIO_SID "AC36135c57a2e51019e7c00f107e9408b0"
#define SMS_TWILIO_TOKEN "db587f4175c3b6360aebf4a11e8699c3"
#define SMS_PHONE_TO "5511981368780"
#define SMS_PHONE_FROM "17312014022"
#define SMS_MESSAGE "Pepino Ricky"
#define parametros "sid=" SMS_TWILIO_SID "&token=" SMS_TWILIO_TOKEN "&to=" SMS_PHONE_TO "&from=" SMS_PHONE_FROM "&body=" SMS_MESSAGE
#define CONTENT_TYPE "application/x-www-form-urlencoded"


int LED = 2;
//Setup


void setup() {
  Serial.begin(9600);
  // Connect via DHCP
  if (Ethernet.begin(mac)) {
    Serial.println(F("Conectado via DHCP"));
    Serial.print(F("IP recebido:")); Serial.println(Ethernet.localIP());
  }

  /*
    pinMode(pinLedVermelho, OUTPUT);
    pinMode(pinLedVerde, OUTPUT);
    pinMode(A0, INPUT);
    pinMode(pinoSensorLuz, INPUT);
    pinMode(pinoBuzzer, OUTPUT); // saida do buzzer alertando entrada de intruso
  */

  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin();
  // Attach a function to trigger when something is received.
  // Wire.onReceive(receiveEvent);
}

/*void receiveEvent(int bytes) {
  byte x = Wire.read();    // read one character from the I2C
  if (x != 0) {
    Serial.println(F("Recebi o valor de X"));
    Serial.println(x);
  }


  //If value received is 0 blink LED for 200 ms
  if (x == 1) {
    digitalWrite(LED, HIGH);
    enviarSMS();

    // x = 0;

  }
  //If value received is 3 blink LED for 400 ms
  if (x == 3) {
    digitalWrite(LED, LOW);

  }
}*/
void enviarSMS() {
  Serial.println(F(parametros));
  client.post(("/sms"), (CONTENT_TYPE), (parametros));

  int statusCode = client.responseStatusCode();
  Serial.print(("Status da resposta: "));
  Serial.println(statusCode);

  String response = client.responseBody();
  Serial.print(("Resposta do servidor: "));
  Serial.println(response);
}


void loop() {
  Wire.requestFrom(9, 1);    // request 1 byte from slave device #9
byte c = 0;
  while (Wire.available()) { // slave may send less than requested
    c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
 


  //If value received is 0 blink LED for 200 ms
  if (c == 1) {
    digitalWrite(LED, HIGH);
    enviarSMS();

    // x = 0;

  }
  //If value received is 3 blink LED for 400 ms
  if (c == 3) {
    digitalWrite(LED, LOW);

  }
  delay(1000);
}


/*
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
