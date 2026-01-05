// =======================================================================
// Bibliotecas Necessárias
// =======================================================================
#include <ESP8266WiFi.h>
#include <WiFiManager.h>        // Biblioteca para o portal de configuração
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>        // Biblioteca para acesso por nome (ex: gel.local)
#include <SD.h>
#include <SPI.h>
#include <vector>

// =======================================================================
// Configuração de Pinos e Hardware
// =======================================================================
/*
  ESP12E → Módulo SD
    3.3V   → VCC
    GND    → GND
    D7     → MISO (GPIO13)
    D6     → MOSI (GPIO12)
    D5     → SCK  (GPIO14)
    D8     → CS   (GPIO15)
*/
const int sdCardCsPin = D8;

// =======================================================================
// Variáveis Globais e Objetos
// =======================================================================
ESP8266WebServer server(80);
File uploadFile;

// =======================================================================
// Página HTML Principal (Interface do Servidor de Arquivos) - **COM CSS RESPONSIVO**
// =======================================================================
const char* SERVER_FILE_HTML = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP8266 File Server</title>
    <style>
        :root {
            --primary-color: #007bff;
            --primary-hover: #0056b3;
            --background-color: #f4f4f4;
            --container-bg: #ffffff;
            --text-color: #333;
            --border-color: #ddd;
        }
        body { 
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; 
            margin: 0; 
            padding: 10px; 
            background-color: var(--background-color); 
            color: var(--text-color); 
        }
        .container { 
            max-width: 800px; 
            margin: auto; 
            background: var(--container-bg); 
            padding: 20px; 
            box-shadow: 0 2px 15px rgba(0,0,0,0.1); 
            border-radius: 8px; 
        }
        h1, h2 { 
            color: var(--primary-hover);
            border-bottom: 2px solid var(--background-color);
            padding-bottom: 10px;
        }
        .upload-section, .file-list-section { margin-bottom: 30px; }
        form {
            display: flex;
            flex-direction: column;
            gap: 15px;
        }
        input[type="file"] {
            border: 1px solid var(--border-color);
            padding: 10px;
            border-radius: 5px;
            background-color: #fafafa;
        }
        input[type="submit"] { 
            background-color: var(--primary-color); 
            color: white; 
            cursor: pointer; 
            border: none;
            padding: 12px; 
            border-radius: 5px; 
            font-size: 16px;
            font-weight: bold;
            transition: background-color 0.3s ease;
        }
        input[type="submit"]:hover { background-color: var(--primary-hover); }
        table { width: 100%; border-collapse: collapse; }
        th, td { padding: 12px 15px; border-bottom: 1px solid var(--border-color); text-align: left; }
        thead th { background-color: #f9f9f9; }
        tr:hover { background-color: #f1f1f1; }
        .actions a, .actions button {
            text-decoration: none; color: var(--primary-color); margin-right: 10px;
            background: none; border: 1px solid var(--primary-color); padding: 5px 10px; 
            border-radius: 4px; cursor: pointer; font-size: 14px;
            display: inline-block; /* Garante alinhamento */
            margin-bottom: 5px; /* Espaçamento no mobile */
        }
        .actions a:hover, .actions button:hover { background-color: #e6f2ff; }
        .pagination { margin-top: 20px; text-align: center; }
        .pagination button { padding: 8px 12px; cursor: pointer; border-radius: 4px; border: 1px solid var(--border-color); }
        .pagination button:disabled { cursor: not-allowed; opacity: 0.5; }
        #status { margin-top: 15px; font-weight: bold; }
        
        /* === ESTILOS PARA CELULAR (RESPONSIVO) === */
        @media (max-width: 600px) {
            body { padding: 5px; }
            .container { padding: 15px; }
            
            /* Transforma a tabela em lista de cards */
            thead { display: none; }
            tr {
                display: block;
                border: 1px solid var(--border-color);
                border-radius: 5px;
                margin-bottom: 15px;
                padding: 10px;
            }
            td {
                display: block;
                text-align: right;
                padding: 8px;
                border-bottom: none;
                position: relative;
                padding-left: 50%;
            }
            td:before {
                content: attr(data-label);
                position: absolute;
                left: 10px;
                font-weight: bold;
                text-align: left;
            }
            .actions {
                text-align: center;
                padding-left: 0;
            }
             .actions:before { display: none; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Servidor de Arquivos ESP8266</h1>
        <div class="upload-section">
            <h2>Enviar Arquivo</h2>
            <form id="uploadForm" enctype="multipart/form-data">
                <input type="file" name="fileInput" id="fileInput" required>
                <input type="submit" value="Enviar">
            </form>
            <div id="status"></div>
        </div>
        <div class="file-list-section">
            <h2>Arquivos no SD Card</h2>
            <table>
                <thead>
                    <tr>
                        <th>Nome do Arquivo</th>
                        <th>Tamanho</th>
                        <th>Ações</th>
                    </tr>
                </thead>
                <tbody id="fileList">
                </tbody>
            </table>
            <div class="pagination">
                <button id="prevPage" disabled>Anterior</button>
                <span id="pageInfo">Página 1</span>
                <button id="nextPage" disabled>Próxima</button>
            </div>
        </div>
    </div>

    <script>
        let currentPage = 0;
        const filesPerPage = 5; // Aumentei um pouco para caber mais
        let totalFiles = 0;

        document.getElementById('uploadForm').addEventListener('submit', function(event) {
            event.preventDefault();
            const fileInput = document.getElementById('fileInput');
            if (fileInput.files.length === 0) {
                alert('Por favor, selecione um arquivo.');
                return;
            }
            const formData = new FormData();
            formData.append('file', fileInput.files[0], fileInput.files[0].name);
            
            const statusDiv = document.getElementById('status');
            statusDiv.textContent = 'Enviando...';
            statusDiv.style.color = 'blue';

            fetch('/upload', {
                method: 'POST',
                body: formData
            })
            .then(response => {
                if (!response.ok) {
                   throw new Error(`Erro no servidor: ${response.statusText}`);
                }
                return response.text();
            })
            .then(data => {
                statusDiv.textContent = data;
                statusDiv.style.color = 'green';
                fileInput.value = '';
                loadFiles(0); // Volta para a primeira página para ver o novo arquivo
            })
            .catch(error => {
                statusDiv.textContent = 'Erro no envio: ' + error;
                statusDiv.style.color = 'red';
            });
        });

        function loadFiles(page) {
            fetch(`/listfiles?page=${page}&limit=${filesPerPage}`)
                .then(response => response.json())
                .then(data => {
                    const fileListBody = document.getElementById('fileList');
                    fileListBody.innerHTML = '';
                    totalFiles = data.total;

                    data.files.forEach(file => {
                        const row = document.createElement('tr');
                        row.innerHTML = `
                            <td data-label="Nome:">${file.name}</td>
                            <td data-label="Tamanho:">${formatBytes(file.size)}</td>
                            <td class="actions">
                                <a href="/view?name=${file.name}" target="_blank">Ver</a>
                                <a href="/download?name=${file.name}">Baixar</a>
                                <button onclick="deleteFile('${file.name}')">Excluir</button>
                            </td>
                        `;
                        fileListBody.appendChild(row);
                    });

                    updatePagination(page);
                });
        }

        function updatePagination(page) {
            currentPage = page;
            const totalPages = Math.ceil(totalFiles / filesPerPage);
            document.getElementById('pageInfo').textContent = `Página ${page + 1} de ${totalPages || 1}`;
            document.getElementById('prevPage').disabled = page === 0;
            document.getElementById('nextPage').disabled = (page + 1) >= totalPages;
        }

        document.getElementById('prevPage').addEventListener('click', () => {
            if (currentPage > 0) {
                loadFiles(currentPage - 1);
            }
        });

        document.getElementById('nextPage').addEventListener('click', () => {
            const totalPages = Math.ceil(totalFiles / filesPerPage);
            if (currentPage + 1 < totalPages) {
                loadFiles(currentPage + 1);
            }
        });

        function deleteFile(fileName) {
            if (confirm(`Tem certeza que deseja excluir o arquivo ${fileName}?`)) {
                fetch('/delete?name=' + encodeURIComponent(fileName), {
                    method: 'POST'
                })
                .then(response => response.text())
                .then(data => {
                    alert(data);
                    // Recarrega a página atual ou a anterior se for o último item
                    if (document.getElementById('fileList').rows.length === 1 && currentPage > 0) {
                        loadFiles(currentPage - 1);
                    } else {
                        loadFiles(currentPage);
                    }
                });
            }
        }
        
        function formatBytes(bytes, decimals = 2) {
            if (bytes === 0) return '0 Bytes';
            const k = 1024;
            const dm = decimals < 0 ? 0 : decimals;
            const sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
            const i = Math.floor(Math.log(bytes) / Math.log(k));
            return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i];
        }

        window.onload = () => loadFiles(0);
    </script>
</body>
</html>
)rawliteral";

// =======================================================================
// Protótipos de Funções
// =======================================================================
void handleRootFile();
void handleUpload();
void handleListFiles();
void handleDownload();
void handleViewFile();
void handleDelete();
void handleNotFound();
void listAllFilesSerial();
String getContentType(String filename);

// =======================================================================
// FUNÇÃO SETUP
// =======================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n\nIniciando dispositivo...");

  // 1. Configuração do WiFi com WiFiManager
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(180); // O portal fica ativo por 3 minutos
  if (!wifiManager.autoConnect("ESP-FileServer-Config")) {
    Serial.println("Falha ao conectar e o tempo limite expirou. Reiniciando...");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  // Se chegou aqui, o ESP está conectado à sua rede!
  Serial.println("");
  Serial.println("==============================================");
  Serial.println("WiFi conectado com sucesso!");
  Serial.print("IP: http://");
  Serial.println(WiFi.localIP());
  
  // 2. Inicialização do mDNS para acesso por nome (esp-server.local)
  if (MDNS.begin("esp-server")) {
    MDNS.addService("http", "tcp", 80);
    Serial.println("Servidor mDNS iniciado!");
    Serial.println("Nome: http://esp-server.local");
  } else {
    Serial.println("Erro ao iniciar mDNS!");
  }
  Serial.println("==============================================");


  // 3. Inicialização do Cartão SD
  Serial.println("Inicializando SD card...");
  if (!SD.begin(sdCardCsPin)) {
    Serial.println("FALHA NA INICIALIZAÇÃO DO SD CARD! Verifique as conexões.");
    return; // Para a execução se o SD falhar
  }
  Serial.println("SD card inicializado.");

  // Cria o diretório de uploads se não existir
  if (!SD.exists("/uploads")) {
    Serial.println("Criando diretorio /uploads...");
    SD.mkdir("/uploads");
  }

  listAllFilesSerial();

  // 4. Configuração do Servidor Web
  server.on("/", HTTP_GET, handleRootFile);
  
  // == LINHA CORRIGIDA PARA O UPLOAD ==
  // A função handleUpload gerencia os dados recebidos.
  // A resposta de sucesso/falha é enviada DENTRO de handleUpload.
  server.on("/upload", HTTP_POST, 
    []() { /* Vazio, pois a resposta é gerenciada em handleUpload */ }, 
    handleUpload
  );

  server.on("/listfiles", HTTP_GET, handleListFiles);
  server.on("/view", HTTP_GET, handleViewFile);
  server.on("/download", HTTP_GET, handleDownload);
  server.on("/delete", HTTP_POST, handleDelete);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

// =======================================================================
// FUNÇÃO LOOP
// =======================================================================
void loop() {
  server.handleClient();
  MDNS.update(); // Mantenha o mDNS rodando
}

// =======================================================================
// Funções de Callback do Servidor (Handlers)
// =======================================================================
void handleRootFile() {
  server.send(200, "text/html", SERVER_FILE_HTML);
}

void handleUpload() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = "/uploads/" + upload.filename;
    if (filename.length() > 31) { // Limitação de nome de arquivo do SD
        filename = filename.substring(0, 31);
    }
    Serial.print("Recebendo arquivo: "); Serial.println(filename);
    if (SD.exists(filename)) { 
        Serial.println("Arquivo ja existe. Removendo versao antiga...");
        SD.remove(filename); 
    }
    uploadFile = SD.open(filename, FILE_WRITE);
    if(!uploadFile){
        Serial.println("Nao foi possivel criar o arquivo!");
        return;
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
      Serial.println("Upload do arquivo concluido.");
      server.send(200, "text/plain", "Arquivo enviado com sucesso!");
    } else {
      Serial.println("Falha no upload.");
      server.send(500, "text/plain", "Falha no upload: nao foi possivel salvar no SD Card.");
    }
  }
}

void handleListFiles() {
  int page = server.hasArg("page") ? server.arg("page").toInt() : 0;
  int filesPerPage = server.hasArg("limit") ? server.arg("limit").toInt() : 5;
  
  std::vector<String> fileList;
  std::vector<size_t> fileSizes;
  
  File root = SD.open("/uploads");
  if(!root){
    server.send(500, "application/json", "{\"error\":\"Nao foi possivel abrir o diretorio de uploads\"}");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      fileList.push_back(String(file.name()));
      fileSizes.push_back(file.size());
    }
    file.close();
    file = root.openNextFile();
  }
  root.close();
  
  int totalFiles = fileList.size();
  int startIndex = page * filesPerPage;
  int endIndex = min(startIndex + filesPerPage, totalFiles);
  
  String json = "{\"files\":[";
  for (int i = startIndex; i < endIndex; i++) {
    if (i > startIndex) json += ",";
    String fileName = fileList[i];
    
    // Extrai apenas o nome do arquivo, removendo o caminho do diretório
    int lastSlash = fileName.lastIndexOf('/');
    if (lastSlash != -1) {
      fileName = fileName.substring(lastSlash + 1);
    }
    
    json += "{\"name\":\"" + fileName + "\",\"size\":" + String(fileSizes[i]) + "}";
  }
  json += "], \"total\":" + String(totalFiles) + "}";
  
  server.send(200, "application/json", json);
}

void handleViewFile() {
  if (!server.hasArg("name") || server.arg("name") == "") {
    server.send(400, "text/plain", "Nome do arquivo nao fornecido.");
    return;
  }
  String fileName = server.arg("name");
  String filePath = "/uploads/" + fileName;
  
  if (SD.exists(filePath)) {
    File file = SD.open(filePath, "r");
    server.streamFile(file, getContentType(fileName));
    file.close();
  } else {
    server.send(404, "text/plain", "Arquivo nao encontrado.");
  }
}

void handleDownload() {
  if (!server.hasArg("name") || server.arg("name") == "") {
    server.send(400, "text/plain", "Nome do arquivo nao fornecido.");
    return;
  }
  String fileName = server.arg("name");
  String filePath = "/uploads/" + fileName;

  if (SD.exists(filePath)) {
    File file = SD.open(filePath, "r");
    server.sendHeader("Content-Disposition", "attachment; filename=" + fileName);
    server.streamFile(file, "application/octet-stream");
    file.close();
  } else {
    server.send(404, "text/plain", "Arquivo nao encontrado.");
  }
}

void handleDelete() {
  if (server.hasArg("name")) {
    String fileName = server.arg("name");
    String filePath = "/uploads/" + fileName;
    Serial.print("Tentando excluir: "); Serial.println(filePath);
    if (SD.exists(filePath)) {
      if (SD.remove(filePath)) {
        server.send(200, "text/plain", "Arquivo excluido com sucesso.");
      } else {
        server.send(500, "text/plain", "Erro ao excluir arquivo.");
      }
    } else {
       server.send(404, "text/plain", "Arquivo nao encontrado para excluir.");
    }
  } else {
    server.send(400, "text/plain", "Nome do arquivo nao fornecido.");
  }
}

void handleNotFound() {
    server.send(404, "text/plain", "Pagina nao encontrada.");
}

// =======================================================================
// Funções Auxiliares
// =======================================================================
void listAllFilesSerial() {
  Serial.println("--- LISTANDO ARQUIVOS NO SD CARD (/uploads) ---");
  File root = SD.open("/uploads");
  if (!root) {
    Serial.println("Falha ao abrir diretorio /uploads");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Nao e um diretorio");
    return;
  }
  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      Serial.printf("Arquivo: %s - Tamanho: %d Bytes\n", file.name(), file.size());
    }
    file.close();
    file = root.openNextFile();
  }
  root.close();
  Serial.println("--- FIM DA LISTAGEM ---");
}

String getContentType(String filename) {
  if (filename.endsWith(".html") || filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".pdf")) return "application/pdf";
  else if (filename.endsWith(".mp4")) return "video/mp4";
  else if (filename.endsWith(".mp3")) return "audio/mp3";
  else if (filename.endsWith(".txt")) return "text/plain";
  else return "application/octet-stream"; // Tipo genérico para download
}