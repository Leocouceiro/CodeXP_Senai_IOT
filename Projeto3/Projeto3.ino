#include <RestClient.h>
#include <UIPEthernet.h>
#include <Keypad.h>
#include <utility/logging.h>

const int pinLedVermelho = 2;
const int pinLedVerde = A1;
const int pinoBuzzer = 9;
const int pinoSensorLuz = A2;

bool digitandoSenha = false;
bool alarmeAtivado = false;
float seno;
int frequencia;

int numeroSenhaTentativas = 0;
String senhaDigitada = "000";
const char* senha = "123";
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns


byte rowPins[ROWS] = {5, 4, 3, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // possivel problem

// Alterar o último valor para o id do seu kit
const byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 };
EthernetClient ethclient;

RestClient client = RestClient("192.168.3.186", 3000, ethclient);
const char* parametros = "sid=AC36135c57a2e51019e7c00f107e9408b0&token=db587f4175c3b6360aebf4a11e8699c3&to=5511981368780&from=17312014022&body=Rickyyy";

String response = "";

/*bool autorizado;
  int entradaSaida;
  int entrada;
  int saida;
  int tentativa;*/

//Setup
void setup() {
  Serial.begin(9600);
  // Connect via DHCP
  if (Ethernet.begin(mac)) {
    Serial.println("Conectado via DHCP");
    Serial.print("IP recebido:"); Serial.println(Ethernet.localIP());
  }

  int statusCode = client.post("/sms", parametros, &response);
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
        /*autorizado = true;
          entradaSaida += 1;
          manterHistorico();
          if (entradaSaida % 2 == 0 && entradaSaida != 0) {
          saida += 1;
          } else {
          entrada += 1;
          }*/

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
    //autorizado = false;
    //tentativa += 1;
    //manterHistorico();
  }

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
