#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dict.h>

void imprimirArvore23(Portuguese23Tree *raiz, int nivel)
{
    if (raiz == NULL)
        return;

    // Indentacao para cada nível da arvore
    for (int i = 0; i < nivel; i++)
    {
        printf("    "); // Indentacao
    }

    // Imprime as chaves do nó
    if (raiz->infoCount == 1)
    {
        printf("[ %s ]\n", raiz->info1.portugueseWord);
    }
    else if (raiz->infoCount == 2)
    {
        printf("[ %s | %s ]\n", raiz->info1.portugueseWord, raiz->info2.portugueseWord);
    }

    // Se o nó tem filhos, imprime-os
    if (raiz->left != NULL || raiz->middle != NULL || raiz->right != NULL)
    {
        // Imprime os filhos à leftuerda
        if (raiz->left != NULL)
        {
            for (int i = 0; i < nivel + 1; i++)
            {
                printf("    ");
            }
            printf("left -> ");
            imprimirArvore23(raiz->left, nivel + 1); // Filhos à leftuerda
        }

        // Imprime o filho middleral
        if (raiz->middle != NULL)
        {
            for (int i = 0; i < nivel + 1; i++)
            {
                printf("    ");
            }
            printf("middle -> ");
            imprimirArvore23(raiz->middle, nivel + 1); // Filho middleral
        }

        // Imprime os filhos à righteita
        if (raiz->right != NULL)
        {
            for (int i = 0; i < nivel + 1; i++)
            {
                printf("    ");
            }
            printf("right -> ");
            imprimirArvore23(raiz->right, nivel + 1); // Filho à righteita
        }
    }
}
void carregarArquivo(const char *nomeArquivo, Portuguese23Tree **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo != NULL)
    {
        char linha[256];

        char unidadeAtual[50];

        while (fgets(linha, sizeof(linha), arquivo))
        {

            if (linha[0] == '%')
            {
                // Atualiza a unidade corretamente
                sscanf(linha, "%% Unidade %[^\n]", unidadeAtual);
            }
            else
            {
                char palavraIngles[50], traducoesPortugues[200];
                sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);
                printf("Inserindo palavra em ingles: %s na unidade: %s\n", palavraIngles, unidadeAtual);
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL)
                {
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    insertPortugueseWord(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }

        fclose(arquivo);
        printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
    }
}

void menu()
{
    printf("\n------------------------------------------------------------------------------------------------- \n");
    printf("\nMenu de opcoes:\n");
    printf("1 - Informar uma unidade e imprimir todas as palavras em portugues e as equivalentes em ingles.\n");
    printf("2 - Informar uma palavra em portugues e imprimir todas as palavras em ingles equivalentes.\n");
    printf("3 - Informar uma palavra em ingles e a unidade, remove-la da arvore binaria e da arvore 2-3.\n");
    printf("4 - Informar uma palavra em portugues e a unidade, remove-la da arvore binaria e da arvore 2-3.\n");
    printf("5 - Imprimir a arvore completa\n");
    printf("6 - Imprimir a arvore completa na ordem 2-3\n");
    printf("0 - Sair\n");
    printf("Escolha uma opcao: \n");
    printf("\n------------------------------------------------------------------------------------------------- \n");
}

int main()
{
    Portuguese23Tree *raiz = NULL;

    char palavra[50];
    char unidade[50];
    int removido;
    carregarArquivo("../input.txt", &raiz);
    int op;

    do
    {
        menu();
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a unidade que deseja imprimir as palavras: ");
            scanf("%s", unidade);
            displayWordsForUnit(raiz, unidade);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 2:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja imprimir as palavras em ingles: ");
            scanf("%s", palavra);
            displayTranslationPortuguese(&raiz, palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em ingles que deseja remover: ");
            scanf("%s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%s", unidade);
            removido = removeEnglishFindUnit(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja remover: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%s", unidade);
            removido = removePortugueseWordFindUnit(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 5:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nArvore 2-3 em ordem:\n");
            displayTree23(raiz);

            printf("\n--------------------------------------------------------------- \n");
            break;
        case 6:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nArvore 2-3:\n");
            imprimirArvore23(raiz, 0);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 0:
            printf("\n--------------------------------------------------------------- \n");
            printf("Saindo do programa...\n");
            printf("\n--------------------------------------------------------------- \n");
            break;
        default:
            printf("Insira um valor valido. \n");
            break;
        }
    } while (op != 0);

    freeTree(raiz);


    return 0;
}