#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aeroporto.h"
#include "matriz_esparsa.h"
#include "voo.h"

int main() {

    /* ===== Estruturas principais ===== */
    VetorAeroportos *aeroportos = criarVetor();
    MatrizEsparsa *malha = criarMatriz();

    if (aeroportos == NULL || malha == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    /* ===== Aeroportos iniciais ===== */
    cadastrarAeroporto(aeroportos, "CNF", "Belo Horizonte");
    cadastrarAeroporto(aeroportos, "BSB", "Brasilia");
    cadastrarAeroporto(aeroportos, "GIG", "Rio de Janeiro");
    cadastrarAeroporto(aeroportos, "SSA", "Salvador");
    cadastrarAeroporto(aeroportos, "GRU", "Sao Paulo");

    int resposta = -1;

    while (resposta != 0) {

        printf("\n====================================\n");
        printf("Sistema de Malha Aerea - ANAC\n");
        printf("====================================\n\n");

        printf("0 - Sair\n");
        printf("1 - Cadastrar novo aeroporto\n");
        printf("2 - Cadastrar voo\n");
        printf("3 - Remover voo\n");
        printf("4 - Listar voos de um aeroporto\n");
        printf("5 - Listar trajetos\n\n");

        printf("Resposta: ");
        scanf("%d", &resposta);

        switch (resposta) {

            case 1: {
                char codigo[4];
                char cidade[50];

                printf("Codigo do aeroporto: ");
                scanf("%s", codigo);

                printf("Cidade: ");
                scanf(" %[^\n]", cidade);

                Status s = cadastrarAeroporto(aeroportos,
                                              codigo,
                                              cidade);

                if (s == OK)
                    printf("Aeroporto cadastrado com sucesso!\n");
                else
                    printf("Erro ao cadastrar aeroporto.\n");

                break;
            }

            case 2: {
                char origem[4];
                char destino[4];
                int numero;

                printf("Codigo origem: ");
                scanf("%s", origem);

                printf("Codigo destino: ");
                scanf("%s", destino);

                printf("Numero do voo: ");
                scanf("%d", &numero);

                Status s = cadastrarVoo(aeroportos,
                                        malha,
                                        origem,
                                        destino,
                                        numero);

                if (s == OK)
                    printf("Voo cadastrado com sucesso!\n");
                else
                    printf("Erro ao cadastrar voo.\n");

                break;
            }

            case 4: {
                char codigo[4];

                printf("Codigo do aeroporto: ");
                scanf("%s", codigo);

                listarVoosSaindoDe(aeroportos,
                                   malha,
                                   codigo);

                break;
            }

            case 0:
                printf("Encerrando sistema...\n");
                break;

            default:
                printf("Opcao ainda nao implementada.\n");
        }

        sleep(1);
    }

    liberarVetor(aeroportos);
    liberarMatriz(malha);

    return 0;
}
