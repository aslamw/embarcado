#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Configurações do ponto de acesso Wi-Fi
const char* ssid = "ESP32_Servo_Control";
const char* password = "12345678";

// Pino do servo motor
const int servoPin = 13;

// Criação dos objetos
WebServer server(80);
Servo myservo;

// Variáveis para controle do servo
int servoPos = 90;
bool rotatingCW = false;  // Rotação no sentido horário
bool rotatingCCW = false; // Rotação no sentido anti-horário
unsigned long lastRotationTime = 0;
const int rotationDelay = 15; // Atraso entre movimentos (ms)

// Página HTML atualizada
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Controle de Servo Motor</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 20px;
      background-color: #f4f4f4;
    }
    .container {
      max-width: 400px;
      margin: 0 auto;
      background: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
    }
    .slider-container {
      margin: 30px 0;
    }
    .slider {
      width: 100%;
      height: 15px;
      -webkit-appearance: none;
      appearance: none;
      background: #ddd;
      outline: none;
      border-radius: 10px;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #4CAF50;
      cursor: pointer;
    }
    .value-display {
      font-size: 24px;
      margin: 10px 0;
    }
    .btn {
      background-color: #4CAF50;
      color: white;
      border: none;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 5px;
    }
    .btn-rotate {
      background-color: #2196F3;
    }
    .btn-stop {
      background-color: #f44336;
    }
    .btn-group {
      margin: 15px 0;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Controle de Servo Motor</h1>
    <div class="slider-container">
      <input type="range" min="0" max="180" value="90" class="slider" id="servoSlider" oninput="updateServo(this.value)">
      <div class="value-display">Posição: <span id="servoValue">90</span>°</div>
    </div>
    
    <div class="btn-group">
      <button class="btn" onclick="moveServo(0)">0°</button>
      <button class="btn" onclick="moveServo(90)">90°</button>
      <button class="btn" onclick="moveServo(180)">180°</button>
    </div>
    
    <div class="btn-group">
      <button class="btn btn-rotate" onclick="rotateServo('cw')">Girar Horário</button>
      <button class="btn btn-rotate" onclick="rotateServo('ccw')">Girar Anti-Horário</button>
      <button class="btn btn-stop" onclick="stopRotation()">Parar</button>
    </div>
  </div>

  <script>
    function updateServo(pos) {
      document.getElementById("servoValue").innerHTML = pos;
      fetch("/servo?value=" + pos)
        .then(response => {
          console.log("Posição atualizada para " + pos + "°");
        });
    }
    
    function moveServo(pos) {
      document.getElementById("servoSlider").value = pos;
      updateServo(pos);
      // Envia comando para parar rotação contínua
      fetch("/rotate?dir=stop");
    }
    
    function rotateServo(direction) {
      fetch("/rotate?dir=" + direction)
        .then(response => {
          console.log("Comando de rotação: " + direction);
        });
    }
    
    function stopRotation() {
      rotateServo('stop');
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  
  // Configura o servo motor
  ESP32PWM::allocateTimer(0);
  myservo.setPeriodHertz(50);  // Standard 50hz servo
  myservo.attach(servoPin, 500, 2400); // Ajuste esses valores se necessário
  myservo.write(servoPos);
  
  // Cria o ponto de acesso Wi-Fi
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Endereço IP do AP: ");
  Serial.println(IP);
  
  // Configura as rotas do servidor web
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", htmlPage);
  });
  
  server.on("/servo", HTTP_GET, []() {
    if (server.hasArg("value")) {
      rotatingCW = false;
      rotatingCCW = false;
      servoPos = server.arg("value").toInt();
      myservo.write(servoPos);
      Serial.print("Posição do servo atualizada para: ");
      Serial.println(servoPos);
    }
    server.send(200, "text/plain", "OK");
  });
  
  server.on("/rotate", HTTP_GET, []() {
    if (server.hasArg("dir")) {
      String direction = server.arg("dir");
      if (direction == "cw") {
        rotatingCW = true;
        rotatingCCW = false;
        Serial.println("Girando no sentido horário");
      } else if (direction == "ccw") {
        rotatingCW = false;
        rotatingCCW = true;
        Serial.println("Girando no sentido anti-horário");
      } else if (direction == "stop") {
        rotatingCW = false;
        rotatingCCW = false;
        Serial.println("Parando rotação");
      }
    }
    server.send(200, "text/plain", "OK");
  });
  
  // Inicia o servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
  
  // Controle da rotação contínua
  if (rotatingCW || rotatingCCW) {
    if (millis() - lastRotationTime >= rotationDelay) {
      if (rotatingCW && servoPos < 180) {
        servoPos++;
      } else if (rotatingCCW && servoPos > 0) {
        servoPos--;
      }
      
      myservo.write(servoPos);
      lastRotationTime = millis();
    }
  }
}