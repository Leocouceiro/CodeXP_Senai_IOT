#include <Wire.h>


#include <RestClient.h>
#include <UIPEthernet.h>
#include <utility/logging.h>

const int pinLedVermelho = 2;
const int pinLedVerde = A1;
const int pinoBuzzer = 9;
const int pinoSensorLuz = A2;

bool digitandoSenha = false;
bool alarmeAtivado = false;
float seno;
int frequencia;

// Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // possivel problem

// Alterar o último valor para o id do seu kit
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 };
EthernetClient ethclient;

RestClient client = RestClient("192.168.3.186", 3000, ethclient);


int LED = 5;
int x = 0; // varial usada para receber do outro arduino
//Setup


void setup() {
  Serial.begin(9600);
  // Connect via DHCP
  if (Ethernet.begin(mac)) {
    Serial.println("Conectado via DHCP");
    Serial.print("IP recebido:"); Serial.println(Ethernet.localIP());
  }


  pinMode(pinLedVermelho, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(pinoSensorLuz, INPUT);
  pinMode(pinoBuzzer, OUTPUT); // saida do buzzer alertando entrada de intruso


  // Define the LED pin as Output
  pinMode (LED, OUTPUT);
  // Start the I2C Bus as Slave on address 9
  Wire.begin(9);
  // Attach a function to trigger when something is received.
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  if (x != 0) {
    Serial.println("Recebi o valor de X");
    Serial.println(x);
  }

  x = Wire.read();    // read one character from the I2C

  //If value received is 0 blink LED for 200 ms
  if (x == 1) {
    digitalWrite(LED, HIGH);
    enviarSMS();

    x = 0;

  }
  //If value received is 3 blink LED for 400 ms
  if (x == 3) {
    digitalWrite(LED, LOW);

  }
}
void enviarSMS() {
  Serial.println("enviando SMS");
  return;
  const char* parametros = "sid=AC36135c57a2e51019e7c00f107e9408b0&token=db587f4175c3b6360aebf4a11e8699c3&to=5511981368780&from=17312014022&body=Rickyyy";
  String response = "";
  int statusCode = client.post("/sms", parametros, &response);
  Serial.print("Status da resposta: ");
  Serial.println(statusCode);
  Serial.print("Resposta do servidor: ");
  Serial.println(response);
}


void loop() {

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
