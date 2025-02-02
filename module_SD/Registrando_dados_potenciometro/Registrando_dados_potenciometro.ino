/*
Exemplo que obtém os dados de dois potenciômetros e armazena estes dados 
em um arquivo no cartão micro SD.

www.bitabittecnologia.com.br
*/

#include <SPI.h>
#include <SD.h>

#define CS 4 //Chip Select.

File arquivo;

int pot1;
int pot2;

void setup() 
{
  Serial.begin(9600);

  //Inicializa cartão.
  if(!SD.begin(CS))
  {
    Serial.println("Não foi possível acessar o cartão.");
    Serial.println("Verifique se o cartão está inserido corretamente.");
    Serial.println("Verifique as conexões no módulo e o Arduino.");
    
    while(1); //Evita executar o loop quando der erro.
  }
  else
  {
    Serial.println("Cartão inicializado corretamente!!!");
  }

  Serial.println();

}

void loop() 
{
  String dados = "";
  
  pot1 = analogRead(0);
  pot2 = analogRead(1);

  dados = "POT 1: " + String(pot1) + " | POT2: " + String(pot2); //Prepara a string com os dados a serem gravados

  //Cria o arquivo para inserir os dados, caso o arquivo já exista
  //os dados vão ser inseridos nele.
  arquivo = SD.open("dados.txt", FILE_WRITE);
  if(arquivo)
  {
    Serial.print("POT 1: ");
    Serial.print(pot1);
    
    Serial.print(" | POT 2: ");
    Serial.println(pot2);

    arquivo.println(dados); //Grava no arquivo.
    arquivo.close();
  }
  else
  {
    Serial.println("Erro ao abrir o arquivo");
  }

  delay(20);
}
