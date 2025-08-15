#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configurações do jogo
#define CAR_WIDTH 8
#define CAR_HEIGHT 8
#define OBSTACLE_WIDTH 6
#define ROAD_WIDTH 3
#define LANE_WIDTH 32

// Posições das pistas
#define LANE_LEFT 32
#define LANE_CENTER 64
#define LANE_RIGHT 96

// Estados do jogo
int carPosition = LANE_CENTER;
int score = 0;
int gameSpeed = 10;
bool gameOver = false;
unsigned long lastUpdate = 0;
int updateInterval = 300; // ms

// Obstáculos
struct Obstacle {
  int x;
  int y;
  int lane;
  bool active;
};

Obstacle obstacles[3];
int obstacleSpeed = 2;

// Pinos dos botões
#define BUTTON_LEFT 12
#define BUTTON_RIGHT 14

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
  
  // Inicializa obstáculos
  for (int i = 0; i < 3; i++) {
    obstacles[i].active = false;
  }
  
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
  if (digitalRead(BUTTON_LEFT) == LOW) {
    if (carPosition > LANE_LEFT) {
      carPosition -= LANE_WIDTH;
    }
    delay(200); // Debounce
  } else if (digitalRead(BUTTON_RIGHT) == LOW) {
    if (carPosition < LANE_RIGHT) {
      carPosition += LANE_WIDTH;
    }
    delay(200); // Debounce
  }
}

void updateGame() {
  // Gera novos obstáculos aleatoriamente
  if (random(0, 100) < 20) {
    for (int i = 0; i < 3; i++) {
      if (!obstacles[i].active) {
        obstacles[i].active = true;
        obstacles[i].y = 0;
        
        // Escolhe uma pista aleatória (0=esquerda, 1=centro, 2=direita)
        int lane = random(0, 3);
        if (lane == 0) obstacles[i].x = LANE_LEFT - OBSTACLE_WIDTH/2;
        else if (lane == 1) obstacles[i].x = LANE_CENTER - OBSTACLE_WIDTH/2;
        else obstacles[i].x = LANE_RIGHT - OBSTACLE_WIDTH/2;
        
        obstacles[i].lane = lane;
        break;
      }
    }
  }
  
  // Move os obstáculos
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      obstacles[i].y += obstacleSpeed;
      
      // Verifica se o obstáculo saiu da tela
      if (obstacles[i].y > SCREEN_HEIGHT) {
        obstacles[i].active = false;
        score++;
        
        // Aumenta a dificuldade a cada 5 pontos
        if (score % 5 == 0 && updateInterval > 100) {
          updateInterval -= 20;
          obstacleSpeed += 1;
        }
      }
      
      // Verifica colisão
      if (obstacles[i].active && 
          ((obstacles[i].lane == 0 && carPosition == LANE_LEFT) ||
           (obstacles[i].lane == 1 && carPosition == LANE_CENTER) ||
           (obstacles[i].lane == 2 && carPosition == LANE_RIGHT)) &&
          obstacles[i].y + OBSTACLE_WIDTH > SCREEN_HEIGHT - CAR_HEIGHT) {
        gameOver = true;
      }
    }
  }
}

void drawGame() {
  display.clearDisplay();
  
  // Desenha a estrada
  for (int i = 0; i < SCREEN_WIDTH; i += ROAD_WIDTH * 2) {
    display.drawFastVLine(LANE_LEFT - LANE_WIDTH/2, 0, SCREEN_HEIGHT, SSD1306_WHITE);
    display.drawFastVLine(LANE_RIGHT + LANE_WIDTH/2, 0, SCREEN_HEIGHT, SSD1306_WHITE);
  }
  
  // Desenha os obstáculos
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      display.fillRect(obstacles[i].x, obstacles[i].y, OBSTACLE_WIDTH, OBSTACLE_WIDTH, SSD1306_WHITE);
    }
  }
  
  // Desenha o carro
  display.fillRect(carPosition - CAR_WIDTH/2, SCREEN_HEIGHT - CAR_HEIGHT - 2, CAR_WIDTH, CAR_HEIGHT, SSD1306_WHITE);
  
  // Desenha o score
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
  
  display.display();
}

void resetGame() {
  score = 0;
  gameOver = false;
  carPosition = LANE_CENTER;
  updateInterval = 300;
  obstacleSpeed = 2;
  
  // Reseta os obstáculos
  for (int i = 0; i < 3; i++) {
    obstacles[i].active = false;
  }
}