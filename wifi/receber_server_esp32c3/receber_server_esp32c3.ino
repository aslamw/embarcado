#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configurações da tela OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1 // Reset pin (ou -1 se não houver)
#define SCREEN_ADDRESS 0x3C // Endereço I2C da tela OLED

#define SDA_PIN 6
#define SCL_PIN 7

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "ESP32_AP"; // Nome da rede Wi-Fi criada pelo cliente
const char* password = "12345678"; // Senha da rede Wi-Fi

void print_text(const char* text, int X, int Y) {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(X, Y);
  display.print(text);
  display.display();
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Inicializa a tela OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha ao inicializar a tela OLED"));
    for (;;); // Trava o programa se a tela não inicializar
  }
  display.display(); // Exibe o buffer inicial
  delay(2000); // Pausa por 2 segundos
  display.clearDisplay(); // Limpa a tela

  // Conecta ao Wi-Fi criado pelo cliente
  Serial.println("Conectando ao Wi-Fi...");
  print_text("Conectando ao Wi-Fi...", 10, 10);

  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    tentativas++;
    if (tentativas > 10) { // Limite de 10 tentativas
      Serial.println("\nFalha ao conectar ao Wi-Fi. Verifique SSID e senha.");
      print_text("Falha ao conectar ao Wi-Fi", 10, 10);
      return; // Sai do setup se não conseguir conectar
    }
  }

  Serial.println("\nConectado ao Wi-Fi!");
  Serial.print("IP do servidor: ");
  Serial.println(WiFi.localIP());
  print_text("Conectado ao Wi-Fi", 10, 10);
}

void loop() {
  // Cria uma conexão TCP com o servidor
  WiFiClient client;
  if (!client.connect("192.168.4.1", 8080)) { // IP do AP (cliente)
    Serial.println("Falha na conexão com o servidor");
    print_text("Falha na conexão com o servidor", 10, 10);
    delay(5000);
    return;
  }

  Serial.println("Conectado ao servidor!");

  // Aguarda a mensagem do servidor
  while (client.connected()) {
    if (client.available()) {
      String frase = client.readString();
      Serial.println("Frase recebida: " + frase);

      // Exibe a frase na tela OLED
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Mensagem:");
      display.println(frase);
      display.display();
    }
  }

  // Fecha a conexão
  client.stop();
  Serial.println("Conexão encerrada");
}