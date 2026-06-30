#include "ranking.h"
#include <stdio.h>
#include <string.h>

Ranking ranking_carregar(const char *caminho) {
    Ranking r;
    r.quantidade = 0;

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        return r;
    }

    char nome_lido[MAX_NOME];
    int score_lido;
    while (r.quantidade < RANKING_TOP_N &&
           fscanf(f, "%49s %d", nome_lido, &score_lido) == 2) {
        strncpy(r.entradas[r.quantidade].nome, nome_lido, MAX_NOME - 1);
        r.entradas[r.quantidade].nome[MAX_NOME - 1] = '\0';
        r.entradas[r.quantidade].score = score_lido;
        r.quantidade++;
    }

    fclose(f);
    return r;
}

void ranking_salvar(const Ranking *ranking, const char *caminho) {
    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        return; // Programador B pode querer logar essa falha; aqui so
                 // protegemos contra crash se o disco estiver protegido/cheio.
    }

    for (int i = 0; i < ranking->quantidade; i++) {
        fprintf(f, "%s %d\n", ranking->entradas[i].nome, ranking->entradas[i].score);
    }

    fclose(f);
}

int ranking_score_qualifica(const Ranking *ranking, int score) {
    if (ranking->quantidade < RANKING_TOP_N) {
        return 1; // ainda ha vaga livre no Top 3
    }
    return score > ranking->entradas[RANKING_TOP_N - 1].score;
}

int ranking_inserir(Ranking *ranking, const char *nome, int score) {
    if (!ranking_score_qualifica(ranking, score)) {
        return 0;
    }

    if (ranking->quantidade < RANKING_TOP_N) {
        ranking->quantidade++;
    }

    int i = ranking->quantidade - 1;
    while (i > 0 && ranking->entradas[i - 1].score < score) {
        ranking->entradas[i] = ranking->entradas[i - 1];
        i--;
    }

    strncpy(ranking->entradas[i].nome, nome, MAX_NOME - 1);
    ranking->entradas[i].nome[MAX_NOME - 1] = '\0';
    ranking->entradas[i].score = score;

    return 1;
}
