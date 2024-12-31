#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void toupperString(char *string)
{
    for (int i = 0; string[i] != '\0'; i++)
        string[i] = toupper(string[i]);
}

Tree23 *createNode(Info info, Tree23 *leftChild, Tree23 *centerChild)
{
    Tree23 *node = (Tree23 *)malloc(sizeof(Tree23));

    if (!node)
        printf("Nao foi possivel criar no\n");

    if (node)
    {
        node->info1 = info;
        node->left = leftChild;
        node->center = centerChild;
        node->infoCount = 1;
    }

    return (node);
}

void addKey(Tree23 *node, const Info info, Tree23 *child)
{
    if (strcmp(info.word, node->info1.word) > 0)
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
}
Tree23 *breakNode(Tree23 **node, const Info info, Info *promote, Tree23 **child)
{
    Tree23 *bigger;

    if (strcmp(info.word, (*node)->info2.word) > 0)
    {
        *promote = (*node)->info2;
        if (child)
            bigger = criaNo(info, (*node)->right, *child);
        else
            bigger = criaNo(info, (*node)->right, NULL);
    }
    else if (strcmp(info.word, (*node)->info1.word) > 0)
    {
        *promote = info;
        if (child)
            bigger = criaNo((*node)->info2, *child, (*node)->right);
        else
            bigger = criaNo((*node)->info2, NULL, (*node)->right);
    }
    else
    {
        *promote = (*node)->info1;
        bigger = criaNo((*node)->info2, (*node)->center, (*node)->center);
        (*node)->info1 = info;
        (*node)->center = (child ? *child : NULL);
    }
    (*node)->infoCount = 1;

    return (bigger);
}

int isLeaf(const Tree23 *node)
{
    return (node->left == NULL);
}

Tree23 *insertTree23(Tree23 **node, Info info, Info *promote, Tree23 **father)
{
    Info promote1;
    Tree23 *biggerNode = NULL;

    if (*node == NULL)
        *node = createNode(info, NULL, NULL);
    else
    {
        if (isLeaf(*node))
        {
            if ((*node)->infoCount == 1)
                addKey(*node, info, NULL);
            else
            {
                biggerNode = breakNode(node, info, promote, NULL);
                if (father && !(*father))
                {
                    *node = criaNo(*promote, *node, biggerNode);
                    biggerNode = NULL;
                }
            }
        }
        else
        {
            if (strcmp(info.word, (*node)->info1.word) < 0)
            {
                biggerNode = insertTree23(&((*node)->left), info, promote, node);
            }
            else if (((*node)->infoCount == 1) || (strcmp(info.word, (*node)->info2.word) < 0))
            {
                biggerNode = insertTree23(&((*node)->center), info, promote, node);
            }
            else
            {
                biggerNode = insertTree23(&((*node)->right), info, promote, node);
            }

            if (biggerNode)
            {
                if ((*node)->infoCount == 1)
                {
                    addKey(*node, *promote, biggerNode);
                    biggerNode = NULL;
                }
                else
                {
                    biggerNode = breakNode(node, *promote, &promote1, &biggerNode);
                    if (father && !(*father))
                    {
                        *node = createNode(promote1, *node, biggerNode);
                        biggerNode = NULL;
                    }
                }
            }
        }
    }

    return (biggerNode);
}
void freeTree(Tree23 *node)
{
    if (node != NULL)
    {
        freeTree(node->left);
        freeTree(node->center);
        freeTree(node->right);
        free(node);
    }
}

void insertUnit(Units **units, int unit)
{
    if (*units == NULL)
    {
        *units = (Units *)malloc(sizeof(Units));
        (*units)->unit = unit;
        (*units)->next = NULL;
    }
    else
        insertUnit(&(*units)->next, unit);
}

int insertBinaryTree(EngPT **root, const InfoBin *info)
{
    int sucess = 0;
    if ((*root) == NULL)
    {
        EngPT *new = (EngPT *)malloc(sizeof(EngPT));
        new->info->word = (char *)malloc(strlen(info->word) + 1);
        strcpy(new->info->word, info->word);
        toupperString(new->info->word);
        new->left = NULL;
        new->right = NULL;
        *root = new;
        sucess = 1;
    }
    else
    {
        if (strcmp((*root)->info->word, info->word) == 0)
            insertUnit(&(*root)->info->units, info->units);
        if (strcmp(info->word, (*root)->info->word) < 0)
            sucess = insertBinaryTree(&(*root)->left, info->word);
        else
            sucess = insertBinaryTree(&(*root)->right, info->word);
    }
}
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - informar uma unidade e então imprima todas as palavras da unidade em português seguida das
// equivalentes em inglês
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void displayBinary(const EngPT *root)
{
    if (root != NULL)
    {
        displayBinary(root->left);
        printf("%s, ", root->info->word);
        displayBinary(root->right);
    }
}

int travelList(const Units *unit, int findUnit)
{
    int find = 0;
    if (unit != NULL)
    {
        if (unit == findUnit)
            find = 0;
        else
            find = travelList(unit->next, findUnit);
    }

    return (find);
}

void displayWordForUnit(Tree23 *root, int unit)
{
    if (root != NULL)
    {
        displayWordForUnit(root->left, unit);
        if (percorreLista(root->info1.versionEnglish->info->word, unit))
        {
            printf("%s: ", root->info1.word);
            exibirEN(root->info1.versionEnglish);
            printf("\n");
        }

        displayWordForUnit(root->center, unit);

        if (root->infoCount == 2)
        {
            if (percorreLista(root->info2.versionEnglish->info->units, unit))
            {
                printf("%s: ", root->info2.word);
                exibirEN(root->info2.versionEnglish);
                printf("\n");
            }
            displayWordForUnit(root->right, unit);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - informar uma palavra em português e então imprima todas as palavras em inglês equivalente à palavra em
// português dada, independente da unidade
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void displayWordsEnglishFindPortuguese(Tree23 *root, char *portugueseWord)
{
    if (root != NULL)
    {
        displayWordsEnglishFindPortuguese(root->left, portugueseWord);
        if (strcmp(portugueseWord, root->info1.word) == 0)
        {
            printf("Correspondentes de %s:\n", portugueseWord);
            exibirEN(root->info1.versionEnglish);
        }

        displayWordsEnglishFindPortuguese(root->center, portugueseWord);

        if (root->infoCount == 2)
        {
            if (strcmp(portugueseWord, root->info2.word) == 0)
            {
                printf("Correspondentes de %s:\n", portugueseWord);
                exibirEN(root->info2.versionEnglish);
            }
            displayWordsEnglishFindPortuguese(root->right, portugueseWord);
        }
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III - informar uma palavra em inglês e a unidade a qual a mesma pertence removê-la das árvores binárias
// das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da
// árvore 2-3
// ---------------------------------------------------- XXXXXX -------------------------------------------------
static int waveMoviment(Info leaving, Info *input, Tree23 *father, Tree23 **origin, Tree23 **root, Tree23 **bigger, char (*removeFunction)(Tree23 **, char, Tree23 *, Tree23 **, Tree23 **))
{
    int removed = removeFunction(root, leaving.word, father, origin, bigger);
    *input = leaving;

    return (removed);
}

int possibleRemove(Tree23 *root)
{
    int possible = 0;

    if (root != NULL)
    {
        possible = root->infoCount == 2;

        if (!possible)
        {
            possible = possibleRemove(root->center);

            if (!possible)
                possible = possibleRemove(root->left);
        }
    }

    return (possible);
}

static Tree23 *lookMinorChild(Tree23 *root, Tree23 **father)
{
    Tree23 *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        child = child->left;
    }

    return (child);
}

Info biggerChild(Tree23 *root)
{
    return (root->infoCount == 2 ? root->info2 : root->info1);
}

Tree23 *sourcebiggerChild(Tree23 *root, Tree23 **father, Info *biggerValue)
{
    Tree23 *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        if (child->infoCount == 1)
            child = child->center;
        else
            child = child->right;
    }

    if (child != NULL)
        *biggerValue = biggerChild(child);

    return (child);
}

void deallocateNode(Tree23 **node)
{
    free(*node);
    *node = NULL;
}

Tree23 *togetherNode(Tree23 *child1, Tree23 *child2, Tree23 **child3)
{
    Tree23 *bigger = NULL;
    if (!isLeaf(child2))
    {
        bigger = togetherNode(child2->left, child2->center, &(child1->right));

        const Info aux = child2->info1;
        child2->info1 = bigger->info1;
        bigger->info1 = aux;
    }

    child1->info2 = child2->info1;
    bigger = child1;
    *child3 = bigger;
    deallocateNode(&child2);

    return (bigger);
}

int isInfo1(const Tree23 node, const char *word)
{
    int aux = 0;
    if (node.infoCount == 1 && word && node.info1.word)
        aux = strcmp(node.info1.word, word) == 0;

    return (aux);
}

int isInfo2(const Tree23 node, const char *word)
{
    int aux = 0;
    if (node.infoCount == 2 && word && node.info2.word)
        aux = strcmp(node.info2.word, word) == 0;

    return (aux);
}

Tree23 *sourceFather(Tree23 *root, char *word)
{
    Tree23 *father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, word) && !isInfo2(*root, word))
        {
            if (strcmp(word, root->info1.word) < 0)
                father = sourceFather(root->left, word);
            else if (root->infoCount == 1 || strcmp(word, root->info1.word) < 0)
                father = sourceFather(root->center, word);
            else
                father = sourceFather(root->right, word);

            if (!father)
                father = root;
        }
    }
    return (father);
}

Tree23 *sourceBiggerFather(Tree23 *root, char *word)
{
    Tree23 *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, word) && !isInfo2(*root, word))
        {
            if (strcmp(word, root->info1.word) < 0)
                father = sourceBiggerFather(root->left, word);
            else if (root->infoCount == 1 || strcmp(word, root->info2.word))
                father = sourceBiggerFather(root->center, word);
            else
                father = sourceBiggerFather(root->right, word);

            if (father == NULL && ((root->infoCount == 1 && strcmp(root->info1.word, word) > 0 || (root->infoCount == 2 && strcmp(root->info2.word, word) > 0))))
                father = root;
        }
    }

    return (father);
}

Tree23 *sourceBiggerFather2Infos(Tree23 *root, char *word)
{
    Tree23 *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, word) && !isInfo2(*root, word))
        {
            if (strcmp(word, root->info1.word))
                father = sourceBiggerFather2Infos(root->left, word);
            else if (root->infoCount == 1 || strcmp(word, root->info2.word) < 0)
                father = sourceBiggerFather2Infos(root->center, word);
            else
                father = sourceBiggerFather2Infos(root->right, word);

            if (father == NULL && root->infoCount == 2 && strcmp(root->info1.word, word) > 0)
                father = root;
        }
    }
    return (father);
}

Tree23 *sourceMinorFather(Tree23 *root, char word)
{
    Tree23 *father;
    father = NULL;

    if (root != NULL)
    {
        if (!ehInfo1(*root, word) && !ehInfo2(*root, word))
        {
            if (strcmp(word, root->info1.word) < 0)
                father = sourceMinorFather(root->left, word);
            else if (root->infoCount == 1 || strcmp(word, root->info2.word))
                father = sourceMinorFather(root->center, word);
            else
                father = sourceMinorFather(root->right, word);

            if (father == NULL && strcmp(root->info1.word, word) < 0)
                father = root;
        }
    }
    return father;
}

Tree23 *sourceMinorFather2Infos(Tree23 *root, char *palavra)
{
    Tree23 *father;
    father = NULL;

    if (root != NULL)
    {
        if (!ehInfo1(*root, palavra) && !ehInfo2(*root, palavra))
        {
            if (strcmp(palavra, root->info1.word) < 0)
                father = buscarMenorfather(root->left, palavra);
            else if (root->infoCount == 1 || strcmp(palavra, root->info2.word))
                father = buscarMenorfather(root->center, palavra);
            else
                father = buscarMenorfather(root->right, palavra);

            if (father == NULL && root->infoCount == 2 && strcmp(root->info2.word, palavra) < 0)
                father = root;
        }
    }
    return (father);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - informar uma palavra em português e a unidade a qual a mesma pertence e então removê-la, para isto
// deve remover a word em inglês da árvore binária correspondente a palavra em português da mesma
// unidade. Caso ela seja a única word na árvore binária, a word em português deve ser removida da
// árvore 2-3 */
// ---------------------------------------------------- XXXXXX -------------------------------------------------