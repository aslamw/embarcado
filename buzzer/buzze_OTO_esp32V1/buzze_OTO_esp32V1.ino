const int buzzerPin = 2; // Pino GPIO conectado ao buzzer

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void beep(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin); // Desliga o som
  delay(50);         // Pequena pausa entre os bips
}

void somR2D2_1() {
  beep(600, 80);
  beep(750, 120);
}

void somR2D2_2() {
  beep(900, 150);
  beep(1100, 100);
  beep(1000, 200);
}

void somR2D2_3() {
  beep(450, 100);
  delay(50);
  beep(550, 100);
  delay(80);
  beep(650, 100);
}

void loop() {
  // Tenta imitar alguns sons do R2-D2 com sequências de bips
  //somR2D2_1();
  //somR2D2_2();
  somR2D2_3();
  delay(500);
}