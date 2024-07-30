#include <WiFi.h>
#include <FirebaseESP32.h>

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

void setup() {
  Serial.begin(115200);

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
}

void loop() {
  Serial.println("esta no loop");
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
Serial.println("cria json");
    // Criar JSON para enviar
    FirebaseJson json;
    json.set("dutyCycle", dutyCycle);
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

//   delay(5000);  // Esperar 5 segundos antes de enviar novamente
// }

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
