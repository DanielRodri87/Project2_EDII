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

void DisplayMenu()
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
    int start = 0, end = 0, status = 2, requiredSpace, control_menu = 0, bloco, space, start2 = 0;
    char opc;

    srand(time(NULL));

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

    newInfo = CreateInfo(start, end, status);
    inserirArv23(&tree, newInfo, &promove, NULL);
    start = end + 1;

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
    do
    {
        DisplayMenu();
        scanf("%d", &control_menu);

        switch (control_menu)
        {
        case 1:
            printf("\nInforme o espaço necessário para alocação: ");
            scanf("%d", &requiredSpace);
            space = AllocateSpace(&tree, requiredSpace, &start2);
            if (space == requiredSpace)
            {
                if (findFirstStart(tree) == start2)
                {
                    int remover_inicio;
                    mergeNodesStart(&tree, &remover_inicio);
                    Memory_remover(&tree, remover_inicio);
                    printf("Compactação realizada no início da memória.\n");
                }
                else if (findLastStart(tree) == start2)
                {
                    int remover_inicio_fim;
                    mergeNodesEnd(&tree, &remover_inicio_fim);
                    Memory_remover(&tree, remover_inicio_fim);
                    printf("Compactação realizada no fim da memória.\n");
                }
                else
                {
                    int aux1, aux2;
                    mergeNodesMiddle(&tree, &aux1, &aux2);
                    Memory_remover(&tree, aux1);
                    Memory_remover(&tree, aux2);
                    printf("Compactação realizada no meio da memória.\n");
                }
            }
            break;

        case 2:
            printf("\nInforme o início da memória a ser desocupada: ");
            scanf("%d", &start);
            printf("Informe o fim da memória a ser desocupada: ");
            scanf("%d", &end);
            FreeSpace(tree, start, end);
            if (findFirstStart(tree) == start)
            {
                int remover_inicio;
                mergeNodesStart(&tree, &remover_inicio);
                Memory_remover(&tree, remover_inicio);
                printf("Compactação realizada no início da memória.\n");
            }
            else if (findLastStart(tree) == start)
            {
                int remover_inicio_fim;
                mergeNodesEnd(&tree, &remover_inicio_fim);
                Memory_remover(&tree, remover_inicio_fim);
                printf("Compactação realizada no fim da memória.\n");
            }
            else
            {
                int aux1, aux2;
                mergeNodesMiddle(&tree, &aux1, &aux2);
                Memory_remover(&tree, aux1);
                Memory_remover(&tree, aux2);
                printf("Compactação realizada no meio da memória.\n");
            }
            break;

        case 3:
            printf("\nExibindo alocação de memória...\n");
            DisplayInfos(tree);
            break;

        case 4:
            printf("\nSaindo da aplicação...\n");
            break;

        default:
            printf("\nOpção inválida! Por favor, escolha uma opção entre 1 e 6.\n");
            break;
        }
    } while (control_menu != 4);

    return 0;
}