#include <stdio.h>
#include <stdlib.h>
#include "voo.h"

typedef struct {
    VetorAeroportos *aeroportos;
} ContextoListagem;

static void imprimirVoo(int linha, int coluna, void *valor, void *contexto) {
    ContextoListagem *ctx = (ContextoListagem*) contexto;
    int numeroVoo = *((int*) valor);

    printf("Voo %d -> %s\n",
           numeroVoo,
           ctx->aeroportos->dados[coluna].cidade);
}

Status cadastrarVoo(VetorAeroportos *aeroportos,
                    MatrizEsparsa *malha,
                    const char *codigoOrigem,
                    const char *codigoDestino,
                    int numeroVoo) {
    if (aeroportos == NULL || malha == NULL ||
        codigoOrigem == NULL || codigoDestino == NULL ||
        numeroVoo <= 0) {
        return ERRO_INVALIDO;
    }

    int origem = buscarIndiceAeroporto(aeroportos, codigoOrigem);
    int destino = buscarIndiceAeroporto(aeroportos, codigoDestino);

    if (origem == -1 || destino == -1) {
        return ERRO_NAO_ENCONTRADO;
    }

    int *valor = malloc(sizeof(int));
    if (valor == NULL) {
        return ERRO_MEMORIA;
    }

    *valor = numeroVoo;

    if (!inserirMatriz(malha, origem, destino, valor)) {
        free(valor);
        return ERRO_MEMORIA;
    }

    return OK;
}

Status listarVoosSaindoDe(VetorAeroportos *aeroportos,
                          MatrizEsparsa *malha,
                          const char *codigoOrigem) {
    if (aeroportos == NULL || malha == NULL || codigoOrigem == NULL) {
        return ERRO_INVALIDO;
    }

    int origem = buscarIndiceAeroporto(aeroportos, codigoOrigem);

    if (origem == -1) {
        return ERRO_NAO_ENCONTRADO;
    }

    printf("Voos saindo de %s - %s:\n",
           aeroportos->dados[origem].codigo,
           aeroportos->dados[origem].cidade);

    ContextoListagem ctx;
    ctx.aeroportos = aeroportos;

    percorrerLinha(malha, origem, imprimirVoo, &ctx);

    return OK;
}
