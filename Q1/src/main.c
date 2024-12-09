#include "dict.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void InittialMenu()
{
    printf("\n------------------------------------------------------------------------------------------------- \n");
    printf("\nMenu de opções:\n");
    printf("1 - Informar uma unidade e imprimir todas as palavras em português e as equivalentes em inglês.\n");
    printf("2 - Informar uma palavra em português e imprimir todas as palavras em inglês equivalentes.\n");
    printf("3 - Informar uma palavra em inglês e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("4 - Informar uma palavra em português e a unidade, removê-la da árvore binária e da árvore 2-3.\n");
    printf("5 - Imprimir a árvore completa\n");
    printf("6 - Sair\n");
    printf("Escolha uma opção: \n");
    printf("\n------------------------------------------------------------------------------------------------- \n");
}

void LoadingFile(const char *nomeArquivo, PortugueseEnglish **arvore) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0; 

        if (linha[0] == '%') {
            sscanf(linha, "%% Unidade %d", &unidadeAtual);
        } else {
            char palavraIngles[50], traducoesPortugues[200];
            sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);

            char *traducaoPortugues = strtok(traducoesPortugues, ",;");
            while (traducaoPortugues != NULL) {
                while (*traducaoPortugues == ' ')
                    traducaoPortugues++; 

                Info novoInfo;
                novoInfo.portugueseWord = traducaoPortugues;
                novoInfo.unit = unidadeAtual;

                EnglishPortuguese *novaTraducao = (EnglishPortuguese *)malloc(sizeof(EnglishPortuguese));
                if (novaTraducao == NULL) {
                    printf("Erro de alocação de memória para tradução em inglês.\n");
                    return;
                }
                novaTraducao->word = palavraIngles; 
                novaTraducao->left = novaTraducao->right = NULL; 

                novoInfo.englishTranslation = novaTraducao; 

                Info promote;
                PortugueseEnglish *pai = NULL;

                insertPortugueseWord(arvore, novoInfo, &promote, &pai);

                traducaoPortugues = strtok(NULL, ",;");
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}



int main()
{
    printf("oi\n\n");
    PortugueseEnglish *root = NULL;
    PortugueseEnglish *father = NULL;
    int unit, removed, option, result;
    char word[100];
    printf("oi\n\n");


    LoadingFile("../../input.txt", &root);
    printf("oi\n\n");

    do
    {
        InittialMenu();
        scanf("%d", &option);

        switch (option)
        {
        case 1:
            printf("Informe a unidade das palavras que deseja exibir: ");
            scanf("%d", &unit);
            printWordsByUnit(root, unit);
            break;

        case 2:
            printf("Insira a palavra em português que deseja imprimir as palavras em inglês: ");
            scanf("%s", word);
            findEnglishByPortuguese(root, word);
            break;

        case 3:
            printf("Insira a palavra em inglês que deseja remover: ");
            scanf("%s", word);
            printf("Insira a unidade que deseja remover: ");
            scanf("%d", &unit);
            SearchEnglishWord(&root, word, unit, &father);
            break;

        case 4:
            printf("Insira a palavra em português que deseja remover: ");
            scanf("%s", word);
            removed = remover23(&father, &root, word);
            if (removed)
                printf("Palavra '%s' removida com sucesso.\n", word);
            
            break;

        case 5:
            printf("Imprimindo a árvore completa:\n");
            displayWords(root);
            break;

        case 6:
            printf("Saindo do programa.\n");
            break;

        default:
            printf("Opção inválida! Tente novamente.\n");
            break;
        }

    } while (option != 6);

    return 0;
}
