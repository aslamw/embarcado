#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PulseSensorPlayground.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pulse Sensor
const int PulseWire = A0;
PulseSensorPlayground pulseSensor;

int BPM;

void setup() {
  Serial.begin(9600);

  // Inicializa OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Iniciando...");
  display.display();

  // Inicializa Pulse Sensor
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(550); // Ajuste se necessário
  if (!pulseSensor.begin()) {
    Serial.println("Erro ao iniciar Pulse Sensor");
    while(true);
  }
}

void loop() {
  BPM = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(BPM);

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Batimento Cardiaco");

    display.setTextSize(2);
    display.setCursor(0,16);
    display.print("BPM: ");
    display.print(BPM);
    display.display();
  }

  delay(20);
}
