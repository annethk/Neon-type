#ifndef ECONOMIA_H
#define ECONOMIA_H

#include "jogador.h"
#include "fase.h"

typedef enum {
    ITEM_REFRIGERACAO = 1,  // +5s no cronometro, custa 50 moedas
    ITEM_OVERCLOCK = 2,     // +10s no cronometro, custa 90 moedas
    ITEM_BACKUP_DADOS = 3,  // restaura vidas se zerar, 1x por fase, custa 200
    ITEM_VIDA_EXTRA = 4     // +1 vida (max 5), custa 80 moedas
} ItemLoja;

typedef enum {
    COMPRA_SUCESSO,
    COMPRA_FALHA_SEM_MOEDAS,
    COMPRA_FALHA_VIDAS_JA_NO_MAXIMO,      // tentou comprar Vida Extra com vidas == 5
    COMPRA_FALHA_BACKUP_JA_USADO,         // tentou comprar Backup 2x na mesma fase
    COMPRA_FALHA_ITEM_INVALIDO
} ResultadoCompra;


extern const int PRECOS_LOJA[5];

ResultadoCompra economia_comprar_item(Jogador *jogador, TimerFase *timer, ItemLoja item);

#endif // ECONOMIA_H
