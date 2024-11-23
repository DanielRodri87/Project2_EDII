#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EnglishPortuguese* createEnglishTranslation(const char *word) {
    EnglishPortuguese *translation = (EnglishPortuguese *)malloc(sizeof(EnglishPortuguese));
    translation->word = strdup(word);
    translation->right = translation->left = NULL;
    return translation;
}

Info createInfo(const char *portugueseWord, const char *englishWord, int unit) {
    Info info;
    info.portugueseWord = strdup(portugueseWord);
    info.unit = unit;
    info.englishTranslation = createEnglishTranslation(englishWord);
    return info;
}

int main() {
    PortugueseEnglish *root = NULL;
    Info promote;

    // Insert words into the tree
    insertPortugueseWord(&root, createInfo("house", "casa", 1), &promote, NULL);
    insertPortugueseWord(&root, createInfo("car", "carro", 1), &promote, NULL);
    insertPortugueseWord(&root, createInfo("cat", "gato", 2), &promote, NULL);
    
    insertPortugueseWord(&root, createInfo("dog", "cachorro", 2), &promote, NULL);
    insertPortugueseWord(&root, createInfo("tree", "árvore", 3), &promote, NULL);
    insertPortugueseWord(&root, createInfo("computer", "computador", 3), &promote, NULL);
    
    insertPortugueseWord(&root, createInfo("book", "livro", 3), &promote, NULL);
    insertPortugueseWord(&root, createInfo("window", "janela", 4), &promote, NULL);
    insertPortugueseWord(&root, createInfo("door", "porta", 4), &promote, NULL);
    
    insertPortugueseWord(&root, createInfo("table", "mesa", 4), &promote, NULL);
    insertPortugueseWord(&root, createInfo("chair", "cadeira", 4), &promote, NULL);
    insertPortugueseWord(&root, createInfo("chair", "poltrona", 4), &promote, NULL);

    insertPortugueseWord(&root, createInfo("phone", "telefone", 5), &promote, NULL);
    
    insertPortugueseWord(&root, createInfo("keyboard", "teclado", 5), &promote, NULL);
    insertPortugueseWord(&root, createInfo("notebook", "caderno", 5), &promote, NULL);
    insertPortugueseWord(&root, createInfo("pen", "caneta", 5), &promote, NULL);


    for (int i = 1; i <= 5; i++)
    {
        printf("\n--------------------\n");
        printf("Palavras da Unidade %d\n", i);
        printWordsByUnit(root, i);
        printf("\n--------------------\n\n");
    }


    findEnglishByPortuguese(root, "cadeira");

    return 0;
}
