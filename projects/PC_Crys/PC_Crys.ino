#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "image.h"
#include "games.h"

//tela
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

#define SDA_PIN 6
#define SCL_PIN 7

//BOTÃO
#define BTN_LEFT 3
#define BTN_MIDDLE 1
#define BTN_RIGHT 0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//games
SnakeGame snakeGame(BTN_LEFT, BTN_RIGHT, BTN_MIDDLE);

/*void menu(char list_menu[]){
  for(int i = 0; i < )
}*/

void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_MIDDLE, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Display não encontrado!"));
    while(1);
  }

  //games
  snakeGame.begin();

  //logo
  logo(display);
  display.display();

  delay(2000);

}

void loop() {

  snakeGame.update();
  delay(10);
  

}
