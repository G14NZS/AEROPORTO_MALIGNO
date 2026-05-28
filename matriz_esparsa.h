#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

/* ---------- Estruturas ----------
 * Arvore externa: indexada por LINHA.
 * Cada no-linha guarda a raiz de uma arvore interna de COLUNAS.
 * Cada no-coluna guarda o valor (void* para ser generico).
 */

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

/* Insere ou atualiza o valor em (linha, coluna).
 * Retorna 1 em sucesso, 0 em falha. */
int inserirMatriz(MatrizEsparsa *m, int linha, int coluna, void *valor);

/* Retorna o valor em (linha, coluna), ou NULL se vazia. */
void* buscarMatriz(MatrizEsparsa *m, int linha, int coluna);

/* Remove o elemento em (linha, coluna).
 * Retorna 1 se removeu, 0 se nao existia.
 * Obs.: nao libera o conteudo apontado por 'valor'. */
int removerMatriz(MatrizEsparsa *m, int linha, int coluna);

/* Libera toda a estrutura (mas nao os 'valor' apontados). */
void liberarMatriz(MatrizEsparsa *m);

/* ---------- Iteracao ----------
 * Percorre todos os elementos nao-nulos em ordem (linha, coluna crescentes),
 * chamando 'callback' para cada um. 'contexto' e passado adiante sem modificacao
 * (util pra passar vetores, contadores, arquivos, etc).
 */

 typedef void (*CallbackMatriz)(int linha, int coluna, void *valor, void *contexto);
 

void percorrerMatriz(MatrizEsparsa *m, CallbackMatriz cb, void *contexto);

/* Percorre apenas os elementos de UMA linha especifica. */
void percorrerLinha(MatrizEsparsa *m, int linha, CallbackMatriz cb, void *contexto);

#endif
