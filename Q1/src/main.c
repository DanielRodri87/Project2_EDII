#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dict.h>

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
                sscanf(linha, "%% Unidade %[^\n]", unidadeAtual);

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
        ExibirMenu();
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            getchar();
            printf("Informe a Unidade: ");
            scanf("%s", unidade);
            displayWordsForUnit(raiz, unidade);
            break;

        case 2:
            getchar();
            printf("Informe a palavra em portugues: ");
            scanf("%[^\n]", palavra);
            displayTranslationPortuguese(&raiz, palavra);
            break;

        case 3:
            getchar();
            printf("Informe a palavra em ingles: ");
            scanf("%[^\n]", palavra);
            getchar();
            printf("Informe a Unidade: ");
            scanf("%s", unidade);
            removido = removeEnglishFindUnit(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            break;

        case 4:
            printf("\n--------------------------------------------------------------- \n");
            printf("Insira a palavra em portugues: ");
            setbuf(stdin, NULL);
            scanf("%[^\n]", palavra);
            printf("Informe a Unidade: ");
            scanf("%s", unidade);
            removido = removePortugueseWordFindUnit(&raiz, palavra, unidade);
            if (removido)
                printf("A palavra %s foi removida com sucesso!\n\n", palavra);
            break;

        case 5:
            displayTranslationPortuguesePath(raiz, 0);
            break;
        case 6:
            getchar();

            printf("Informe a palavra em portugues: ");
            scanf("%[^\n]", palavra);

            clock_t start, end;
            double cpu_time_used;

            start = clock();
            displayTranslationPortuguese(&raiz, palavra);
            end = clock();

            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("Tempo gasto para executar a busca: %.6f segundos\n", cpu_time_used);
            break;
        case 7:
            printf("Saindo...\n");
            break;
        default:
            printf("Insira um valor valido. \n");
            break;
        }
    } while (op != 7);

    freeTree(raiz);

    return 0;
}