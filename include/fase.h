#ifndef FASE_H
#define FASE_H

#include "jogador.h"

#define TOTAL_FASES 8

typedef struct {
    int numero;
    int meta_palavras;
    int tempo_segundos;     // tempo TOTAL da fase (configuracao fixa)
    int linguagem_id;
} Fase;

typedef enum {
    FASE_EM_ANDAMENTO,
    FASE_VITORIA,    // meta de palavras atingida dentro do tempo
    FASE_GAME_OVER   // vidas == 0 OU tempo esgotado antes da meta
} ResultadoFase;


typedef struct {
    float tempo_restante;
} TimerFase;

extern const Fase TABELA_FASES[TOTAL_FASES];

const Fase *fase_obter_config(int numero_fase);

TimerFase fase_timer_iniciar(const Fase *fase);

void fase_timer_atualizar(TimerFase *timer, float delta_tempo);

void fase_timer_adicionar_tempo(TimerFase *timer, int segundos);

int fase_timer_esgotado(const TimerFase *timer);

ResultadoFase fase_avaliar_resultado(Jogador *jogador, const TimerFase *timer, const Fase *fase);

#endif // FASE_H
