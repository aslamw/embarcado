#define RELE 13
#define BT 12
bool test = false;
void setup() {
  Serial.begin(115200);
  pinMode(RELE, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

}

void loop() {
  
  int status = digitalRead(BT);

  if (status == LOW){
    if (!test){
      digitalWrite(RELE, HIGH);
      Serial.println("Botão pressionado! Relé LIGADO.");
      test = !test;
    }
    else{
      digitalWrite(RELE, LOW);
      Serial.println("Botão pressionado! Relé desligado.");
      test = !test;
    
    
    }
  }
  delay(200);

}
