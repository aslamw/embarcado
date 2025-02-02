/*
Exemplo que lê um arquivo no cartão micro SD e de acordo com os dados do arquivo acende ou apaga os leds.

Exemplo do conteúdo do arquivo:
vermelho acender
verde apagar

Para funcionar corretamente deve-se dar um enter depois da última linha,
no caso do exemplo acima depois da linha "verde apagar"

www.bitabittecnologia.com.br
*/

#include <SPI.h>
#include <SD.h>

#define CS 4 //Chip Select.

#define LED_VERMELHO 6
#define LED_VERDE 7

const int quantidade_dados = 2; //O arquivo irá conter 2 linhas de dados, caso tiver mais ou menos alterar aqui.

File arquivo;

//Método para abstrair os dados e de acordo com os dados apagar ou acender os leds.
void setarLed(String dados[quantidade_dados])
{
  String cor_led;
  String estado_led;

  int led;
  int estado;
  
  for(int i=0; i<quantidade_dados; i++)
  {
    led = 0;
    estado = false;
    
    int indice = dados[i].indexOf(' ');
    int tamanho = dados[i].length();

    cor_led = dados[i].substring(0, indice); //Obtém a cor na linha de dados.
    
    //indice+1 elimina o espaço e tamanho-1 elimina a quebra de linha
    //lembrando, no arquivo dar um enter na última entrada para criar a quebra de linha.
    estado_led = dados[i].substring(indice+1, tamanho-1); //Obtém o estado na linha de dados.

    if(cor_led == "vermelho")
    {
      led = LED_VERMELHO;
    }
    else
    {
      led = LED_VERDE;
    }

    if(estado_led == "acender")
    {
      estado = HIGH;
    }
    else
    {
      estado = LOW;
    }

    digitalWrite(led, estado);
  }
}

void setup() 
{
  Serial.begin(9600);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);

  String dados[quantidade_dados]; //Vetor para armazenar cada linha do arquivo.

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

  //Abre o arquivo para leitura.
  arquivo = SD.open("dados.txt");
  if(arquivo)
  {
    char caractere;
    int controle = 0; 
    
    //Organiza cada linha do arquivo em um vetor de String.
    while(arquivo.available())
    {
      caractere = arquivo.read();
      if(caractere == '\n')
      {
        controle++;
      }
      else
      {
        dados[controle] += caractere;
      }
    }
    setarLed(dados);
  }
  else
  {
    Serial.println("Erro ao abrir o arquivo");
  }
}

void loop() 
{

}
