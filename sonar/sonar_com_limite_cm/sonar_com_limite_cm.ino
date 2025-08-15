// Definição dos pinos
const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 8; // Pino do buzzer

// Variáveis para calcular a distância
long duration;
int distance;

void setup() {
  // Inicialização dos pinos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Inicialização da comunicação serial
  Serial.begin(9600);
}

void loop() {
  // Limpa o pino Trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Gera um pulso de 10 microssegundos no pino Trig
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede a duração do pulso no pino Echo
  duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  distance = duration * 0.034 / 2;

  // Exibe a distância no monitor serial
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Verifica se a distância é menor que um limite (ex: 20 cm)
  if (distance < 20) {
    // Emite um som no buzzer
    tone(buzzerPin, 1000); // Frequência de 1000 Hz
    delay(200); // Duração do som
  } else {
    // Desliga o buzzer
    noTone(buzzerPin);
  }

  // Aguarda um curto período antes da próxima leitura
  delay(100);
}