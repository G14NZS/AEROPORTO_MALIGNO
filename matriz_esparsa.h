#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

/* ---------- Estruturas ---------- */

typedef struct NoCol {
    int coluna;
    void *valor;
    struct NoCol *esq, *dir;
} NoCol;

typedef struct NoLinha {
    int linha;
    NoCol *colunas;              /* raiz da arvore de colunas dessa linha */
    struct NoLinha *esq, *dir;
} NoLinha;

typedef struct {
    NoLinha *raiz;               /* raiz da arvore de linhas */
} MatrizEsparsa;

/* ---------- Operacoes basicas ---------- */

MatrizEsparsa* criarMatriz(void);


int inserirMatriz(MatrizEsparsa *m, int linha, int coluna, void *valor);

void* buscarMatriz(MatrizEsparsa *m, int linha, int coluna);


int removerMatriz(MatrizEsparsa *m, int linha, int coluna);

/* Libera toda a estrutura (mas nao os 'valor' apontados). */
void liberarMatriz(MatrizEsparsa *m);

/* ---------- Iteracao ----------*/
typedef void (*CallbackMatriz)(int linha, int coluna, void *valor, void *contexto);

void percorrerMatriz(MatrizEsparsa *m, CallbackMatriz cb, void *contexto);

void percorrerLinha(MatrizEsparsa *m, int linha, CallbackMatriz cb, void *contexto);

#endif
