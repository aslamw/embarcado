//cartão
#include <SPI.h>
#include <SD.h>
// tela
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1

#define POWER 13

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 1
#define BUTTON_PIN_3 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



int button1 = HIGH;
int button2 = HIGH;
int button3 = HIGH;

void setup(){

   //Inicializa I2c
  //pinMode(POWER, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);

  // Inicializa Display no endereço 0x3c do I2C
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);

  int16_t x, r


}
void main(){

  //digitalWrite(POWER, HIGH);

  
  display.clearDisplay();

  display.setTextColor(WHITE);

  display.getTextBounds("welcome Glol sistem".c_str(), 0, 0, &x1, &y1, &w, &h);
  
  display.display();
  delay(1000);

  display.clearDisplay();
  

  do{

    button1 = digitalRead(BUTTON_PIN_1);
    button2 = digitalRead(BUTTON_PIN_2);
    button3 = digitalRead(BUTTON_PIN_3);

    if (button1 == LOW){
      Serial.print("aperto 1");

      delay(300);
    }

    if(button3 == LOW){
      
      Serial.print("aperto 3");
      
      delay(300);
    }

    if(button2 == LOW){
      Serial.print("aperto 2");
      
      delay(300);
    }

    display.display();
  }while(1);

}