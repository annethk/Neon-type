#include "economia.h"


const int PRECOS_LOJA[5] = {
    0,    // [0] nao usado
    50,   // [ITEM_REFRIGERACAO]
    90,   // [ITEM_OVERCLOCK]
    150,  // [ITEM_BACKUP_DADOS]
    80    // [ITEM_VIDA_EXTRA]
};

ResultadoCompra economia_comprar_item(Jogador *jogador, TimerFase *timer, ItemLoja item) {

    switch (item) {

        case ITEM_REFRIGERACAO:
            if (jogador->moedas < PRECOS_LOJA[ITEM_REFRIGERACAO]) {
                return COMPRA_FALHA_SEM_MOEDAS;
            }
            jogador->moedas -= PRECOS_LOJA[ITEM_REFRIGERACAO];
            jogador->tempo_extra_acumulado += 5; // Salva para usar depois
            return COMPRA_SUCESSO;

        case ITEM_OVERCLOCK:
            if (jogador->moedas < PRECOS_LOJA[ITEM_OVERCLOCK]) {
                return COMPRA_FALHA_SEM_MOEDAS;
            }
            jogador->moedas -= PRECOS_LOJA[ITEM_OVERCLOCK];
            jogador->tempo_extra_acumulado += 10; // Salva para usar depois
            return COMPRA_SUCESSO;

        case ITEM_BACKUP_DADOS:
        // Se já estiver como 1, significa que ele já comprou o backup
            if (jogador->backup_usado == 1) { 
             return COMPRA_FALHA_BACKUP_JA_USADO; 
            }

            if (jogador->moedas < PRECOS_LOJA[ITEM_BACKUP_DADOS]) {
                return COMPRA_FALHA_SEM_MOEDAS;
            }

            jogador->moedas -= PRECOS_LOJA[ITEM_BACKUP_DADOS];
            jogador->backup_usado = 1; // Agora ele tem o "seguro" ativo!

            return COMPRA_SUCESSO;

        case ITEM_VIDA_EXTRA:
            
            if (jogador->vidas >= VIDAS_MAX) {
            return COMPRA_FALHA_VIDAS_JA_NO_MAXIMO;
            }

            if (jogador->moedas < PRECOS_LOJA[ITEM_VIDA_EXTRA]) {
            return COMPRA_FALHA_SEM_MOEDAS;
            }
         jogador->moedas -= PRECOS_LOJA[ITEM_VIDA_EXTRA];
            jogador_ganhar_vida(jogador);

            return COMPRA_SUCESSO;

        default:
            return COMPRA_FALHA_ITEM_INVALIDO;
    }
}
