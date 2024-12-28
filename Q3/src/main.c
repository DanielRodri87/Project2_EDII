#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src.h"

void DisplayMenu()
{
    printf("\n===============================================================\n");
    printf("                          MENU PRINCIPAL                      \n");
    printf("===============================================================\n");
    printf("  [1] Ocupar Nós Livres\n");
    printf("  [2] Desocupar Nós\n");
    printf("  [3] Compactar Memória\n");
    printf("  [4] Remover Bloco de Memória\n");
    printf("  [5] Exibir Alocação de Memória\n");
    printf("  [6] Sair\n");
    printf("===============================================================\n");
    printf("Escolha uma opção (1-6): ");
}

void DisplaySubMenu()
{
    printf("\n===============================================================\n");
    printf("              OPÇÕES DE COMPACTAÇÃO DE MEMÓRIA                  \n");
    printf("===============================================================\n");
    printf("  [1] Compactar no Início\n");
    printf("  [2] Compactar no Meio\n");
    printf("  [3] Compactar no Fim\n");
    printf("  [4] Cancelar\n");
    printf("===============================================================\n");
    printf("Escolha uma opção (1-4): ");
}

int main()
{
    Memory *tree = NULL;
    Info *newInfo;
    Info promove;
    int start = 0, end = 0, status = 2, requiredSpace, control_menu = 0, sub_menu = 0, bloco;
    char opc;
    srand(time(NULL));

    // Solicitar o tamanho da memória
    printf("===============================================================\n");
    printf("          Bem-vindo ao Gerenciador de Memória Dinâmica          \n");
    printf("===============================================================\n");
    printf("Indique o tamanho total da memória (em unidades): ");
    scanf("%d", &bloco);

    do
    {
        printf("\nInforme o valor de fim para alocação de memória:\n");
        printf("Valor de Início = 0\n");
        printf("Informe o valor de fim: ");
        scanf("%d", &end);
    } while (start < 0 || end > bloco || end < start);

    do
    {
        printf("\nO bloco de memória está livre ou ocupado? (L - Livre / O - Ocupado): ");
        scanf(" %c", &opc);

        switch (opc)
        {
        case 'L':
            status = FREE;
            break;
        case 'O':
            status = OCCUPIED;
            break;
        default:
            printf("\nOpção inválida! Por favor, escolha entre 'L' para Livre ou 'O' para Ocupado.\n");
        }

    } while (status != FREE && status != OCCUPIED);

    // Primeira inserção de memória
    newInfo = CreateInfo(start, end, status);
    inserirArv23(&tree, newInfo, &promove, NULL);
    start = end + 1;

    // Loop para alocar mais memória até atingir o total
    while (end != bloco)
    {
        printf("\nInforme o valor final da memória (entre %d e %d): ", start, bloco);
        scanf("%d", &end);

        if (end >= start && end <= bloco)
        {
            if (status == FREE)
                status = OCCUPIED;
            else
                status = FREE;

            newInfo = CreateInfo(start, end, status);
            inserirArv23(&tree, newInfo, &promove, NULL);
            start = end + 1;
        }
        else
            printf("\nValor de fim inválido. Informe um valor entre %d e %d.\n", start, bloco);
    }

    // Menu principal
    do
    {
        DisplayMenu();
        scanf("%d", &control_menu);

        switch (control_menu)
        {
        case 1:
            printf("\nInforme o espaço necessário para alocação: ");
            scanf("%d", &requiredSpace);
            AllocateSpace(&tree, requiredSpace);
            break;

        case 2:
            printf("\nInforme o início da memória a ser desocupada: ");
            scanf("%d", &start);
            printf("Informe o fim da memória a ser desocupada: ");
            scanf("%d", &end);
            FreeSpace(tree, start, end);
            break;

        case 3:
            DisplaySubMenu();
            scanf("%d", &sub_menu);

            switch (sub_menu)
            {
            case 1:
                {
                    int remover_inicio, remover_fim;
                    remover_fim = mergeNodesStart(&tree, &remover_inicio);
                    newInfo = CreateInfo(remover_inicio, remover_fim, 0);
                    removeFromMemory(NULL, &tree, newInfo);
                    printf("Compactação realizada no início da memória.\n");
                }
                break;

            case 2:
                {
                    int remover_inicio_meio1, remover_fim_meio1;
                    int remover_inicio_meio2, remover_fim_meio2;
                    mergeNodesMiddle(&tree, &remover_inicio_meio1, &remover_fim_meio1, &remover_inicio_meio2, &remover_fim_meio2);

                    newInfo = CreateInfo(remover_inicio_meio1, remover_fim_meio1, 0);
                    removeFromMemory(&tree, &tree, newInfo);

                    newInfo = CreateInfo(remover_inicio_meio2, remover_fim_meio2, 0);
                    removeFromMemory(&tree, &tree, newInfo);

                    printf("Compactação realizada no meio da memória.\n");
                }
                break;

            case 3:
                {
                    int remover_inicio_fim, remover_fim_fim;
                    remover_fim_fim = mergeNodesEnd(&tree, &remover_inicio_fim);
                    newInfo = CreateInfo(remover_inicio_fim, remover_fim_fim, 0);
                    removeFromMemory(NULL, &tree, newInfo);
                    printf("Compactação realizada no fim da memória.\n");
                }
                break;

            case 4:
                printf("\nVoltando ao menu principal...\n");
                break;

            default:
                printf("\nOpção inválida! Escolha uma opção entre 1 e 4.\n");
                break;
            }
            break;

        case 4:
            printf("\nDigite o endereço de início do bloco de memória a ser removido: ");
            scanf("%d", &start);

            printf("Digite o endereço final do bloco de memória a ser removido: ");
            scanf("%d", &end);

            newInfo = CreateInfo(start, end, 0);
            removeFromMemory(NULL, &tree, newInfo);
            break;

        case 5:
            printf("\nExibindo alocação de memória...\n");
            DisplayInfos(tree);
            break;

        case 6:
            printf("\nSaindo da aplicação...\n");
            break;

        default:
            printf("\nOpção inválida! Por favor, escolha uma opção entre 1 e 6.\n");
            break;
        }
    } while (control_menu != 6);

    return 0;
}
