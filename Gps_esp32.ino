#include <HardwareSerial.h> // Biblioteca para a serial de hardware
#include <TinyGPS++.h> // Biblioteca do GPS

// Crie uma instância de HardwareSerial para o GPS
HardwareSerial serial1(1); // Utilize UART1 do ESP32

TinyGPSPlus gps1; // Criando o GPS

void setup() {
  serial1.begin(9600, SERIAL_8N1, 18, 19); // Inicializa a serial com 9600 baud, RX=18, TX=19
  Serial.begin(9600); // Velocidade do serial para monitoramento

  Serial.println("Aguardando a conexão com os satélites");
}

void loop() {
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

    // Obtém o Dia e Hora no GMT, ou seja, há 3 horas de diferença com o horário do Brasil
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
    delay(5000);
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
  }
}
