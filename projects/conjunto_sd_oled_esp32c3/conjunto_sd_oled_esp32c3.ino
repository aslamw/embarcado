#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Define o pino CS do cartão SD
#define SD_CS 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// Define os pinos SDA e SCL
#define SDA_PIN 6
#define SCL_PIN 7

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  // Inicializa a comunicação I2C com os pinos especificados
  Wire.begin(SDA_PIN, SCL_PIN);
}
  

void loop() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Ola Mundo!");
  display.display();


  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  // Inicializa o cartão SD com o pino CS definido
  if (!SD.begin(SD_CS)) {
    display.println("falha no sd");
    while (1);
  }
  display.println("Cartão SD inicializado com sucesso!");
  delay(2000);

  // Exemplo: Criar e escrever em um arquivo no cartão SD
  File arquivo = SD.open("/teste.txt", FILE_WRITE);
  if (arquivo) {
    arquivo.println("Olá, mundo!");
    arquivo.close();
    display.println("Arquivo escrito com sucesso!");
  } else {
    display.println("Erro ao abrir o arquivo!");
  }
  delay(2000);
  
  
  display.display();

}