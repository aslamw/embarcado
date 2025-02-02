#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Define e configura display
#define OLED_RESET -1

#define POWER 13

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define BUTTON_PIN_1 0
#define BUTTON_PIN_2 1
#define BUTTON_PIN_3 2



int button1 = HIGH;
int button2 = HIGH;
int button3 = HIGH;

int Xx = 0;
int Xy = 0;

int jogador1[7];
int jogador2[7];
int vez = 1;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void desenho (int box, int vez){

  switch (box){
    case 1:
    if (vez == 1){
      display.fillRect(0, 0, 38, 9, BLACK);
      display.drawLine(0, 0, 38, 9, WHITE);
      display.drawLine(0, 9, 38, 0, WHITE);
    }
      
      break;

    case 2:
      break;

    case 3:
      break;

    case 4:
      break;

    case 5:
      break;

    case 6:
      break;

    case 7:
      break;

    case 8:
      break;

    case 9:
      break;
    
  }

  
}

void size (int box){

  switch (box){
    case 1://corrigir base 0 e 1
      display.fillRect(42, 0, 36, 9, BLACK);
      display.fillRect(80, 20, 36, 9, BLACK);
      display.fillRect(0, 0, 38, 9, WHITE);
      break;
    case 2:
      display.fillRect(80, 0, 38, 9, BLACK);
      display.fillRect(0, 0, 38, 9, BLACK);
      display.fillRect(42, 0, 36, 9, WHITE);
      break;
    case 3:
      display.fillRect(0, 11, 38, 9,BLACK);
      display.fillRect(42, 0, 36, 9, BLACK);
      display.fillRect(80, 0, 38, 9, WHITE);
      break;
    case 4:
      display.fillRect(42, 11, 36, 9, BLACK);
      display.fillRect(80, 0, 38, 9, BLACK);
      display.fillRect(0, 11, 38, 9, WHITE);
      break;
    case 5:
      display.fillRect(0, 11, 38, 9,BLACK);
      display.fillRect(80, 11, 38, 9, BLACK);
      display.fillRect(42, 11, 36, 9, WHITE);
      break;
    case 6:
      display.fillRect(0, 22, 38, 9, BLACK);
      display.fillRect(42, 11, 36, 9, BLACK);
      display.fillRect(80, 11, 38, 9, WHITE);
      break;
    case 7:
      display.fillRect(80, 11, 38, 9, BLACK);
      display.fillRect(42, 22, 36, 9, BLACK);
      display.fillRect(0, 22, 38, 9, WHITE);
      break;
    case 8:
      display.fillRect(80, 20, 36, 9, BLACK);
      display.fillRect(0, 22, 38, 9, BLACK);
      display.fillRect(42, 22, 36, 9, WHITE);
      break;
    case 9:
      display.fillRect(42, 22, 36, 9, BLACK);
      display.fillRect(80, 20, 36, 9, WHITE);
      break;
  }
}


void setup() {
  //Inicializa I2c
  //pinMode(POWER, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);

  // Inicializa Display no endereço 0x3c do I2C
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);

}

void loop() {

  int box = 0;
  //digitalWrite(POWER, HIGH);

  
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(25,10);
  display.print("jogo da velha");
  
  display.display();
  delay(1000);

  display.clearDisplay();
  

  do{

    button1 = digitalRead(BUTTON_PIN_1);
    button2 = digitalRead(BUTTON_PIN_2);
    button3 = digitalRead(BUTTON_PIN_3);

    
    // | 1
    //display.drawLine(40,0, 40,32, WHITE);
    display.drawFastVLine(40,0, 32, WHITE);

    //- 1
    //display.drawLine(0,10 , 128, 10, WHITE);
    display.drawFastHLine(0,10, 128, WHITE);

    // | 2
    //display.drawLine(80,0, 80,32, WHITE);
    display.drawFastVLine(80,0, 32, WHITE);

    //- 2
    //display.drawLine(0,20, 128, 20, WHITE);
    display.drawFastHLine(0,20, 128, WHITE);

    if (button1 == LOW){
      Serial.print("aperto 1");
      
      if (box > 0 ){
        box--;
        size(box);
      }
      else{
        box = 9;
        size(box);
      }

      delay(300);
    }

    if(button3 == LOW){
      
      Serial.print("aperto 3");
      if (box < 9){
        box++;
        size(box);
      }
      else{
        box = 1;
        size(box);
      }
      delay(300);
    }

    if(button2 == LOW){
      Serial.print("aperto 2");

      desenho(box, vez);
      
      delay(300);
    }

    display.display();
  }while(1);


}
