#ifndef RANKING_H
#define RANKING_H

#include "jogador.h"

#define RANKING_TOP_N 3

typedef struct {
    char nome[MAX_NOME];
    int score;
} EntradaRanking;

typedef struct {
    EntradaRanking entradas[RANKING_TOP_N];
    int quantidade; // 0..RANKING_TOP_N
} Ranking;

Ranking ranking_carregar(const char *caminho);

void ranking_salvar(const Ranking *ranking, const char *caminho);

int ranking_score_qualifica(const Ranking *ranking, int score);

int ranking_inserir(Ranking *ranking, const char *nome, int score);

#endif // RANKING_H
