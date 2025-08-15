// Pinos que serão utilizados
const int botaoPin = 4;     // Pino do botão
const int pinoAlvo = 16;    // Pino alvo que será reconfigurado

// Variável para armazenar o estado atual do pino alvo (0 = INPUT, 1 = OUTPUT, 2 = PULLUP)
int estadoPino = 0;

void setup() {
  // Inicia a comunicação serial para debug
  Serial.begin(115200);
  Serial.println("Programa iniciado.");

  // Configura o pino do botão como entrada e ativa o resistor de pull-up interno
  pinMode(botaoPin, INPUT_PULLUP);

  // Configura o pino alvo inicialmente como INPUT
  pinMode(pinoAlvo, INPUT);
  Serial.println("Pino 16 configurado como INPUT.");
}

void loop() {
  // Lê o estado do botão. O INPUT_PULLUP faz com que o estado seja LOW (0) quando pressionado.
  if (digitalRead(botaoPin) == LOW) {
    // Adiciona um pequeno delay para debounce (anti-ruído)
    delay(50);

    // Verifica novamente o estado para garantir que não foi um ruído
    if (digitalRead(botaoPin) == LOW) {

      // Incrementa o estado para o próximo modo
      estadoPino++;

      // Reseta para o início se o estado for maior que 2
      if (estadoPino > 2) {
        estadoPino = 0;
      }

      // Configura o pino 16 com base no novo estado
      switch (estadoPino) {
        case 0:
          pinMode(pinoAlvo, INPUT);
          Serial.println("Pino 16 configurado como INPUT.");
          break;
        case 1:
          pinMode(pinoAlvo, OUTPUT);
          Serial.println("Pino 16 configurado como OUTPUT.");
          break;
        case 2:
          pinMode(pinoAlvo, INPUT_PULLUP);
          Serial.println("Pino 16 configurado como INPUT_PULLUP.");
          break;
      }

      // Espera até que o botão seja solto para evitar múltiplas execuções
      while (digitalRead(botaoPin) == LOW);
    }
  }
}