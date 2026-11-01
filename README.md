# ⚡ Neon Type — Hacker Edition

<div align="center">

![C](https://img.shields.io/badge/Linguagem-C99-00599C?style=for-the-badge&logo=c&logoColor=white)
![Raylib](https://img.shields.io/badge/Engine-Raylib-white?style=for-the-badge&logo=raylib&logoColor=black)
![Plataforma](https://img.shields.io/badge/Plataforma-PC%20Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Status](https://img.shields.io/badge/Versão-1.0%20Final-00FF66?style=for-the-badge)

<br/>

**Um jogo arcade de digitação veloz e temática hacker retrô cyberpunk, desenvolvido em C puro com Raylib.**

[Sobre o Jogo](#-sobre-o-jogo) • [Como Jogar](#-como-jogar) • [Mecânicas](#-mecânicas-do-jogo) • [Fases & Servidores](#-progressão-de-fases) • [Interface](#-interface-e-hud) • [Como Compilar](#-compilação-e-execução)

</div>

---

## 📖 Sobre o Jogo

Em **Neon Type**, você assume o controle de um hacker de elite em uma corrida contra o relógio para quebrar firewalls e invadir servidores centrais. A chave para descriptografar os bancos de dados corporativos é a sua velocidade e precisão no teclado: você deve digitar comandos reais, estruturas e funções das linguagens de programação que protegem cada máquina.

A jornada começa em servidores legados protegidos por linguagens de baixo nível (como **C** e **Assembly**) e avança até servidores em nuvem de alto nível (como **JavaScript**, **Python** e **SQL**).

### ✨ Destaques
- **Estética Retrô Híbrida:** Pixel art clássica inspirada na paleta verde/cinza do Game Boy combinada com efeitos neon vibrantes e dinâmicos.
- **Trilha Sonora Chiptune Dinâmica:** O tom e a velocidade da música reagem ao seu desempenho e ao nível do servidor.
- **Feedback Visual Expressivo:** O hacker reage em tempo real com base no seu combo, entrando em estado de frenesi ao atingir sequências perfeitas.
- **Lojinha de Hardware Entre Fases:** Compre upgrades de refrigeração, memória e backups com as *Neon Coins* mineradas durante a invasão.

---

## 🎮 Como Jogar

### 🎯 Objetivo Principal
Para cada fase (servidor), você deve digitar com precisão a **meta de palavras/comandos** antes que o **cronômetro chegue a zero** e sem esgotar suas **vidas**.

### ⌨️ Controles
- **Teclado Alfanumérico (`A-Z`, símbolos e números quando aplicáveis):** Digitação direta das letras das palavras exibidas no monitor.
- **Navegação nos Menus:** Utilize as teclas numéricas (`1`, `2`, `3`) ou clique nos botões interativos para selecionar opções.
- **Login / Terminal:** Digite seu codinome no terminal de entrada e pressione `Enter` para inicializar a sessão de invasão.

### 🔄 Fluxo de Jogo

```mermaid
flowchart TD
    A["Cutscene de Terminal (Texto Letra por Letra)"] --> B["Identificação: LOGIN de Hacker"]
    B --> C["Menu Principal"]
    C -->|1: Iniciar Invasão| D["Gameplay (Servidor Ativo)"]
    C -->|2: Ranking| E["Tela de Ranking (Top 3)"]
    C -->|3: Sair| F["Encerrar Execução"]
    
    D -->|Tempo Esgotado ou Vidas = 0| G["Game Over (Salva Score no Ranking)"]
    D -->|Meta Atingida no Tempo| H["Level Clear!"]
    
    H --> I["Lojinha do Hacker (Upgrades)"]
    I -->|Próxima Fase| D
    I -->|Concluiu Level 8 (SQL)| J["🏆 Vitória Final (Final Hacker Top 3)"]
```

---

## ⚙️ Mecânicas do Jogo

### 1. ⌨️ Sistema de Input & Forca em Tempo Real
- A cada nova invasão, o sistema sorteia um comando da linguagem do servidor atual.
- A palavra é exibida na tela do notebook tijolão em formato de lacunas (`_ _ _ _ _`).
- A captura de teclas é feita em tempo real via Raylib:
  - **Acerto:** A letra correspondente preenche o espaço e emite um efeito sonoro agudo (*blip* estilo retro).
  - **Erro:** Se a tecla pressionada não corresponder à próxima letra esperada, você recebe um alerta sonoro grave e **perde 1 vida**.
- Assim que uma palavra é concluída com sucesso, o campo de entrada é limpo e um novo comando é carregado imediatamente.

---

### 2. ❤️ Sistema de Vidas
- O jogador inicia com **5 vidas** (representadas por corações verdes neon).
- **Perda de Vida:** Cada letra errada desconta 1 vida.
- **Recuperação:** Atingir um combo de **x20 ou superior** restaura **+1 vida** (respeitando o limite máximo de 5).
- **Fim de Jogo:** Se as vidas chegarem a zero, o sistema de defesa do servidor expulsa o invasor, resultando em **Game Over**.

---

### 3. 🔥 Sistema de Combos & Modo Frenesi
Digitar sequências de palavras corretas consecutivas eleva o multiplicador de combo e desbloqueia reações no hacker e no cenário:

| Nível de Combo | Efeito Visual no Hacker | Áudio & Trilha | Bônus Adicional |
| :--- | :--- | :--- | :--- |
| **Padrão (x0–x1)** | Hacker concentrado de costas com capuz | Pitch normal | Multiplicador de moedas x1 |
| **Combo x2** | Olhinhos felizes em arco `^_^` e bochechas coradas | Pitch da música sobe levemente | Multiplicador de moedas x2 |
| **Combo x10** | Olhos com brilho intenso em formato de estrela pixel | Pitch sobe ainda mais | Multiplicador de moedas x3 |
| **Combo x20+** <br>*(Modo Frenesi)* | Bordas e título com gradiente arco-íris pulsante | Música acelera e ganha batidas agudas | Multiplicador de moedas x4 + **Recupera +1 vida** |

> [!WARNING]
> Errar uma única letra quebra o combo imediatamente, resetando o multiplicador para zero!

---

### 4. 🪙 Economia: Neon Coins
Ao completar cada palavra com sucesso, o jogador mina **Neon Coins** (bitcoins verdes neon):
$$\text{Moedas Ganhas} = 1 \times \text{Multiplicador de Combo}$$

As moedas acumuladas persistem entre as fases e podem ser gastas na lojinha inter-fases.

---

### 5. ☕ A Lojinha do Hacker (Entre-Fases)
Após concluir um servidor, o hacker faz uma pausa para tomar café ou checar o celular. Nesse intervalo, você pode trocar suas moedas por vantagens vitais:

| Item de Hardware | Descrição do Efeito | Custo |
| :--- | :--- | :--- |
| ❄️ **Kit de Refrigeração** | Adiciona **+5 segundos** ao cronômetro da próxima invasão | 50 moedas |
| ⚡ **Overclock de Processador** | Adiciona **+10 segundos** ao cronômetro da próxima invasão | 90 moedas |
| 💾 **Backup de Dados** | Garante uma segunda chance: revive com **5 vidas cheias** ao zerar (1x por fase) | 200 moedas |
| 🔋 **Vida Extra** | Restaura **+1 vida** imediatamente (respeitando o máximo de 5) | 80 moedas |

---

## 💻 Progressão de Fases

O jogo é composto por 8 servidores protegidos por 8 linguagens de programação distintas, com dificuldade e cadência progressivas:

| Level | Linguagem | Meta | Tempo | Dificuldade | Contexto do Servidor |
|:---:|:---:|:---:|:---:|:---:|:---|
| **01** | **C** | 10 palavras | 30s | Normal | *"Servidor legado detectado. Quebre a barreira em C."* |
| **02** | **C++** | 15 palavras | 30s | Normal | Servidor orientado a objetos com firewalls virtuais. |
| **03** | **Assembly** | 15 palavras | 28s | Médio-Baixo | Instruções de baixo nível e registradores de CPU. |
| **04** | **Java** | 20 palavras | 28s | Médio | Máquina virtual corporativa com coleta de lixo ativa. |
| **05** | **Python** | 20 palavras | 25s | Médio | *"Script automatizado interceptado. Sobrescreva o código!"* |
| **06** | **JavaScript** | 25 palavras | 25s | Médio-Alto | Camada Web dinâmica com requisições assíncronas em massa. |
| **07** | **Kotlin / Swift** | 25 palavras | 22s | Alto | Servidores móveis nativos com criptografia estrita. |
| **08** | **SQL** | 30 palavras | 20s | Extremo | *"Banco de dados corporativo central. Extraia as informações!"* |

---

## 🖥️ Interface e HUD

A tela principal do jogo foi projetada para emular um ambiente de terminal retrô integrado à mesa de trabalho do hacker:

```text
┌─────────────────────────────────────────────────────────────────┐
│  [SCORE: 00450]                       [TIME: 00:24s]            │
│                       N E O N   T Y P E                         │
├──┬──────────────────────────────────────────────────────────────┤
│█ │                                                              │
│█ │                   [Sprite do Hacker]                         │
│█ │             ┌─────────────────────────────┐                  │
│█ │             │  printf(_______________)   │                  │
│█ │             │  _ _ _ _ _ _ _ _ _ _        │                  │
│  │             └─────────────────────────────┘                  │
│  │                                                              │
├──┴──────────────────────────────────────────────────────────────┤
│  💚 💚 💚 💚 🖤        [COMBO: x12]          [LEVEL: 04]        │
│  > input_                                                       │
└─────────────────────────────────────────────────────────────────┘
 ↑ Termômetro de progresso da fase (enche a cada palavra certa)
```

- **Termômetro Vertical (Esquerda):** Indica graficamente a proximidade de atingir a meta da fase.
- **Barra Superior:** Score acumulado do jogador e contador regressivo de tempo.
- **Tela do Notebook Central:** Exibe o comando alvo e o progresso da digitação em tempo real.
- **Barra Inferior:** Marcador de vidas (corações), multiplicador de combo atual, indicador de nível e linha de comando ativa.

---

## 🏆 Sistema de Ranking

- O jogo mantém o histórico dos **Top 3 melhores hackers** salvo localmente no arquivo `data/ranking.txt`.
- Ao final de cada partida (vitória ou derrota), a pontuação final é comparada com a tabela de recordes. Caso entre no pódio, os dados são atualizados automaticamente com seu codinome e score.

---

## 🎵 Audiovisual & Estética

- **Trilhas Adaptativas em Chiptune 8-bit:**
  - **Trilha A (Fases 1 a 4):** Batidas industriais, pesadas e cadenciadas que refletem a rigidez do baixo nível.
  - **Trilha B (Fases 5 a 8):** Sintetizadores rápidos, acelerados e agudos marcando a alta frequência das camadas modernas.
- **Efeitos de Áudio Chiptune:**
  - Sons distintos para acertos de letras, falhas de digitação, cliques de seleção, compras na loja, vitória de fase e Game Over.
- **Renderização com Pixel Art Fiel:**
  - Renderizado internamente na resolução retrô de **320×240**, sofrendo upscaling suave de 3x para **960×720** sem perda de nitidez dos pixels.

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** [C (C99)](https://en.wikipedia.org/wiki/C99)
- **Biblioteca Multimídia:** [Raylib 5.x](https://www.raylib.com/) (janela gráfica, renderização de texturas, fontes TTF e reprodução de áudio)
- **Compilador Recomendado:** GCC / MinGW-w64 (MSYS2 UCRT64)
- **Sistema Operacional Suportado:** Windows (x86_64)

---

## 🚀 Compilação e Execução

### Pré-requisitos
Para compilar o jogo do código-fonte no Windows, você precisará do **GCC** e da biblioteca **Raylib** instalados no ambiente (por exemplo, via [MSYS2](https://www.msys2.org/) com o pacote `ucrt64`).

### Compilação Automática (Script `.bat`)
O repositório já inclui um arquivo automatizado que compila os recursos de ícone e todos os módulos em C:

1. Dê um duplo clique em `build.bat` ou execute no terminal:
   ```cmd
   build.bat
   ```
2. O script compilará o executável `neontype.exe` e iniciará o jogo automaticamente.

### Compilação Manual via Terminal
Se preferir compilar manualmente com o GCC:

```bash
# 1. Gerar o arquivo de recursos do ícone
windres recursos.rc -O coff -o recursos.res

# 2. Compilar os fontes C com a Raylib
gcc -std=c99 -Iinclude src/*.c recursos.res -o neontype.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32

# 3. Executar o jogo
./neontype.exe
```

---

<div align="center">

Desenvolvido para amantes de programação, jogos de digitação e estética hacker retrô.  
*Hack the planet!* ⌨️⚡

</div>
