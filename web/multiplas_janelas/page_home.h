#ifndef PAGE_HOME_H
#define PAGE_HOME_H

// Certifique-se de que a instância do servidor seja "extern" para que
// este arquivo possa acessá-la.

const char* htmlRoot = R"rawliteral(
  <!DOCTYPE html>
<html>
<head>
    <title>Portal</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
    <h1>Bem-vindo ao Portal!</h1>
    <p>Este site foi aberto automaticamente.</p>
    <a href='/sobre'><button>Ir para a página 'Sobre'</button></a>
</body>
</html>
)rawliteral";

#endif