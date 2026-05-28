#ifndef AEROPORTO_H
#define AEROPORTO_H

/* =========================================================
 * Codigos de retorno usados pelas operacoes do projeto.
 * Definidos aqui porque sao compartilhados entre modulos.
 * ========================================================= */
typedef enum {
    OK,
    ERRO_DUPLICADO,
    ERRO_NAO_ENCONTRADO,
    ERRO_MEMORIA,
    ERRO_INVALIDO
} Status;

/* =========================================================
 * Estrutura de um aeroporto.
 * codigo[4]: 3 letras + '\0' (ex: "CNF")
 * cidade[50]: nome da cidade
 * ========================================================= */
typedef struct {
    char codigo[4];
    char cidade[50];
} Aeroporto;

/* =========================================================
 * Vetor dinamico de aeroportos.
 * dados      : array alocado dinamicamente
 * qtd        : numero de aeroportos cadastrados
 * capacidade : tamanho atual alocado (qtd <= capacidade)
 * ========================================================= */
typedef struct {
    Aeroporto *dados;
    int qtd;
    int capacidade;
} VetorAeroportos;

/* =========================================================
 * Operacoes
 * ========================================================= */

/* Cria um vetor vazio. Retorna NULL em caso de falha de memoria. */
VetorAeroportos* criarVetor(void);

/* Libera o vetor e seus dados internos. */
void liberarVetor(VetorAeroportos *v);

/* Busca um aeroporto pelo codigo.
 * Retorna o indice no vetor, ou -1 se nao encontrar. */
int buscarIndiceAeroporto(VetorAeroportos *v, const char *codigo);

// Busca recursiva de rotas entre dois aeroportos usando backtracking.
void buscarRotas(VetorAeroportos *v, int origem, int destino, int *visitados, int *rota, int nivel, int totalAeroportos)

/* Cadastra um novo aeroporto.
 * Cresce o vetor automaticamente se necessario.
 * Retorna:
 *   OK              -> cadastrado com sucesso
 *   ERRO_DUPLICADO  -> ja existe aeroporto com esse codigo
 *   ERRO_INVALIDO   -> codigo ou cidade vazios/nulos
 *   ERRO_MEMORIA    -> falha ao alocar memoria */
Status cadastrarAeroporto(VetorAeroportos *v,
                          const char *codigo,
                          const char *cidade);

#endif
