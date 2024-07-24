#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial serial1(4, 3); // RX, TX
TinyGPS gps;

#define rxPin 8
#define txPin 9
SoftwareSerial sim800(txPin, rxPin);

String dado1, dado2;
int ano;
byte mes, dia, hora, minuto, segundo, centesimo;
bool interruptFlag = false;
void enviarsms();
const int interruptPin = 2; // Define o pino para a interrupção externa
int cont =0;
bool readingGPS = true; // Variável para indicar se estamos lendo dados do GPS ou não
unsigned long delayDuration = 120000;
const char FIREBASE_HOST[] = "bancodedados-a7591-default-rtdb.firebaseio.com";
const String FIREBASE_AUTH = "09fFbaRrhJkNPoDVwRE3TszPG2m7TeUZKWuoAJUF";
const String FIREBASE_PATH = "dados";
const int SSL_PORT = 443;
String testebotao="";

// char apn[] = "zap.vivo.com.br";
char apn[] = "java.claro.com.br";
char user[] = "";
char pass[] = "";

TinyGsm modem(sim800);
TinyGsmClientSecure gsm_client_secure_modem(modem, 0);
HttpClient http_client = HttpClient(gsm_client_secure_modem, FIREBASE_HOST, SSL_PORT);

unsigned long previousMillis = 0;
const long interval = 60000;  // Intervalo de 2 min em segundos (em milissegundos)

void setup() {
  pinMode(interruptPin, INPUT); // Define o pino como entrada
  attachInterrupt(digitalPinToInterrupt(interruptPin), enviarsms, FALLING); // Configura a interrupção para ocorrer na borda de descida (FALLING)
  serial1.begin(9600);
  Serial.begin(9600);
  sim800.begin(9600);
 
  Serial.println(F("Inicializando..."));

  Serial.println(F("Inicializando módulo SIM800L..."));

  Serial.println(F("Inicializando modem..."));
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print(F("Modem: "));
  Serial.println(modemInfo);

  http_client.setHttpResponseTimeout(10 * 1000); // 10 segundos de timeout para resposta HTTP
}

void loop() {
  if (interruptFlag) {
    // Faça o que precisa ser feito após a interrupção, como enviar um SMS
    enviarsms();
  }

  if (readingGPS) {
    lergps();
  }
}


void PostToFirebase(const char *method, const String &path, const String &data, HttpClient *http) {
  String response;
  int statusCode = 0;
  http->connectionKeepAlive();
  String url;
  if (path[0] != '/') {
    url = "/";
  }
  url += path + ".json";
  url += "?auth=" + FIREBASE_AUTH;
  Serial.print("POST:");
  Serial.println(url);
  Serial.print("Data:");
  Serial.println(data);

  String contentType = "application/json";
  http->post(url, contentType, data); // Usando POST para adicionar um novo registro

  statusCode = http->responseStatusCode();
  Serial.print(F("Código de status: "));
  Serial.println(statusCode);
  response = http->responseBody();
  Serial.print(F("Resposta: "));
  Serial.println(response);

  if (!http->connected()) {
    Serial.println();
    http->stop();
    Serial.println(F("HTTP POST desconectado"));
  }
}

void gps_loop()
{
  Serial.println(dado1);//lat
  Serial.println(dado2);//long
  Serial.println(testebotao); // dados se o botao foi pressionado ou nao
  
String Data = "{";
Data += "\"dado1\":\"" + dado1 + "\",";
Data += "\"dado2\":\"" + dado2 + "\",";
Data += "\"botaoState\":\"" + testebotao + "\""; // Assuming testebotao is already a string or convertible to a string
Data += "\"data\":\"" + String(dia) + "/" + String(mes) + "/" + String(ano) + "\",";
Data += "\"hora\":\"" + String(hora) + ":" + String(minuto) + ":" + String(segundo) + ":" + String(centesimo) + "\"";
Data += "}";

  PostToFirebase("PATCH", FIREBASE_PATH, Data, &http_client);
  delay(60000); // Aguarda 1 minuto antes de obter novos dados do GPS
  Serial.println("apos o gps loop");//long
  serial1.listen();
}

void lergps(){
  serial1.listen();
  bool gpsDataReceived = false;

  while (serial1.available()) {
    char cIn = serial1.read(); // guarda em cIn o dado recebido no pino RX
    gpsDataReceived = gps.encode(cIn);
  }
  Serial.println("----------------------------------------");

  long latitude;
  long longitude;
  unsigned long idadeInfo;

  gps.get_position(&latitude, &longitude, &idadeInfo);

  if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
    Serial.print("Latitude: ");
    Serial.println(float(latitude) / 100000, 6);
  }

  if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
    Serial.print("Longitude: ");
    Serial.println(float(longitude) / 100000, 6);
  }
  if (idadeInfo != TinyGPS::GPS_INVALID_AGE) {
        Serial.print("Idade da Informacao (ms): ");
        Serial.println(idadeInfo);
    }
     gps.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo);

     Serial.print("Data (GMT): ");
     Serial.print(dia);
     Serial.print("/");
     Serial.print(mes);
     Serial.print("/");
     Serial.println(ano);

     Serial.print("Horario (GMT): ");
     Serial.print(hora);
     Serial.print(":");
     Serial.print(minuto);
     Serial.print(":");
     Serial.print(segundo);
     Serial.print(":");
     Serial.println(centesimo);

  dado1 = String(latitude);
  dado2 = String(longitude);

  Serial.println(dado1);
  Serial.println(dado2);
  

  delay(250);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Muda para ouvir a porta do módulo GSM
     sim800.listen();

    Serial.print(F("Conectando a "));
    Serial.print(apn);
    
    if (!modem.gprsConnect(apn, user, pass)) {
      Serial.println(F(" falha"));
      delay(1000); // Adiciona atraso antes de tentar novamente
      return;
    }
    Serial.println(F(" OK"));

    http_client.connect(FIREBASE_HOST, SSL_PORT);

    while (true) {
      if (!http_client.connected()) {
        Serial.println();
        http_client.stop();
        Serial.println(F("HTTP não conectado"));
        break;
      }
      else
      {
          gps_loop();
     
    }

    // Volta para ouvir a porta do módulo GPS
    Serial.println("esta aqui");//long
    // serial1.listen();

    previousMillis = currentMillis;
  }
}
}
void enviarsms(){
     testebotao=cont++;
}
