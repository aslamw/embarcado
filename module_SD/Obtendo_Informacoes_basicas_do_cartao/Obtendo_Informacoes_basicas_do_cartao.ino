/*
Exemplo para obter algumas informações do cartão micro SD e exibir na serial.

www.bitabittecnologia.com.br
*/

#include <SPI.h>
#include <SD.h>

#define CS 4 //Chip Select.

Sd2Card cartao;
SdVolume volume;
SdFile arquivo;

void setup() 
{
  Serial.begin(9600);

  //Inicializa o cartão com a metade do clock SPI.
  //caso não for possível inicializar, a mensagem avisando será exibida.
  //Caso inicializar corretamente a mensagem de sucesso será exibida.
  //Utiliza o Sd2Card.
  if(!cartao.init(SPI_HALF_SPEED, CS))
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

  //Verifica o tipo do cartão e exibe.
  Serial.println();
  Serial.print("Este cartão é do tipo: ");

  switch (cartao.type()) 
  {
    case SD_CARD_TYPE_SD1:
      Serial.print("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.print("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.print("SDHC");
      break;
    default:
      Serial.print("Cartão desconhecido");
  }

  Serial.println();

  //Obtém informações do sistema de arquivo, clusters e blocos.
  //Utiliza o SdVolume passando Sd2Card como argumento.
  //Caso não encontre um sistema de arquivos compatível, uma mensagem é exibida.
  if(!volume.init(cartao))
  {
    Serial.println();
    Serial.println("Não foi possível acessar o volume.");
    Serial.println("Verifique se o cartão está formatado com o sistema de arquivos FAT16 ou FAT32.");

    return;
  }

  //Verifica o tipo do sistema de arquivos.
  Serial.print("Este cartão está formatado com o tipo: FAT");
  Serial.println(volume.fatType());

  //Verifica a quantidade de clusters e blocos por clusters.
  Serial.print("Quantidade de clusters: ");
  Serial.println(volume.clusterCount());

  Serial.print("Quantidade de blocos em cada cluster: ");
  Serial.println(volume.blocksPerCluster());

  //Verifica a capacidade de armazenamento do cartão.
  //Cada cluster possui uma certa quantidade de blocos.
  //Cada bloco dos cartões de memória sempre tem tamanho de 512 bytes.
  //Para saber a quantidade em Kb, temos que dividir o total de blocos por 2.
  //Cada 2 blocos têm 1KB.
  uint32_t tamanho_armazenamento;
  tamanho_armazenamento = volume.blocksPerCluster();
  tamanho_armazenamento *= volume.clusterCount();
  tamanho_armazenamento /= 2;

  Serial.println();
  Serial.print("Tamanho do armazenamento (KB):  ");
  Serial.println(tamanho_armazenamento);

  tamanho_armazenamento /= 1024;
  Serial.print("Tamanho do armazenamento (MB):  ");
  Serial.println(tamanho_armazenamento);
  
  Serial.print("Tamanho do armazenamento (GB):  ");
  Serial.println((float)tamanho_armazenamento / 1024.0);

  //Lista todos os arquivos exibindo o nome, data e tamanho em bytes.
  //Utiliza o SdFile e passa SdVolume como argumento.
  Serial.println();
  Serial.println("Arquivos NOME   DATA   TAMANHO EM BYTES: ");
  arquivo.openRoot(volume);
  arquivo.ls(LS_R | LS_DATE | LS_SIZE);
}

void loop() 
{

}
