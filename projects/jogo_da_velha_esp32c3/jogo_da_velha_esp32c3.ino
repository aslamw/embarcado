#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdbool.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN 6
#define SCL_PIN 7

#define led_pin 8

const int BUTTON_PIN_1 = 2;
const int BUTTON_PIN_2 = 3;
const int BUTTON_PIN_3 = 4;

int button1 = HIGH;
int button2 = HIGH;
int button3 = HIGH;

int Xx = 0;
int Xy = 0;

bool game;
int jogador1[9] = {0}; // Aumentado para 9 para cobrir todas as posições
int jogador2[9] = {0};
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

int jogador1_index = 0, jogador2_index = 0;
int vez = 1;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_text(char text[100], int X, int Y){

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(X,Y);
  display.print(text);
  display.display();
  delay(1000);
}

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

void desenho(int box, int vez){
  int x = (box - 1) % 3 * 40;
  int y = (box - 1) / 3 * 10;

  display.fillRect(x, y, 38, 9, BLACK);

  if (vez == 1){
    display.drawLine(x, y, x + 38, y + 9, WHITE);
    display.drawLine(x, y + 9, x + 38, y, WHITE);
  } else {
    display.drawCircle(x + 19, y + 4, 4, WHITE);
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
  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  print_text("Jogo da velha", 40, 10);
  
}

void loop() {
  int box = 1;
  game = true;

  display.clearDisplay();

  while(game){
    button1 = digitalRead(BUTTON_PIN_1);
    button2 = digitalRead(BUTTON_PIN_2);
    button3 = digitalRead(BUTTON_PIN_3);

    display.drawFastVLine(40, 0, 32, WHITE);
    display.drawFastVLine(80, 0, 32, WHITE);
    display.drawFastHLine(0, 10, 128, WHITE);
    display.drawFastHLine(0, 20, 128, WHITE);

    if (button1 == LOW){
      if (box > 0 ){
        box--;
        size(box);
      }
      else{
        box = 9;
        size(box);
      }

      delay(500);
    }

    if (button3 == LOW){
      if (box < 9){
        box++;
        size(box);
      }
      else{
        box = 1;
        size(box);
      }
      delay(500);
    }

    if (button2 == LOW){
      if (vez == 1 && jogador1[box - 1] == 0){
        jogador1[box - 1] = box;
        jogador1_index++;
        vez = 0;

        if (ganhador(jogador1, 9)){
          display.clearDisplay();
          display.setCursor(25,10);
          display.print("Jogador 1 ganhou!");
          display.display();
          delay(2000);
          game = false;
        }
      } else if (vez == 0 && jogador2[box - 1] == 0){
        jogador2[box - 1] = box;
        jogador2_index++;
        vez = 1;

        if (ganhador(jogador2, 9)){
          display.clearDisplay();
          display.setCursor(25,10);
          display.print("Jogador 2 ganhou!");
          display.display();

          delay(2000);
          game = false;
        }
      }
      delay(500);
    }

    for (int i = 0; i < 9; i++){
      if (jogador1[i] != 0) desenho(jogador1[i], 1);
      if (jogador2[i] != 0) desenho(jogador2[i], 0);
    }

    display.display();
  }

  // Reinicia o jogo
  memset(jogador1, 0, sizeof(jogador1));
  memset(jogador2, 0, sizeof(jogador2));
  jogador1_index = 0;
  jogador2_index = 0;
  vez = 1;
}