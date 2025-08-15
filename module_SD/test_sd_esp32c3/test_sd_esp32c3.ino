#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Define o pino CS do cartão SD
#define SD_CS 5

void setup() {
  Serial.begin(9600);

  // Inicializa o cartão SD com o pino CS definido
  if (!SD.begin(SD_CS)) {
    Serial.println("Falha na inicialização do cartão SD!");
    while (1);
  }
  Serial.println("Cartão SD inicializado com sucesso!");

  // Exemplo: Criar e escrever em um arquivo no cartão SD
  File arquivo = SD.open("/teste.txt", FILE_WRITE);
  if (arquivo) {
    arquivo.println("Olá, mundo!");
    arquivo.close();
    Serial.println("Arquivo escrito com sucesso!");
  } else {
    Serial.println("Erro ao abrir o arquivo!");
  }
}

void loop() {
  // Seu código principal aqui
}