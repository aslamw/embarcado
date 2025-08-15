#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configurações do jogo
#define GRID_SIZE 4
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / GRID_SIZE)

// Estrutura para a cobra
struct Point {
  int x;
  int y;
};

Point snake[100];
int snakeLength = 3;
int direction = 0; // 0=right, 1=up, 2=left, 3=down
Point food;
bool gameOver = false;
unsigned long lastUpdate = 0;
int updateSpeed = 150; // ms

// Pinos dos botões (ajuste conforme sua placa)
#define BUTTON_UP 12
#define BUTTON_DOWN 13
#define BUTTON_LEFT 14
#define BUTTON_RIGHT 15

void setup() {
  Serial.begin(115200);
  
  // Inicializa o display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha na alocação do SSD1306"));
    for(;;);
  }
  
  // Configura os botões
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  
  // Inicializa a cobra
  snake[0].x = 5;
  snake[0].y = 4;
  snake[1].x = 4;
  snake[1].y = 4;
  snake[2].x = 3;
  snake[2].y = 4;
  
  // Gera a primeira comida
  generateFood();
  
  // Limpa o display
  display.clearDisplay();
  display.display();
}

void loop() {
  if (!gameOver) {
    // Verifica os botões
    checkButtons();
    
    // Atualiza a posição da cobra
    if (millis() - lastUpdate > updateSpeed) {
      updateSnake();
      lastUpdate = millis();
    }
    
    // Desenha o jogo
    drawGame();
  } else {
    // Tela de game over
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 5);
    display.print("GAME OVER");
    display.setCursor(10, 20);
    display.print("Score: ");
    display.print(snakeLength - 3);
    display.display();
    
    // Reinicia o jogo se qualquer botão for pressionado
    if (digitalRead(BUTTON_UP) == LOW || digitalRead(BUTTON_DOWN) == LOW || 
        digitalRead(BUTTON_LEFT) == LOW || digitalRead(BUTTON_RIGHT) == LOW) {
      resetGame();
    }
  }
}

void checkButtons() {
  if (digitalRead(BUTTON_UP) == LOW && direction != 3) {
    direction = 1;
  } else if (digitalRead(BUTTON_DOWN) == LOW && direction != 1) {
    direction = 3;
  } else if (digitalRead(BUTTON_LEFT) == LOW && direction != 0) {
    direction = 2;
  } else if (digitalRead(BUTTON_RIGHT) == LOW && direction != 2) {
    direction = 0;
  }
}

void updateSnake() {
  // Move o corpo da cobra
  for (int i = snakeLength - 1; i > 0; i--) {
    snake[i].x = snake[i-1].x;
    snake[i].y = snake[i-1].y;
  }
  
  // Move a cabeça da cobra
  switch (direction) {
    case 0: // Direita
      snake[0].x++;
      break;
    case 1: // Cima
      snake[0].y--;
      break;
    case 2: // Esquerda
      snake[0].x--;
      break;
    case 3: // Baixo
      snake[0].y++;
      break;
  }
  
  // Verifica colisão com as bordas
  if (snake[0].x < 0 || snake[0].x >= GRID_WIDTH || 
      snake[0].y < 0 || snake[0].y >= GRID_HEIGHT) {
    gameOver = true;
    return;
  }
  
  // Verifica colisão com o próprio corpo
  for (int i = 1; i < snakeLength; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      gameOver = true;
      return;
    }
  }
  
  // Verifica se comeu a comida
  if (snake[0].x == food.x && snake[0].y == food.y) {
    snakeLength++;
    generateFood();
    
    // Aumenta a velocidade a cada 5 pontos
    if ((snakeLength - 3) % 5 == 0 && updateSpeed > 50) {
      updateSpeed -= 10;
    }
  }
}

void generateFood() {
  bool onSnake;
  do {
    onSnake = false;
    food.x = random(0, GRID_WIDTH);
    food.y = random(0, GRID_HEIGHT);
    
    // Verifica se a comida não está em cima da cobra
    for (int i = 0; i < snakeLength; i++) {
      if (food.x == snake[i].x && food.y == snake[i].y) {
        onSnake = true;
        break;
      }
    }
  } while (onSnake);
}

void drawGame() {
  display.clearDisplay();
  
  // Desenha a cobra
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE, GRID_SIZE, GRID_SIZE, SSD1306_WHITE);
  }
  
  // Desenha a comida
  display.fillRect(food.x * GRID_SIZE, food.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, SSD1306_WHITE);
  
  // Desenha o score
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(snakeLength - 3);
  
  display.display();
}

void resetGame() {
  snakeLength = 3;
  direction = 0;
  gameOver = false;
  updateSpeed = 150;
  
  // Reinicia a cobra
  snake[0].x = 5;
  snake[0].y = 4;
  snake[1].x = 4;
  snake[1].y = 4;
  snake[2].x = 3;
  snake[2].y = 4;
  
  generateFood();
}