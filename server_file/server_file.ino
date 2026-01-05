#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SD.h>
#include <SPI.h>

// Configurações do Access Point
const char* ssid = "ESP12E-FileServer";
const char* password = "12345678";

ESP8266WebServer server(80);
File uploadFile;

// Variáveis para medição de progresso
unsigned long uploadStartTime = 0;
unsigned long totalUploadSize = 0;
unsigned long uploadedBytes = 0;
bool isUploading = false;

void setup() {
  Serial.begin(115200);
  
  // Inicializa o SD card
  Serial.println("Inicializando SD card...");
  if (!SD.begin(D8)) {
    Serial.println("Falha na inicialização do SD card!");
    return;
  }
  Serial.println("SD card inicializado.");
  
  // Cria diretório de uploads se não existir
  if (!SD.exists("/uploads")) {
    SD.mkdir("/uploads");
  }
  
  // Configura o Access Point
  WiFi.softAP(ssid, password);
  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Configura as rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/upload", HTTP_GET, handleUploadForm);
  server.on("/upload", HTTP_POST, handleUploadSuccess, handleFileUpload);
  server.on("/files", HTTP_GET, handleFileList);
  server.on("/view", HTTP_GET, handleViewFile);
  server.on("/download", HTTP_GET, handleDownloadFile);
  server.on("/delete", HTTP_GET, handleDeleteFile);
  server.on("/progress", HTTP_GET, handleProgress);
  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

// Página principal
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>ESP12E File Server</title>";
  html += "<style>body{font-family:Arial,sans-serif;margin:40px;background:#f0f0f0;}";
  html += ".card{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);margin:10px;}";
  html += "a{text-decoration:none;color:#007bff;margin:5px;display:inline-block;padding:10px 15px;background:#007bff;color:white;border-radius:5px;}";
  html += "a:hover{background:#0056b3;}</style></head><body>";
  html += "<h1>ESP12E File Server</h1>";
  html += "<div class='card'><h2>Opções</h2>";
  html += "<a href='/upload'>Enviar Arquivo</a>";
  html += "<a href='/files'>Ver Arquivos</a>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

// Formulário de upload com JavaScript para acompanhar progresso
void handleUploadForm() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>Upload de Arquivo</title>";
  html += "<style>body{font-family:Arial,sans-serif;margin:40px;background:#f0f0f0;}";
  html += ".card{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}";
  html += "input,button{padding:10px;margin:5px;}";
  html += ".progress{width:100%;background:#f0f0f0;border-radius:5px;margin:10px 0;}";
  html += ".progress-bar{height:20px;background:#007bff;border-radius:5px;width:0%;transition:width 0.3s;}";
  html += ".progress-info{margin:10px 0;}</style>";
  
  // JavaScript para acompanhar o progresso
  html += "<script>";
  html += "function formatBytes(bytes) {";
  html += "  if (bytes < 1024) return bytes + ' B';";
  html += "  else if (bytes < 1048576) return (bytes / 1024).toFixed(1) + ' KB';";
  html += "  else return (bytes / 1048576).toFixed(1) + ' MB';";
  html += "}";
  html += "function updateProgress() {";
  html += "  var xhr = new XMLHttpRequest();";
  html += "  xhr.open('GET', '/progress', true);";
  html += "  xhr.onreadystatechange = function() {";
  html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
  html += "      var data = JSON.parse(xhr.responseText);";
  html += "      if (data.uploading) {";
  html += "        document.getElementById('progressBar').style.width = data.percentage + '%';";
  html += "        document.getElementById('progressText').innerHTML = 'Progresso: ' + data.percentage + '%';";
  html += "        document.getElementById('uploaded').innerHTML = 'Enviado: ' + formatBytes(data.uploaded) + ' de ' + formatBytes(data.total);";
  html += "        document.getElementById('timeLeft').innerHTML = 'Tempo restante: ' + data.timeLeft;";
  html += "        document.getElementById('speed').innerHTML = 'Velocidade: ' + data.speed;";
  html += "        setTimeout(updateProgress, 500);";
  html += "      } else {";
  html += "        document.getElementById('progressText').innerHTML = 'Upload concluído!';";
  html += "        setTimeout(function() { window.location='/'; }, 2000);";
  html += "      }";
  html += "    }";
  html += "  };";
  html += "  xhr.send();";
  html += "}";
  html += "function startUpload() {";
  html += "  document.getElementById('uploadForm').style.display = 'none';";
  html += "  document.getElementById('progressContainer').style.display = 'block';";
  html += "  setTimeout(updateProgress, 500);";
  html += "}";
  html += "</script>";
  html += "</head><body>";
  html += "<div class='card'><h2>Upload de Arquivo</h2>";
  html += "<div id='uploadForm'>";
  html += "<form method='POST' action='/upload' enctype='multipart/form-data' onsubmit='startUpload()'>";
  html += "<input type='file' name='file' required><br>";
  html += "<button type='submit'>Enviar Arquivo</button>";
  html += "</form></div>";
  
  // Container para mostrar o progresso
  html += "<div id='progressContainer' style='display:none'>";
  html += "<h3>Enviando arquivo...</h3>";
  html += "<div class='progress'><div class='progress-bar' id='progressBar'></div></div>";
  html += "<div class='progress-info'>";
  html += "<p id='progressText'>Progresso: 0%</p>";
  html += "<p id='uploaded'>Enviado: 0 B</p>";
  html += "<p id='speed'>Velocidade: 0 KB/s</p>";
  html += "<p id='timeLeft'>Tempo restante: calculando...</p>";
  html += "</div></div>";
  
  html += "<br><a href='/'>Voltar</a></div></body></html>";
  
  server.send(200, "text/html", html);
}

// Upload de arquivo - todos os arquivos vão para /uploads
void handleFileUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    // Garante que o arquivo seja salvo no diretório uploads
    if (!filename.startsWith("/uploads/")) {
      filename = "/uploads/" + filename;
    }
    uploadFile = SD.open(filename, "w");
    Serial.println("Upload iniciado: " + filename);
    
    // Inicializa variáveis de medição
    uploadStartTime = millis();
    totalUploadSize = upload.totalSize;
    uploadedBytes = 0;
    isUploading = true;
  } 
  else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
      uploadedBytes += upload.currentSize;
    }
  } 
  else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.println("Upload completo. Total de bytes: " + String(uploadedBytes));
      
      // Reseta variáveis de medição
      uploadStartTime = 0;
      totalUploadSize = 0;
      uploadedBytes = 0;
      isUploading = false;
    }
  }
  else if (upload.status == UPLOAD_FILE_ABORTED) {
    isUploading = false;
    Serial.println("Upload abortado");
  }
}

// Retorna informações de progresso em JSON
void handleProgress() {
  String json = "{";
  
  if (isUploading && totalUploadSize > 0) {
    int percentage = (uploadedBytes * 100) / totalUploadSize;
    unsigned long elapsedTime = millis() - uploadStartTime;
    float uploadSpeed = (elapsedTime > 0) ? (uploadedBytes / (elapsedTime / 1000.0)) : 0;
    
    // Calcula tempo restante
    unsigned long remainingBytes = totalUploadSize - uploadedBytes;
    int secondsLeft = (uploadSpeed > 0) ? (remainingBytes / uploadSpeed) : 0;
    
    // Formata velocidade e tempo
    String speedStr = String(uploadSpeed / 1024, 1) + " KB/s";
    String timeLeftStr = formatTime(secondsLeft);
    
    json += "\"uploading\": true,";
    json += "\"percentage\": " + String(percentage) + ",";
    json += "\"uploaded\": " + String(uploadedBytes) + ",";
    json += "\"total\": " + String(totalUploadSize) + ",";
    json += "\"speed\": \"" + speedStr + "\",";
    json += "\"timeLeft\": \"" + timeLeftStr + "\"";
  } else {
    json += "\"uploading\": false";
  }
  
  json += "}";
  
  server.send(200, "application/json", json);
}

// Formata segundos em string legível
String formatTime(int seconds) {
  if (seconds <= 0) {
    return "calculando...";
  } else if (seconds < 60) {
    return String(seconds) + " segundos";
  } else if (seconds < 3600) {
    int minutes = seconds / 60;
    int secs = seconds % 60;
    return String(minutes) + " min " + String(secs) + " seg";
  } else {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    return String(hours) + " h " + String(minutes) + " min";
  }
}

void handleUploadSuccess() {
  server.send(200, "text/html", "<script>alert('Upload realizado com sucesso!'); window.location='/';</script>");
}

// Listar arquivos organizados por categoria
void handleFileList() {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<title>Arquivos no SD</title>";
  html += "<style>body{font-family:Arial,sans-serif;margin:40px;background:#f0f0f0;}";
  html += ".card{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);margin:10px 0;}";
  html += "a{text-decoration:none;color:#007bff;margin:2px;display:inline-block;padding:5px 10px;background:#007bff;color:white;border-radius:3px;}";
  html += "a:hover{background:#0056b3;} .file-item{margin:10px 0;padding:10px;border-bottom:1px solid #eee;}";
  html += ".category{margin-top:20px;padding:10px;background:#e9ecef;border-radius:5px;}";
  html += "</style></head><body>";
  html += "<h1>Arquivos no Cartão SD</h1>";
  html += "<div class='card'>";
  
  // Lista todos os arquivos do diretório uploads organizados por tipo
  listFilesByCategory("/uploads", html);
  
  html += "</div><br><a href='/'>Voltar</a></body></html>";
  
  server.send(200, "text/html", html);
}

// Lista arquivos organizados por categoria
void listFilesByCategory(String path, String &html) {
  File dir = SD.open(path);
  
  if (!dir) {
    html += "<p>Erro ao abrir diretório: " + path + "</p>";
    return;
  }
  
  if (!dir.isDirectory()) {
    html += "<p>" + path + " não é um diretório</p>";
    dir.close();
    return;
  }
  
  // Categorias de arquivos
  struct FileCategory {
    String name;
    String icon;
    String extensions[10];
    int count;
  };
  
  FileCategory categories[] = {
    {"Imagens", "🖼️", {".jpg", ".jpeg", ".png", ".gif", ".bmp"}, 5},
    {"Vídeos", "🎬", {".mp4", ".avi", ".mov", ".mkv"}, 4},
    {"Músicas", "🎵", {".mp3", ".wav", ".ogg", ".flac"}, 4},
    {"Documentos", "📄", {".pdf", ".txt", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx"}, 8},
    {"Outros", "📁", {}, 0}
  };
  
  const int numCategories = 5;
  
  // Arrays para armazenar arquivos por categoria
  String categoryFiles[numCategories] = {""};
  int fileCounts[numCategories] = {0};
  
  // Processa todos os arquivos
  File file = dir.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      String filename = String(file.name());
      String fileExt = getFileExtension(filename);
      
      // Encontra a categoria do arquivo
      int categoryIndex = numCategories - 1; // Inicia com "Outros"
      
      for (int i = 0; i < numCategories - 1; i++) {
        for (int j = 0; j < categories[i].count; j++) {
          if (fileExt.equals(categories[i].extensions[j])) {
            categoryIndex = i;
            break;
          }
        }
        if (categoryIndex != numCategories - 1) break;
      }
      
      // Adiciona o arquivo à categoria correspondente
      categoryFiles[categoryIndex] += "<div class='file-item'>";
      categoryFiles[categoryIndex] += categories[categoryIndex].icon + " " + filename;
      categoryFiles[categoryIndex] += " (" + String(file.size()) + " bytes) ";
      
      // Links para visualizar, baixar e deletar
      categoryFiles[categoryIndex] += "<a href='/view?file=" + String(file.name()) + "' target='_blank'>Visualizar</a>";
      categoryFiles[categoryIndex] += "<a href='/download?file=" + String(file.name()) + "'>Baixar</a>";
      categoryFiles[categoryIndex] += "<a href='/delete?file=" + String(file.name()) + "' onclick='return confirm(\"Tem certeza?\")'>Deletar</a>";
      
      categoryFiles[categoryIndex] += "</div>";
      fileCounts[categoryIndex]++;
    }
    file = dir.openNextFile();
  }
  
  dir.close();
  
  // Exibe as categorias com arquivos
  for (int i = 0; i < numCategories; i++) {
    if (fileCounts[i] > 0) {
      html += "<div class='category'>";
      html += "<h3>" + categories[i].name + " (" + String(fileCounts[i]) + ")</h3>";
      html += categoryFiles[i];
      html += "</div>";
    }
  }
  
  if (fileCounts[0] + fileCounts[1] + fileCounts[2] + fileCounts[3] + fileCounts[4] == 0) {
    html += "<p>Nenhum arquivo encontrado no diretório uploads.</p>";
  }
}

// Obtém a extensão do arquivo
String getFileExtension(String filename) {
  int lastDot = filename.lastIndexOf('.');
  if (lastDot == -1) return "";
  return filename.substring(lastDot);
}

// Visualizar arquivo - sempre do diretório uploads
void handleViewFile() {
  String filename = server.arg("file");
  
  if (filename.length() == 0) {
    server.send(400, "text/plain", "Nome do arquivo não especificado");
    return;
  }
  
  // Garante que o arquivo está no diretório uploads
  if (!filename.startsWith("/uploads/")) {
    filename = "/uploads/" + filename;
  }
  
  File file = SD.open(filename, "r");
  if (!file) {
    server.send(404, "text/plain", "Arquivo não encontrado: " + filename);
    return;
  }
  
  String contentType = getContentType(filename);
  
  if (contentType.startsWith("image/") || contentType == "application/pdf") {
    // Para imagens e PDFs, exibe inline
    server.sendHeader("Content-Type", contentType);
    server.streamFile(file, contentType);
  } 
  else if (contentType.startsWith("text/") || contentType.startsWith("audio/")) {
    // Para textos e áudios
    server.sendHeader("Content-Type", contentType);
    server.streamFile(file, contentType);
  }
  else if (contentType.startsWith("video/")) {
    // Para vídeos
    server.setContentLength(file.size());
    server.sendHeader("Content-Type", contentType);
    server.sendHeader("Accept-Ranges", "bytes");
    server.send(200, contentType, "");
    
    // Stream do vídeo
    uint8_t buffer[256];
    size_t bytesRead;
    while ((bytesRead = file.read(buffer, sizeof(buffer))) > 0) {
      server.sendContent((const char*)buffer, bytesRead);
    }
  }
  else {
    // Para outros tipos, força download
    server.sendHeader("Content-Type", "application/octet-stream");
    server.sendHeader("Content-Disposition", "attachment; filename=\"" + getFileNameFromPath(filename) + "\"");
    server.streamFile(file, "application/octet-stream");
  }
  
  file.close();
}

// Baixar arquivo - sempre do diretório uploads
void handleDownloadFile() {
  String filename = server.arg("file");
  
  if (filename.length() == 0) {
    server.send(400, "text/plain", "Nome do arquivo não especificado");
    return;
  }
  
  // Garante que o arquivo está no diretório uploads
  if (!filename.startsWith("/uploads/")) {
    filename = "/uploads/" + filename;
  }
  
  File file = SD.open(filename, "r");
  if (!file) {
    server.send(404, "text/plain", "Arquivo não encontrado: " + filename);
    return;
  }
  
  server.sendHeader("Content-Type", "application/octet-stream");
  server.sendHeader("Content-Disposition", "attachment; filename=\"" + getFileNameFromPath(filename) + "\"");
  server.streamFile(file, "application/octet-stream");
  file.close();
}

// Deletar arquivo - sempre do diretório uploads
void handleDeleteFile() {
  String filename = server.arg("file");
  
  if (filename.length() == 0) {
    server.send(400, "text/plain", "Nome do arquivo não especificado");
    return;
  }
  
  // Garante que o arquivo está no diretório uploads
  if (!filename.startsWith("/uploads/")) {
    filename = "/uploads/" + filename;
  }
  
  if (SD.remove(filename)) {
    server.send(200, "text/html", "<script>alert('Arquivo deletado com sucesso!'); window.location='/files';</script>");
  } else {
    server.send(500, "text/plain", "Erro ao deletar arquivo: " + filename);
  }
}

// Extrai o nome do arquivo do caminho completo
String getFileNameFromPath(String path) {
  int lastSlash = path.lastIndexOf('/');
  if (lastSlash == -1) return path;
  return path.substring(lastSlash + 1);
}

// Determina o tipo de conteúdo baseado na extensão do arquivo
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".pdf")) return "application/pdf";
  else if (filename.endsWith(".txt")) return "text/plain";
  else if (filename.endsWith(".mp3")) return "audio/mpeg";
  else if (filename.endsWith(".wav")) return "audio/wav";
  else if (filename.endsWith(".mp4")) return "video/mp4";
  else if (filename.endsWith(".avi")) return "video/x-msvideo";
  else if (filename.endsWith(".mov")) return "video/quicktime";
  else if (filename.endsWith(".doc")) return "application/msword";
  else if (filename.endsWith(".docx")) return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
  else if (filename.endsWith(".xls")) return "application/vnd.ms-excel";
  else if (filename.endsWith(".xlsx")) return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  return "application/octet-stream";
}