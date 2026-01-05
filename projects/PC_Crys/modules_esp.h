#ifndef MODULES_ESP_H
#define MODULES_ESP_H



int module_heart(){
  const int sensorBPM = 4;

  pinMode(sensorBPM, INPUT);

  // Variáveis globais para o cálculo do BPM
  volatile long lastBeat = 0;   // Armazena o tempo do último batimento
  const int THRESHOLD = 500;    // Limite para detectar um pulso (ajuste conforme o seu sensor)

  // Lê o valor analógico do sensor
  int sensorValue = analogRead(sensorBPM);
  
  // Lógica para encontrar o pico do pulso
  if (sensorValue > THRESHOLD) {
    long currentTime = millis();

    // Evita múltiplos picos em um único batimento, garantindo que o tempo
    // entre os batimentos seja de pelo menos 250ms
    if ((currentTime - lastBeat) > 250) { 
      // Um batimento foi detectado
      
      // Calcula o tempo desde o último batimento
      float beatPeriod = (currentTime - lastBeat);
      
      // Atualiza o tempo do último batimento
      lastBeat = currentTime;

      // Calcula o BPM
      int bpm = (int)(60000 / beatPeriod);
      
      // Retorna o valor do BPM
      return bpm;
    }
  }

  // Retorna -1 se nenhum batimento válido foi detectado
  return -1; 

}

#endif 