#include <Wire.h>
#include <stdio.h>
#include <string.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "image.h"
#include "games.h"
#include "menu.h"
#include "modules_esp.h"

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
int state_menu = 0;
int state_action = 0;
int index_menu = 0;
int table = 0;


void menu_home(char menus[]){


  if (index_menu > 2){
    table = 3;
  }
  else if(index_menu > 5){
    table = 6;
  }
  else table = 0;

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("--------MENU--------");

  display.print(menu_list[0 + table]);
  (index_menu == 0 + table) ? display.print(" *\n") : display.print("\n");

  display.print(menu_list[1 + table]);

  (index_menu == 1 + table) ? display.print(" *\n") : display.print("\n");
  display.print(menu_list[2 + table]);

  (index_menu == 2 + table) ? display.print(" *\n") : display.print("\n");
  display.display();
}


void setup() {
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(BTN_LEFT, 0x05);
  pinMode(BTN_MIDDLE, 0x05);
  pinMode(BTN_RIGHT, 0x05);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, true)) {
    Serial.println(F("Display não encontrado!"));
    
  }else {
    //logo
    logo(display);
    display.display();
    delay(3000);
  }

  //games
  snakeGame.begin();

}

void loop() {


  if (digitalRead(BTN_LEFT) == 0x0) {
    index_menu--;
    if (index_menu < 0) {
      index_menu = 5; // Volta para o último item
    }
  }

  if (digitalRead(BTN_RIGHT) == 0x0) {
    index_menu++;
    if (index_menu > 5) {
      index_menu = 0; // Volta para o primeiro item
    }
  }
    
  
  if(digitalRead(BTN_MIDDLE) == 0x0){
   
   if(state_menu == 0){
      state_action= 1;
      
    }
   
  }
  //snakeGame.update();
  delay(200);
  
  

  if (state_action > 0){
    switch (index_menu){
      case 2:
          int currentBPM = module_heart();

          // Verifica se a função retornou um valor de BPM válido (não -1)
          if (currentBPM != -1) {
            box_heart(display, currentBPM);
          }else text(display, "conect o modulo do BPM");

          if(digitalRead(BTN_MIDDLE) == 0x0) state_action= 0;
          delay(200);

          break;
    }
  }else menu_home();

  
  //delay(6000);
  

}
