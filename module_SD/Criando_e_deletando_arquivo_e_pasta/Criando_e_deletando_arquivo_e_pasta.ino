/*
Exemplo que cria e deleta pastas e um arquivo no cartão micro SD.

www.bitabittecnologia.com.br
*/

#include <SPI.h>
#include <SD.h>

#define CS 4 //Chip Select.

File arquivo;

void setup() 
{
   Serial.begin(9600);

  //Inicializa cartão.
  if(!SD.begin(CS))
  {
    Serial.println("Não foi possível acessar o cartão.");
    Serial.println("Verifique se o cartão está inserido corretamente.");
    Serial.println("Verifique as conexões no módulo e o Arduino.");
    
    return;
  }
  else
  {
    Serial.println("Cartão inicializado corretamente!!!");
  }

  Serial.println();

  //Verifica se a pasta existe, caso não, cria a pasta.
  bool pasta_existe = SD.exists("Pasta1/Pasta1.1");

  if(pasta_existe)
  {
    Serial.println("Pasta existe");
  }
  else
  {
    Serial.println("Pasta não existe");
    Serial.println("Criando pasta");
    
    SD.mkdir("Pasta1/Pasta1.1");
  }

//  Serial.println();
//
//  //Verifica se o arquivo existe, caso não, cria o arquivo.
//  bool arquivo_existe = SD.exists("Pasta1/Pasta1.1/teste.txt");
//
//  if(arquivo_existe)
//  {
//    Serial.println("Arquivo existe");
//  }
//  else
//  {
//    Serial.println("Arquivo não existe");
//    Serial.println("Criando arquivo");
//
//    //Caso o arquivo não exista, ele é criado e aberto para escrita.
//    arquivo = SD.open("Pasta1/Pasta1.1/teste.txt", FILE_WRITE); 
//    arquivo.close();
//  }

   Serial.println();

   //Exclui o arquivo e pastas.
   Serial.println("Excluindo o arquivo");
   SD.remove("Pasta1/Pasta1.1/teste.txt");

   Serial.println();

   Serial.println("Excluindo as pastas");
   SD.rmdir("Pasta1/Pasta1.1");
   SD.rmdir("Pasta1");
}

void loop() 
{

}
