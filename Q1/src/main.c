#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dict.h>

void imprimirArvore23(Portugues23 *raiz, int nivel)
{
    if (raiz == NULL)
        return;

    // Indentacao para cada nível da arvore
    for (int i = 0; i < nivel; i++)
    {
        printf("    "); // Indentacao
    }

    // Imprime as chaves do nó
    if (raiz->nInfos == 1)
    {
        printf("[ %s ]\n", raiz->info1.palavraPortugues);
    }
    else if (raiz->nInfos == 2)
    {
        printf("[ %s | %s ]\n", raiz->info1.palavraPortugues, raiz->info2.palavraPortugues);
    }

    // Se o nó tem filhos, imprime-os
    if (raiz->esq != NULL || raiz->cent != NULL || raiz->dir != NULL)
    {
        // Imprime os filhos à esquerda
        if (raiz->esq != NULL)
        {
            for (int i = 0; i < nivel + 1; i++)
            {
                printf("    ");
            }
            printf("Esq -> ");
            imprimirArvore23(raiz->esq, nivel + 1); // Filhos à esquerda
        }

        // Imprime o filho central
        if (raiz->cent != NULL)
        {
            for (int i = 0; i < nivel + 1; i++)
            {
                printf("    ");
            }
            printf("Cent -> ");
            imprimirArvore23(raiz->cent, nivel + 1); // Filho central
        }

        // Imprime os filhos à direita
        if (raiz->dir != NULL)
        {
            for (int i = 0; i < nivel + 1; i++)
            {
                printf("    ");
            }
            printf("Dir -> ");
            imprimirArvore23(raiz->dir, nivel + 1); // Filho à direita
        }
    }
}
void carregarArquivo(const char *nomeArquivo, Portugues23 **arvore)
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

                    inserirPalavraPortugues(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
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
    Portugues23 *raiz = NULL;

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
            imprimirPalavrasUnidade(raiz, unidade);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 2:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues que deseja imprimir as palavras em ingles: ");
            scanf("%s", palavra);
            exibir_traducao_Portugues(&raiz, palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 3:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em ingles que deseja remover: ");
            scanf("%s", palavra);
            printf("Insira a unidade da palavra que deseja remover: ");
            scanf("%s", unidade);
            removido = Remove_palavra_ingles_unidade(&raiz, palavra, unidade);
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
            removido = Remove_palavra_portugues_unidade(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            printf("\n--------------------------------------------------------------- \n");
            break;
        case 5:
            printf("\n--------------------------------------------------------------- \n");
            printf("\nArvore 2-3 em ordem:\n");
            exibir_tree23(raiz);

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