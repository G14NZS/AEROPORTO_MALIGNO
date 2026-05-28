#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <windows.h>
#include <unistd.h>
#include "aeroporto.h"
#include "matriz_esparsa.h"

//Belo Horizonte (CNF), Brasília (BSB), Rio de Janeiro (GIG), Salvador (SSA) e São Paulo (GRU)

/* =========================================================
 * Cadastro inicial dos aeroportos do PDF
 * Ordem: BSB=0, CNF=1, GIG=2, GRU=3, SSA=4
 * ========================================================= */
void cadastrarAeroportosIniciais(VetorAeroportos *v) {
    cadastrarAeroporto(v, "BSB", "Brasilia");
    cadastrarAeroporto(v, "CNF", "Belo Horizonte");
    cadastrarAeroporto(v, "GIG", "Rio de Janeiro");
    cadastrarAeroporto(v, "GRU", "Sao Paulo");
    cadastrarAeroporto(v, "SSA", "Salvador");
    printf(">> 5 aeroportos iniciais cadastrados.\n\n");
}

/* =========================================================
 * Topico 1 - Cadastrar novo aeroporto
 * ========================================================= */
void opcaoCadastrarAeroporto(VetorAeroportos *v) {
    char codigo[10], cidade[60];

    printf("\nCodigo do aeroporto (ex: VCP): ");
    scanf("%9s", codigo);
    printf("Nome da cidade: ");
    scanf(" %59[^\n]", cidade);

    switch (cadastrarAeroporto(v, codigo, cidade)) {
        case OK:
            printf(">> Aeroporto %s (%s) cadastrado com sucesso!\n",
                   codigo, cidade);
            break;
        case ERRO_DUPLICADO:
            printf(">> Falha: ja existe um aeroporto com codigo %s.\n",
                   codigo);
            break;
        case ERRO_MEMORIA:
            printf(">> Falha: memoria insuficiente.\n");
            break;
        default:
            printf(">> Falha: dados invalidos.\n");
            break;
    }
}

/* =========================================================
 * Topico 4 - Listar voos saindo de um aeroporto
 * ========================================================= */
typedef struct {
    VetorAeroportos *v;
    int contador;
} CtxListagem;

void imprimirVoo(int linha, int coluna, void *valor, void *ctx) {
    (void)linha;
    CtxListagem *c = (CtxListagem*) ctx;
    int *numeroVoo = (int*) valor;
    printf("  Voo %3d  ->  %s (%s)\n",
           *numeroVoo,
           c->v->dados[coluna].cidade,
           c->v->dados[coluna].codigo);
    c->contador++;
}

void opcaoListarVoos(MatrizEsparsa *m, VetorAeroportos *v) {
    char codigo[10];

    printf("\nCodigo do aeroporto de origem: ");
    scanf("%9s", codigo);

    int idx = buscarIndiceAeroporto(v, codigo);
    if (idx < 0) {
        printf(">> Falha: aeroporto %s nao esta cadastrado.\n", codigo);
        return;
    }

    printf("\nVoos saindo de %s (%s):\n",
           v->dados[idx].cidade, v->dados[idx].codigo);

    CtxListagem ctx = { v, 0 };
    percorrerLinha(m, idx, imprimirVoo, &ctx);

    if (ctx.contador == 0)
        printf("  (nenhum voo cadastrado partindo deste aeroporto)\n");
    else
        printf("Total: %d voo(s).\n", ctx.contador);
}

/* =========================================================
 * Topico 5 - Listar possíveis trajetos com ou sem baldeação
 * ========================================================= */
void listarTrajetos(MatrizEsparsa *m, VetorAeroportos *v) {
    char origem[10], destino[10];

    //Solicita aeroportos ao usuario e valida se ambos existem e sao diferentes
    printf("\nCodigo do aeroporto de origem: ");
    scanf("%9s", origem);
    printf("Codigo do aeroporto de destino: ");
    scanf("%9s", destino);

    int indexOrigem = buscarIndiceAeroporto(v, origem);
    int indexDestino = buscarIndiceAeroporto(v, destino);

    if (indexOrigem < 0 || indexDestino < 0) {
        printf(">> Um ou ambos os aeroportos nao estao cadastrados.\n");
        return;
    }
    if (indexDestino == indexOrigem) {
        printf(">> O aeroporto de origem e o de destino sao o mesmo.\n");
        return;
    }

    printf("\nTrajetos possiveis de %s (%s) para %s (%s):\n",
           v->dados[indexOrigem].cidade, v->dados[indexOrigem].codigo,
           v->dados[indexDestino].cidade, v->dados[indexDestino].codigo);
    
    int *visitados = calloc(v->qtd, sizeof(int)); // vai iniciar sempre limpo
    int *caminho = malloc(v->qtd * sizeof(int));

    buscarRotas(v, indexOrigem, indexDestino, visitados, caminho, 0, v->qtd);

    free(visitados);
    free(caminho);
}
    
int main(){
    int resposta;
    printf("Escolha uma das opções:\n\n0- Sair\n1- Cadastrar um novo aeroporto\n2- Cadastrar voo (com números de aeroportos)\n3- Remoção de voo (por número)\n4- Listar todos os voos (número e nome da cidade destino) de um aeroporto \n5- Listar possíveis trajetos com ou sem baldeação\n\nResposta: ");
    while (resposta<0 || resposta>5){
        scanf("%i", &resposta);
        if (resposta<0 || resposta>5){
            printf("\nNúmero incompatível, tente novamente com os números possíveis");
            fflush(stdout);

            sleep(2.5);

            printf("Escolha uma das opções:\n\n0- Sair\n1- Cadastrar um novo aeroporto\n2- Cadastrar voo (com números de aeroportos)\n3- Remoção de voo (por número)\n4- Listar todos os voos (número e nome da cidade destino) de um aeroporto \n5- Listar possíveis trajetos com ou sem baldeação\n\nResposta: ");
        };
    };
    printf("%i",resposta);


    return 0;
}