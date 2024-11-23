#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test()
{
    int wait;
    printf("Hello World");
    scanf("%d", &wait);
}

int isLeaf(PortugueseEnglish *node)
{
    return node->left == NULL;
}

PortugueseEnglish *createNode(const Info information, PortugueseEnglish *leftChild, PortugueseEnglish *centerChild)
{
    PortugueseEnglish *node = (PortugueseEnglish *)malloc(sizeof(PortugueseEnglish));

    if (!node)
    {
        printf("Failed to create node\n");
    }

    if (node)
    {
        node->info1 = information;
        node->left = leftChild;
        node->center = centerChild;
        node->infoCount = 1;
    }

    return node;
}

PortugueseEnglish *addKey(PortugueseEnglish *node, Info info, PortugueseEnglish *child)
{
    if (strcmp(info.portugueseWord, node->info1.portugueseWord) > 0)
    {
        node->info2 = info;
        node->right = child;
    }
    else
    {
        node->info2 = node->info1;
        node->right = node->center;
        node->info1 = info;
        node->center = child;
    }

    node->infoCount = 2;
    return node;
}

PortugueseEnglish *splitNode(PortugueseEnglish **node, const Info information, Info *promote, PortugueseEnglish **child)
{
    PortugueseEnglish *greater;

    if (strcmp(information.portugueseWord, (*node)->info2.portugueseWord) > 0)
    {
        *promote = (*node)->info2;
        if (child)
            greater = createNode(information, (*node)->right, *child);
        else
            greater = createNode(information, (*node)->right, NULL);
    }
    else if (strcmp(information.portugueseWord, (*node)->info1.portugueseWord) > 0)
    {
        *promote = information;
        if (child)
            greater = createNode((*node)->info2, *child, (*node)->right);
        else
            greater = createNode((*node)->info2, NULL, (*node)->right);
    }
    else
    {
        *promote = (*node)->info1;
        greater = createNode((*node)->info2, (*node)->center, (*node)->center);
        (*node)->info1 = information;
        (*node)->center = (child ? *child : NULL);
    }
    (*node)->infoCount = 1;

    return greater;
}

PortugueseEnglish *insertPortugueseWord(PortugueseEnglish **node, Info info, Info *promote, PortugueseEnglish **parent)
{
    PortugueseEnglish *greaterNode;
    Info promote1;
    greaterNode = NULL;

    if (*node == NULL)
    {
        *node = createNode(info, NULL, NULL);
    }
    else
    {
        if (isLeaf(*node))
        {
            if ((*node)->infoCount == 1)
            {
                *node = addKey(*node, info, NULL);
            }
            else
            {
                greaterNode = splitNode(node, info, promote, NULL);
                if (parent != NULL && *parent == NULL)
                {
                    *node = createNode(*promote, *node, greaterNode);
                    greaterNode = NULL;
                }
            }
        }
        else
        {
            if (strcmp(info.portugueseWord, (*node)->info1.portugueseWord) < 0)
            {
                greaterNode = insertPortugueseWord(&((*node)->left), info, promote, node);
            }
            else
            {
                if (((*node)->infoCount == 1) || (strcmp(info.portugueseWord, (*node)->info2.portugueseWord) < 0))
                {
                    greaterNode = insertPortugueseWord(&((*node)->center), info, promote, node);
                }
                else
                {
                    greaterNode = insertPortugueseWord(&((*node)->right), info, promote, node);
                }
            }
        }

        if (greaterNode)
        {
            if ((*node)->infoCount == 1)
            {
                *node = addKey(*node, *promote, greaterNode);
                greaterNode = NULL;
            }
            else
            {
                greaterNode = splitNode(node, *promote, &promote1, &greaterNode);
                if (*parent)
                {
                    *node = createNode(promote1, *node, greaterNode);
                    greaterNode = NULL;
                }
            }
        }
    }

    return greaterNode;
}

int LargestUnit(PortugueseEnglish *root)
{
    int bigger = 0;
    if (root != NULL)
    {
        if (root->infoCount != 2)
        {
            if (root->info1.unit > bigger)
                bigger = root->info1.unit;

            displayWords(root->left);
            displayWords(root->center);
        }
        else
        {
            if (root->info2.unit > bigger)
                bigger = root->info2.unit;

            displayWords(root->left);
            displayWords(root->center);
            displayWords(root->right);
        }
    }

    return (bigger);
}

void displayWords(PortugueseEnglish *root)
{
    if (root != NULL)
    {
        if (root->infoCount >= 1)
        {
            printf("\n\n%% Unidade %d:\n", root->info1.unit);
            printf("Inglês: %s | Português: %s\n", root->info1.englishTranslation->word, root->info1.portugueseWord);
        }

        if (root->infoCount == 2)
        {
            printf("\n\n%% Unidade %d:\n", root->info1.unit);
            printf("Inglês: %s | Português: %s\n", root->info2.englishTranslation->word, root->info2.portugueseWord);
        }

        displayWords(root->left);
        displayWords(root->center);

        if (root->infoCount == 2)
        {
            displayWords(root->right);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - informar uma unidade e então imprima todas as palavras da unidade em português seguida das
// equivalentes em inglês
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void printWordsByUnit(PortugueseEnglish *root, int unit)
{
    if (root != NULL)
    {
        if (root->infoCount >= 1 && root->info1.unit == unit)
        {
            printf("Português: %s | Inglês: %s\n",
                   root->info1.portugueseWord,
                   root->info1.englishTranslation->word);
        }
        if (root->infoCount == 2 && root->info2.unit == unit)
        {
            printf("Português: %s | Inglês: %s\n",
                   root->info2.portugueseWord,
                   root->info2.englishTranslation->word);
        }
        printWordsByUnit(root->left, unit);
        printWordsByUnit(root->center, unit);
        if (root->infoCount == 2)
        {
            printWordsByUnit(root->right, unit);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - informar uma palavra em português e então imprima todas as palavras em inglês equivalente à palavra em
// português dada, independente da unidade
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void findEnglishByPortuguese(PortugueseEnglish *root, const char *portugueseWord)
{
    if (root != NULL)
    {
        if (root->infoCount >= 1 && strcmp(root->info1.portugueseWord, portugueseWord) == 0)
            printf("Inglês: %s\n", root->info1.englishTranslation->word);
        if (root->infoCount == 2 && strcmp(root->info2.portugueseWord, portugueseWord) == 0)
            printf("Inglês: %s\n", root->info2.englishTranslation->word);

        findEnglishByPortuguese(root->left, portugueseWord);
        findEnglishByPortuguese(root->center, portugueseWord);
        if (root->infoCount == 2)
            findEnglishByPortuguese(root->right, portugueseWord);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III - informar uma palavra em inglês e a unidade a qual a mesma pertence removê-la das árvores binárias
// das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da
// árvore 2-3
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - informar uma palavra em português e a unidade a qual a mesma pertence e então removê-la, para isto
// deve remover a palavra em inglês da árvore binária correspondente a palavra em português da mesma
// unidade. Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida da
// árvore 2-3 */
// ---------------------------------------------------- XXXXXX -------------------------------------------------
