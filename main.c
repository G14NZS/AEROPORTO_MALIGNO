#include <stdio.h>
//#include <windows.h>
#include <unistd.h>


//Belo Horizonte (CNF), Brasília (BSB), Rio de Janeiro (GIG), Salvador (SSA) e São Paulo (GRU)




int main()
{
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
