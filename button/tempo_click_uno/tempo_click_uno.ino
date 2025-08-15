const int buttonPin = 2;
const int led = 4;
unsigned long lastPressStartTime = 0; // Tempo em que o botão foi pressionado pela última vez
unsigned long timeBetweenClicksStart = 0;
int buttonState;
int lastButtonState = HIGH;
const long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  Serial.println("Iniciado!"); // Mensagem para indicar que o programa começou
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // Detecta o início de uma pressão
  if (buttonState == LOW && lastButtonState == HIGH && (millis() - lastPressStartTime > debounceDelay)) {
    if (lastPressStartTime != 0) {
      timeBetweenClicksStart = millis() - lastPressStartTime;
      Serial.print("Tempo entre o início dos cliques: ");
      Serial.print(timeBetweenClicksStart);
      Serial.println(" ms");
    }
    lastPressStartTime = millis();

    digitalWrite(led, HIGH);
  }
  digitalWrite(led, LOW);

  lastButtonState = buttonState;
  delay(10);
}