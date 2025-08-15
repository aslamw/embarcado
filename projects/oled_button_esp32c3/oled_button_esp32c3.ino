#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdbool.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

//#define BUTTON_PIN_1 2
//#define BUTTON_PIN_2 3
//#define BUTTON_PIN_3 4

#define SDA_PIN 6
#define SCL_PIN 7

#define led_pin 8

const int BUTTON_PIN_1 = 2;
const int BUTTON_PIN_2 = 3;
const int BUTTON_PIN_3 = 4;

int button1 = 0;
int button2 = 0;
int button3 = 0;

int jogador1_index = 0, jogador2_index = 0;
int vez = 1;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void desenho(int resul){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(25,10);
  display.print(resul);
  display.display();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(74880);
  
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(led_pin, OUTPUT);
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(25,10);
  display.print("Jogo da Velha");
  display.display();
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  button1 = digitalRead(BUTTON_PIN_1);
  button2 = digitalRead(BUTTON_PIN_2);
  button3 = digitalRead(BUTTON_PIN_3);

  if (button1 == LOW){
    desenho(1);
    delay(500);
  }
  if (button2 == LOW){
    Serial.println("2");
    desenho(2);
    delay(500);
  }

  if (button3 == LOW){
    desenho(3);
    delay(500);
  }


}
