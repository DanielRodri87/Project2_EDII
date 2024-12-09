#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exibirMenu() {
    printf("\n===============================================================\n");
    printf("                          MENU PRINCIPAL                      \n");
    printf("===============================================================\n");
    printf("  [1] Exibir palavras em português e inglês de uma unidade\n");
    printf("  [2] Traduzir uma palavra em português para inglês\n");
    printf("  [3] Remover uma palavra em inglês de uma unidade\n");
    printf("  [4] Remover uma palavra em português de uma unidade\n");
    printf("  [5] Exibir toda a árvore 2-3\n");
    printf("  [0] Sair\n");
    printf("===============================================================\n");
    printf("Escolha uma opção: ");
}

void carregarArquivo(const char *nomeArquivo, Portuguese23 **arvore) {
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

                insertPortugueseWord(arvore, traducaoPortugues, palavraIngles, unidadeAtual);
                traducaoPortugues = strtok(NULL, ",;");
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}

int main() {
    Portuguese23 *raiz = NULL;
    Portuguese23 *pai = NULL;

    char palavra[50];
    int unidade;
    int removido;
    carregarArquivo("../input.txt", &raiz);
    int op;

    do {
        exibirMenu();
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("\n---------------- EXIBIR PALAVRAS DE UMA UNIDADE ----------------\n");
                printf("Digite o número da unidade: ");
                scanf("%d", &unidade);
                printWordsByUnit(raiz, unidade);
                printf("\n---------------------------------------------------------------\n");
                break;

            case 2:
                printf("\n----------- TRADUZIR PALAVRA EM PORTUGUÊS PARA INGLÊS -----------\n");
                printf("Digite a palavra em português: ");
                scanf("%s", palavra);
                displayPortugueseTranslation(&raiz, palavra);
                printf("\n---------------------------------------------------------------\n");
                break;

            case 3:
                printf("\n------------------- REMOVER PALAVRA EM INGLÊS ------------------\n");
                printf("Digite a palavra em inglês: ");
                scanf("%s", palavra);
                printf("Digite a unidade: ");
                scanf("%d", &unidade);
                searchEnglishWord(&raiz, palavra, unidade, &pai);
                printf("\n---------------------------------------------------------------\n");
                break;

            case 4:
                printf("\n---------------- REMOVER PALAVRA EM PORTUGUÊS ----------------\n");
                printf("Digite a palavra em português: ");
                scanf("%s", palavra);
                removido = remove23(&pai, &raiz, palavra);
                if (removido)
                    printf("A palavra '%s' foi removida com sucesso!\n\n", palavra);
                else
                    printf("A palavra '%s' não foi encontrada.\n\n", palavra);
                printf("\n---------------------------------------------------------------\n");
                break;

            case 5:
                printf("\n------------------- EXIBIR TODA A ÁRVORE 2-3 ------------------\n");
                displayTree23(raiz);
                printf("\n---------------------------------------------------------------\n");
                break;

            case 0:
                printf("\n======================= SAINDO DO PROGRAMA =====================\n");
                break;

            default:
                printf("\nOpção inválida! Por favor, escolha uma opção válida.\n\n");
                break;
        }
    } while (op != 0);

    freeTree(raiz);

    return 0;
}
