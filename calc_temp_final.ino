#define pino 7
int cont = 0, n_amostral = 200;
float vetor_on[200] = {0}, vetor_off[200] = {0};

void setup() {
  pinMode(pino, INPUT); //Define o pino digital 7 como uma entrada de dados
  Serial.begin(9600);   //Inicia a comunicação serial para a depuração
}

void duty_cycle(){
  int i;
  float duty_cycle = 0, tempo_alto = 0, soma_tempo_alto = 0, tempo_baixo = 0, soma_tempo_baixo = 0, temperatura;
  for(i = 0; i < n_amostral; i++){ 
    soma_tempo_alto += vetor_on[i];             //Somatorio dos tempos de onda alta
  }
  tempo_alto = soma_tempo_alto/n_amostral;      //Cálculo da média do tempo baixo
  for(i = 0; i < n_amostral; i++){ 
    soma_tempo_baixo += vetor_off[i];           //Somatório dos tempos de onda baixa
  }
  tempo_baixo = soma_tempo_baixo/n_amostral;    //Cálculo da média do tempo baixo
  duty_cycle = tempo_alto/tempo_baixo;          //Cálculo do duty cycle
  if(duty_cycle < 1.6){
    temperatura = (duty_cycle-1.0182)/0.0119;   //Cálculo de temperatura para a primeira reta
  }
  else{
    temperatura = (duty_cycle+0.45)/0.04;       //Cálculo de temperatura para a segunda reta
  }
  Serial.print("Tempo de onda alta: ");
  Serial.println(tempo_alto, 5);
  Serial.print("Tempo de onda baixa: ");
  Serial.println(tempo_baixo, 5);
  Serial.print("Duty cycle médio medido: ");
  Serial.println(duty_cycle, 5);    
  Serial.print("A temperatura, portanto, é: ");
  Serial.println(temperatura, 5); 
  Serial.print("\n");
}

void loop() {
  int estado_pino = digitalRead(pino);  //Lê o estado do pino (HIGH ou LOW)
  float tempo_on = 0, tempo_off = 0;
  while(estado_pino == HIGH){
    tempo_on += 1;
    estado_pino = digitalRead(pino);
  }
  while(estado_pino == LOW){
    tempo_off += 1;
    estado_pino = digitalRead(pino);
  }
  vetor_on[cont] = tempo_on;
  vetor_off[cont] = tempo_off;
  cont++;
  if(cont == n_amostral){
    duty_cycle();
    cont = 0; //Reinicialização do vetor
    delay(3000);
  }
}
