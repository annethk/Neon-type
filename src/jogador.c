#include "jogador.h"
#include <string.h>

Jogador jogador_criar(const char *nome) {
    Jogador j;
    strncpy(j.nome, nome, MAX_NOME - 1);
    j.nome[MAX_NOME - 1] = '\0';

    j.score = 0;
    j.score_checkpoint = 0;
    j.combo = 0;
    j.level = 1;
    j.vidas = VIDAS_MAX;
    j.moedas = 0;
    j.palavras_fase = 0;
    j.backup_usado = 0;
    j.tempo_extra_acumulado = 0;
    return j;
}

void jogador_resetar_fase(Jogador *j) {
    j->palavras_fase = 0;
    j->backup_usado = 0;
}

void jogador_resetar_total(Jogador *j) {
    j->score = 0;
    j->combo = 0;
    j->level = 1;     
    j->vidas = VIDAS_MAX;
    j->moedas = 0;
    j->palavras_fase = 0;
    j->backup_usado = 0;
    j->tempo_extra_acumulado = 0;
}

int jogador_perder_vida(Jogador *j) {
    if (j->vidas > 0) {
        j->vidas--;
    }
    return (j->vidas <= 0);
}

void jogador_ganhar_vida(Jogador *j) {
    if (j->vidas < VIDAS_MAX) {
        j->vidas++;
    }
}

void jogador_restaurar_vidas_full(Jogador *j) {
    j->vidas = VIDAS_MAX;
}

void jogador_incrementar_combo(Jogador *j) {
    j->combo++;
    if (j->combo == COMBO_FRENESI) {
        jogador_ganhar_vida(j);
    }
}

void jogador_zerar_combo(Jogador *j) {
    j->combo = 0;
}

int jogador_multiplicador_combo(const Jogador *j) {
    if (j->combo >= 20) return 4;
    if (j->combo >= 10) return 3;
    if (j->combo >= 2)  return 2;
    return 1;
}

void jogador_completar_palavra(Jogador *j) {
    int mult = jogador_multiplicador_combo(j);
    j->moedas += 1 * mult;   // formula exata do GDD secao 12
    j->score  += 10 * mult; // score escalado pelo mesmo multiplicador
    j->palavras_fase++;
}

// Salva o score atual como o ponto de partida para a fase atual
void jogador_salvar_checkpoint(Jogador *j) {
    j->score_checkpoint = j->score;
}

// Volta o score para o valor que ele tinha no início da fase
void jogador_reiniciar_fase(Jogador *j) {
    j->score = j->score_checkpoint;
}

// Reseta tudo (usado no Novo Jogo)
void jogador_resetar_total(Jogador *j) {
    j->score = 0;
    j->score_checkpoint = 0;
    j->moedas = 0;
    j->level = 1;
    // ... reset de outras variáveis desejar
}
