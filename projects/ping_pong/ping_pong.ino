#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configurações do jogo
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 3
#define PADDLE_Y (SCREEN_HEIGHT - 2)
#define BALL_SIZE 3
#define WALL_WIDTH 2
#define WALL_HEIGHT 8
#define WALL_X (SCREEN_WIDTH - WALL_WIDTH)

// Variáveis do jogo
int paddleX = SCREEN_WIDTH/2 - PADDLE_WIDTH/2;
int ballX = SCREEN_WIDTH/2;
int ballY = SCREEN_HEIGHT/2;
int ballSpeedX = 1;
int ballSpeedY = 1;
int wallY = 0;
int wallSpeed = 1;
int score = 0;
int lives = 3;
bool gameOver = false;
unsigned long lastUpdate = 0;
int updateInterval = 30; // ms

// Pinos dos botões
#define BUTTON_LEFT 7
#define BUTTON_RIGHT 6

void setup() {
  Serial.begin(115200);
  
  // Inicializa o display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha na alocação do SSD1306"));
    for(;;);
  }
  
  // Configura os botões
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  
  // Limpa o display
  display.clearDisplay();
  display.display();
}

void loop() {
  if (!gameOver) {
    // Verifica os botões
    checkInput();
    
    // Atualiza o jogo
    if (millis() - lastUpdate > updateInterval) {
      updateGame();
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
    display.print(score);
    display.display();
    
    // Reinicia o jogo se qualquer botão for pressionado
    if (digitalRead(BUTTON_LEFT) == LOW || digitalRead(BUTTON_RIGHT) == LOW) {
      resetGame();
    }
  }
}

void checkInput() {
  if (digitalRead(BUTTON_LEFT) == LOW && paddleX > 0) {
    paddleX -= 3;
  }
  if (digitalRead(BUTTON_RIGHT) == LOW && paddleX < SCREEN_WIDTH - PADDLE_WIDTH) {
    paddleX += 3;
  }
}

void updateGame() {
  // Move a bola
  ballX += ballSpeedX;
  ballY += ballSpeedY;
  
  // Move a parede (oponente)
  wallY += wallSpeed;
  if (wallY <= 0 || wallY >= SCREEN_HEIGHT - WALL_HEIGHT) {
    wallSpeed = -wallSpeed;
  }
  
  // Colisão com as bordas
  if (ballX <= 0) {
    ballSpeedX = -ballSpeedX;
  }
  if (ballY <= 0) {
    ballSpeedY = -ballSpeedY;
  }
  
  // Colisão com a parede (oponente)
  if (ballX >= WALL_X - BALL_SIZE && 
      ballY >= wallY && 
      ballY <= wallY + WALL_HEIGHT) {
    ballSpeedX = -ballSpeedX;
    score++;
    
    // Aumenta a dificuldade a cada 5 pontos
    if (score % 5 == 0) {
      if (ballSpeedX > 0) ballSpeedX++;
      else ballSpeedX--;
      
      if (ballSpeedY > 0) ballSpeedY++;
      else ballSpeedY--;
    }
  }
  
  // Colisão com a raquete
  if (ballY >= PADDLE_Y - BALL_SIZE && 
      ballX >= paddleX && 
      ballX <= paddleX + PADDLE_WIDTH) {
    ballSpeedY = -ballSpeedY;
    
    // Efeito de direção baseado onde a bola acertou a raquete
    int hitPos = ballX - (paddleX + PADDLE_WIDTH/2);
    ballSpeedX = constrain(hitPos / 3, -3, 3);
  }
  
  // Bola saiu pela parte inferior
  if (ballY >= SCREEN_HEIGHT) {
    lives--;
    if (lives <= 0) {
      gameOver = true;
    } else {
      // Reseta a bola
      ballX = SCREEN_WIDTH/2;
      ballY = SCREEN_HEIGHT/2;
      ballSpeedX = random(0, 2) ? 1 : -1;
      ballSpeedY = 1;
    }
  }
}

void drawGame() {
  display.clearDisplay();
  
  // Desenha a parede (oponente)
  display.fillRect(WALL_X, wallY, WALL_WIDTH, WALL_HEIGHT, SSD1306_WHITE);
  
  // Desenha a raquete
  display.fillRect(paddleX, PADDLE_Y, PADDLE_WIDTH, PADDLE_HEIGHT, SSD1306_WHITE);
  
  // Desenha a bola
  display.fillRect(ballX, ballY, BALL_SIZE, BALL_SIZE, SSD1306_WHITE);
  
  // Desenha o score e vidas
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
  display.setCursor(SCREEN_WIDTH - 30, 0);
  display.print("Lives: ");
  display.print(lives);
  
  display.display();
}

void resetGame() {
  score = 0;
  lives = 3;
  gameOver = false;
  paddleX = SCREEN_WIDTH/2 - PADDLE_WIDTH/2;
  ballX = SCREEN_WIDTH/2;
  ballY = SCREEN_HEIGHT/2;
  ballSpeedX = random(0, 2) ? 1 : -1;
  ballSpeedY = 1;
  wallY = 0;
  wallSpeed = 1;
}