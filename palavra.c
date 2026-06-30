#include "palavra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct {
    const char *tag;
    int linguagem_id;
} MAPA_SECOES[TOTAL_LINGUAGENS] = {
    {"[C]",      LANG_C},
    {"[CPP]",    LANG_CPP},
    {"[ASM]",    LANG_ASM},
    {"[JAVA]",   LANG_JAVA},
    {"[PYTHON]", LANG_PYTHON},
    {"[JS]",     LANG_JS},
    {"[KOTLIN]", LANG_KOTLIN_SWIFT},
    {"[SQL]",    LANG_SQL}
};

int palavras_indice_da_linguagem(int linguagem_id) {
    // linguagem_id vai de 1 a 8 (enum LinguagemId); indice interno e 0..7.
    return linguagem_id - 1;
}

int palavras_carregar_arquivo(const char *caminho, BancoPalavras *banco) {
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        return 0;
    }

    memset(banco, 0, sizeof(BancoPalavras));

    char linha[MAX_TEXTO_PALAVRA];
    int linguagem_atual = 0; // 0 = nenhuma secao ainda identificada

    while (fgets(linha, sizeof(linha), f) != NULL) {
        size_t len = strlen(linha);
        while (len > 0 && (linha[len - 1] == '\n' || linha[len - 1] == '\r')) {
            linha[len - 1] = '\0';
            len--;
        }

        if (len == 0) {
            continue; // ignora linhas vazias
        }

        // Verifica se a linha e um cabecalho de secao, ex: "[PYTHON]"
        int eh_secao = 0;
        for (int i = 0; i < TOTAL_LINGUAGENS; i++) {
            if (strcmp(linha, MAPA_SECOES[i].tag) == 0) {
                linguagem_atual = MAPA_SECOES[i].linguagem_id;
                eh_secao = 1;
                break;
            }
        }
        if (eh_secao) {
            continue;
        }

        if (linguagem_atual == 0) {
            continue;
        }

        int idx = palavras_indice_da_linguagem(linguagem_atual);
        int *total = &banco->total_carregadas[idx];

        if (*total < MAX_PALAVRAS_POR_LINGUAGEM) {
            strncpy(banco.palavras[idx][*total].texto, linha, MAX_TEXTO_PALAVRA - 1);
            banco.palavras[idx][*total].texto[MAX_TEXTO_PALAVRA - 1] = '\0';
            banco.palavras[idx][*total].linguagem_id = linguagem_atual;
            (*total)++;
        }
    }

    fclose(f);
    return 1;
}

void palavras_embaralhar_linguagem(BancoPalavras *banco, int linguagem_id) {
    int idx = palavras_indice_da_linguagem(linguagem_id);
    int n = banco->total_carregadas[idx];

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Palavra tmp = banco->palavras[idx][i];
        banco->palavras[idx][i] = banco->palavras[idx][j];
        banco->palavras[idx][j] = tmp;
    }

    banco->cursor[idx] = 0;
    banco->ultima_sorteada[idx][0] = '\0';
}

const Palavra *palavras_sortear(BancoPalavras *banco, int linguagem_id) {
    int idx = palavras_indice_da_linguagem(linguagem_id);
    int n = banco->total_carregadas[idx];

    if (n == 0) {
        return NULL; // dicionario vazio/nao carregado: chamador deve tratar
    }

    if (banco->cursor[idx] >= n) {
        palavras_embaralhar_linguagem(banco, linguagem_id);

        if (n > 1 && strcmp(banco->palavras[idx][0].texto, banco->ultima_sorteada[idx]) == 0) {
            Palavra tmp = banco->palavras[idx][0];
            banco->palavras[idx][0] = banco->palavras[idx][1];
            banco->palavras[idx][1] = tmp;
        }
    }

    const Palavra *escolhida = &banco->palavras[idx][banco->cursor[idx]];
    banco->cursor[idx]++;

    strncpy(banco->ultima_sorteada[idx], escolhida->texto, MAX_TEXTO_PALAVRA - 1);
    banco->ultima_sorteada[idx][MAX_TEXTO_PALAVRA - 1] = '\0';

    return escolhida;
}
