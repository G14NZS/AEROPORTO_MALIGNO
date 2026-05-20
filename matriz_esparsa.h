#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

/* Matriz esparsa genérica (lista encadeada de células não-vazias).
   O valor é void* para aceitar qualquer tipo de dado. */

typedef struct No {
    int linha;
    int coluna;
    void *valor;
    struct No *prox;
} No;

typedef struct {
    No *inicio;
} MatrizEsparsa;

MatrizEsparsa* criarMatriz(void);
int            inserirMatriz(MatrizEsparsa *m, int linha, int coluna, void *valor);
void*          buscarMatriz (MatrizEsparsa *m, int linha, int coluna);
void           liberarMatriz(MatrizEsparsa *m);

#endif
