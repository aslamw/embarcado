#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

// Define os pinos SDA e SCL
#define SDA_PIN 6
#define SCL_PIN 7

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pinos dos botões
#define BTN_LEFT 3
#define BTN_MIDDLE 1
#define BTN_RIGHT 0

void setup() {
  Serial.begin(115200);
  
  // Inicializa a comunicação I2C com os pinos especificados
  Wire.begin(SDA_PIN, SCL_PIN); 
  
  // Configurar botões
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_MIDDLE, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  
  // Inicializar display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(1000);
  
  // Tela de boas-vindas
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(10, 0);
  display.println("DIAGNOSTICO");
  display.setCursor(5, 10);
  display.println("BOTOES ESP32-C3");
  
  display.setCursor(0, 20);
  display.println("GPIO0,1,2");
  
  display.display();
  delay(2000);
  
  Serial.println("Diagnóstico iniciado. Pressione os botões...");
}

void loop() {
  // Inicializa a comunicação I2C com os pinos especificados
  Wire.begin(SDA_PIN, SCL_PIN);

  int left = digitalRead(BTN_LEFT);
  int middle = digitalRead(BTN_MIDDLE);
  int right = digitalRead(BTN_RIGHT);
  
  // Serial Monitor
  Serial.printf("L:%d M:%d R:%d", left, middle, right);
  if (left == LOW) Serial.print(" [LEFT]");
  if (middle == LOW) Serial.print(" [MIDDLE]");
  if (right == LOW) Serial.print(" [RIGHT]");
  Serial.println();
  
  // Display OLED
  display.clearDisplay();
  display.setTextSize(1);
  
  // Cabeçalho
  display.setCursor(0, 0);
  display.println("ESTADO DOS BOTOES:");
  display.drawLine(0, 9, 128, 9, SSD1306_WHITE);
  
  // Botão Left (GPIO2)
  display.setCursor(0, 12);
  display.print("GPIO2: ");
  if (left == 0x0) {
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("PRESSIONADO");
    display.setTextColor(SSD1306_WHITE);
  } else {
    display.println("LIVRE     ");
  }
  
  
  // Botão Middle (GPIO1)
  display.setCursor(0, 20);
  display.print("GPIO1: ");
  if (middle == 0x0) {
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("PRESSIONADO");
    display.setTextColor(SSD1306_WHITE);
  } else {
    display.println("LIVRE     ");
  }
  
  // Botão Right (GPIO0)
  display.setCursor(0, 28);
  display.print("GPIO0: ");
  if (right == LOW) {
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    display.println("PRESSIONADO");
    display.setTextColor(SSD1306_WHITE);
  } else {
    display.println("LIVRE     ");
  }
  
  // Indicador de atividade
  static int counter = 0;
  display.setCursor(115, 0);
  display.print(counter % 2 == 0 ? "*" : " ");
  counter++;
  
  display.display();
  delay(250);
}