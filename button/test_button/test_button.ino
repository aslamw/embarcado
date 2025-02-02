#define BUTTON_PIN 13

int button = HIGH;

void setup() {

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.begin(9600);


}

void loop() {
  button = digitalRead(BUTTON_PIN);

  if (button == LOW){
    Serial.println("apertado");
    delay(300);
  }

}
