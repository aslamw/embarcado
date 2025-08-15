#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configurações do jogo
#define TARGET_SIZE 8
#define SIGHT_SIZE 6
#define BULLET_SPEED 3
#define MAX_TARGETS 3

// Variáveis do jogo
struct Target {
  int x;
  int y;
  bool active;
  int speed;
};

Target targets[MAX_TARGETS];
int sightX = SCREEN_WIDTH/2;
int sightY = SCREEN_HEIGHT/2;
int score = 0;
unsigned long lastTargetSpawn = 0;
int targetSpawnInterval = 2000; // ms
bool gameRunning = true;

// Pinos dos botões
#define BUTTON_LEFT 12
#define BUTTON_RIGHT 14
#define BUTTON_UP 13
#define BUTTON_DOWN 15
#define BUTTON_SHOOT 16

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
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SHOOT, INPUT_PULLUP);
  
  // Inicializa alvos
  for (int i = 0; i < MAX_TARGETS; i++) {
    targets[i].active = false;
  }
  
  // Limpa o display
  display.clearDisplay();
  display.display();
}

void loop() {
  if (gameRunning) {
    // Verifica os botões
    checkInput();
    
    // Atualiza o jogo
    updateGame();
    
    // Desenha o jogo
    drawGame();
  } else {
    // Tela de game over
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 5);
    display.print("FIM DE JOGO");
    display.setCursor(10, 20);
    display.print("Pontos: ");
    display.print(score);
    display.display();
    
    // Reinicia o jogo se qualquer botão for pressionado
    if (digitalRead(BUTTON_SHOOT) == LOW) {
      resetGame();
    }
  }
}

void checkInput() {
  // Movimento da mira
  if (digitalRead(BUTTON_LEFT) == LOW && sightX > SIGHT_SIZE/2) {
    sightX -= 2;
  }
  if (digitalRead(BUTTON_RIGHT) == LOW && sightX < SCREEN_WIDTH - SIGHT_SIZE/2) {
    sightX += 2;
  }
  if (digitalRead(BUTTON_UP) == LOW && sightY > SIGHT_SIZE/2) {
    sightY -= 2;
  }
  if (digitalRead(BUTTON_DOWN) == LOW && sightY < SCREEN_HEIGHT - SIGHT_SIZE/2) {
    sightY += 2;
  }
  
  // Disparo
  if (digitalRead(BUTTON_SHOOT) == LOW) {
    checkHit();
    delay(200); // Debounce
  }
}

void updateGame() {
  // Move os alvos ativos
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].active) {
      targets[i].x += targets[i].speed;
      
      // Verifica se o alvo saiu da tela
      if (targets[i].x > SCREEN_WIDTH + TARGET_SIZE || targets[i].x < -TARGET_SIZE) {
        targets[i].active = false;
      }
    }
  }
  
  // Gera novos alvos aleatoriamente
  if (millis() - lastTargetSpawn > targetSpawnInterval) {
    spawnTarget();
    lastTargetSpawn = millis();
    
    // Aumenta a dificuldade diminuindo o intervalo
    if (targetSpawnInterval > 500) {
      targetSpawnInterval -= 50;
    }
  }
}

void spawnTarget() {
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (!targets[i].active) {
      targets[i].active = true;
      targets[i].y = random(TARGET_SIZE, SCREEN_HEIGHT - TARGET_SIZE);
      
      // Decide se vem da esquerda ou direita
      if (random(0, 2)) {
        targets[i].x = -TARGET_SIZE;
        targets[i].speed = random(1, 4);
      } else {
        targets[i].x = SCREEN_WIDTH + TARGET_SIZE;
        targets[i].speed = -random(1, 4);
      }
      break;
    }
  }
}

void checkHit() {
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].active) {
      // Verifica colisão entre a mira e o alvo
      if (abs(sightX - targets[i].x) < (SIGHT_SIZE + TARGET_SIZE)/2 &&
          abs(sightY - targets[i].y) < (SIGHT_SIZE + TARGET_SIZE)/2) {
        targets[i].active = false;
        score++;
        
        // Efeito visual de acerto
        display.fillRect(targets[i].x - TARGET_SIZE/2, targets[i].y - TARGET_SIZE/2, 
                        TARGET_SIZE, TARGET_SIZE, SSD1306_WHITE);
        display.display();
        delay(50);
        return;
      }
    }
  }
}

void drawGame() {
  display.clearDisplay();
  
  // Desenha alvos
  for (int i = 0; i < MAX_TARGETS; i++) {
    if (targets[i].active) {
      display.drawCircle(targets[i].x, targets[i].y, TARGET_SIZE/2, SSD1306_WHITE);
      display.fillCircle(targets[i].x, targets[i].y, TARGET_SIZE/4, SSD1306_WHITE);
    }
  }
  
  // Desenha mira
  display.drawLine(sightX - SIGHT_SIZE/2, sightY, sightX + SIGHT_SIZE/2, sightY, SSD1306_WHITE);
  display.drawLine(sightX, sightY - SIGHT_SIZE/2, sightX, sightY + SIGHT_SIZE/2, SSD1306_WHITE);
  
  // Desenha pontuação
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Pontos: ");
  display.print(score);
  
  display.display();
}

void resetGame() {
  score = 0;
  gameRunning = true;
  targetSpawnInterval = 2000;
  
  // Reseta todos os alvos
  for (int i = 0; i < MAX_TARGETS; i++) {
    targets[i].active = false;
  }
  
  // Reseta a mira
  sightX = SCREEN_WIDTH/2;
  sightY = SCREEN_HEIGHT/2;
}