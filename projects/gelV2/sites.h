#ifndef SITES_H
#define SITES_H

const char* HOME = R"rawliteral(
  <!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>GEL</title>
      
      <style>
          body {
              margin: 0;
              font-family: Arial, sans-serif;
              display: flex;
              justify-content: center;
              align-items: center;
              min-height: 100vh;
              background-color: #f0f2f5;
          }

          #loading-container {
              position: fixed;
              top: 0;
              left: 0;
              width: 100%;
              height: 100%;
              background-color: rgba(255, 255, 255, 0.95);
              display: flex;
              justify-content: center;
              align-items: center;
              z-index: 9999;
              opacity: 1;
              transition: opacity 0.8s ease-out;
          }

          .loader {
              width: 60px;
              height: 60px;
              border: 6px solid #e0e0e0;
              border-top-color: #4CAF50; 
              border-radius: 50%;
              animation: spin 1.2s cubic-bezier(0.5, 0, 0.5, 1) infinite, 
                          pulse 1.8s ease-in-out infinite alternate; 
              box-shadow: 0 0 15px rgba(76, 175, 80, 0.3);
          }

          @keyframes spin {
              0% { transform: rotate(0deg); }
              100% { transform: rotate(360deg); }
          }

          @keyframes pulse {
              0% { transform: scale(1); }
              50% { transform: scale(1.1); }
              100% { transform: scale(1); }
          }
      </style>

  </head>
  <body>

      <div id="loading-container">
          <div class="loader"></div>
      </div>

      <main>
          <h1>Games</h1>
          <p>local de diverção</p>
          <a href="/server">server</a>
      </main>

      <script>
          window.onload = function() {
              
              const loadingContainer = document.getElementById('loading-container');
              const content = document.getElementById('content');

              
              loadingContainer.style.opacity = '0';

              setTimeout(() => {
                  
                  loadingContainer.style.display = 'none';

                  content.style.display = 'block';
              }, 800);
          };
      </script>
  </body>
  </html>
)rawliteral";

const char* SERVER_FILE = R"rawliteral(

   <!DOCTYPE html>
  <html lang="pt-br">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <title>Gerenciador de Arquivos</title>
      <style>
          body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background-color: #f0f2f5; }
          .container { max-width: 800px; margin: auto; background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
          h1, h2 { color: #333; }
          .file-list, .upload-form { margin-bottom: 20px; padding: 15px; border: 1px solid #ddd; border-radius: 6px; }
          .file-item { display: flex; justify-content: space-between; align-items: center; padding: 8px 0; border-bottom: 1px solid #eee; }
          .file-item:last-child { border-bottom: none; }
          .file-name { flex-grow: 1; }
          .file-size { color: #666; font-size: 0.9em; margin-right: 10px; }
          .file-item a { text-decoration: none; color: #007BFF; margin-left: 10px; }
          .file-item a.delete { color: #dc3545; }
          .upload-form input, .upload-form button { margin-top: 10px; }
          .upload-form input[type="file"] { width: 100%; padding: 5px; }
          .upload-form button { background-color: #28a745; color: white; border: none; padding: 10px 15px; border-radius: 4px; cursor: pointer; }
          .upload-form button:hover { background-color: #218838; }
          #message { margin-top: 10px; padding: 10px; border-radius: 4px; display: none; }
          #message.success { background-color: #d4edda; color: #155724; }
          #message.error { background-color: #f8d7da; color: #721c24; }
          #pagination { display: flex; justify-content: center; gap: 20px; margin-top: 20px; }
          #pagination button { padding: 10px 20px; cursor: pointer; }
          #pagination button:disabled { opacity: 0.5; cursor: not-allowed; }

          /* Estilos para o Pop-up */
          .popup-overlay {
              position: fixed;
              top: 0;
              left: 0;
              width: 100%;
              height: 100%;
              background-color: rgba(0, 0, 0, 0.5);
              display: none;
              justify-content: center;
              align-items: center;
              z-index: 1000;
          }
          .popup {
              background-color: white;
              padding: 30px;
              border-radius: 10px;
              text-align: center;
              box-shadow: 0 5px 15px rgba(0, 0, 0, 0.3);
              display: flex;
              flex-direction: column;
              align-items: center;
          }
          .loader {
              border: 5px solid #f3f3f3;
              border-radius: 50%;
              border-top: 5px solid #3498db;
              width: 40px;
              height: 40px;
              -webkit-animation: spin 2s linear infinite;
              animation: spin 2s linear infinite;
              margin-bottom: 20px;
          }
          @-webkit-keyframes spin { 0% { -webkit-transform: rotate(0deg); } 100% { -webkit-transform: rotate(360deg); } }
          @keyframes spin { 0% { transform: rotate(0deg); } 100% { transform: rotate(360deg); } }
      </style>
  </head>
  <body>
      <div class="container">
          <h1>Gerenciador de Arquivos</h1>
          <div class="upload-form">
              <h2>Enviar Arquivo</h2>
              <form id="uploadForm" enctype="multipart/form-data" method="POST" action="/upload">
                  <input type="file" name="fileToUpload" id="fileToUpload">
                  <button type="submit">Enviar</button>
              </form>
              <div id="message"></div>
          </div>
          <div class="file-list">
              <h2>Arquivos no SD Card</h2>
              <ul id="files"></ul>
              <div id="pagination">
                  <button id="prevBtn" disabled>Página Anterior</button>
                  <button id="nextBtn">Próxima Página</button>
              </div>
          </div>
      </div>

      <div id="uploadPopup" class="popup-overlay">
          <div class="popup">
              <div class="loader"></div>
              <h3>Enviando Arquivo...</h3>
              <p>Por favor, aguarde.</p>
          </div>
      </div>
      
      <script>
          let currentPage = 0;
          const filesPerPage = 4;

          function getFileExtension(filename) {
              return filename.split('.').pop().toLowerCase();
          }

          function isViewable(ext) {
              return ['jpg', 'jpeg', 'png', 'gif', 'mp4', 'mp3', 'wav', 'pdf'].includes(ext);
          }
          
          function fetchFiles() {
              fetch('/listfiles?page=' + currentPage)
                  .then(response => response.json())
                  .then(data => {
                      const filesList = document.getElementById('files');
                      filesList.innerHTML = ''; 
                      data.files.forEach(file => {
                          const li = document.createElement('li');
                          li.className = 'file-item';
                          const fileNameSpan = document.createElement('span');
                          fileNameSpan.className = 'file-name';
                          fileNameSpan.textContent = file.name;
                          li.appendChild(fileNameSpan);
                          
                          const fileSizeSpan = document.createElement('span');
                          fileSizeSpan.className = 'file-size';
                          fileSizeSpan.textContent = '(' + (file.size / 1024).toFixed(2) + ' KB)';
                          li.appendChild(fileSizeSpan);
                          
                          const fileExt = getFileExtension(file.name);
                          const actionLink = document.createElement('a');
                          
                          if (isViewable(fileExt)) {
                              actionLink.href = '/view?name=' + encodeURIComponent(file.name);
                              actionLink.textContent = 'Visualizar';
                              actionLink.target = '_blank';
                          } else {
                              actionLink.href = '/view?name=' + encodeURIComponent(file.name);
                              actionLink.textContent = 'Download';
                          }
                          li.appendChild(actionLink);

                          const deleteLink = document.createElement('a');
                          deleteLink.href = '#';
                          deleteLink.className = 'delete';
                          deleteLink.textContent = 'Excluir';
                          deleteLink.onclick = function() {
                              if (confirm('Tem certeza que deseja excluir o arquivo ' + file.name + '?')) {
                                  deleteFile(file.name);
                              }
                          };
                          li.appendChild(deleteLink);
                          
                          filesList.appendChild(li);
                      });
                      
                      const totalPages = Math.ceil(data.total / filesPerPage);
                      document.getElementById('prevBtn').disabled = currentPage === 0;
                      document.getElementById('nextBtn').disabled = currentPage >= totalPages - 1;

                  })
                  .catch(error => console.error('Erro ao buscar arquivos:', error));
          }

          function deleteFile(fileName) {
              fetch('/delete', {
                  method: 'POST',
                  headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                  body: 'name=' + encodeURIComponent(fileName)
              })
              .then(response => response.text())
              .then(text => {
                  showMessage(text, 'success');
                  fetchFiles();
              })
              .catch(error => {
                  console.error('Erro ao excluir arquivo:', error);
                  showMessage('Erro ao excluir arquivo.', 'error');
              });
          }

          function showMessage(msg, type) {
              const messageBox = document.getElementById('message');
              messageBox.textContent = msg;
              messageBox.className = type;
              messageBox.style.display = 'block';
              setTimeout(() => { messageBox.style.display = 'none'; }, 5000);
          }
          
          document.getElementById('uploadForm').addEventListener('submit', function(event) {
              event.preventDefault();

              // Mostra o pop-up de upload
              document.getElementById('uploadPopup').style.display = 'flex';

              const formData = new FormData(this);
              fetch('/upload', {
                  method: 'POST',
                  body: formData
              })
              .then(response => response.text())
              .then(text => {
                  // Esconde o pop-up e mostra a mensagem de sucesso
                  document.getElementById('uploadPopup').style.display = 'none';
                  showMessage(text, 'success');
                  currentPage = 0; 
                  fetchFiles();
              })
              .catch(error => {
                  // Esconde o pop-up e mostra a mensagem de erro
                  document.getElementById('uploadPopup').style.display = 'none';
                  console.error('Erro no upload:', error);
                  showMessage('Erro no upload do arquivo.', 'error');
              });
          });
          
          document.getElementById('prevBtn').addEventListener('click', () => {
              if (currentPage > 0) {
                  currentPage--;
                  fetchFiles();
              }
          });

          document.getElementById('nextBtn').addEventListener('click', () => {
              currentPage++;
              fetchFiles();
          });
          
          window.onload = fetchFiles;
      </script>
  </body>
  </html>
)rawliteral";

#endif