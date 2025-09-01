#ifndef GAMES_H
#define GAMES_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

class SnakeGame {
private:
  Adafruit_SSD1306 display;
  
  // Estrutura para a cobra
  struct Segment {
    int x, y;
    Segment* next;
  };
  
  Segment* head;
  int direction; // 0: up, 1: right, 2: down, 3: left
  int foodX, foodY;
  int score;
  bool gameOver;
  unsigned long lastUpdate;
  int updateDelay;
  
  // Pinos dos botões
  int btnUpPin;
  int btnDownPin;
  int btnSelectPin;
  
  void generateFood() {
    bool onSnake;
    do {
      onSnake = false;
      foodX = random(2, SCREEN_WIDTH - 2);
      foodY = random(2, SCREEN_HEIGHT - 2);
      
      // Arredonda para múltiplos de 2 para alinhar com a grade
      foodX = (foodX / 2) * 2;
      foodY = (foodY / 2) * 2;
      
      Segment* current = head;
      while (current != nullptr) {
        if (current->x == foodX && current->y == foodY) {
          onSnake = true;
          break;
        }
        current = current->next;
      }
    } while (onSnake);
  }
  
  void addSegment() {
    Segment* newSegment = new Segment;
    newSegment->x = head->x;
    newSegment->y = head->y;
    newSegment->next = head->next;
    head->next = newSegment;
  }
  
  void moveSnake() {
    // Salva a posição atual da cabeça
    int prevX = head->x;
    int prevY = head->y;
    
    // Move a cabeça
    switch (direction) {
      case 0: head->y -= 2; break; // Up
      case 1: head->x += 2; break; // Right
      case 2: head->y += 2; break; // Down
      case 3: head->x -= 2; break; // Left
    }
    
    // Verifica colisão com as bordas
    if (head->x < 0 || head->x >= SCREEN_WIDTH || 
        head->y < 0 || head->y >= SCREEN_HEIGHT) {
      gameOver = true;
      return;
    }
    
    // Verifica colisão consigo mesma
    Segment* current = head->next;
    while (current != nullptr) {
      if (head->x == current->x && head->y == current->y) {
        gameOver = true;
        return;
      }
      current = current->next;
    }
    
    // Move os segmentos do corpo
    current = head->next;
    while (current != nullptr) {
      int tempX = current->x;
      int tempY = current->y;
      current->x = prevX;
      current->y = prevY;
      prevX = tempX;
      prevY = tempY;
      current = current->next;
    }
    
    // Verifica se comeu a comida
    if (head->x == foodX && head->y == foodY) {
      score++;
      addSegment();
      generateFood();
      // Aumenta a velocidade
      if (updateDelay > 50) {
        updateDelay -= 5;
      }
    }
  }
  
  void drawSnake() {
    Segment* current = head;
    while (current != nullptr) {
      display.fillRect(current->x, current->y, 2, 2, SSD1306_WHITE);
      current = current->next;
    }
  }
  
  void drawFood() {
    display.fillRect(foodX, foodY, 2, 2, SSD1306_WHITE);
  }
  
  void drawScore() {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(SCREEN_WIDTH - 20, 0);
    display.print(score);
  }
  
  void clearSnake() {
    Segment* current = head;
    while (current != nullptr) {
      Segment* next = current->next;
      delete current;
      current = next;
    }
    head = nullptr;
  }

public:
  SnakeGame(int upPin, int downPin, int selectPin) : 
    display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
    btnUpPin(upPin), btnDownPin(downPin), btnSelectPin(selectPin) {
    head = nullptr;
    direction = 1; // Começa indo para a direita
    score = 0;
    gameOver = false;
    updateDelay = 150;
    lastUpdate = 0;
  }
  
  ~SnakeGame() {
    clearSnake();
  }
  
  void begin() {
    // Inicializa o display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    
    // Configura os pinos dos botões
    pinMode(btnUpPin, INPUT_PULLUP);
    pinMode(btnDownPin, INPUT_PULLUP);
    pinMode(btnSelectPin, INPUT_PULLUP);
    
    // Inicializa a cobra com 3 segmentos
    head = new Segment;
    head->x = 10;
    head->y = 16;
    head->next = nullptr;
    
    addSegment();
    addSegment();
    
    generateFood();
    
    display.display();
    delay(1000);
  }
  
  void update() {
    if (gameOver) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(20, 10);
      display.print(F("Game Over!"));
      display.setCursor(30, 20);
      display.print(F("Score: "));
      display.print(score);
      display.display();
      
      // Aguarda botão para reiniciar
      while (digitalRead(btnSelectPin) == HIGH) {
        delay(10);
      }
      reset();
      return;
    }
    
    // Verifica input dos botões
    if (digitalRead(btnUpPin) == LOW && direction != 2) {
      direction = 0;
    } else if (digitalRead(btnDownPin) == LOW && direction != 0) {
      direction = 2;
    } else if (digitalRead(btnSelectPin) == LOW && direction != 3) {
      direction = 1;
    }
    
    // Atualiza a posição da cobra no tempo certo
    unsigned long currentTime = millis();
    if (currentTime - lastUpdate > updateDelay) {
      moveSnake();
      lastUpdate = currentTime;
      
      // Desenha o jogo
      display.clearDisplay();
      drawSnake();
      drawFood();
      drawScore();
      display.display();
    }
  }
  
  void reset() {
    clearSnake();
    head = new Segment;
    head->x = 10;
    head->y = 16;
    head->next = nullptr;
    
    addSegment();
    addSegment();
    
    direction = 1;
    score = 0;
    gameOver = false;
    updateDelay = 150;
    
    generateFood();
  }
  
  bool isGameOver() {
    return gameOver;
  }
};

#endif