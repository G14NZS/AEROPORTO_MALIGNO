#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aeroporto.h"

/* =========================================================
 * Criacao e liberacao
 * ========================================================= */

VetorAeroportos* criarVetor(void) {
    VetorAeroportos *v = (VetorAeroportos*) malloc(sizeof(VetorAeroportos));
    if (!v) return NULL;
    v->dados = NULL;
    v->qtd = 0;
    v->capacidade = 0;
    return v;
}

void liberarVetor(VetorAeroportos *v) {
    if (!v) return;
    free(v->dados);   /* free(NULL) e seguro, entao nao precisa testar */
    free(v);
}

/* =========================================================
 * Busca
 * ========================================================= */

int buscarIndiceAeroporto(VetorAeroportos *v, const char *codigo) {
    if (!v || !codigo) return -1;
    for (int i = 0; i < v->qtd; i++) {
        if (strcmp(v->dados[i].codigo, codigo) == 0)
            return i;
    }
    return -1;
}

/* =========================================================
 * Cadastro
 * ========================================================= */

Status cadastrarAeroporto(VetorAeroportos *v,
                          const char *codigo,
                          const char *cidade) {
    /* Validacao de entrada */
    if (!v || !codigo || !cidade) return ERRO_INVALIDO;
    if (strlen(codigo) == 0 || strlen(cidade) == 0) return ERRO_INVALIDO;

    /* Nao permitir duplicatas */
    if (buscarIndiceAeroporto(v, codigo) >= 0)
        return ERRO_DUPLICADO;

    /* Crescer o vetor se cheio (capacidade dobra a cada vez) */
    if (v->qtd == v->capacidade) {
        int novaCap = (v->capacidade == 0) ? 4 : v->capacidade * 2;
        Aeroporto *novo = (Aeroporto*) realloc(v->dados,
                                               novaCap * sizeof(Aeroporto));
        if (!novo) return ERRO_MEMORIA;
        v->dados = novo;
        v->capacidade = novaCap;
    }

    /* Copia segura do codigo */
    strncpy(v->dados[v->qtd].codigo, codigo,
            sizeof(v->dados[v->qtd].codigo) - 1);
    v->dados[v->qtd].codigo[sizeof(v->dados[v->qtd].codigo) - 1] = '\0';

    /* Copia segura da cidade */
    strncpy(v->dados[v->qtd].cidade, cidade,
            sizeof(v->dados[v->qtd].cidade) - 1);
    v->dados[v->qtd].cidade[sizeof(v->dados[v->qtd].cidade) - 1] = '\0';

    v->qtd++;
    return OK;
}
