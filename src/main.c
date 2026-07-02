#include "raylib.h"
#include <stdlib.h> // Necessário para a função exit(0)
#include <stdio.h>
#include <time.h>   // srand(time(NULL)) para sorteio de palavras
#include <string.h>

// ===== Módulo de lógica/dados do Programador A =====
// Substitui as struc#include "raylib.h"
#include <stdlib.h> // Necessário para a função exit(0)
#include <stdio.h>
#include <time.h>   // srand(time(NULL)) para sorteio de palavras

// ===== Módulo de lógica/dados do Programador A =====
// Substitui as structs e a lógica "mock" que existiam antes aqui:
// Jogador, Fase, BancoPalavras/Palavra, TimerFase, Ranking e a loja
// (economia) agora vêm prontos desses headers, em vez de serem
// redeclarados e simulados localmente no main.c.
#include "jogador.h"
#include "palavra.h"
#include "fase.h"
#include "economia.h"
#include "ranking.h"

//Structs principais 
typedef struct {
    char texto[10];
    float x, y;
    float alpha;
    Color cor;
    int tamanho;
    int ativo;
} BonusVisual;

#define MAX_BONUS 10
BonusVisual listaBonus[MAX_BONUS] = {0};

// Função para disparar o efeito
void adicionar_bonus_visual(const char* texto, Color cor, int tamanho) {
    for (int i = 0; i < MAX_BONUS; i++) {
        if (!listaBonus[i].ativo) {
            listaBonus[i].ativo = 1;
            listaBonus[i].alpha = 1.0f;
            listaBonus[i].x = 230; 
            listaBonus[i].y = 40;  // Começa mais abaixo do cabeçalho
            listaBonus[i].cor = cor;
            listaBonus[i].tamanho = tamanho;
            strcpy(listaBonus[i].texto, texto);
            break;
        }
    }
}

bool faseIniciando = false;

char inputGameplay[50] = {0}; 
int tamanhoInputGameplay = 0;

// função de limpar input, correção de erro do "Input Fantasma" 
void LimparInput(char *buffer, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        buffer[i] = '\0'; // Preenche o buffer com caracteres nulos
    }
}

void ResetarInputGameplay() {
    tamanhoInputGameplay = 0;
    LimparInput(inputGameplay, 50); // Usa a função acima para limpar o buffer
}

//Telas 
typedef enum { MENU, LOGIN, INTRO_FASE, GAMEPLAY, LOJA, PAUSA, RANKING, GAMEOVER, VICTORY } TelasJogo;

// Nome, custo e descrição (texto de exibição) dos itens da loja.
// Renomeado para ItemLojaUI (era "ItemLoja") porque esse nome agora
// colide com o enum ItemLoja vindo de economia.h, que representa os
// IDs reais dos itens (ITEM_REFRIGERACAO, ITEM_OVERCLOCK, etc.) usados
// pela função economia_comprar_item(). Esta struct aqui é só para
// desenhar nome/descrição/preço na tela; quem manda na regra de compra
// é o enum do Programador A.
typedef struct {
    char nome[25];
    char desc[25];
    int custo;
} ItemLojaUI;

// Frases de narrativa de cada fase (GDD seção 2). Ficam fora da struct
// Fase do Programador A de propósito: Fase (fase.h) é dado de jogo puro
// (meta, tempo, linguagem) compartilhável com qualquer interface; texto
// de narrativa é responsabilidade exclusiva da camada visual (main.c).
static const char *HISTORIA_FASES[TOTAL_FASES] = {
    "Servidor legado detectado. Quebre a barreira em C.",
    "Seguranca atualizada. Filtre a entrada em C++.",
    "Kernel protegido. Injete instrucoes direto em Assembly.",
    "Maquina virtual corporativa ativa. Intercepte o Java.",
    "Script automatizado interceptado. Sobrescreva o codigo!",
    "Interface web de monitoramento. Buga o JavaScript!",
    "Mainframe de aplicativo mobile. Contorne as diretivas.",
    "Banco de dados corporativo. Extraia as informacoes!"
};

// Obter o nome da linguagem da fase
const char* ObterNomeLinguagem(int id) {
    switch(id) {
        case 1: return "C";
        case 2: return "C++";
        case 3: return "Assembly";
        case 4: return "Java";
        case 5: return "Python";
        case 6: return "JavaScript";
        case 7: return "Kotlin/Swift";
        case 8: return "SQL";
        default: return "Desconhecida";
    }
}


//FUNÇÃO PRINCIPAL (MAIN)
int main(void) {
    //Cofiguração de telas
    InitWindow(960, 720, "Neon Type");
    SetTargetFPS(60);  
    RenderTexture2D jogo = LoadRenderTexture(320, 240);

    //Configuração Audio
    InitAudioDevice();
    Music musica = LoadMusicStream("music_level_low.mp3");
    PlayMusicStream(musica);
    Music musica_level_high = LoadMusicStream("music_level_high.mp3"); // <---
    Sound selectSom = LoadSound("select.button.wav");
    Music vitoria = LoadMusicStream("victory.wav");
    Music gameover = LoadMusicStream("lose.wav");
    Music combo10 = LoadMusicStream("music_level_high.mp3");
    Sound letraErrada = LoadSound("wrong.letter.wav");
    Sound letraCorreta = LoadSound("correct.letter.wav");
    Sound moeda = LoadSound("moeda.wav");
    Sound erro = LoadSound("erro.wav");
    //Configuração de fonte
    Font fontepalavra = LoadFont("fontepalavras.ttf");
    //Design (Objetos, Telas...)
    // -- MENU --
    Texture2D fundoMenu = LoadTexture("interface.menu.png");
    SetTextureFilter(fundoMenu, TEXTURE_FILTER_POINT);
    Texture2D interfaceVitoria = LoadTexture("interface.levelclear.png");
    SetTextureFilter(interfaceVitoria, TEXTURE_FILTER_POINT);   
    Texture2D interfaceTerminal = LoadTexture("terminal.interface.png");
    SetTextureFilter(interfaceTerminal, TEXTURE_FILTER_POINT);
    Texture2D hackerNormal[3];
    hackerNormal[0] = LoadTexture("idle.sprite1.png");
    SetTextureFilter(hackerNormal[0], TEXTURE_FILTER_POINT);
    hackerNormal[1] = LoadTexture("idle.sprite2.png");
    SetTextureFilter(hackerNormal[1], TEXTURE_FILTER_POINT);
    hackerNormal[2] = LoadTexture("idle.sprite3.png");
    SetTextureFilter(hackerNormal[2], TEXTURE_FILTER_POINT);
    int framehackerNormal = 0;  
    Texture2D hackerLevelclear = LoadTexture("levelclear.sprite1.png");
    SetTextureFilter(hackerLevelclear, TEXTURE_FILTER_POINT);
    Texture2D vitoriaHacker1 = LoadTexture("victory.sprite1.png");
    SetTextureFilter(vitoriaHacker1, TEXTURE_FILTER_POINT);
    Texture2D vitoriaHacker2 = LoadTexture("victory.sprite2.png");
    SetTextureFilter(vitoriaHacker2, TEXTURE_FILTER_POINT);
    Texture2D spriteAtual;
    Texture2D hackersentado[3];
    hackersentado[0] = LoadTexture("shopping.sprite1.png");
    SetTextureFilter(hackersentado[0], TEXTURE_FILTER_POINT);
    hackersentado[1] = LoadTexture("shopping.sprite2.png");
    SetTextureFilter(hackersentado[1], TEXTURE_FILTER_POINT);
    hackersentado[2] = LoadTexture("shopping.sprite3.png");
    SetTextureFilter(hackersentado[2], TEXTURE_FILTER_POINT);
    Texture2D notebookfechado = LoadTexture("notebook.fechado.png");
    SetTextureFilter(notebookfechado, TEXTURE_FILTER_POINT);
    
    int frameLogin = 0;
    int contadorframeLogin = 0;
    
    //Combo x2
    Texture2D hackercombo2[2];
    hackercombo2[0] = LoadTexture("combox2.sprite1.png");
    SetTextureFilter(hackercombo2[0], TEXTURE_FILTER_POINT);
    hackercombo2[1] = LoadTexture("combox2.sprite2.png");
    SetTextureFilter(hackercombo2[1], TEXTURE_FILTER_POINT);
    //Combo x10
    Texture2D combo10hacker[3];
    combo10hacker[0] = LoadTexture("combox10.sprite1.png");
    SetTextureFilter(combo10hacker[0], TEXTURE_FILTER_POINT);
    combo10hacker[1] = LoadTexture("combox10.sprite2.png");
    SetTextureFilter(combo10hacker[1], TEXTURE_FILTER_POINT);
    combo10hacker[2] = LoadTexture("combox10.sprite3.png");
    SetTextureFilter(combo10hacker[2], TEXTURE_FILTER_POINT);
    //Combo Frenesi x20
    Texture2D combofrenesi[3];
    combofrenesi[0] = LoadTexture("frenesi.sprite1.png");
    SetTextureFilter(combofrenesi[0], TEXTURE_FILTER_POINT);
    combofrenesi[1] = LoadTexture("frenesi.sprite2.png");
    SetTextureFilter(combofrenesi[1], TEXTURE_FILTER_POINT);
    combofrenesi[2] = LoadTexture("frenesi.sprite3.png");
    SetTextureFilter(combofrenesi[2], TEXTURE_FILTER_POINT);
    
    // -- GAMEPLAY
    Texture2D fundoGameplay = LoadTexture("gameplay.combox10bg.png");
    SetTextureFilter(fundoGameplay, TEXTURE_FILTER_POINT);
    Texture2D spritefixo1 = LoadTexture("barras.normal - sem.colchete.png");
    SetTextureFilter(spritefixo1, TEXTURE_FILTER_POINT);
    Texture2D spritefixo2 = LoadTexture("barras.combox10.png");
    SetTextureFilter(spritefixo2, TEXTURE_FILTER_POINT);
    Texture2D coracao = LoadTexture("vida.png");
    SetTextureFilter(coracao, TEXTURE_FILTER_POINT);
    Texture2D inputbar = LoadTexture("gameplay.inputbar2.png");
    SetTextureFilter(inputbar, TEXTURE_FILTER_POINT);
    Texture2D notebookNormal = LoadTexture("notebook.normal.png");
    SetTextureFilter(notebookNormal, TEXTURE_FILTER_POINT);
    
    // -- LOJA
    Texture2D Moeda[3];
    Moeda[0] = LoadTexture("moeda.frente.png");
    Moeda[1] = LoadTexture("moeda.lado.png");
    Moeda[2] = LoadTexture("moeda.meiodelado.png");
    int frameMoeda = 0;
    int contadorframeMoeda = 0;
    Texture2D shop = LoadTexture("Shopping.png");
    SetTextureFilter(shop, TEXTURE_FILTER_POINT);
    Texture2D vidaextra = LoadTexture("extralife.png");
    SetTextureFilter(vidaextra, TEXTURE_FILTER_POINT);
    Texture2D kit = LoadTexture("kit.png");
    SetTextureFilter(kit, TEXTURE_FILTER_POINT);
    Texture2D databackup = LoadTexture("backupdata.png");
    SetTextureFilter(databackup, TEXTURE_FILTER_POINT);
    Texture2D overlock = LoadTexture("overlock.png");
    SetTextureFilter(overlock, TEXTURE_FILTER_POINT);
    
    //Inicialização inicial
    TelasJogo telaAtual = MENU;
    Jogador jogador = jogador_criar("Hacker");

    // Semente do sorteio de palavras (Fisher-Yates em palavra.c). Sem
    // isso, rand() sempre devolveria a mesma sequência a cada execução.
    srand((unsigned int)time(NULL));

    // Banco de palavras: 30 termos por linguagem, carregados uma única
    // vez de data/palavras.txt (GDD seção 11.2). Se o arquivo não for
    // encontrado, o jogo ainda abre, mas sem palavras para digitar --
    // por isso o aviso no terminal, útil para depurar o caminho do
    // executável durante o desenvolvimento.
    BancoPalavras banco;
    if (!palavras_carregar_arquivo("data/palavras.txt", &banco)) {
        TraceLog(LOG_WARNING, "Nao foi possivel abrir data/palavras.txt");
    }

    // Ranking persistente (Top 3) lido do disco (GDD seção 10).
    Ranking ranking = ranking_carregar("data/ranking.txt");
    bool rankingFoiAtualizado = false; // evita salvar 2x no mesmo Game Over

    //Variaveis de captura de teclado
    char input[100] = "";
    int tamanhoinput = 0;
    char inputLogin[50] = "";
    int tamanhoInputLogin = 0;

    // Índice de fase 0-based (faseAtual+1 == numero_fase em TABELA_FASES).
    int faseAtual = 0;
    int totalFases = TOTAL_FASES;

    // Aponta sempre para a configuração fixa (meta/tempo/linguagem) da
    // fase em andamento. Vem de fase.c (TABELA_FASES), que já contém os
    // valores exatos do GDD seção 3 -- não duplicamos esses números aqui.
    const Fase *faseConfig = fase_obter_config(faseAtual + 1);
    // Cronômetro regressivo da fase (não existia antes: o "TIME" do
    // layout do GDD seção 5 nunca tinha sido implementado). Reiniciado
    // sempre que uma fase nova começa.
    TimerFase timerFase = fase_timer_iniciar(faseConfig);

    //ITENS DA LOJA (texto de exibição; preço/regra real vêm de economia.c)
    ItemLojaUI catalogoLoja[] = {
        {"Kit Refrigeracao", "+5s no cronometro", PRECOS_LOJA[ITEM_REFRIGERACAO]},
        {"Overclock Proc.", "+10s no cronometro", PRECOS_LOJA[ITEM_OVERCLOCK]},
        {"Backup de Dados", "Novas 5 vidas se zerar", PRECOS_LOJA[ITEM_BACKUP_DADOS]},
        {"Vida Extra", "+1 vida (max 5)", PRECOS_LOJA[ITEM_VIDA_EXTRA]}
    };

    // Palavra atual sorteada pelo banco (substitui o mock fixo
    // "printf"/"return"/"include"). Embaralha o dicionário da linguagem
    // da fase 1 antes do primeiro sorteio, conforme o fluxo descrito no
    // README do Programador A.
    palavras_embaralhar_linguagem(&banco, faseConfig->linguagem_id);
    char palavraAtual[MAX_TEXTO_PALAVRA] = "";
    const Palavra *palavraSorteada = palavras_sortear(&banco, faseConfig->linguagem_id);
    if (palavraSorteada != NULL) TextCopy(palavraAtual, palavraSorteada->texto);

    // ANIMAÇÃO DE TERMINAL NA TELA DE INTRODUÇÃO DA FASE
    
    // Movimento (Descer o texto)
    int yTexto = -20;
    int destinoY = 15;
    //Animação de maquina de escrever (Letra por letra)
    int letrasVisiveis = 0;
    float timerTexto = 0.0f;
    char textoHacker[150] = "";
    // Aparece frase por frase
    int estadoIntro = 0; 
    
    //Variaveis das telas com opcao
    int opcaoMenu = 0;
    int opcaoPausa = 0;
    int opcaoLoja = 0;
    // efeito ao comprar um item
    float timerBrilhoLoja = 0.0f;
    bool SairPausa = false;
    
    //Guardar scoreAtual do jogador
    int scoreAtual = 0;
    
    // Antes do while, ele aponta para uma música que carregou:
    Music *musicaAtual = &musica;

    while (!WindowShouldClose()) {
       Music *musicaDesejada = &musica; // Começa na trilha 1-4

        if (faseAtual >= 4) { // Nível 5 (index 4) em diante
            musicaDesejada = &musica_level_high;
        }

        if (jogador.combo >= 10) {
            musicaDesejada = &combo10; // Frenesi ativado
        }

    // Troca apenas se mudou
        if (musicaAtual != musicaDesejada) {
            StopMusicStream(*musicaAtual);
            PlayMusicStream(*musicaDesejada);
            musicaAtual = musicaDesejada;
        }

        UpdateMusicStream(*musicaAtual); 

        // ETAPA 1: LÓGICA
        switch (telaAtual) {
            case MENU:
                //função do menu
                if (IsKeyPressed(KEY_DOWN)){ 
                    (opcaoMenu)++; 
                    PlaySound(selectSom);
                    }
                if (IsKeyPressed(KEY_UP)) {
                    (opcaoMenu)--; 
                    PlaySound(selectSom); 
                    }
                if (opcaoMenu > 2) 
                    opcaoMenu = 0;
                if (opcaoMenu < 0) 
                    opcaoMenu = 2;
    
                if (IsKeyPressed(KEY_ENTER)) {
                    switch (opcaoMenu) {
                        case 0: PlaySound(selectSom);
                        telaAtual = LOGIN; break;
                        case 1: PlaySound(selectSom);
                        telaAtual = RANKING; break;
                        case 2: PlaySound(selectSom);
                        exit(0); break; 
                }
            }
                break;
            
            case LOGIN: {
                // SE ESTIVERMOS COMEÇANDO AGORA, LIMPA O BUFFER 
                static bool precisaLimpar = true; 
                if (precisaLimpar) {
                    tamanhoInputLogin = 0;
                    inputLogin[0] = '\0';
                    precisaLimpar = false; 
                }
                
                contadorframeLogin++;
                if (contadorframeLogin >= 20) { // troca a cada 15 frames 
                    frameLogin++;
                    contadorframeLogin = 0;
                    if (frameLogin >= 3)
                        frameLogin = 0;
                }
                int tecla = GetCharPressed();
                //captura de teclado
                while (tecla > 0) {
                    if ((tecla >= 32) && (tecla <= 125) && tamanhoInputLogin < 10) {
                        inputLogin[tamanhoInputLogin] = (char)tecla;
                        tamanhoInputLogin++;
                        inputLogin[tamanhoInputLogin] = '\0';
                    }
                    tecla = GetCharPressed();
                }
                //sistema de backspace (apagar)
                if (IsKeyPressed(KEY_BACKSPACE) && tamanhoInputLogin > 0) {
                    tamanhoInputLogin--;
                    inputLogin[tamanhoInputLogin] = '\0';
                }
                // Salva o nome do jogador
                if (IsKeyPressed(KEY_ENTER)) {
                    if (tamanhoInputLogin == 0) {
                        TextCopy(jogador.nome, "Hacker");
                    } else {
                        TextCopy(jogador.nome, inputLogin);
                    }

                    faseAtual = 0; // Volta para o Level 1
                    faseConfig = fase_obter_config(faseAtual + 1); // Carrega config do Level 1
                    timerFase = fase_timer_iniciar(faseConfig);    // Reinicia o timer do Level 1
                    jogador = jogador_criar(jogador.nome);         // Recria o jogador do zero (limpa nível, score, tudo)

                    // --- RESETAR O INTERRUPTOR ---
                    precisaLimpar = true;

                    // Animação para a prox tela (gameplay)
                    yTexto = -20; 
                    letrasVisiveis = 0; 
                    timerTexto = 0.0f;
                    estadoIntro = 0; 
                    jogador_resetar_fase(&jogador);
                    jogador_resetar_total(&jogador); // Zera score, checkpoint e moedas 
                    jogador_salvar_checkpoint(&jogador); // O checkpoint começa em 0
                    // Limpa os bônus visuais para não transbordar entre fases
                    for (int i = 0; i < MAX_BONUS; i++) {
                        listaBonus[i].ativo = 0;
                        listaBonus[i].alpha = 0.0f; // Garante que fique invisível
                    }

                    telaAtual = INTRO_FASE;
                }
                break;
            }
            
            case INTRO_FASE:
                if (faseAtual > 0 && estadoIntro < 3) {
                    estadoIntro = 3; 
                }
                //Mostrar frases prontas
                if (estadoIntro == 0) {
                    TextCopy(textoHacker, "Iniciando Protocolo de Invasao...");
                } else if (estadoIntro == 1) {
                    TextCopy(textoHacker, "Conectando ao Servidor Central...");
                } else if (estadoIntro == 2) {
                    TextCopy(textoHacker, TextFormat("%s, precisamos desses dados agora!", jogador.nome));
                } else if (estadoIntro == 3) {
                    TextCopy(textoHacker, TextFormat("> %s", HISTORIA_FASES[faseAtual]));
                }
                //Efeito terminal (letra por letra)
                if (yTexto < destinoY) {
                    yTexto += 2;
                } else {
                    timerTexto += GetFrameTime();
                    //velocidade que a letras aparecem
                    if (timerTexto >= 0.03f) { 
                        if (letrasVisiveis < TextLength(textoHacker)) letrasVisiveis++;
                        timerTexto = 0.0f;
                    }
                }
                //verifica se todas as letras da frase apareceram e muda para a prx texto de introduçaõ da fase
                if (IsKeyPressed(KEY_ENTER)) {
                    if (letrasVisiveis >= TextLength(textoHacker)) {
                        if (estadoIntro < 3) {
                            estadoIntro++;
                            letrasVisiveis = 0;
                            timerTexto = 0.0f;
                        } else {

                            // AQUI: Reseta o estado do jogador para garantir que ele comece do Level 1
                            jogador_resetar_total(&jogador);

                            faseIniciando = true;
                            telaAtual = GAMEPLAY; 
                        }
                    } else {
                        letrasVisiveis = TextLength(textoHacker); 
                    }
                }
                break;
                
            case GAMEPLAY: 
            // Se for o início de uma nova partida, zera tudo
                if (faseIniciando) {
                    tamanhoinput = 0;
                    input[0] = '\0';
                    faseIniciando = false; // Agora que limpamos, desativa a flag
                }

                if (SairPausa) {
                    while (GetCharPressed() != 0);
                    SairPausa = false;
                }
                ResultadoFase checagemTempo = fase_avaliar_resultado(&jogador, &timerFase, faseConfig);
                if (checagemTempo == FASE_GAME_OVER) {
                    telaAtual = GAMEOVER;
                    rankingFoiAtualizado = false;
                    StopMusicStream(musica);
                    PlayMusicStream(gameover);
                    break; 
                }
                // Cronômetro regressivo da fase (GDD seção 3: "Tempo").
                // Antes não existia nenhuma contagem real; agora decrementa
                // a cada frame via fase_timer_atualizar (fase.c), que nunca
                // deixa o valor ficar negativo.
                fase_timer_atualizar(&timerFase, GetFrameTime());

                int maxLetras = TextLength(palavraAtual);
                int tecla = GetCharPressed();
                //loica de capturar teclado
                while (tecla > 0) {
                    if ((tecla >= 32) && (tecla <= 125) && tamanhoinput < maxLetras) {
                        input[tamanhoinput] = (char)tecla;
                        framehackerNormal++;
                        if(framehackerNormal >= 3){
                            framehackerNormal = 0;
                        }
                        if (input[tamanhoinput] != palavraAtual[tamanhoinput]) {
                            PlaySound(letraErrada);
                            jogador_zerar_combo(&jogador);
                        } else {
                            PlaySound(letraCorreta);
                        }
                        tamanhoinput++;
                        input[tamanhoinput] = '\0';
                    }
                    tecla = GetCharPressed();
                }
                //sistema de apagar letra
                if (IsKeyPressed(KEY_BACKSPACE) && tamanhoinput > 0) {
                    tamanhoinput--;
                    input[tamanhoinput] = '\0';
                }
                //verifica se a palavra a letra é correta
                if (tamanhoinput == maxLetras && maxLetras > 0) {
                    int palavraCorreta = 1;
                    for (int i = 0; i < maxLetras; i++) {
                        if (input[i] != palavraAtual[i]) {
                            palavraCorreta = 0;
                            break;
                        }
                    }
                    int morreuNestaJogada = 0;

                    //sistema de acerto e recompensas
                    if (palavraCorreta) {
                        // jogador_incrementar_combo cuida do bônus de vida
                        // automático ao cruzar combo==20 (GDD seção 4.3);
                        // jogador_completar_palavra aplica moedas e score
                        // usando o multiplicador de combo (GDD seção 12).
                        jogador_incrementar_combo(&jogador);
                        jogador_completar_palavra(&jogador);

                        // --- NOVA LÓGICA DE TEMPO ---
                        // Verifica se o combo é par (maior que 0) e adiciona 1 segundo
                        // No combo par: +1s (Amarelo, tamanho padrão do timer)
                        if (jogador.combo > 0 && jogador.combo % 2 == 0) {
                            fase_timer_adicionar_tempo(&timerFase, 1);
                            adicionar_bonus_visual("+1s", (Color){ 255, 255, 0, 255 }, 20);
                            }

                        // No combo múltiplo de 5: +2s (Magenta, maior)
                        if (jogador.combo > 0 && jogador.combo % 5 == 0) {
                            fase_timer_adicionar_tempo(&timerFase, 1); 
                            adicionar_bonus_visual("+2s", (Color){ 255, 0, 255, 255 }, 25);
                        }

                        if (jogador.combo >= 20) {
                            if (jogador.vidas < 5) {
                                jogador.vidas++;
                                PlaySound(moeda); // Toca um som de feedback de que ganhou algo
                            }
                        }
                    } else {
                        morreuNestaJogada = jogador_perder_vida(&jogador);
                        jogador_zerar_combo(&jogador);
                        // "Seguro" do item Backup de Dados (GDD seção 7):
                        // se as vidas zeraram e o backup ainda não foi
                        // usado nesta fase, ele se consome agora e restaura
                        // as 5 vidas em vez de ir para Game Over.
                        if (morreuNestaJogada && jogador.backup_usado) {
                            jogador_restaurar_vidas_full(&jogador);
                            jogador.backup_usado = 0;
                            morreuNestaJogada = 0;
                            PlaySound(moeda);
                        }
                    }
                   
                    // fase_avaliar_resultado centraliza a regra do GDD
                    // seção 3: vitória (meta atingida) tem prioridade sobre
                    // derrota, mesmo que vidas/tempo tenham zerado no mesmo
                    // instante.
                    ResultadoFase resultado = fase_avaliar_resultado(&jogador, &timerFase, faseConfig);

                    if (resultado == FASE_VITORIA) {
                        telaAtual = VICTORY;
                        StopMusicStream(musica);
                        PlayMusicStream(vitoria);
                    } else if (resultado == FASE_GAME_OVER) {
                        telaAtual = GAMEOVER;
                        rankingFoiAtualizado = false;
                        StopMusicStream(musica);
                        PlayMusicStream(gameover);
                    } else {
                        tamanhoinput = 0; input[0] = '\0';
                        // Só sorteia a próxima palavra se a jogada foi um
                        // acerto -- ao errar, a MESMA palavra continua na
                        // tela para o jogador tentar de novo (consistente
                        // com o comportamento original do jogo).
                        if (palavraCorreta) {
                            const Palavra *prox = palavras_sortear(&banco, faseConfig->linguagem_id);
                            if (prox != NULL) TextCopy(palavraAtual, prox->texto);
                        }
                    }
                }
                //Pausa (incompleto por enquanto)
                if (IsKeyPressed(KEY_ENTER)) telaAtual = PAUSA;
                break;
            
            case PAUSA:
                if (IsKeyPressed(KEY_DOWN)) {
                    (opcaoPausa)++;
                    PlaySound(selectSom);
                    }
                if (IsKeyPressed(KEY_UP)) {
                    (opcaoPausa)--;
                    PlaySound(selectSom);
                    }
                if (opcaoPausa > 2) 
                    opcaoPausa = 0;
                if (opcaoPausa < 0) 
                    opcaoPausa = 2;
    
                if (IsKeyPressed(KEY_ENTER)) {
                    switch (opcaoPausa) {
                        case 0:           
                            SairPausa = true;
                            PlaySound(selectSom);

                            jogador_resetar_total(&jogador);

                            faseIniciando = true;
                            telaAtual = GAMEPLAY; 
                        break;
                        case 1: PlaySound(selectSom);
                            telaAtual = MENU; break;
                        case 2: PlaySound(selectSom);
                            jogador.combo = 0;
                            jogador.palavras_fase = 0; 
                            jogador.vidas = 5; 
                            jogador_reiniciar_fase(&jogador);
                            tamanhoinput = 0;
                            input[0] = '\0';
                            timerFase = fase_timer_iniciar(faseConfig);

                            palavras_embaralhar_linguagem(&banco, faseConfig->linguagem_id);
                            const Palavra *prox = palavras_sortear(&banco, faseConfig->linguagem_id);
                            if (prox != NULL) TextCopy(palavraAtual, prox->texto);
                            yTexto = -20;
                            letrasVisiveis = 0;
                            timerTexto = 0.0f;
                            estadoIntro = 0;
                            telaAtual = INTRO_FASE; break;
                    }
                }
                break;
                
            case LOJA:
                contadorframeMoeda++;

                if (contadorframeMoeda >= 10) {  // troca a cada 10 frames
                    contadorframeMoeda = 0;
                    frameMoeda++;
                    if (frameMoeda >= 3)
                        frameMoeda = 0;
                }
                if (timerBrilhoLoja > 0.0f) 
                    timerBrilhoLoja -= GetFrameTime();
                if (IsKeyPressed(KEY_DOWN)) {
                     PlaySound(selectSom);
                    (opcaoLoja)++; 
                if (opcaoLoja > 4)
                    opcaoLoja = 0; 
                }
                if (IsKeyPressed(KEY_UP)) {
                    PlaySound(selectSom);
                    (opcaoLoja)--; 
                    if (opcaoLoja < 0) 
                        opcaoLoja = 4; 
                }
    
                if (IsKeyPressed(KEY_ENTER)) {
                    switch (opcaoLoja) {
                        case 0: {
                            // economia_comprar_item já verifica saldo e
                            // aplica o efeito (+5s no timerFase) em uma
                            // única chamada -- nenhum estado fica "pela
                            // metade" se a compra falhar.
                            ResultadoCompra r = economia_comprar_item(&jogador, &timerFase, ITEM_REFRIGERACAO);
                            if (r == COMPRA_SUCESSO) {
                                PlaySound(moeda);
                                timerBrilhoLoja = 0.5f;
                            } else {
                                PlaySound(erro);
                            }
                            break;
                        }
                        case 1: {
                            ResultadoCompra r = economia_comprar_item(&jogador, &timerFase, ITEM_OVERCLOCK);
                            if (r == COMPRA_SUCESSO) {
                                PlaySound(moeda);
                                timerBrilhoLoja = 0.5f;
                            } else {
                                PlaySound(erro);
                            }
                            break;
                        }
                        case 2: {
                            ResultadoCompra r = economia_comprar_item(&jogador, &timerFase, ITEM_BACKUP_DADOS);
                            if (r == COMPRA_SUCESSO) {
                                PlaySound(moeda);
                                timerBrilhoLoja = 0.5f;
                            } else {
                                PlaySound(erro);
                            }
                            break;
                        }
                        case 3: {
                            ResultadoCompra r = economia_comprar_item(&jogador, &timerFase, ITEM_VIDA_EXTRA);
                            if (r == COMPRA_SUCESSO) {
                                PlaySound(moeda);
                                timerBrilhoLoja = 0.5f;
                            } else {
                                PlaySound(erro);
                            }
                            break;
                        }
                        case 4: 
                            PlaySound(selectSom);
                            (faseAtual)++;
                            // Salva o score que foi acumulado até o final da fase anterior
                            // e o gasto na loja, estabelecendo o novo saldo base.
                            jogador_salvar_checkpoint(&jogador); 
                            // --------------------------

                            tamanhoinput = 0;
                            input[0] = '\0';

                            if (faseAtual >= totalFases) {
                                // Vitória Final (GDD seção 6: Level 8
                                // concluído). O score do jogador também
                                // entra na disputa do Top 3 aqui -- antes
                                // só o Game Over salvava no ranking, então
                                // quem terminasse o jogo nunca aparecia
                                // no ranking.txt.
                                if (ranking_score_qualifica(&ranking, jogador.score)) {
                                    ranking_inserir(&ranking, jogador.nome, jogador.score);
                                    ranking_salvar(&ranking, "data/ranking.txt");
                                }
                                faseAtual = 0;
                                telaAtual = RANKING;
                            } else {

                               // --- AQUI GARANTE O  LIMPO ---
                                tamanhoinput = 0;
                                input[0] = '\0';

                                // Reconfigura fase/timer/banco de palavras
                                // para a próxima fase (faseAtual já foi
                                // incrementado acima). jogador_resetar_fase
                                // zera palavras_fase e a flag de backup,
                                // preservando vidas/combo/score/moedas,
                                // conforme o GDD (moedas e score persistem
                                // entre fases; vidas/combo continuam de
                                // onde estavam).
                                scoreAtual = jogador.score;
                                jogador_resetar_fase(&jogador);

                                // Limpa os bônus visuais para não transbordar entre fases
                                for (int i = 0; i < MAX_BONUS; i++) {
                                    listaBonus[i].ativo = 0;
                                    listaBonus[i].alpha = 0.0f; // Garante que fique invisível
                                }

                                faseConfig = fase_obter_config(faseAtual + 1);
                                timerFase = fase_timer_iniciar(faseConfig);
                                 
                                // --- AQUI APLICA O TEMPO EXTRA ---
                                timerFase.tempo_restante += jogador.tempo_extra_acumulado;
                                jogador.tempo_extra_acumulado = 0; // Reseta para não acumular para sempre
                                
                                palavras_embaralhar_linguagem(&banco, faseConfig->linguagem_id);
                                const Palavra *prox = palavras_sortear(&banco, faseConfig->linguagem_id);
                                if (prox != NULL) TextCopy(palavraAtual, prox->texto);

                                yTexto = -20;
                                letrasVisiveis = 0;
                                timerTexto = 0.0f;
                                estadoIntro = 0; 
                                telaAtual = INTRO_FASE;
                            }
                    }
                }
                            break;
                break;
                
            case GAMEOVER:
                StopMusicStream(musica);
                UpdateMusicStream(gameover);
                // Ranking (GDD seção 10): ao perder, se o score qualifica
                // para o Top 3, grava o nome digitado no LOGIN + score no
                // ranking.txt. rankingFoiAtualizado evita salvar de novo a
                // cada frame enquanto o jogador ainda está na tela de
                // Game Over olhando o resultado.
                if (!rankingFoiAtualizado) {
                    if (ranking_score_qualifica(&ranking, jogador.score)) {
                        ranking_inserir(&ranking, jogador.nome, jogador.score);
                        ranking_salvar(&ranking, "data/ranking.txt");
                    }
                    rankingFoiAtualizado = true;
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    StopMusicStream(gameover);
                    PlayMusicStream(musica);
                    
                // --- LIMPEZA TOTAL ---
                ResetarInputGameplay();
        
                // --- RESET DO JOGADOR ---
                jogador = jogador_criar(jogador.nome);
                faseAtual = 0;
                faseConfig = fase_obter_config(faseAtual + 1);
                timerFase = fase_timer_iniciar(faseConfig);
                palavras_embaralhar_linguagem(&banco, faseConfig->linguagem_id);
                const Palavra *prox = palavras_sortear(&banco, faseConfig->linguagem_id);
                
                if (prox != NULL) TextCopy(palavraAtual, prox->texto);
                telaAtual = MENU;
                }
            break; 
              

            case VICTORY:
                StopMusicStream(musica);
                UpdateMusicStream(vitoria);
                if (IsKeyPressed(KEY_ENTER)) {
                    StopMusicStream(vitoria);
                    PlayMusicStream(musica);
                    jogador_zerar_combo(&jogador);
                    // Nota: não há bônus fixo de moedas aqui -- o GDD
                    // (seção 4.4) define moedas só como
                    // "palavras_certas * multiplicador_combo", o que já
                    // foi aplicado palavra a palavra por
                    // jogador_completar_palavra() durante o GAMEPLAY.
                    // Um bônus extra de +100 ao vencer a fase não consta
                    // na especificação, então foi removido aqui.
                    telaAtual = LOJA;      
                }
                break;
                
            case RANKING:
                if (IsKeyPressed(KEY_ENTER)) {
                    // Reinicia o ciclo completo de jogo (igual ao "Novo
                    // Jogo" do menu): sem isso, voltar do Ranking depois
                    // de uma Vitória Final levaria pro Menu com o jogador
                    // ainda no estado de quem just acabou de vencer (5
                    // vidas, score acumulado etc.), e a próxima partida
                    // começaria "herdando" esse progresso indevidamente.
                    jogador = jogador_criar(jogador.nome);
                    faseAtual = 0;
                    faseConfig = fase_obter_config(faseAtual + 1);
                    timerFase = fase_timer_iniciar(faseConfig);
                    palavras_embaralhar_linguagem(&banco, faseConfig->linguagem_id);
                    const Palavra *prox = palavras_sortear(&banco, faseConfig->linguagem_id);
                    if (prox != NULL) TextCopy(palavraAtual, prox->texto);
                    telaAtual = MENU;
                }
                break;
        }
        
        // ETAPA 2: DESENHO
        BeginTextureMode(jogo);
            ClearBackground(BLACK);
            //Define a cor do jogo para verde
            Color corNeonBase = GREEN;
            
            //efeito visual do combo (arco-iris) (incompleto por enquanto)
            if (jogador.combo >= 20) {
                // Modo Frenesi: Arco-íris dinâmico ao longo do tempo
                corNeonBase = ColorFromHSV(GetTime() * 200, 1.0f, 1.0f); 
            } else if (jogador.combo >= 10) {
                //calcula o tempo em dezenas de segundo e divide por 3 (inteiros)
                int pulso = ((int)(GetTime() * 10)) % 3;
                // 3 estados (cada um uma cor diferente)
                if (pulso == 0) corNeonBase = LIME;
                if (pulso == 1) corNeonBase = YELLOW;
                if (pulso == 2) corNeonBase = SKYBLUE;
            } else if (jogador.combo >= 2) {
                corNeonBase = SKYBLUE;
            }
            
            //switch de telas
            switch (telaAtual) {
                case MENU:
                    ClearBackground(BLACK);
                    
                    DrawTexturePro(interfaceTerminal, (Rectangle){0, 0, (float)interfaceTerminal.width, (float)interfaceTerminal.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    Color cor = ColorFromHSV(GetTime() * 80, 1.0f, 1.0f);

                    DrawText("NEON TYPE", 40, 43, 40, Fade(cor, 0.25f));
                    DrawText("NEON TYPE", 42, 43, 40, Fade(cor, 0.25f));
                    DrawText("NEON TYPE", 40,45, 40, Fade(cor, 0.25f));
                    DrawText("NEON TYPE", 42,45, 40, Fade(cor, 0.25f));
                    DrawText("NEON TYPE", 40, 45, 40, cor);
                    
                    if (opcaoMenu == 0) DrawText("> NOVO JOGO", 90, 95, 20, YELLOW);
                    else DrawText("  NOVO JOGO", 90, 95, 17, WHITE);
                    if (opcaoMenu == 1) DrawText("> RANKING", 90, 120, 20, YELLOW);
                    else DrawText("  RANKING", 90, 120, 17, WHITE);
                    if (opcaoMenu == 2) DrawText("> SAIR", 90, 145, 20, YELLOW);
                    else DrawText("  SAIR", 90, 145, 17, WHITE);
                    break;
                    
                case LOGIN:
                    DrawTexturePro(fundoGameplay, (Rectangle){0, 0, (float)fundoGameplay.width, (float)fundoGameplay.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    DrawTexturePro(hackersentado[frameLogin],
                    (Rectangle){0, 0, (float)hackersentado[frameLogin].width, (float)hackersentado[frameLogin].height},
                    (Rectangle){-28, 70, 180, 180}, (Vector2){0, 0}, 0, WHITE);
                    DrawTexturePro(notebookfechado, (Rectangle){0, 0, (float)notebookfechado.width, (float)notebookfechado.height},
                    (Rectangle){120, 114, 90, 90},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    //Desenhar notebook
                    DrawText("Insira seu ID de Hacker", 10, 45, 25, GREEN);
                    DrawRectangleLines(80, 90, 180, 20, WHITE);
                    DrawText(TextFormat("> %s_", inputLogin), 88, 95, 10, WHITE);
                    DrawText("[ Pressione ENTER para Entrar ]", 125, 220, 6, WHITE);
                    break;
                    
                case INTRO_FASE:
                    ClearBackground(WHITE);
                    DrawTexturePro(interfaceTerminal, (Rectangle){0, 0, (float)interfaceTerminal.width, (float)interfaceTerminal.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    Color corIntro = ColorFromHSV(GetTime() * 80, 1.0f, 1.0f);

                    DrawText(TextFormat("LEVEL %d", faseConfig->numero), 68, 43, 40, Fade(corIntro, 0.25f));
                    DrawText(TextFormat("LEVEL %d", faseConfig->numero), 70, 43, 40, Fade(corIntro, 0.25f));
                    DrawText(TextFormat("LEVEL %d", faseConfig->numero), 68, 45, 40, Fade(corIntro, 0.25f));
                    DrawText(TextFormat("LEVEL %d", faseConfig->numero), 70, 45, 40, Fade(corIntro, 0.25f));
                    DrawText(TextFormat("LEVEL %d", faseConfig->numero), 68, 45, 40, corIntro);
                    
                    DrawText(TextSubtext(textoHacker, 0, letrasVisiveis), 15, 115, 2, GREEN);
                    
                    if (estadoIntro == 3) {
                        DrawText(TextFormat("META  : %d palavras.", faseConfig->meta_palavras), 25, 145, 10, WHITE);
                        DrawText(TextFormat("TEMPO : %d segundos.", faseConfig->tempo_segundos), 25, 165, 10, WHITE);
                        DrawText("[ENTER] INICIAR INVASAO", 75, 210, 10, LIME);
                    } else {
                        DrawText("[ENTER] AVANCAR", 110, 210, 10, WHITE);
                    }
                    break;
                    
                case GAMEPLAY:
                    //FUNDO
                    ClearBackground(BLACK);
                    DrawTexturePro(fundoGameplay, (Rectangle){0, 0, (float)fundoGameplay.width, (float)fundoGameplay.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    DrawTexturePro(notebookNormal, (Rectangle){0, 0, (float)notebookNormal.width, (float)notebookNormal.height},
                    (Rectangle){80, 40, 190, 190},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    //HACKER
                    Color corBarra = GREEN;
                    if (jogador.combo >= 20){
                        spriteAtual = combofrenesi[framehackerNormal];
                        if (((int)(GetTime()/0.10f)) % 2 == 0)
                            corBarra = ColorFromHSV(GetTime()*250,1,1);
                    }
                    else if (jogador.combo >= 10)
                        spriteAtual = combo10hacker[framehackerNormal];
                    else if (jogador.combo >= 2)
                        spriteAtual = hackercombo2[framehackerNormal % 2];
                    else
                        spriteAtual = hackerNormal[framehackerNormal];
                    
                    DrawTexturePro(spriteAtual, (Rectangle){0, 0, (float)spriteAtual.width, (float)spriteAtual.height},
                    (Rectangle){120, 30, 220, 220},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    DrawTexturePro(inputbar, (Rectangle){0, 0, (float)inputbar.width, (float)inputbar.height},
                    (Rectangle){-10, 92, 340, 220},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    
                    //barra
                    int barraX = 12; 
                    int barraY = 12;
                    int barraLargura = 15;
                    int barraAltura = 168;
                    // jogador.palavras_fase substitui a antiga variável
                    // paralela "palavrasCorretas" -- agora a barra lê
                    // diretamente o contador oficial do Jogador (jogador.c),
                    // e faseConfig->meta_palavras vem da TABELA_FASES real
                    // do GDD (10/15/15/20/20/25/25/30), não dos valores
                    // reduzidos de teste que existiam antes.
                    int alturaPreenchida = (jogador.palavras_fase * barraAltura) /
                        faseConfig->meta_palavras;
                    
                    //Desenhar o combo
                    if (jogador.combo >= 2){
                        Color cor;
                        if (((int)(GetTime()/0.12f)) % 2 == 0){
                            cor = ColorFromHSV(GetTime()*250, 1.0f, 1.0f);
                        } else {
                            cor = WHITE;
                        }
                            DrawTexturePro(
                            spritefixo2,
                            (Rectangle){0,0,spritefixo2.width,spritefixo2.height},
                            (Rectangle){0,0,320,240},
                            (Vector2){0,0},
                            0,
                            cor
                            );
                            }
                        else {
                            DrawTexturePro(
                            spritefixo1,
                            (Rectangle){0,0,spritefixo1.width,spritefixo1.height},
                            (Rectangle){0,0,320,240},
                            (Vector2){0,0},
                            0,
                            WHITE
                            );
                        }
                    DrawRectangle(
                        barraX,
                        barraY + (barraAltura - alturaPreenchida),
                        barraLargura,
                        alturaPreenchida,
                        corBarra
                        );
                    //Cabeçalho da tela de gameplay
                    DrawText(TextFormat("%05d", jogador.score), 113, 8, 10, GREEN);
                    // Cronômetro regressivo (GDD seção 5: "[TIME: 30s]").
                    // Antes não existia exibição nenhuma do tempo restante
                    // -- nem o timer em si era atualizado. Fica vermelho
                    // nos últimos 5s como aviso visual de urgência.
                    Color corTempo = (timerFase.tempo_restante <= 5.0f) ? RED : GREEN;
                    DrawText(TextFormat("%02d", (int)timerFase.tempo_restante), 270, 8, 10, corTempo);

                    // --- CÓDIGO DO BÔNUS AQUI ---
                    for (int i = 0; i < MAX_BONUS; i++) {
                        if (listaBonus[i].ativo) {
                            listaBonus[i].alpha -= 0.015f; // Fade um pouco mais rápido para parecer "brilho"
                            listaBonus[i].y += 0.8f;       // Desce mais rápido para ganhar dinamismo
        
                        DrawText(listaBonus[i].texto, (int)listaBonus[i].x, (int)listaBonus[i].y, 
                        listaBonus[i].tamanho, Fade(listaBonus[i].cor, listaBonus[i].alpha));
        
                        if (listaBonus[i].alpha <= 0) listaBonus[i].ativo = 0;
                        }
                    }

                    // -------CÓDIGO DOS CORAÇÕES CONTINUAM AQUI---------

                        for (int i = 0; i < 5; i++){
                             if (i < jogador.vidas){ 
                            DrawTexturePro(
                                coracao,
                                (Rectangle){0, 0, (float)coracao.width, (float)coracao.height},
                                (Rectangle){12 + i * 15, 222, 20, 20},
                                (Vector2){0, 0},
                                0,
                                WHITE
                                );
                             }
                        }

                    //variaveis para descobrir o tamanho da palavra e quantas letras o jogador digitou
                    int tamP = TextLength(palavraAtual);
                    int tamI = TextLength(input);
                    
                    //centralizar a palavra na tela
                    int centroX = (320 - (tamP * 8)) / 2;
                    int centroY = 110;
                    //sistema de combo (incompleto por enquanto)
                    Color corLetraAcertada = WHITE;
                    //percorre toda a palavra
                    for (int i = 0; i < tamP; i++) {
                        char letra[2] = { palavraAtual[i], '\0' };
                        Vector2 posicaoLetra = { (float)(centroX + (i * 8)), (float)centroY };
                        if (i < tamI) {
                            if (input[i] == palavraAtual[i]) 
                                DrawTextEx(fontepalavra, letra, posicaoLetra, 12.0f, 2.0f, corLetraAcertada);
                            else 
                                DrawTextEx(fontepalavra, letra, posicaoLetra, 12.0f, 2.0f, RED);
                            } else {
                                DrawTextEx(fontepalavra, letra, posicaoLetra, 12.0f, 2.0f, DARKGREEN);
                            }
                    }
                    //Rodape da tela de gameplay
                    Vector2 posicaoInput = { 73.0f, 196.0f }; 
                    DrawTextEx(fontepalavra, TextFormat(" %s_", input), posicaoInput, 12.0f, 1.0f, WHITE);
                    DrawText(TextFormat("x%d", jogador.combo), 185, 225, 12, (jogador.combo >= 20)? corNeonBase : GREEN);
                    DrawText(TextFormat("%d", faseAtual + 1), 292, 226, 12, corNeonBase);

                    break;
                case LOJA: {
                    DrawTexturePro(interfaceTerminal, (Rectangle){0, 0, (float)interfaceTerminal.width, (float)interfaceTerminal.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    //efeito de brilho ao comprar um item
                    Color corTxtL = (timerBrilhoLoja > 0.0f) ? LIME : WHITE;
                    //desenho do cabeçalho
                    DrawText("LOJA", 15, 10, 10, GREEN);
                    DrawTexturePro(Moeda[frameMoeda], (Rectangle){0, 0, Moeda[frameMoeda].width, Moeda[frameMoeda].height},
                    (Rectangle){280, 6, 16, 16}, (Vector2){0, 0}, 0, WHITE );
                    DrawText(TextFormat("$: %d ", jogador.moedas), 252, 10, 10, GREEN);
                    
                    //variaveis para os retangulos da loja
                    int cardX = 15;
                    int cardW = 290;
                    int cardH = 32; 
                    //percorre os 4 itens da loja
                    for (int i = 0; i < 4; i++) {
                        //calcula a altura do item
                        int cardY = 40 + (i * 38);
                        //muda a cor do retangulo selecionado
                        Color corBordaCard = (i == opcaoLoja) ? YELLOW : DARKGREEN;
                        Color corTextoCard = (i == opcaoLoja) ? YELLOW : corTxtL;
                        //desenho dos retangulos
                        DrawRectangleLines(cardX, cardY, cardW, cardH, corBordaCard);
                        DrawRectangleLines(cardX + 5, cardY + 4, 24, 24, DARKGRAY);
                        //desenha os textos
                        DrawText(catalogoLoja[i].nome, cardX + 35, cardY + 5, 10, corTextoCard);
                        DrawText(TextFormat("$ %d", catalogoLoja[i].custo), cardX + 240, cardY + 5, 10, YELLOW);
                        DrawText(catalogoLoja[i].desc, cardX + 35, cardY + 18, 9, LIGHTGRAY);
                        // DESENHA O ÍCONE
                        Texture2D item;

                        switch (i)
                        {
                            case 0: item = kit; break;
                            case 1: item = overlock; break;
                            case 2: item = databackup; break;   // use o nome correto da sua variável
                            case 3: item = vidaextra; break;
                        }

                        DrawTexturePro(
                            item,
                            (Rectangle){0, 0, item.width, item.height},
                            (Rectangle){cardX + 7, cardY + 6, 20, 20},
                            (Vector2){0, 0},
                            0,
                            WHITE
                        );
                    }

                    int botaoY = 198;
                    Color corBotao = (opcaoLoja == 4) ? YELLOW : corTxtL;
                    DrawRectangleLines(cardX, botaoY, cardW, 20, corBotao);
                    DrawText("[ PROSSEGUIR PARA PROXIMA FASE ]", cardX + 45, botaoY + 5, 10, corBotao);

                    if (timerBrilhoLoja > 0.0f) {
                        DrawRectangleLines(1, 1, 318, 238, LIME);
                    }
                    break;
                }
                // pausa com botoes
                case PAUSA:
                    DrawTexturePro(fundoGameplay, (Rectangle){0, 0, (float)fundoGameplay.width, (float)fundoGameplay.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    DrawTexturePro(hackersentado[frameLogin],
                    (Rectangle){0, 0, (float)hackersentado[frameLogin].width, (float)hackersentado[frameLogin].height},
                    (Rectangle){-28, 70, 180, 180}, (Vector2){0, 0}, 0, WHITE);
                    DrawTexturePro(notebookfechado, (Rectangle){0, 0, (float)notebookfechado.width, (float)notebookfechado.height},
                    (Rectangle){120, 114, 90, 90},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    DrawText("PAUSA", 100, 45, 25, GREEN);
                    if (opcaoPausa == 0) DrawText("> CONTINUAR", 120, 95, 18, GREEN);
                    else DrawText("  CONTINUAR", 120, 95, 15, WHITE);
                    if (opcaoPausa == 1) DrawText("> MENU", 120, 120, 18, GREEN);
                    else DrawText("  MENU", 120, 120, 15, WHITE);
                    if (opcaoPausa == 2) DrawText("> REINICIAR", 120, 145, 18, GREEN);
                    else DrawText("  REINICIAR", 120, 145, 15, WHITE);
                    break;

                case GAMEOVER:
                    ClearBackground(WHITE);
                    DrawTexturePro(fundoGameplay, (Rectangle){0, 0, (float)fundoGameplay.width, (float)fundoGameplay.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    Color corGameOver = RED;
                    DrawText("GAMEOVER", 46, 36, 40, Fade(corGameOver, 0.25f));
                    DrawText("GAMEOVER", 48, 38, 40, Fade(corGameOver, 0.25f));
                    DrawText("GAMEOVER", 46,36, 40, Fade(corGameOver, 0.25f));
                    DrawText("GAMEOVER", 48,38, 40, Fade(corGameOver, 0.25f));
                    DrawText("GAMEOVER", 46, 36, 40, corGameOver);
                    DrawTexturePro(notebookNormal, (Rectangle){0, 0, (float)notebookNormal.width, (float)notebookNormal.height},
                    (Rectangle){80, 40, 190, 190},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    Color corTexto2 = Fade(DARKGREEN, 0.3f);
                        if (((int)(GetTime() / 0.3f)) % 2 == 0){
                            corTexto2 = DARKGREEN;
                            DrawText("GAME", 143, 108, 10, corTexto2);
                            DrawText("OVER", 143, 116, 10, corTexto2);
                        }
                        DrawTexturePro(hackerNormal[0], (Rectangle){0, 0, (float)hackerNormal[0].width, (float)hackerNormal[0].height},
                        (Rectangle){117, 18, 260, 260},
                        (Vector2){0, 0}, 0.0f, WHITE);
                    // GDD seção 6.2: "Exibe score, nome e salva no
                    // ranking" -- antes a tela só mostrava o texto
                    // "GAME OVER" piscando, sem nenhum dado da partida.
                    DrawText(TextFormat("%s", jogador.nome), 20, 200, 10, DARKGREEN);
                    DrawText(TextFormat("SCORE: %05d", jogador.score), 20, 215, 10, DARKGREEN);
                    DrawText("[ENTER] MENU", 230, 215, 8, DARKGREEN);
                    break;
                    
                case VICTORY:
                
                    ClearBackground(WHITE);
                    DrawTexturePro(fundoGameplay, (Rectangle){0, 0, (float)fundoGameplay.width, (float)fundoGameplay.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    Color corVitoria = ColorFromHSV(GetTime() * 80, 1.0f, 1.0f);
                    DrawText("VITORIA", 76, 36, 40, Fade(corVitoria, 0.25f));
                    DrawText("VITORIA", 78, 38, 40, Fade(corVitoria, 0.25f));
                    DrawText("VITORIA", 76,36, 40, Fade(corVitoria, 0.25f));
                    DrawText("VITORIA", 78,38, 40, Fade(corVitoria, 0.25f));
                    DrawText("VITORIA", 76, 36, 40, corVitoria);
                    DrawTexturePro(notebookNormal, (Rectangle){0, 0, (float)notebookNormal.width, (float)notebookNormal.height},
                    (Rectangle){80, 40, 190, 190},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    Color corTexto = Fade(DARKGREEN, 0.3f);
                        if (((int)(GetTime() / 0.3f)) % 2 == 0){
                            corTexto = DARKGREEN;
                            DrawText("LEVEL", 143, 108, 10, corTexto);
                            DrawText("CLEAR", 143, 116, 10, corTexto);
                        }
                    DrawTexturePro(hackerLevelclear, (Rectangle){0, 0, (float)hackerLevelclear.width, (float)hackerLevelclear.height},
                    (Rectangle){120, 30, 255, 255},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    
                    break;
                    
                case RANKING:
                    ClearBackground(BLACK);
                    DrawTexturePro(interfaceTerminal, (Rectangle){0, 0, (float)interfaceTerminal.width, (float)interfaceTerminal.height},
                    (Rectangle){0, 0, 320, 240},
                    (Vector2){0, 0}, 0.0f, WHITE);
                    
                    // Título
                    Color corRanking = ColorFromHSV(GetTime() * 80, 1.0f, 1.0f);
                    DrawText("RANKING", 54, 43, 40, Fade(corRanking, 0.25f));
                    DrawText("RANKING", 56, 43, 40, Fade(corRanking, 0.25f));
                    DrawText("RANKING", 54,45, 40, Fade(corRanking, 0.25f));
                    DrawText("RANKING", 56,45, 40, Fade(corRanking, 0.25f));
                    DrawText("RANKING", 54, 45, 40, corRanking);


                    // Divisórias
                    DrawLine(90, 130, 230, 130, corRanking);
                    DrawLine(90, 160, 230, 160, corRanking);
                    
                    // Top 3 real, lido de data/ranking.txt via ranking.c
                    // (GDD seção 10). Antes os 3 valores eram fixos no
                    // código ("00:35"/"00:42"/"00:50"), sem relação com
                    // quem realmente jogou. ranking.quantidade pode ser
                    // menor que 3 (ex: primeira execução do jogo), por
                    // isso desenhamos "---" nos slots ainda vazios em vez
                    // de ler memória fora do vetor preenchido.
                    for (int i = 0; i < RANKING_TOP_N; i++) {
                        int posY = 110 + i * 30;
                        DrawText(TextFormat("TOP %d:", i + 1), 100, posY, 10, corRanking);
                        if (i < ranking.quantidade) {
                            DrawText(ranking.entradas[i].nome, 165, posY, 10, WHITE);
                            DrawText(TextFormat("%d", ranking.entradas[i].score), 250, posY, 10, YELLOW);
                        } else {
                            DrawText("---", 165, posY, 10, GRAY);
                        }
                    }
                    DrawText("[ENTER] VOLTAR AO MENU", 95, 205, 8, WHITE);
                    break;
            }
        EndTextureMode();
        
        // ETAPA 3: Escalonamento(320x240 -> 960x720)
        BeginDrawing();
            ClearBackground(BLACK);
            
            DrawTexturePro(
                jogo.texture,
                (Rectangle){ 0, 0, 320, -240 }, 
                (Rectangle){ 0, 0, 960, 720 },  
                (Vector2){ 0, 0 }, 0.0f, WHITE
            );
        EndDrawing(); 
    }
    //JOGO
    UnloadRenderTexture(jogo);
    UnloadTexture(Moeda[0]);
    UnloadTexture(Moeda[1]);
    UnloadTexture(Moeda[2]);
    UnloadTexture(shop);
    UnloadTexture(vidaextra);
    UnloadTexture(kit);
    UnloadTexture(databackup);
    UnloadTexture(overlock);
    UnloadTexture(fundoGameplay);
    UnloadTexture(spritefixo1);
    UnloadTexture(spritefixo2);
    UnloadTexture(inputbar);
    UnloadTexture(notebookNormal);
    UnloadTexture(coracao);
    UnloadTexture(hackercombo2[0]);
    UnloadTexture(hackercombo2[1]);
    UnloadTexture(combo10hacker[0]);
    UnloadTexture(combo10hacker[1]);
    UnloadTexture(combo10hacker[2]);
    UnloadTexture(combofrenesi[0]);
    UnloadTexture(combofrenesi[1]);
    UnloadTexture(combofrenesi[2]);
    UnloadTexture(fundoMenu);
    UnloadTexture(interfaceVitoria);
    UnloadTexture(interfaceTerminal);
    UnloadTexture(hackerNormal[0]);
    UnloadTexture(hackerNormal[1]);
    UnloadTexture(hackerNormal[2]);
    UnloadTexture(hackerLevelclear);
    UnloadTexture(vitoriaHacker1);
    UnloadTexture(vitoriaHacker2);
    UnloadTexture(notebookfechado);
    UnloadTexture(hackersentado[0]);
    UnloadTexture(hackersentado[1]);
    UnloadTexture(hackersentado[2]);
    //UnloadFont
    UnloadFont(fontepalavra);
    //Unload Audio
    UnloadSound(erro);
    UnloadSound(letraCorreta);
    UnloadSound(letraErrada);
    UnloadSound(selectSom);
    UnloadSound(moeda);
    UnloadMusicStream(vitoria);
    UnloadMusicStream(gameover);
    UnloadMusicStream(musica);
    UnloadMusicStream(combo10);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}