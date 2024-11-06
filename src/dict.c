#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void exibir_mensagem()
{
    printf("hello world\n\n");
}

// Função para criar um novo nó em inglês
EnglishNode *createEnglishNode(const char *englishWord, const char *unit) {
    EnglishNode *newNode = (EnglishNode *)malloc(sizeof(EnglishNode));
    newNode->englishWord = strdup(englishWord);
    newNode->unit = strdup(unit);
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Função para criar um novo nó em português
PortugueseNode *createPortugueseNode(const char *portugueseWord) {
    PortugueseNode *newNode = (PortugueseNode *)malloc(sizeof(PortugueseNode));
    newNode->portugueseWord = strdup(portugueseWord);
    newNode->englishWords = NULL;
    newNode->left = newNode->middle = newNode->right = NULL;
    newNode->nInfos = 1;
    return newNode;
}

// Função auxiliar para adicionar uma palavra em inglês à árvore binária do nó português
EnglishNode *addEnglishWord(EnglishNode *root, const char *englishWord, const char *unit) {
    if (root == NULL) {
        return createEnglishNode(englishWord, unit);
    }

    int cmp = strcmp(englishWord, root->englishWord);
    if (cmp == 0) {
        // Palavra já existe, mas podemos atualizar a unidade ou ignorar
        free(root->unit);  // Liberar a unidade antiga
        root->unit = strdup(unit);
    } else if (cmp < 0) {
        root->left = addEnglishWord(root->left, englishWord, unit);
    } else {
        root->right = addEnglishWord(root->right, englishWord, unit);
    }
    return root;
}

// Função auxiliar para buscar ou inserir uma palavra em português na árvore 2-3
PortugueseNode *addPortugueseNode(PortugueseNode *root, const char *portugueseWord) {
    if (root == NULL) {
        return createPortugueseNode(portugueseWord);
    }

    int cmp = strcmp(portugueseWord, root->portugueseWord);
    if (cmp == 0) {
        return root; // Palavra já existe, retorna o nó atual
    } else if (cmp < 0) {
        root->left = addPortugueseNode(root->left, portugueseWord);
    } else {
        root->right = addPortugueseNode(root->right, portugueseWord);
    }

    // Nota: Aqui não estamos implementando o balanceamento completo da árvore 2-3
    return root;
}

// Função principal para adicionar uma palavra ao dicionário
void addWord(Dictionary *dict, const char *portugueseWord, const char *englishWord, const char *unit) {
    if (dict->root == NULL) {
        // Cria o primeiro nó da árvore
        dict->root = createPortugueseNode(portugueseWord);
        dict->root->englishWords = addEnglishWord(dict->root->englishWords, englishWord, unit);
    } else {
        // Procura o nó português, ou o adiciona se não existir
        PortugueseNode *portugueseNode = addPortugueseNode(dict->root, portugueseWord);
        // Adiciona a palavra inglesa ao nó português correspondente
        portugueseNode->englishWords = addEnglishWord(portugueseNode->englishWords, englishWord, unit);
    }
}

void viewWordsTest(PortugueseNode *dict)
{
    if (dict != NULL) {
        printf("Palavra em Português: %s\n", dict->portugueseWord);

        EnglishNode *englishNode = dict->englishWords;
        while (englishNode != NULL) {
            printf(" - Palavra em Inglês: %s, Unidade: %s\n", englishNode->englishWord, englishNode->unit);
            englishNode = englishNode->left;  
        }

        viewWordsTest(dict->left);
        viewWordsTest(dict->middle);
        if (dict->nInfos == 2)
            viewWordsTest(dict->right);
    }
}
