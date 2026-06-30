#ifndef PALAVRA_H
#define PALAVRA_H

#define MAX_TEXTO_PALAVRA 100
#define MAX_PALAVRAS_POR_LINGUAGEM 30
#define TOTAL_LINGUAGENS 8

// IDs de linguagem, na mesma ordem da tabela de progressao do GDD (secao 3).
typedef enum {
    LANG_C = 1,
    LANG_CPP = 2,
    LANG_ASM = 3,
    LANG_JAVA = 4,
    LANG_PYTHON = 5,
    LANG_JS = 6,
    LANG_KOTLIN_SWIFT = 7,
    LANG_SQL = 8
} LinguagemId;

typedef struct {
    char texto[MAX_TEXTO_PALAVRA];
    int linguagem_id;
} Palavra;

typedef struct {
    Palavra palavras[TOTAL_LINGUAGENS][MAX_PALAVRAS_POR_LINGUAGEM];
    int total_carregadas[TOTAL_LINGUAGENS]; // quantas palavras essa linguagem tem (deveria ser 30)
    int cursor[TOTAL_LINGUAGENS];           // posicao atual no vetor embaralhado
    char ultima_sorteada[TOTAL_LINGUAGENS][MAX_TEXTO_PALAVRA]; // guarda a ultima por seguranca
} BancoPalavras;

int palavras_carregar_arquivo(const char *caminho, BancoPalavras *banco);

void palavras_embaralhar_linguagem(BancoPalavras *banco, int linguagem_id);

const Palavra *palavras_sortear(BancoPalavras *banco, int linguagem_id);

int palavras_indice_da_linguagem(int linguagem_id);

#endif // PALAVRA_H
