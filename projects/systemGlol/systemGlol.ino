#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define e configura display
#define OLED_RESET -1

#define BUTTON_LEFT 9
#define BUTTON_OK 10
#define BUTTON_RIGHT 11

Adafruit_SSD1306 display(OLED_RESET);

//------------------------------------------------------------------logo--------------------------------

const unsigned char logo1 [] PROGMEM = {
0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x07, 0xff, 0xbf, 0xe0, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x0f, 0xff, 0x13, 0xf0, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x1f, 0x8f, 0xbf, 0x78, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x3f, 0x8f, 0xff, 0x3c, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x3f, 0xcf, 0xfd, 0xfc, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x7e, 0xff, 0x3f, 0xfe, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x7b, 0xff, 0xff, 0x3e, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x2f, 0xff, 0x00, 0xf1, 0xfc, 0xff, 0x1f, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xfd, 0xe7, 0xf7, 0xbf, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0xff, 0xe7, 0xe3, 0xff, 0x00, 0x3f, 0xfc, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xf3, 0xe7, 0xe7, 0xff, 0x00, 0x7f, 0xa0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xf6, 0x7f, 0xff, 0x77, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0xe0, 0xfe, 0xdf, 0xfb, 0x77, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7f, 0xff, 0xff, 0xce, 0x01, 0x90, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7d, 0xff, 0xff, 0xce, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3f, 0xbf, 0xfe, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x3f, 0xcf, 0x3f, 0x7c, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1f, 0xde, 0x3f, 0xf8, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x07, 0xfc, 0x7d, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x03, 0xff, 0xff, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x01, 0xff, 0xff, 0x80, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x3f, 0xfc, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x07, 0xe0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char logo2 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x03, 0xff, 0xf7, 0xe0, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x0f, 0xfc, 0x7f, 0xf0, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x1f, 0xbc, 0x7c, 0xf8, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x3f, 0x2e, 0xf8, 0xfc, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x7f, 0xbf, 0xff, 0x3c, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x79, 0xff, 0xff, 0x7e, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x79, 0xfb, 0xe7, 0xfe, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x2f, 0xff, 0x00, 0xfe, 0x73, 0x7f, 0xe6, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xfe, 0xf3, 0x7f, 0x27, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0xf3, 0xff, 0xff, 0xff, 0x00, 0x3f, 0xfc, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xbf, 0x00, 0x7f, 0xa0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xc7, 0x1f, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0xe0, 0x7d, 0xff, 0x5f, 0x9e, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x7c, 0xff, 0xff, 0xfe, 0x01, 0x90, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7f, 0xff, 0xdf, 0xfe, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3f, 0xe7, 0xfb, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1f, 0x87, 0xff, 0xb8, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1f, 0x8f, 0xdc, 0x78, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x07, 0xff, 0xdd, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x03, 0xfe, 0xff, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0xfe, 0x7f, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x7f, 0xfc, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x05, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char logo3 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x1b, 0xe0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0xf0, 0x00, 0x03, 0xf9, 0xff, 0xe0, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x0f, 0xf9, 0xf7, 0xf0, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x1f, 0x99, 0x73, 0xf8, 0x00, 0x01, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x3f, 0xff, 0xe3, 0xfc, 0x00, 0x03, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfc, 0x00, 0x3c, 0x9f, 0xf7, 0xfc, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1f, 0xfe, 0x00, 0x7d, 0xfd, 0xff, 0xfe, 0x00, 0x07, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x7f, 0xfc, 0xff, 0x9e, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x2f, 0xff, 0x00, 0xfd, 0xdc, 0x9f, 0x9e, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0xf9, 0xdf, 0xbb, 0xff, 0x00, 0x1f, 0xff, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0xf9, 0xf7, 0xfb, 0xcf, 0x00, 0x3f, 0xfc, 0x80, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x00, 0x7f, 0xa0, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xfd, 0xff, 0xff, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x02, 0xe0, 0xf9, 0x7f, 0x1f, 0x6e, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x78, 0x3f, 0x8f, 0xce, 0x01, 0x90, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x7e, 0x7e, 0xfc, 0xdc, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3f, 0xff, 0xfc, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x3f, 0xff, 0x3d, 0xfc, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x1f, 0xf7, 0xff, 0xf0, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x07, 0x37, 0xff, 0xf0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x03, 0xfe, 0x0f, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x01, 0xff, 0xff, 0x80, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x3f, 0xfc, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x02, 0xc0, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00
};

//---------------------------------------------------------------------------------------------



//-------------------------------------variaveis------------------------------------------------

int time_logo = 0;

int button_left, button_ok, button_right = HIGH;
int button_index = 0;

//------------------------------------funções-----------------------------------------------------

void logo_animation (){
  //logo
  for (int8_t i = 0; i<2; i++){
    display.clearDisplay();

    display.drawBitmap(0, 0, logo1, 128, 32, WHITE);

    display.display();

    display.clearDisplay();

    display.drawBitmap(0, 0, logo2, 128, 32, WHITE);

    display.display();

    display.clearDisplay();

    display.drawBitmap(0, 0, logo3, 128, 32, WHITE);

    display.display();
  }

  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setCursor(30, 12); // Posiciona o cursor
  display.println("System Glol");

  display.display();

  delay(2000);
  
}

//----------------------------------------------------------------------

void menu(){

  do{

    display.clearDisplay();

    display.setTextColor(WHITE);

    display.setCursor(30,2); // Posiciona o cursor
    display.println("Number Game");

    display.setCursor(30, 12); // Posiciona o cursor
    display.println("tic tac toe");

    display.setCursor(30, 22); // Posiciona o cursor
    display.println("spectrum");

    button_right = digitalRead(BUTTON_RIGHT);
    button_ok = digitalRead(BUTTON_OK);
    button_left = digitalRead(BUTTON_LEFT);

    switch (button_index){
      case 1:
        display.drawRect(26, 0, 75, 12, WHITE);
        break;

      case 2:
        display.drawRect(26, 10, 75, 12, WHITE);
        break;

      case 3:
        display.drawRect(26, 20, 75, 12, WHITE);
        break;
    }

    display.display();



    if (button_left == LOW){
      Serial.print("aperto esquerda");
      
      if (button_index > 0 ){
        button_index--;
      }
      else{
        button_index = 3;
      }

      delay(300);
    }

    if(button_right == LOW){
      
      Serial.print("aperto direita");
      if (button_index < 3){
        button_index++;
      }
      else{
        button_index = 1;
      }
      delay(300);
    }

    if(button_ok == LOW){
      Serial.print("aperto ok");
      button_index = 0;
      break;
    }
      
  }while(1);

}

void setup() {
  //Inicializa I2c
  Wire.begin();

  // Inicializa Display no endereço 0x3c do I2C
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);

  pinMode(BUTTON_LEFT,INPUT_PULLUP);
  pinMode(BUTTON_OK, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);

}

void loop() {
  

  logo_animation();
  menu();



}
