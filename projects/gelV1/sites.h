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

#endif