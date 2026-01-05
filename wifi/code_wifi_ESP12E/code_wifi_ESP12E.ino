#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <LittleFS.h>

// Configurações
#define OTA_PORT 8266
const char* AP_SSID = "ESP8266-Config";
const char* AP_PASSWORD = "12345678";

// Variáveis globais
ESP8266WebServer server(80);

// HTML compacto
const char MAIN_PAGE[] PROGMEM = R"rawliteral(
<html><body>
<h1>ESP8266 Sistema</h1>
<p>IP: %IP%</p>
<p><a href='/upload'>Upload Files</a> | <a href='/list'>List Files</a></p>
</body></html>
)rawliteral";

void setup() {
    Serial.begin(115200);
    Serial.println("\n🚀 Iniciando ESP8266 Otimizado");
    
    // Filesystem
    if (!LittleFS.begin()) LittleFS.begin();
    
    // WiFi
    connectWiFi();
    
    // OTA
    setupOTA();
    
    // Servidor Web
    setupWebServer();
    
    Serial.println("✅ Pronto: " + WiFi.localIP().toString());
}

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin("SUA_REDE", "SENHA");
    
    for(int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_AP);
        WiFi.softAP(AP_SSID, AP_PASSWORD);
        Serial.println("\n📡 Modo AP: " + String(AP_SSID));
    } else {
        Serial.println("\n✅ WiFi: " + WiFi.localIP().toString());
    }
}

void setupOTA() {
    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.begin();
}

void setupWebServer() {
    // Página principal
    server.on("/", []() {
        String html = FPSTR(MAIN_PAGE);
        html.replace("%IP%", WiFi.localIP().toString());
        server.send(200, "text/html", html);
    });
    
    // Upload
    server.on("/upload", HTTP_GET, []() {
        server.send(200, "text/html", 
            "<form method='POST' enctype='multipart/form-data'>"
            "<input type='file' name='data' multiple>"
            "<button>Upload</button></form>"
        );
    });
    
    server.on("/upload", HTTP_POST, []() {
        server.send(200, "text/plain", "OK");
    }, []() {
        HTTPUpload& upload = server.upload();
        if (upload.status == UPLOAD_FILE_START) {
            String filename = upload.filename;
            if (!filename.startsWith("/")) filename = "/" + filename;
            Serial.println("📤 " + filename);
        }
    });
    
    // Listar arquivos
    server.on("/list", []() {
        String json = "[";
        File root = LittleFS.open("/", "r");
        File file = root.openNextFile();
        bool first = true;
        while (file) {
            if (!first) json += ",";
            json += "{\"name\":\"" + String(file.name()) + "\",\"size\":" + String(file.size()) + "}";
            first = false;
            file = root.openNextFile();
        }
        json += "]";
        server.send(200, "application/json", json);
    });
    
    server.begin();
}

void loop() {
    server.handleClient();
    ArduinoOTA.handle();
    delay(10);
}