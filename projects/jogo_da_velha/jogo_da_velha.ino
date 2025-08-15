#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdbool.h>

//Define e configura display
#define OLED_RESET -1

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define BUTTON_PIN_1 10
#define BUTTON_PIN_2 9
#define BUTTON_PIN_3 8

int button1 = HIGH;
int button2 = HIGH;
int button3 = HIGH;

int Xx = 0;
int Xy = 0;

bool game;
int jogador1[7] = {0};
int jogador2[7] = {0};
int ganho_game[8][3]{
  {1,2,3},
  {4,5,6},
  {7,8,9},
  {1,5,9},
  {3,5,7},
  {1,4,7},
  {2,5,8},
  {3,6,9}
};

int jogador1_index, jogador2_index =  0;
int vez = 1;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool ganhador(int jogador[], int tamanho){
  int pontos = 0;

  for(int ganhos = 0; ganhos < 8; ganhos++){
    for(int ganho = 0; ganho < 3; ganho++){

      for(int jogada = 0; jogada < tamanho; jogada++){
        if(ganho_game[ganhos][ganho] == jogador[jogada]){
          pontos++;
        }
      }
    }

    if (pontos == 3){
      return true;
    }
    pontos = 0;
  }

  return false;
}

void jogada(){
  for(int i = 0; i < 7; i++){
    desenho(jogador1[i], 1);

    desenho(jogador2[i], 0);
  }
}

void desenho (int box, int vez){

  switch (box){
    case 1:
      if (vez == 1){
        display.fillRect(0, 0, 38, 9, BLACK);
        display.drawLine(0, 0, 38, 9, WHITE);
        display.drawLine(0, 9, 38, 0, WHITE);
      }
      else{
        display.fillRect(0, 0, 38, 9, BLACK);
        display.drawCircle(16, 4, 4, WHITE);
      }
      
      break;

    case 2:

      if (vez == 1){
        display.fillRect(42, 0, 38, 9, BLACK);
        display.drawLine(42, 0, 80, 9, WHITE);
        display.drawLine(42, 9, 80, 0, WHITE);
      }
      else{
        display.fillRect(42, 0, 38, 9, BLACK);
        display.drawCircle(60, 4, 4, WHITE);
      }

      break;

    case 3:

      if (vez == 1){
        display.fillRect(82, 0, 38, 9, BLACK);
        display.drawLine(82, 0, 128, 9, WHITE);
        display.drawLine(82, 9, 128, 0, WHITE);
      }
      else{
        display.fillRect(82, 0, 38, 9, BLACK);
        display.drawCircle(100, 4, 4, WHITE);
      }

      break;

    case 4:

      if (vez == 1){
        display.fillRect(0, 11, 38, 9, BLACK);
        display.drawLine(0, 11, 38, 20, WHITE);
        display.drawLine(0, 20, 38, 11, WHITE);
      }
      else{
        display.fillRect(0, 11, 38, 9, BLACK);
        display.drawCircle(16, 15, 3, WHITE);// circulo tem a base X,y, raio para sua montagem
      }

      break;

    case 5:

      if (vez == 1){
        display.fillRect(42, 11, 36, 9, BLACK);
        display.drawLine(42, 11, 80, 20, WHITE);
        display.drawLine(42, 20, 80, 11, WHITE);
      }
      else{
        display.fillRect(42, 11, 36, 9, BLACK);
        display.drawCircle(60, 15, 3, WHITE);
      }
      break;

    case 6:

      if (vez == 1){
        display.fillRect(80, 11, 38, 9, BLACK);
        display.drawLine(82, 11, 128, 20, WHITE);
        display.drawLine(82, 16, 128, 11, WHITE);
      }
      else{
        display.fillRect(80, 11, 38, 9, BLACK);
        display.drawCircle(100, 15, 3, WHITE);
      }
      
      break;

    case 7:

      if (vez == 1){
        display.fillRect(0, 22, 38, 9, BLACK);
        display.drawLine(0, 20, 38, 30, WHITE);
        display.drawLine(0, 30, 38, 20, WHITE);
      }
      else{
        display.fillRect(0, 22, 38, 9, BLACK);
        display.drawCircle(16, 26, 4, WHITE);
      }
      break;

    case 8:

      if (vez == 1){
        display.fillRect(42, 22, 36, 9, BLACK);
        display.drawLine(42, 20, 80, 30, WHITE);
        display.drawLine(42, 30, 80, 20, WHITE);
      }
      else{
        display.fillRect(42, 22, 36, 9, BLACK);
        display.drawCircle(60, 26, 4, WHITE);
      }
      break;

    case 9:

      if (vez == 1){
        display.fillRect(80, 20, 36, 9, BLACK);
        display.drawLine(82, 20, 128, 30, WHITE);
        display.drawLine(82, 30, 128, 20, WHITE);
      }
      else{
        display.fillRect(82, 22, 40, 12, BLACK);
        display.drawCircle(100, 26, 4, WHITE);
      }
      break;
    
  }
}

void size (int box){

  switch (box){
    case 1://corrigir base 0 e 1
      display.fillRect(42, 0, 36, 9, BLACK);
      display.fillRect(82, 22, 40, 12, BLACK);
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
      display.fillRect(82, 22, 40, 12, BLACK);
      display.fillRect(0, 22, 38, 9, BLACK);
      display.fillRect(42, 22, 36, 9, WHITE);
      break;
    case 9:
      display.fillRect(42, 22, 36, 9, BLACK);
      display.fillRect(82, 22, 40, 12, WHITE);
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
  game = true;
  
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

      if(vez && jogador1[jogador1_index] == 0){
        jogador1[jogador1_index] = box;
        jogador1_index++;
        vez=0;

        if(ganhador(jogador1, 7)){
          display.clearDisplay();

          display.setTextColor(WHITE);
          display.setCursor(25,10);

          display.print("jogador 1 ganhou!!!");
          
          display.display();
          delay(1000);

          break;
        }

      }
      else if(jogador2[jogador2_index] == 0){

        jogador2[jogador2_index] = box;
        jogador2_index++;
        vez++;

        if(ganhador(jogador2, 7)){
          display.clearDisplay();

          display.setTextColor(WHITE);
          display.setCursor(25,10);

          display.print("jogador 2 ganhou!!!");
          
          display.display();
          delay(1000);

          jogador1[7] = {0};
          jogador2[7] = {0};
          jogador1_index, jogador2_index =  0;
          vez = 1;
          game = false;
          box = 0;

          break;
        }

      }
      delay(300);
    }

    jogada();

    display.display();
  }while(game);

}