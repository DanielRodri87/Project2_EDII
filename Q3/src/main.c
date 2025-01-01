#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src.h"

int findFirstStart(Memory *root)
{
    int result = -1;

    if (root != NULL)
    {
        Memory *current = root;

        while (current->left != NULL)
            current = current->left;
        result = current->info1->start;
    }

    return (result);
}

int findLastStart(Memory *root)
{
    int result = -1;

    if (root != NULL)
    {
        Memory *current = root;
        while (current->right != NULL)
            current = current->right;

        if (current->numKeys == 2 && current->info2 != NULL)
            result = current->info2->start;
        else
            result = current->info1->start;
    }

    return (result);
}

void displayMenu()
{
    printf("\n===============================================================\n");
    printf("                          MENU PRINCIPAL                      \n");
    printf("===============================================================\n");
    printf("  [1] Ocupar Nós Livres\n");
    printf("  [2] Desocupar Nós\n");
    printf("  [3] Exibir Alocação de Memória\n");
    printf("  [4] Sair\n");
    printf("===============================================================\n");
    printf("Escolha uma opção (1-4): ");
}


int main()
{
    Memory *tree = NULL;
    Info *newInfo;
    Info promove;
    int start = 0, end = 0, status = 2, requiredSpace, menuControl = 0, block, space, start2 = 0, removeStart;
    char opc;

    srand(time(NULL));

    printf("===============================================================\n");
    printf("          Bem-vindo ao Gerenciador de Memória Dinâmica          \n");
    printf("===============================================================\n");
    printf("Indique o tamanho total da memória (em unidades): ");
    scanf("%d", &block);

    do
    {
        printf("\nInforme o valor de fim para alocação de memória:\n");
        printf("Valor de Início = 0\n");
        printf("Informe o valor de fim: ");
        scanf("%d", &end);
    } while (start < 0 || end > block || end < start);

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

    newInfo = CreateInfo(start, end, status);
    insertTree23(&tree, newInfo, &promove, NULL);
    start = end + 1;

    while (end != block)
    {
        printf("\nInforme o valor final da memória (entre %d e %d): ", start, block);
        scanf("%d", &end);

        if (end >= start && end <= block)
        {
            if (status == FREE)
                status = OCCUPIED;
            else
                status = FREE;

            newInfo = CreateInfo(start, end, status);
            insertTree23(&tree, newInfo, &promove, NULL);
            start = end + 1;
        }
        else
            printf("\nValor de fim inválido. Informe um valor entre %d e %d.\n", start, block);
    }
    do
    {
        displayMenu();
        scanf("%d", &menuControl);

        switch (menuControl)
        {
        case 1:
            printf("\nInforme o espaço necessário para alocação: ");
            scanf("%d", &requiredSpace);
            space = allocateSpace(&tree, requiredSpace, &start2);
            if (space == requiredSpace)
            {
                if (findFirstStart(tree) == start2)
                {
                    mergeNodesStart(&tree, &removeStart);
                    removeMemory(&tree, removeStart);
                    printf("Compactação realizada no início da memória.\n");
                }
                else if (findLastStart(tree) == start2)
                {
                    mergeNodesEnd(&tree, &removeStart);
                    removeMemory(&tree, removeStart);
                    printf("Compactação realizada no fim da memória.\n");
                }
                else
                {
                    int aux1, aux2;
                    mergeNodesMiddle(&tree, &aux1, &aux2);
                    printf("comeco1 %d comeco 2 %d\n", aux1, aux2);

                    removeMemory(&tree, aux1);
                    removeMemory(&tree, aux2);
                    printf("Compactação realizada no meio da memória.\n");
                }
            }
            break;

        case 2:
            printf("\nInforme o início da memória a ser desocupada: ");
            scanf("%d", &start);
            printf("Informe o fim da memória a ser desocupada: ");
            scanf("%d", &end);
            freeSpace(tree, start, end);
            if (findFirstStart(tree) == start)
            {
                int removeStart;
                mergeNodesStart(&tree, &removeStart);
                removeMemory(&tree, removeStart);
                printf("Compactação realizada no início da memória.\n");
            }
            else if (findLastStart(tree) == start)
            {
                mergeNodesEnd(&tree, &removeStart);
                removeMemory(&tree, removeStart);
                printf("Compactação realizada no fim da memória.\n");
            }
            else
            {
                int aux1, aux2;
                mergeNodesMiddle(&tree, &aux1, &aux2);
                removeMemory(&tree, aux1);
                removeMemory(&tree, aux2);
                printf("Compactação realizada no meio da memória.\n");
            }
            break;

        case 3:
            printf("\nExibindo alocação de memória...\n");
            displayInfos(tree);
            break;

        case 4:
            printf("\nSaindo da aplicação...\n");
            break;

        default:
            printf("\nOpção inválida! Por favor, escolha uma opção entre 1 e 6.\n");
            break;
        }
    } while (menuControl != 4);

    return 0;
}