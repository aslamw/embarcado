#ifndef PAGES_WEB_H
#define PAGES_WEB_H


const char* welcomeAtiva = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bem-Vindo ao Mundo Maker & Metodologias Ativas</title>
    <style>
        /* Estilos CSS para o site */
        :root {
            --primary-color: #007bff;
            --background-color: #f4f4f9;
            --text-color: #333;
            --text-light: #777;
            --accent-color: #0056b3;
            --shadow-light: rgba(0, 0, 0, 0.1);
            --shadow-dark: rgba(0, 0, 0, 0.2);
        }

        body {
            font-family: 'Arial', sans-serif;
            margin: 0;
            padding: 0;
            background-color: var(--background-color);
            color: var(--text-color);
            display: flex;
            min-height: 100vh;
            flex-direction: column;
            align-items: center;
            justify-content: center;
        }

        .container {
            width: 100%;
            max-width: 1200px;
            display: flex;
            background-color: #fff;
            box-shadow: 0 4px 20px var(--shadow-light);
            border-radius: 10px;
            overflow: hidden;
            margin: 20px;
        }

        .left-panel {
            flex: 1;
            background: url('https://source.unsplash.com/1000x800/?maker,creativity') center/cover no-repeat;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            color: #0079D0;
            text-shadow: 2px 2px 6px var(--shadow-dark);
            text-align: center;
            padding: 20px;
            position: relative;
        }
        
        .left-panel h1 {
            font-size: clamp(1.5em, 5vw, 2.5em);
            margin: 0;
            line-height: 1.2;
            margin-top: 100px;
        }

        .right-panel {
            flex: 1;
            padding: 40px;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
            text-align: center;
        }

        .welcome-message {
            font-size: clamp(2em, 5vw, 3em);
            margin-bottom: 20px;
            color: var(--primary-color);
            font-weight: bold;
        }

        .phrase-container {
            height: 100px;
            width: 100%;
            overflow: hidden;
            position: relative;
            margin-bottom: 40px;
        }

        .phrase {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            width: 90%;
            font-size: clamp(1em, 3vw, 1.4em);
            opacity: 0;
            transition: opacity 1s ease-in-out;
            line-height: 1.5;
        }

        .phrase.active {
            opacity: 1;
        }

        .learn-more {
            background-color: var(--primary-color);
            color: #fff;
            border: none;
            padding: 15px 30px;
            border-radius: 25px;
            cursor: pointer;
            font-size: 1.1em;
            transition: background-color 0.3s ease, transform 0.2s ease;
            box-shadow: 0 4px 15px var(--shadow-light);
            text-decoration: none;
        }

        .learn-more:hover {
            background-color: var(--accent-color);
            transform: translateY(-2px);
        }

        footer {
            margin-top: 40px;
            text-align: center;
            font-size: 0.8em;
            color: var(--text-light);
        }

        /* Media queries para telas menores */
        @media (max-width: 768px) {
            .container {
                flex-direction: column;
            }

            .left-panel {
                height: 300px;
            }

            .right-panel {
                padding: 30px 20px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <!-- Painel esquerdo com a imagem de fundo -->
        <div class="left-panel">
            <svg width="569" height="219" viewBox="0 0 569 219" fill="none" xmlns="http://www.w3.org/2000/svg">
            <path d="M62.751 129.055L80.4888 75.1683L65.8866 37.3438L31.8379 129.055H62.751Z" fill="#0079D0"/>
            <path d="M77.3936 99.7041L71.0567 118.957L102.325 118.949L106.451 129.055H138.336L100.62 32.9688H69.5332L95.0582 99.7041H77.3936Z" fill="#0079D0"/>
            <path d="M230.661 32.9688H148.107V53.8153H230.661V32.9688Z" fill="#0079D0"/>
            <path d="M205.816 59.1641H173.078V129.051H205.816V59.1641Z" fill="#0079D0"/>
            <path d="M307.449 108.536H297.795V52.9855H307.449V32.9688H256.105V53.1023H265.995V108.774H256.105V129.055H307.449V108.536Z" fill="#0079D0"/>
            <path d="M392.62 129.055L359.586 32.9688H326.906L361.53 129.055H392.62Z" fill="#0079D0"/>
            <path d="M429.531 32.9688H397.383L382.014 83.9416L396.797 127.08L429.531 32.9688Z" fill="#0079D0"/>
            <path d="M478.274 75.7288L464.143 37.1875L430.402 129.051H460.964L478.274 75.7288Z" fill="#0079D0"/>
            <path d="M475.794 99.821L469.257 119.094L500.88 119.273L504.59 129.055H536.56L498.936 32.9688H467.668L493.282 99.6457L475.794 99.821Z" fill="#0079D0"/>
            <path d="M31.6426 193.289H40.3592V217.736H51.0737V193.679H59.555V186.062H31.6426V193.289Z" fill="#0079D0"/>
            <path d="M209.175 199.032H198.989V186.188H188.391L188.564 218.246H199.105V212.889V205.277H209.175V199.032Z" fill="#0079D0"/>
            <path d="M200.686 192.608H207.458V195.347H217.058V186.188H200.686V192.608Z" fill="#0079D0"/>
            <path d="M205.869 211.831H200.686V218.255H217.058V208.617H205.869V211.831Z" fill="#0079D0"/>
            <path d="M365.712 211.51H358.778L357.069 213.295V191.108L359.337 192.655H365.608V199.048H376.149V192.655L369.774 186.219L352.286 186.441L346.281 192.507V212.044L352.286 218.11H370.395L376.311 212.578V207.05L365.712 206.96V211.51Z" fill="#0079D0"/>
            <path d="M505.725 186.547V218.13H516.497L516.262 186.547H505.725Z" fill="#0079D0"/>
            <path d="M525.928 198.448L518.449 198.506V205.523H526.047V218.134H536.584V186.312H525.928V198.448Z" fill="#0079D0"/>
            <path d="M561.382 0.875H0.5V161.687H568.726V0.875H561.382ZM561.382 154.265H7.8474V8.29653H561.382V154.269V154.265Z" fill="#0079D0"/>
            </svg>
            <h1>Inspire-se e Crie!</h1>
        </div>
        <!-- Painel direito com o conteúdo dinâmico -->
        <div class="right-panel">
            <div class="welcome-message">
                Seja Bem-Vindo!
            </div>
            <div class="phrase-container">
                <div class="phrase active">A Cultura Maker transforma ideias em realidade com as próprias mãos.</div>
                <div class="phrase">Metodologias Ativas colocam você no centro do aprendizado, explorando e descobrindo.</div>
                <div class="phrase">Criatividade e colaboração são os pilares da inovação.</div>
                <div class="phrase">Aprender fazendo é a essência do mundo maker e das metodologias ativas.</div>
                <div class="phrase">No mundo Maker, errar faz parte do processo de aprendizado.</div>
            </div>
            <a href="#" class="learn-more">Descubra Mais</a>
        </div>
    </div>
    <footer>
        &copy; 2023 Boas-Vindas Maker & Metodologias Ativas
    </footer>

    <script>
        // JavaScript para a troca de frases
        const phrases = document.querySelectorAll('.phrase');
        let index = 0;

        function nextPhrase() {
            // Remove a classe 'active' da frase atual
            phrases[index].classList.remove('active');
            // Avança para a próxima frase, voltando ao início se necessário
            index = (index + 1) % phrases.length;
            // Adiciona a classe 'active' à próxima frase
            phrases[index].classList.add('active');
        }

        // Troca de frase a cada 5 segundos
        setInterval(nextPhrase, 5000);
    </script>
</body>
</html>


)rawliteral";

#endif