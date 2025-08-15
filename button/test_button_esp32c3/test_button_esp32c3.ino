// Defina os pinos
const int buttonPin = 4;  // Botão conectado ao GPIO0
const int ledPin = 8;     // LED interno conectado ao GPIO8

// Variável para armazenar o estado do botão
int buttonState = 0;

void setup() {
  // Configura o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  
  // Configura o pino do botão como entrada
  pinMode(buttonPin, INPUT_PULLUP); // Usa o resistor pull-up interno
}

void loop() {
  // Lê o estado do botão
  buttonState = digitalRead(buttonPin);

  // Verifica se o botão foi pressionado
  if (buttonState == LOW) { // LOW significa que o botão foi pressionado (conectado ao GND)
    // Pisca o LED
    digitalWrite(ledPin, HIGH); // Liga o LED
    delay(500);                 // Espera 500ms
    digitalWrite(ledPin, LOW);  // Desliga o LED
    delay(500);                 // Espera 500ms
  } else {
    // Mantém o LED desligado
    digitalWrite(ledPin, LOW);
  }
}