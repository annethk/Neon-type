#include "fase.h"
#include "palavra.h"
#include <stddef.h>

// Tabela fixa extraida diretamente da secao 3 do GDD. Mantida como
// 'const' porque essa configuracao NUNCA muda durante a execucao --
// e dado de design, nao estado de jogo.
const Fase TABELA_FASES[TOTAL_FASES] = {
    {1, 10, 30, LANG_C},
    {2, 15, 30, LANG_CPP},
    {3, 15, 28, LANG_ASM},
    {4, 20, 28, LANG_JAVA},
    {5, 20, 25, LANG_PYTHON},
    {6, 25, 25, LANG_JS},
    {7, 25, 22, LANG_KOTLIN_SWIFT},
    {8, 30, 20, LANG_SQL}
};

const Fase *fase_obter_config(int numero_fase) {
    if (numero_fase < 1 || numero_fase > TOTAL_FASES) {
        return NULL;
    }
    // numero_fase e 1-based (Level 1..8); o vetor e 0-based.
    return &TABELA_FASES[numero_fase - 1];
}

TimerFase fase_timer_iniciar(const Fase *fase) {
    TimerFase t;
    t.tempo_restante = (float)fase->tempo_segundos;
    return t;
}

void fase_timer_atualizar(TimerFase *timer, float delta_tempo) {
    timer->tempo_restante -= delta_tempo;
    if (timer->tempo_restante < 0.0f) {
        timer->tempo_restante = 0.0f;
    }
}

void fase_timer_adicionar_tempo(TimerFase *timer, int segundos) {
    timer->tempo_restante += (float)segundos;
}

int fase_timer_esgotado(const TimerFase *timer) {
    return timer->tempo_restante <= 0.0f;
}

ResultadoFase fase_avaliar_resultado(const Jogador *jogador, const TimerFase *timer, const Fase *fase) {
    if (jogador->palavras_fase >= fase->meta_palavras) {
        return FASE_VITORIA;
    }
    if (jogador->vidas <= 0) {
        return FASE_GAME_OVER;
    }
    if (fase_timer_esgotado(timer)) {
        return FASE_GAME_OVER;
    }
    return FASE_EM_ANDAMENTO;
}
