#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "dict.h"

/**
 * @brief insere uma nova informação na árvore rubro-negra.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param info Informações a serem inseridas na árvore.
 */
void insert(RBTree **root, InfoBin info)
{
    int result;
    result = insertIntoRBTree(root, info);
    (*root)->color = 1;
}

/**
 * @brief Exibe o menu principal de opções para o usuário.
 */
void ExibirMenu()
{
    printf("\n===============================================================\n");
    printf("                          MENU PRINCIPAL                         \n");
    printf("=================================================================\n");
    printf("[1] Exibir palavras em português e inglês por unidade\n");
    printf("[2] Mostrar traduções a partir de uma palavra em português\n");
    printf("[3] Remover uma palavra em inglês de uma unidade\n");
    printf("[4] Remover uma palavra em português de uma unidade\n");
    printf("[5] Caminho Percorrido\n");
    printf("[6] Metrificar\n");
    printf("[7] Sair\n");
    printf("=================================================================\n");
    printf("Opção: ");
}

void removeSpace(char *str)
{
    if (str != NULL && str[0] == ' ')
        memmove(str, str + 1, strlen(str));
}

int main()
{
    RBTree *root = NULL;
    char line[200];
    int currentUnit = 0, sucess = 1;

    FILE *file;
    file = fopen("../../input.txt", "r");

    if (file == NULL)
        sucess = 0;

    if (sucess = 1)
    {
        while (fgets(line, sizeof(line), file) != NULL)
        {
            line[strcspn(line, "\n")] = '\0';

            if (strncmp(line, "% Unidade", 9) == 0)
                sscanf(line, "%% Unidade %d", &currentUnit);
            else if (line[0] != '\0' && line[0] != '%')
            {
                char WordIN[50];

                char *token = strtok(line, ":");
                if (token != NULL)
                    strcpy(WordIN, token);

                removeSpace(WordIN);

                token = strtok(NULL, ";");
                if (token != NULL)
                {
                    char *palavra = strtok(token, ",");
                    while (palavra != NULL)
                    {
                        InfoBin info;
                        info.portugueseWord = (char *)malloc(50 * sizeof(char));
                        info.binaryTreeEnglish = (EngPT *)malloc(sizeof(EngPT));
                        info.binaryTreeEnglish->info.units = (Units *)malloc(sizeof(Units));
                        info.binaryTreeEnglish->info.units->next = NULL;
                        info.binaryTreeEnglish->left = NULL;
                        info.binaryTreeEnglish->right = NULL;
                        info.binaryTreeEnglish->info.englishWord = (char *)malloc(50 * sizeof(char));

                        removeSpace(palavra);
                        strcpy(info.portugueseWord, palavra);
                        strcpy(info.binaryTreeEnglish->info.englishWord, WordIN);
                        info.binaryTreeEnglish->info.units->unit = currentUnit;

                        insert(&root, info);
                        palavra = strtok(NULL, ",");
                    }
                }
            }
        }

        fclose(file);
        int enc = 0;
        int op = 0, unit = 0;
        char palavra[50];

        do
        {
            enc = 0;
            ExibirMenu();
            scanf("%d", &op);
            getchar();
            switch (op)
            {
            case 1:
                printf("Informe a Unidade: ");
                scanf("%d", &unit);
                displayUnit(root, unit, &enc);
                break;
            case 2:
                printf("Informe a palavra em portugues: ");
                scanf("%[^\n]", palavra);
                getchar();
                findEnglishByPortuguese(root, palavra);
                break;
            case 3:
                printf("Informe a palavra em ingles: ");
                scanf("%[^\n]", palavra);
                getchar();
                printf("Informe a Unidade: ");
                scanf("%d", &unit);
                removeFromRBTreeEN(&root, palavra, unit);
                break;
            case 4:
                printf("Informe a palavra em portugues: ");
                scanf("%[^\n]", palavra);
                getchar();
                printf("Informe a Unidade: ");
                scanf("%d", &unit);
                removeFromRBTreePT(&root, palavra, unit);
                break;
            case 5:
                printf("Informe a palavra em portugues: ");
                scanf("%[^\n]", palavra);
                getchar();
                findEnglishByPortuguesePath(root, palavra, 0);
                break;
            
            case 6: 
                printf("Informe a palavra em portugues: ");
                scanf("%[^\n]", palavra);
                getchar();
                
                clock_t start, end;
                double cpu_time_used;

                start = clock();
                findEnglishByPortuguese(root, palavra);
                end = clock();
                
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("Tempo gasto para executar a busca: %.6f segundos\n", cpu_time_used);
                break;

            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                break;
            }
        } while (op != 7);
    }

    return sucess == 1 ? 0 : 1;
}