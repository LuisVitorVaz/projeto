#include <WiFi.h>
#include <FirebaseESP32.h>
#include <HardwareSerial.h> // Biblioteca para a serial de hardware
#include <TinyGPS++.h> // Biblioteca do GPS

// Definições WiFi
const char* ssid = "Camuflado";
const char* password = "@veialoka#rumoao60@";

// Definições Firebase
#define FIREBASE_HOST "bancodedados-a7591-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "09fFbaRrhJkNPoDVwRE3TszPG2m7TeUZKWuoAJUF"
const String FIREBASE_PATH = "data";

// Configuração Firebase
FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

// Configuração do sensor e amostragem
const int sensorPin = 34;  // Pino do sensor
const int n_amostral = 200; // Número de amostras
float vetor_on[n_amostral] = {0}; // Vetor para armazenar tempos de onda alta
float vetor_off[n_amostral] = {0}; // Vetor para armazenar tempos de onda baixa
int cont = 0; // Contador para o vetor

float dutyCycle = 0.0;
float temperature = 0.0;

// Crie uma instância de HardwareSerial para o GPS
HardwareSerial serial1(1); // Utilize UART1 do ESP32
TinyGPSPlus gps1; // Criando o GPS

void setup() {
  Serial.begin(9600);

  // Conectando ao WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado ao WiFi");

  // Configurando Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  Serial.println("Inicializando Firebase...");
  if (Firebase.ready()) {
    Serial.println("Firebase inicializado com sucesso!");
  } else {
    Serial.printf("Erro ao inicializar Firebase: %s\n", firebaseData.errorReason().c_str());
  }

  // Inicializa a serial para o GPS
  serial1.begin(9600, SERIAL_8N1, 18, 19); // RX=18, TX=19

  Serial.println("Aguardando a conexão com os satélites");
}

void loop() {
  // Leitura do GPS
  while (serial1.available() > 0) { // Realiza diversas leituras até conseguir se conectar
    char cIn = serial1.read();
    gps1.encode(cIn);
  }

  // Quando conseguir se conectar aos satélites ele irá realizar as leituras
  if (gps1.location.isUpdated()) {
    Serial.println("----------------------------------------");
    
    // Latitude e Longitude
    double latitude = gps1.location.lat();
    double longitude = gps1.location.lng();

    // Se a latitude for algo válido ela será impressa
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);

    // Se a longitude for algo válido ela será impressa
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);

    // Imprime a idade da informação
    Serial.print("Idade da Informação (ms): ");
    Serial.println(gps1.location.age());

    // Obtém o Dia e Hora no GMT
    int ano = gps1.date.year();
    byte mes = gps1.date.month();
    byte dia = gps1.date.day();
    byte hora = gps1.time.hour();
    byte minuto = gps1.time.minute();
    byte segundo = gps1.time.second();
    byte centesimo = gps1.time.centisecond();
    
    // Imprimindo os dados
    Serial.print("Data (GMT): ");
    Serial.print(dia);
    Serial.print("/");
    Serial.print(mes);
    Serial.print("/");
    Serial.println(ano);

    Serial.print("Horário (GMT): ");
    Serial.print(hora);
    Serial.print(":");
    Serial.print(minuto);
    Serial.print(":");
    Serial.print(segundo);
    Serial.print(":");
    Serial.println(centesimo);

    // Obtendo a altitude
    float altitudeGPS = gps1.altitude.meters();

    // Se a altitude for algo válido e diferente de 1000000Cm ela será impressa
    Serial.print("Altitude (m): ");
    Serial.println(altitudeGPS);

    // Obtém a velocidade de locomoção do GPS em km/h
    float velocidade = gps1.speed.kmph();

    Serial.print("Velocidade (km/h): ");
    Serial.println(velocidade, 2); // Conversão de Nós para Km/h

    // Obtém o sentido do movimento em centésima de graus
    float sentido = gps1.course.deg();

    Serial.print("Sentido (grau): ");
    Serial.println(sentido, 2);

    // Obtém a quantidade de satélites e a precisão
    int satelites = gps1.satellites.value();
    int precisao = gps1.hdop.value();

    Serial.print("Satélites: ");
    Serial.println(satelites);

    Serial.print("Precisão (HDOP): ");
    Serial.println(precisao);

    // Criar JSON para enviar
    FirebaseJson json;
    // json.set("latitude", latitude);
    // json.set("longitude", longitude);
    json.set("data", String(dia) + "/" + String(mes) + "/" + String(ano));
    json.set("hora", String(hora) + ":" + String(minuto) + ":" + String(segundo) + ":" + String(centesimo));
    // json.set("velocidade", velocidade);
    // json.set("sentido", sentido);
    // json.set("satelites", satelites);
    // json.set("precisao", precisao);
    // json.set("dutyCycle", dutyCycle);
    json.set("temperature", temperature);

    Serial.println("Enviando dados ao Firebase...");
    // Enviar dados ao Firebase
    if (Firebase.pushJSON(firebaseData, FIREBASE_PATH, json)) {
      Serial.println("Dados enviados com sucesso");
    } else {
      Serial.printf("Erro ao enviar dados: %s\n", firebaseData.errorReason().c_str());
    }

    delay(3000); // Espera 3 segundos antes de iniciar uma nova amostragem
  }

  // Função comentada
  // int estado_pino = digitalRead(sensorPin);  // Lê o estado do pino (HIGH ou LOW)
  // float tempo_on = 0, tempo_off = 0;

  // // Captura o tempo de onda alta
  // while (estado_pino == HIGH) {
  //   tempo_on += 1;
  //   estado_pino = digitalRead(sensorPin);
  // }

  // // Captura o tempo de onda baixa
  // while (estado_pino == LOW) {
  //   tempo_off += 1;
  //   estado_pino = digitalRead(sensorPin);
  // }

  // vetor_on[cont] = tempo_on;
  // vetor_off[cont] = tempo_off;
  // cont++;

  // if (cont == n_amostral) {
  //   duty_cycle(); // Calcula o duty cycle e a temperatura
  //   cont = 0; // Reinicializa o contador do vetor
  // }
  
  // Função comentada
  // void duty_cycle() {
  //   int i;
  //   float duty_cycle = 0, tempo_alto = 0, soma_tempo_alto = 0;
  //   float tempo_baixo = 0, soma_tempo_baixo = 0;
    
  //   for (i = 0; i < n_amostral; i++) { 
  //     soma_tempo_alto += vetor_on[i]; // Somatório dos tempos de onda alta
  //   }
  //   tempo_alto = soma_tempo_alto / n_amostral; // Cálculo da média do tempo alto
    
  //   for (i = 0; i < n_amostral; i++) { 
  //     soma_tempo_baixo += vetor_off[i]; // Somatório dos tempos de onda baixa
  //   }
  //   tempo_baixo = soma_tempo_baixo / n_amostral; // Cálculo da média do tempo baixo
    
  //   duty_cycle = tempo_alto / tempo_baixo; // Cálculo do duty cycle
    
  //   if (duty_cycle < 1.6) {
  //     temperature = (duty_cycle - 1.0182) / 0.0119; // Cálculo de temperatura para a primeira reta
  //   } else {
  //     temperature = (duty_cycle + 0.45) / 0.04; // Cálculo de temperatura para a segunda reta
  //   }
    
  //   // Exibir resultados no Serial Monitor
  //   Serial.print("Tempo de onda alta: ");
  //   Serial.println(tempo_alto, 5);
  //   Serial.print("Tempo de onda baixa: ");
  //   Serial.println(tempo_baixo, 5);
  //   Serial.print("Duty cycle médio medido: ");
  //   Serial.println(duty_cycle, 5);    
  //   Serial.print("A temperatura, portanto, é: ");
  //   Serial.println(temperature, 5);
  //   Serial.print("\n");
  // }
}
