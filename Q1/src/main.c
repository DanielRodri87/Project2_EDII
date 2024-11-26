#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EnglishPortuguese *createEnglishTranslation(const char *word)
{
    EnglishPortuguese *translation = (EnglishPortuguese *)malloc(sizeof(EnglishPortuguese));
    translation->word = strdup(word);
    translation->right = translation->left = NULL;
    return translation;
}

Info createInfo(const char *portugueseWord, const char *englishWord, int unit)
{
    Info info;
    info.portugueseWord = strdup(portugueseWord);
    info.unit = unit;
    info.englishTranslation = createEnglishTranslation(englishWord);
    return info;
}

int main()
{
    PortugueseEnglish *root = NULL;
    Info promote;

    insertPortugueseWord(&root, createInfo("casa", "house", 1), &promote, NULL);
    insertPortugueseWord(&root, createInfo("carro", "car", 1), &promote, NULL);
    insertPortugueseWord(&root, createInfo("gato", "cat", 2), &promote, NULL);

    insertPortugueseWord(&root, createInfo("cachorro", "dog", 2), &promote, NULL);
    insertPortugueseWord(&root, createInfo("árvore", "tree", 3), &promote, NULL);
    insertPortugueseWord(&root, createInfo("computador", "computer", 3), &promote, NULL);

    insertPortugueseWord(&root, createInfo("livro", "book", 3), &promote, NULL);
    insertPortugueseWord(&root, createInfo("janela", "window", 4), &promote, NULL);
    insertPortugueseWord(&root, createInfo("porta", "door", 4), &promote, NULL);

    insertPortugueseWord(&root, createInfo("mesa", "table", 4), &promote, NULL);
    insertPortugueseWord(&root, createInfo("cadeira", "chair", 4), &promote, NULL);
    insertPortugueseWord(&root, createInfo("poltrona", "chair", 4), &promote, NULL);

    insertPortugueseWord(&root, createInfo("telefone", "phone", 5), &promote, NULL);

    insertPortugueseWord(&root, createInfo("teclado", "keyboard", 5), &promote, NULL);
    insertPortugueseWord(&root, createInfo("caderno", "notebook", 5), &promote, NULL);
    insertPortugueseWord(&root, createInfo("caneta", "pen", 5), &promote, NULL);

    // displayWords(root);
    int maior = LargestUnit(root);

    for (int i = 1; i <= maior; i++)
    {
        printf("%% Unidade %d\n", i);
        printWordsByUnit(root, i);
        printf("\n");
    }

    return 0;
}
