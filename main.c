#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "aeroporto.h"
#include "matriz_esparsa.h"
#include "voo.h"

/* =========================================================
 * Cadastro inicial dos aeroportos
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
 * Menu
 * ========================================================= */
void mostrarMenu(void) {
    printf("\n====================================\n");
    printf("Sistema de Malha Aerea - ANAC\n");
    printf("====================================\n\n");

    printf("0 - Sair\n");
    printf("1 - Cadastrar novo aeroporto\n");
    printf("2 - Cadastrar voo\n");
    printf("3 - Remover voo\n");
    printf("4 - Listar voos de um aeroporto\n");
    printf("5 - Listar possiveis trajetos com ou sem baldeacao\n\n");

    printf("Resposta: ");
}

/* =========================================================
 * Topico 1 - Cadastrar novo aeroporto
 * ========================================================= */
void opcaoCadastrarAeroporto(VetorAeroportos *v) {
    char codigo[10];
    char cidade[60];

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
 * Topico 2 - Cadastrar voo
 * ========================================================= */
void opcaoCadastrarVoo(VetorAeroportos *v, MatrizEsparsa *m) {
    char origem[10];
    char destino[10];
    int numeroVoo;

    printf("\nCodigo do aeroporto de origem: ");
    scanf("%9s", origem);

    printf("Codigo do aeroporto de destino: ");
    scanf("%9s", destino);

    printf("Numero do voo: ");
    scanf("%d", &numeroVoo);

    switch (cadastrarVoo(v, m, origem, destino, numeroVoo)) {
        case OK:
            printf(">> Voo %d cadastrado com sucesso!\n", numeroVoo);
            break;

        case ERRO_NAO_ENCONTRADO:
            printf(">> Falha: aeroporto de origem ou destino nao cadastrado.\n");
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
 * Topico 3 - Remover voos pelo número;
 * ========================================================= */

void opcaoRemoverVoo(VetorAeroportos *v, MatrizEsparsa *m) {
    char origem[10];
    char destino[10];

    printf("\nCodigo do aeroporto de origem: ");
    scanf("%9s", origem);

    printf("Codigo do aeroporto de destino: ");
    scanf("%9s", destino);

    switch (removerVoo(v, m, origem, destino)) {
        case OK:
            printf(">> Voo removido com sucesso!\n");
            break;

        case ERRO_NAO_ENCONTRADO:
            printf(">> Falha: voo ou aeroporto nao encontrado.\n");
            break;

        default:
            printf(">> Falha: dados invalidos.\n");
            break;
    }
}

/* =========================================================
 * Topico 4 - Listar voos saindo de um aeroporto
 * ========================================================= */
void opcaoListarVoos(MatrizEsparsa *m, VetorAeroportos *v) {
    char codigo[10];

    printf("\nCodigo do aeroporto de origem: ");
    scanf("%9s", codigo);

    Status s = listarVoosSaindoDe(v, m, codigo);

    if (s == ERRO_NAO_ENCONTRADO) {
        printf(">> Falha: aeroporto %s nao esta cadastrado.\n", codigo);
    } else if (s == ERRO_INVALIDO) {
        printf(">> Falha: dados invalidos.\n");
    }
}

/* =========================================================
 * Topico 5 - Listar trajetos
 * OBS: essa funcao depende da sua buscarRotas estar implementada.
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

/* =========================================================
 * Main
 * ========================================================= */
int main(void) {
    VetorAeroportos *aeroportos = criarVetor();
    MatrizEsparsa *malha = criarMatriz();

    if (aeroportos == NULL || malha == NULL) {
        printf("Erro ao alocar memoria.\n");

        if (aeroportos != NULL)
            liberarVetor(aeroportos);

        if (malha != NULL)
            liberarMatriz(malha);

        return 1;
    }

    cadastrarAeroportosIniciais(aeroportos);

    int resposta = -1;

    while (resposta != 0) {
        mostrarMenu();

        scanf("%d", &resposta);

        switch (resposta) {
            case 0:
                printf("\nEncerrando sistema...\n");
                break;

            case 1:
                opcaoCadastrarAeroporto(aeroportos);
                break;

            case 2:
                opcaoCadastrarVoo(aeroportos, malha);
                break;

            case 3:
                opcaoRemoverVoo(aeroportos, malha);
                break;

            case 4:
                opcaoListarVoos(malha, aeroportos);
                break;

            case 5:
                listarTrajetos(malha, aeroportos);
                break;

            default:
                printf("\nNumero incompativel, tente novamente.\n");
                break;
        }

        sleep(1);
    }

    liberarVetor(aeroportos);
    liberarMatriz(malha);

    return 0;
}
