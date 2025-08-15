#define PIN1 12
#define PIN2 13
#define PIN3 14
#define PIN4 27

void setup() {
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN4, OUTPUT); 

}

void loop() {
  moverServo( 2, 180);
  delay(1000);
  moverServo( 2, 0);
  delay(1000);

  moverServo( 3, -180);
  delay(1000);
  moverServo( 3, 0);
  delay(1000);
}

void moverServo(int pin, int angulo) {
  int pulso = map(angulo, 0, 180, 500, 2500); // Converte ângulo para tempo do pulso (microsegundos)

  switch(pin){
    case 1:
      for (int i = 0; i < 50; i++) {  // Envia 50 pulsos para garantir
        digitalWrite(PIN1, HIGH);
        delayMicroseconds(pulso);
        digitalWrite(PIN1, LOW);
        delayMicroseconds(20000 - pulso); // Completa os 20 ms do ciclo
      }
      break;
    case 2:
      for (int i = 0; i < 50; i++) {  // Envia 50 pulsos para garantir
        digitalWrite(PIN2, HIGH);
        delayMicroseconds(pulso);
        digitalWrite(PIN2, LOW);
        delayMicroseconds(20000 - pulso); // Completa os 20 ms do ciclo
      }
      break;
    case 3:
      for (int i = 0; i < 50; i++) {  // Envia 50 pulsos para garantir
        digitalWrite(PIN3, HIGH);
        delayMicroseconds(pulso);
        digitalWrite(PIN3, LOW);
        delayMicroseconds(20000 - pulso); // Completa os 20 ms do ciclo
      }
      break;
    case 4:
      for (int i = 0; i < 50; i++) {  // Envia 50 pulsos para garantir
        digitalWrite(PIN4, HIGH);
        delayMicroseconds(pulso);
        digitalWrite(PIN4, LOW);
        delayMicroseconds(20000 - pulso); // Completa os 20 ms do ciclo
      }
      break;
  }
}
