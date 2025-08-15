#include <IRremote.h>

const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Inicia o receptor IR
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.print("Código IR recebido: ");
    Serial.println(results.value, HEX); // Mostra o código em hexadecimal
    irrecv.resume(); // Prepara para o próximo sinal
  }
}
