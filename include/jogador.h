#ifndef JOGADOR_H
#define JOGADOR_H

#define MAX_NOME 50
#define VIDAS_MAX 5
#define COMBO_FRENESI 20

typedef struct {
    char nome[MAX_NOME];
    int score;
    int score_checkpoint;
    int combo;
    int level;
    int vidas;          // 0..VIDAS_MAX
    int moedas;
    int palavras_fase;  // quantas palavras corretas na fase atual
    int backup_usado;   // 0 = backup de vida disponivel na fase, 1 = ja usado
    int tempo_extra_acumulado; // tempo extra acumulado para usar na fase seguinte
} Jogador;

Jogador jogador_criar(const char *nome);

void jogador_resetar_fase(Jogador *j);

void jogador_resetar_total(Jogador *j);

void jogador_salvar_checkpoint(Jogador *j);

void jogador_reiniciar_fase(Jogador *j);

int jogador_perder_vida(Jogador *j);

void jogador_ganhar_vida(Jogador *j);

void jogador_restaurar_vidas_full(Jogador *j);

void jogador_incrementar_combo(Jogador *j);

void jogador_zerar_combo(Jogador *j);

int jogador_multiplicador_combo(const Jogador *j);

void jogador_completar_palavra(Jogador *j);

#endif // JOGADOR_H
