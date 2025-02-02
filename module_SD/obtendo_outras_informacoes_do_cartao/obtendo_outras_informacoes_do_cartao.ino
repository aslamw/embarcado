/*
Exemplo para obter algumas informações do cartão micro SD e exibir na serial.

www.bitabittecnologia.com.br
*/

#include <SPI.h>
#include <SD.h>

#define CS 4 //Chip Select.

Sd2Card cartao;

void setup() 
{
  Serial.begin(9600);

  //Inicializa o cartão com a metade do clock SPI.
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

  Serial.println();

  //Exibe os dados na serial.
  cid_t cid;
  cartao.readCID(&cid);
  
  Serial.println("ID de fabricação: " );
  Serial.println(cid.mid);

  Serial.println();
  
  Serial.println("OEM/ID de aplicação: " );
  Serial.println(cid.oid);

  Serial.println();
  
  Serial.println("Nome do produto: " );
  Serial.println(cid.pnm);

  Serial.println();
  
  Serial.println("Revisão do produto n.m: ");
  Serial.println(cid.prv_m);
  Serial.println(cid.prv_n);

  Serial.println();
  
  Serial.println("Número serial do produto: " );
  Serial.println(cid.psn);

  Serial.println();
  
  Serial.println("Data de fabricação: " );
  Serial.println(cid.mdt_year_high);
  Serial.println(cid.reserved);
  Serial.println(cid.mdt_month);
  Serial.println(cid.mdt_year_low);
  Serial.println(cid.always1);
  Serial.println(cid.crc);
}

void loop() 
{

}
