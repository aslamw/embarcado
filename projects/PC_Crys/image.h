#ifndef IMAGE_H
#define IMAGE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void logo(Adafruit_SSD1306& display){
  display.clearDisplay();
  //id: 0 triangle 1 
  display.drawTriangle(58, 1, 93, 15, 59, 18, SSD1306_WHITE);
  //id: 1 triangle 2 
  display.drawTriangle(59, 18, 66, 11, 89, 15, SSD1306_WHITE);
  //id: 2 triangle 3 
  display.drawTriangle(66, 13, 59, 1, 90, 13, SSD1306_WHITE);
  //id: 3 circle 7 
  display.drawCircle(50, 12, 0, SSD1306_WHITE);
  //id: 4 circle 9 
  display.drawCircle(50, 12, 6, SSD1306_WHITE);
  //id: 5 text 10 
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30, 18);
  display.print(F("PC Crys"));

}

void Menu(Adafruit_SSD1306& display){
  display.clearDisplay();



}

#endif IMAGE_H