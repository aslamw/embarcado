/*
Exemplo que cria um arquivo no catão micro SD, insere dados e lê o mesmo.

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

  //Cria e insere dados dentro do arquivo.
  //Caso o arquivo exista ele irá ser aberto.
  arquivo = SD.open("teste.txt", FILE_WRITE);
  if(arquivo)
  {
    Serial.println("Escrevendo no arquivo...");
    
    arquivo.println("Teste de escrita em arquivo no cartão micro SD");
    arquivo.print("Teste de escrita");
    arquivo.println(", inserindo dados");

    arquivo.close();

    Serial.println("Teste finalizado");
  }
  else
  {
    Serial.println("Erro ao abrir o arquivo para gravar");
  }

  Serial.println();

  //Abre o arquivo e exibe os dados na serial.
  arquivo = SD.open("teste.txt");
  if(arquivo)
  {
    Serial.print("Lendo o arquivo: ");
    Serial.println(arquivo.name());

    Serial.print("Tamanho do arquivo em bytes: ");
    Serial.println(arquivo.size());
    Serial.println("Conteúdo do arquivo: ");

    while(arquivo.available())
    {
      Serial.write(arquivo.read()); //Lê caractere por caractere e exibe na serial.
    }

    arquivo.close();
  }
  else
  {
    Serial.println("Erro ao abrir o arquivo para ler");
  }
}

void loop() 
{
  
}
