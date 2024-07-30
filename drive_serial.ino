#define BUFFER_SIZE 1000

char buffer[BUFFER_SIZE];
int bufferIndex = 0;

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  Serial.println("Digite uma data no formato DD/MM/AAAA:");
}

void loop() {
  // Verifica se há dados disponíveis na serial
  while (Serial.available()) {
    char c = Serial.read();
    
    // Adiciona o caractere ao buffer se não for um caractere de nova linha
    if (c != '\n') {
      buffer[bufferIndex] = c;
      bufferIndex = (bufferIndex + 1) % BUFFER_SIZE; // Garante que o buffer não ultrapasse o tamanho
    }
    
    // Se o caractere de nova linha for recebido, processa o comando
    if (c == '\n') {
      buffer[bufferIndex] = '\0'; // Finaliza a string com o caractere nulo
      processBuffer();
      bufferIndex = 0; // Reseta o índice do buffer
      Serial.println("Digite uma nova data:");
    }
  }
}

void processBuffer() {
  String input = String(buffer); // Converte o buffer para String
  input.trim(); // Remove espaços em branco no início e no final

  // Exemplo de formato esperado: DD/MM/AAAA
  if (input.length() == 10 && input[2] == '/' && input[5] == '/') {
    String day = input.substring(0, 2);
    String month = input.substring(3, 5);
    String year = input.substring(6, 10);

    // Aqui você pode implementar a lógica para buscar e mostrar as datas no buffer
    Serial.print("Data recebida: ");
    Serial.print(day);
    Serial.print("/");
    Serial.print(month);
    Serial.print("/");
    Serial.println(year);

    // Aqui você deve implementar a varredura no buffer para encontrar as datas
    // Exemplo simples de como você poderia procurar por datas (em uma aplicação real, você precisaria de uma abordagem mais robusta)
    Serial.println("Buscando datas correspondentes:");
    for (int i = 0; i < BUFFER_SIZE; i++) {
      // Verifica se a string na posição atual do buffer é uma data no formato correto
      if (i + 10 <= BUFFER_SIZE && buffer[i + 2] == '/' && buffer[i + 5] == '/') {
        String foundDate = String(buffer + i);
        if (foundDate.substring(0, 10) == input) {
          Serial.println("Data encontrada no buffer: " + foundDate);
        }
      }
    }
  } else {
    Serial.println("Formato de data inválido.");
  }
}
