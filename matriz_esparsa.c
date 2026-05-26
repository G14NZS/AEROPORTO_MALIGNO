#include <stdio.h>
#include <stdlib.h>
#include "matriz_esparsa.h"

/* =========================================================
 * Criacao
 * ========================================================= */

MatrizEsparsa* criarMatriz(void) {
    MatrizEsparsa *m = (MatrizEsparsa*) malloc(sizeof(MatrizEsparsa));
    if (m) m->raiz = NULL;
    return m;
}

/* =========================================================
 * Insercao
 * ========================================================= */

/* Insere/atualiza um no na arvore de colunas. Retorna a (possivelmente nova) raiz.
 * 'sucesso' vira 1 se a operacao foi bem-sucedida. */
static NoCol* inserirCol(NoCol *raiz, int c, void *valor, int *sucesso) {
    if (!raiz) {
        NoCol *novo = (NoCol*) malloc(sizeof(NoCol));
        if (!novo) { *sucesso = 0; return NULL; }
        novo->coluna = c;
        novo->valor  = valor;
        novo->esq = novo->dir = NULL;
        *sucesso = 1;
        return novo;
    }
    if (c < raiz->coluna)
        raiz->esq = inserirCol(raiz->esq, c, valor, sucesso);
    else if (c > raiz->coluna)
        raiz->dir = inserirCol(raiz->dir, c, valor, sucesso);
    else {
        raiz->valor = valor;   /* ja existe, so atualiza */
        *sucesso = 1;
    }
    return raiz;
}

/* Insere/acha um no-linha na arvore externa e delega pra inserirCol. */
static NoLinha* inserirLinha(NoLinha *raiz, int l, int c, void *valor, int *sucesso) {
    if (!raiz) {
        NoLinha *novo = (NoLinha*) malloc(sizeof(NoLinha));
        if (!novo) { *sucesso = 0; return NULL; }
        novo->linha = l;
        novo->colunas = NULL;
        novo->esq = novo->dir = NULL;
        novo->colunas = inserirCol(NULL, c, valor, sucesso);
        if (!*sucesso) { free(novo); return NULL; }
        return novo;
    }
    if (l < raiz->linha)
        raiz->esq = inserirLinha(raiz->esq, l, c, valor, sucesso);
    else if (l > raiz->linha)
        raiz->dir = inserirLinha(raiz->dir, l, c, valor, sucesso);
    else
        raiz->colunas = inserirCol(raiz->colunas, c, valor, sucesso);
    return raiz;
}

int inserirMatriz(MatrizEsparsa *m, int linha, int coluna, void *valor) {
    if (!m) return 0;
    int sucesso = 0;
    m->raiz = inserirLinha(m->raiz, linha, coluna, valor, &sucesso);
    return sucesso;
}

/* =========================================================
 * Busca
 * ========================================================= */

static NoLinha* buscarNoLinha(NoLinha *r, int l) {
    while (r) {
        if (l < r->linha)      r = r->esq;
        else if (l > r->linha) r = r->dir;
        else                   return r;
    }
    return NULL;
}

static NoCol* buscarNoCol(NoCol *r, int c) {
    while (r) {
        if (c < r->coluna)      r = r->esq;
        else if (c > r->coluna) r = r->dir;
        else                    return r;
    }
    return NULL;
}

void* buscarMatriz(MatrizEsparsa *m, int linha, int coluna) {
    if (!m) return NULL;
    NoLinha *nl = buscarNoLinha(m->raiz, linha);
    if (!nl) return NULL;
    NoCol *nc = buscarNoCol(nl->colunas, coluna);
    return nc ? nc->valor : NULL;
}

/* =========================================================
 * Remocao
 * ========================================================= */

/* Acha o no com menor 'coluna' na subarvore (mais a esquerda). */
static NoCol* minimoCol(NoCol *r) {
    while (r && r->esq) r = r->esq;
    return r;
}
static NoLinha* minimoLinha(NoLinha *r) {
    while (r && r->esq) r = r->esq;
    return r;
}

/* Remove no com coluna 'c'. Retorna nova raiz; 'removeu' fica 1 se removeu. */
static NoCol* removerCol(NoCol *raiz, int c, int *removeu) {
    if (!raiz) { *removeu = 0; return NULL; }

    if (c < raiz->coluna) {
        raiz->esq = removerCol(raiz->esq, c, removeu);
    } else if (c > raiz->coluna) {
        raiz->dir = removerCol(raiz->dir, c, removeu);
    } else {
        /* Achou o no a remover. */
        *removeu = 1;
        if (!raiz->esq) {                /* 0 ou 1 filho (direita) */
            NoCol *aux = raiz->dir;
            free(raiz);
            return aux;
        } else if (!raiz->dir) {         /* 1 filho (esquerda) */
            NoCol *aux = raiz->esq;
            free(raiz);
            return aux;
        } else {                         /* 2 filhos: usa sucessor em-ordem */
            NoCol *suc = minimoCol(raiz->dir);
            raiz->coluna = suc->coluna;
            raiz->valor  = suc->valor;
            int aux;
            raiz->dir = removerCol(raiz->dir, suc->coluna, &aux);
        }
    }
    return raiz;
}

/* Remove o no-linha 'l' (assumindo que sua arvore de colunas ja esta vazia). */
static NoLinha* removerLinhaVazia(NoLinha *raiz, int l) {
    if (!raiz) return NULL;
    if (l < raiz->linha) {
        raiz->esq = removerLinhaVazia(raiz->esq, l);
    } else if (l > raiz->linha) {
        raiz->dir = removerLinhaVazia(raiz->dir, l);
    } else {
        if (!raiz->esq) {
            NoLinha *aux = raiz->dir;
            free(raiz);
            return aux;
        } else if (!raiz->dir) {
            NoLinha *aux = raiz->esq;
            free(raiz);
            return aux;
        } else {
            NoLinha *suc = minimoLinha(raiz->dir);
            /* Copia campos do sucessor (inclusive a arvore de colunas dele) */
            raiz->linha   = suc->linha;
            raiz->colunas = suc->colunas;
            suc->colunas  = NULL;    /* evita liberacao dupla */
            raiz->dir = removerLinhaVazia(raiz->dir, raiz->linha);
        }
    }
    return raiz;
}

int removerMatriz(MatrizEsparsa *m, int linha, int coluna) {
    if (!m) return 0;
    NoLinha *nl = buscarNoLinha(m->raiz, linha);
    if (!nl) return 0;

    int removeu = 0;
    nl->colunas = removerCol(nl->colunas, coluna, &removeu);
    if (!removeu) return 0;

    /* Se a linha ficou vazia, remove o no-linha tambem */
    if (!nl->colunas)
        m->raiz = removerLinhaVazia(m->raiz, linha);

    return 1;
}

/* =========================================================
 * Liberacao
 * ========================================================= */

static void liberarArvoreCol(NoCol *r) {
    if (!r) return;
    liberarArvoreCol(r->esq);
    liberarArvoreCol(r->dir);
    free(r);
}

static void liberarArvoreLinha(NoLinha *r) {
    if (!r) return;
    liberarArvoreLinha(r->esq);
    liberarArvoreLinha(r->dir);
    liberarArvoreCol(r->colunas);
    free(r);
}

void liberarMatriz(MatrizEsparsa *m) {
    if (!m) return;
    liberarArvoreLinha(m->raiz);
    free(m);
}

/* =========================================================
 * Iteracao (percurso em-ordem)
 * ========================================================= */

static void percorrerCols(NoCol *r, int linha, CallbackMatriz cb, void *ctx) {
    if (!r) return;
    percorrerCols(r->esq, linha, cb, ctx);
    cb(linha, r->coluna, r->valor, ctx);
    percorrerCols(r->dir, linha, cb, ctx);
}

static void percorrerLinhas(NoLinha *r, CallbackMatriz cb, void *ctx) {
    if (!r) return;
    percorrerLinhas(r->esq, cb, ctx);
    percorrerCols(r->colunas, r->linha, cb, ctx);
    percorrerLinhas(r->dir, cb, ctx);
}

void percorrerMatriz(MatrizEsparsa *m, CallbackMatriz cb, void *ctx) {
    if (!m || !cb) return;
    percorrerLinhas(m->raiz, cb, ctx);
}

void percorrerLinha(MatrizEsparsa *m, int linha, CallbackMatriz cb, void *ctx) {
    if (!m || !cb) return;
    NoLinha *nl = buscarNoLinha(m->raiz, linha);
    if (!nl) return;
    percorrerCols(nl->colunas, linha, cb, ctx);
}
