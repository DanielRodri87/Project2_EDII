#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exibirMenu()
{
    printf("\n===============================================================\n");
    printf("                          MENU PRINCIPAL                      \n");
    printf("===============================================================\n");
    printf("  [1] Exibir palavras em português e inglês por unidade\n");
    printf("  [2] Mostrar traduções a partir de uma palavra em português\n");
    printf("  [3] Remover uma palavra em inglês de uma unidade\n");
    printf("  [4] Remover uma palavra em português de uma unidade\n");
    printf("  [5] Sair\n");
    printf("===============================================================\n");
    printf("Escolha uma opção: ");
}


void carregarArquivo(const char *nomeArquivo, RedBlackTreeNode **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    char linha[256];
    int unidadeAtual = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // Remover o caractere de nova linha, se presente
        linha[strcspn(linha, "\n")] = '\0';

        if (linha[0] == '%')
        {
            // Detectar unidade atual
            if (sscanf(linha, "%% Unidade %d", &unidadeAtual) != 1)
                printf("Formato de unidade inválido: %s\n", linha);
        }
        else
        {
            char palavraIngles[50], traducoesPortugues[200];

            // Separar palavra em inglês e traduções em português
            if (sscanf(linha, "%[^:]: %[^\n]", palavraIngles, traducoesPortugues) == 2)
            {
                char *traducaoPortugues = strtok(traducoesPortugues, ",;");

                while (traducaoPortugues != NULL)
                {
                    // Remover espaços iniciais
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    // Criar nova informação para o nó
                    RBTreeNodeInfo info;
                    info.portugueseWord = strdup(traducaoPortugues);

                    // Alocar e preencher o nó da árvore binária
                    BinaryTreeNode *novoNo = malloc(sizeof(BinaryTreeNode));
                    if (novoNo == NULL)
                    {
                        printf("Erro ao alocar memória para o nó binário.\n");
                        fclose(arquivo);
                        return;
                    }

                    novoNo->info.englishWord = strdup(palavraIngles);
                    novoNo->info.unit = unidadeAtual;
                    novoNo->left = novoNo->right = NULL;

                    info.binaryTreeEnglish = novoNo;

                    // Inserir na árvore rubro-negra
                    if (insertRedBlackTreeNode(arvore, info) != 0)
                    {
                        printf("Erro ao inserir na árvore rubro-negra.\n");
                        fclose(arquivo);
                        return;
                    }

                    // Próxima tradução
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
            else
            {
                printf("Linha inválida: %s\n", linha);
            }
        }
    }

    fclose(arquivo);
    printf("Arquivo '%s' carregado com sucesso!\n", nomeArquivo);
}



/*
int carregarArquivo(const char *nomeArquivo, RedBlackTreeNode **arvore)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    int resultado = 1;

    if (arquivo == NULL)
    {
        resultado = 0;
    }
    else
    {
        char linha[256];
        int unidadeAtual = 0;

        while (fgets(linha, sizeof(linha), arquivo))
        {
            linha[strcspn(linha, "\n")] = 0;

            if (linha[0] == '%')
                sscanf(linha, "%% Unidade %d", &unidadeAtual);
            else
            {
                char palavraIngles[50], traducoesPortugues[200];
                sscanf(linha, "%[^:]: %[^;]", palavraIngles, traducoesPortugues);

                char *traducaoPortugues = strtok(traducoesPortugues, ",;");
                while (traducaoPortugues != NULL)
                {
                    while (*traducaoPortugues == ' ')
                        traducaoPortugues++;

                    RBTreeNodeInfo info;
                    info.portugueseWord = strdup(traducoesPortugues);
                    info.binaryTreeEnglish->info.englishWord = strdup(palavraIngles);
                    info.binaryTreeEnglish->info.unit = unidadeAtual;

                    // RBTreeNodeInfo info = {
                    //     .portugueseWord = strdup(traducoesPortugues),
                    //     .englishWord = strdup(palavraIngles),
                    //     .unit = unidadeAtual
                    // };
                    insertRedBlackTreeNode(arvore, info);
                    traducaoPortugues = strtok(NULL, ",;");
                }
            }
        }

        fclose(arquivo);
    }

    return resultado;
}
*/

int main()
{

    printf("oii entrou\n\n");
    RedBlackTreeNode *raiz = NULL;
    char palavra[50];
    int unidade;
    int op;

    carregarArquivo("../../input.txt", &raiz);
    printf("saiu\n\n");

    do
    {
        exibirMenu();
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("\n---------------- EXIBIR PALAVRAS DE UMA UNIDADE -----------------\n");
            printf("Digite o número da unidade: ");
            scanf("%d", &unidade);
            printWordsByUnit(raiz, unidade);
            printf("\n-----------------------------------------------------------------\n");
            break;

        case 2:
            printf("\n----------- TRADUZIR PALAVRA EM PORTUGUÊS PARA INGLÊS -----------\n");
            printf("Digite a palavra em português: ");
            scanf("%s", palavra);
            printf("Traduções em inglês para a palavra '%s':\n", palavra);
            findEnglishByPortuguese(raiz, palavra);
            printf("\n-----------------------------------------------------------------\n");
            break;

        case 3:
            printf("\n------------------- REMOVER PALAVRA EM INGLÊS ------------------\n");
            printf("Digite a palavra em inglês: ");
            scanf("%s", palavra);
            printf("Digite a unidade: ");
            scanf("%d", &unidade);

            RedBlackTreeNode *node = raiz;
            while (node != NULL)
            {
                if (strcmp(node->info.portugueseWord, palavra) == 0)
                {
                    if (isLeaf(node))
                    {
                        node = soumfilho(node);
                        free(node);
                    }
                    else
                    {
                        RedBlackTreeNode *smallest = SmallChild(node);
                        free(smallest);
                    }
                    break;
                }
                node = (strcmp(node->info.portugueseWord, palavra) > 0) ? node->left : node->right;
            }
            printf("\n-----------------------------------------------------------------\n");
            break;

        case 4:
            printf("\n---------------- REMOVER PALAVRA EM PORTUGUÊS ----------------\n");
            printf("Digite a palavra em português: ");
            scanf("%s", palavra);

            RedBlackTreeNode *nodeToRemove = raiz;
            while (nodeToRemove != NULL)
            {
                if (strcmp(nodeToRemove->info.portugueseWord, palavra) == 0)
                {
                    if (isLeaf(nodeToRemove))
                    {
                        nodeToRemove = soumfilho(nodeToRemove);
                        free(nodeToRemove);
                    }
                    else
                    {
                        RedBlackTreeNode *smallest = SmallChild(nodeToRemove);
                        free(smallest);
                    }
                    break;
                }
                nodeToRemove = (strcmp(nodeToRemove->info.portugueseWord, palavra) > 0) ? nodeToRemove->left : nodeToRemove->right;
            }
            printf("\n-----------------------------------------------------------------\n");
            break;

        case 5:
            printf("\n======================= SAINDO DO PROGRAMA =====================\n");
            break;

        default:
            printf("\nOpção inválida! Por favor, escolha uma opção válida.\n\n");
            break;
        }
    } while (op != 5);

    return 0;
}
