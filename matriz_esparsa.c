#include <stdio.h>
#include <stdlib.h>
#include "matriz_esparsa.h"

/* Cria uma matriz esparsa vazia. */
MatrizEsparsa* criarMatriz(void) {
    MatrizEsparsa *m = (MatrizEsparsa*) malloc(sizeof(MatrizEsparsa));
    if (m) m->inicio = NULL;
    return m;
}

/* Insere (ou atualiza) o valor na posicao (linha, coluna).
   Retorna 1 em sucesso e 0 em falha. */
int inserirMatriz(MatrizEsparsa *m, int linha, int coluna, void *valor) {
    if (!m) return 0;

    /* Se ja existe celula nessa posicao, so atualiza o valor */
    No *p = m->inicio;
    while (p) {
        if (p->linha == linha && p->coluna == coluna) {
            p->valor = valor;
            return 1;
        }
        p = p->prox;
    }

    /* Senao, cria um no novo no inicio da lista */
    No *novo = (No*) malloc(sizeof(No));
    if (!novo) return 0;
    novo->linha  = linha;
    novo->coluna = coluna;
    novo->valor  = valor;
    novo->prox   = m->inicio;
    m->inicio    = novo;
    return 1;
}

/* Retorna o valor na posicao (linha, coluna) ou NULL se vazia. */
void* buscarMatriz(MatrizEsparsa *m, int linha, int coluna) {
    if (!m) return NULL;
    No *p = m->inicio;
    while (p) {
        if (p->linha == linha && p->coluna == coluna) return p->valor;
        p = p->prox;
    }
    return NULL;
}

/* Libera todos os nos da matriz e a propria estrutura.
   Obs.: nao libera o conteudo apontado por 'valor' (responsabilidade de quem usa). */
void liberarMatriz(MatrizEsparsa *m) {
    if (!m) return;
    No *p = m->inicio;
    while (p) {
        No *tmp = p;
        p = p->prox;
        free(tmp);
    }
    free(m);
}
