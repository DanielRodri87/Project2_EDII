#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #######################################################################################
// Árvore 2-3
// #######################################################################################
TranslationInfo createInfo(char *palavra, char *englishWord, char *unit)
{
    TranslationInfo info;

    info.portugueseWord = malloc(strlen(palavra) + 1);
    strcpy(info.portugueseWord, palavra);

    info.englishWord = malloc(sizeof(EnglishBinTree));
    info.englishWord->englishWord = malloc(strlen(englishWord) + 1);
    strcpy(info.englishWord->englishWord, englishWord);

    info.englishWord->units = NULL;
    InsertLinkedList(&(info.englishWord->units), unit);

    return (info);
}

int insertPortugueseWord(Portuguese23Tree **tree, char *portugueseWord, char *englishWord, char *unit)
{
    TranslationInfo promote;
    Portuguese23Tree *father = NULL;
    int insered;

    TranslationInfo novoInfo = createInfo(portugueseWord, englishWord, unit);
    if (InsertTree23(tree, &novoInfo, &promote, &father))
        insered = 1;

    return (insered);
}

Portuguese23Tree *createNode(const TranslationInfo *info, Portuguese23Tree *leftChild, Portuguese23Tree *centerChild)
{
    Portuguese23Tree *no;
    no = (Portuguese23Tree *)malloc(sizeof(Portuguese23Tree)); 

    no->info1 = *info;
    no->left = leftChild;
    no->middle = centerChild;
    no->infoCount = 1;

    no->info2.englishWord = NULL;
    no->info2.portugueseWord = NULL;
    no->right = NULL;

    return (no);
}

void addKey(Portuguese23Tree **no, const TranslationInfo *info, Portuguese23Tree *child)
{
    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) > 0)
    {
        (*no)->info2 = *info;
        (*no)->right = child; 
    }
    else
    {
        (*no)->info2 = (*no)->info1;
        (*no)->info1 = *info;
        (*no)->right = (*no)->middle;
        (*no)->middle = child;
    }
    (*no)->infoCount = 2; 
}

Portuguese23Tree *breakNode(Portuguese23Tree **no, const TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree *child)
{
    Portuguese23Tree *bigger;

    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
    {
        *promote = (*no)->info1;
        bigger = createNode(&(*no)->info2, (*no)->middle, (*no)->right);

        (*no)->info1 = *info;
        (*no)->middle = child;
    }
    else if (strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
    {
        *promote = *info;
        bigger = createNode(&(*no)->info2, child, (*no)->right);
    }
    else
    {
        *promote = (*no)->info2;
        bigger = createNode(info, (*no)->right, child);
    }

    (*no)->info2.englishWord = NULL;
    (*no)->info2.portugueseWord = NULL;

    (*no)->infoCount = 1;
    (*no)->right = NULL; 

    return (bigger);
}

int isLeaf(const Portuguese23Tree *no)
{
    int troughet = 0;

    if (no->left == NULL)
    {
        troughet = 1; 
    }

    return (troughet);
}

Portuguese23Tree *InsertTree23(Portuguese23Tree **no, TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree **father)
{
    TranslationInfo promote1;
    Portuguese23Tree *biggerNode = NULL;

    if (*no == NULL)
        *no = createNode(info, NULL, NULL);
    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) == 0)
        insertenglishWord(&(*no)->info1.englishWord, info);
    else if ((*no)->infoCount == 2 && strcmp(info->portugueseWord, (*no)->info2.portugueseWord) == 0)
        insertenglishWord(&(*no)->info2.englishWord, info);
    else
    {
        if (isLeaf(*no))
        { 
            if ((*no)->infoCount == 1)
                addKey(no, info, NULL);
            else
            {
                Portuguese23Tree *novo;
                novo = breakNode(no, info, promote, NULL); 
                if (*father == NULL)
                {
                    Portuguese23Tree *newroot;
                    newroot = createNode(promote, *no, novo); 
                    *no = newroot;
                }
                else
                    biggerNode = novo; 
            }
        }
        else
        { 
            if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
                biggerNode = InsertTree23(&((*no)->left), info, promote, no); 
            else if ((*no)->infoCount == 1 || strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
                biggerNode = InsertTree23(&((*no)->middle), info, promote, no); 
            else
                biggerNode = InsertTree23(&((*no)->right), info, promote, no); 

            if (biggerNode != NULL)
            {
                if ((*no)->infoCount == 1)
                {
                    addKey(no, promote, biggerNode);
                    biggerNode = NULL;
                }
                else
                { 
                    Portuguese23Tree *novo;
                    novo = breakNode(no, promote, &promote1, biggerNode); 
                    if (*father == NULL)
                    {
                        Portuguese23Tree *newroot;
                        newroot = createNode(&promote1, *no, novo); 
                        *no = newroot;
                        biggerNode = NULL;
                    }
                    else
                    {
                        biggerNode = novo;
                        *promote = promote1;
                    }
                }
            }
        }
    }

    return (biggerNode);
}


void displayTree23(const Portuguese23Tree *root)
{
    if (root != NULL)
    {
        displayTree23(root->left);
        printf("PT: %s\n", root->info1.portugueseWord);
        printBinaryTree(root->info1.englishWord);
        printf("\n");
        displayTree23(root->middle);
        if (root->infoCount == 2)
        {
            printf("PT: %s\n", root->info2.portugueseWord);
            printBinaryTree(root->info2.englishWord);
            printf("\n");
            displayTree23(root->right);
        }
    }
}

void displayWordsForUnit(Portuguese23Tree *tree, char *unit)
{
    if (tree)
    {
        displayWordsForUnit(tree->left, unit);
        displayTranslacions(tree->info1.englishWord, unit, tree->info1.portugueseWord);

        displayWordsForUnit(tree->middle, unit);
        if (tree->infoCount == 2)
            displayTranslacions(tree->info2.englishWord, unit, tree->info2.portugueseWord);

        displayWordsForUnit(tree->right, unit);
    }
}

void displayTranslacions(EnglishBinTree *node, char *unit, const char *portugueseWord)
{
    if (node)
    {
        if (sourceLinkedList(node->units, unit))
        {
            printf("PT: %s\n", portugueseWord);
            printf("ENG: %s\n", node->englishWord);
        }
        displayTranslacions(node->left, unit, portugueseWord);
        displayTranslacions(node->right, unit, portugueseWord);
    }
}

Portuguese23Tree *sourceWords(Portuguese23Tree **no, const char *portugueseWord)
{
    Portuguese23Tree *insered = NULL; 

    if (no != NULL)
    {
        if (strcmp(portugueseWord, (*no)->info1.portugueseWord) == 0)
        {
            insered = (*no);
        }
        else if ((*no)->infoCount == 2 && strcmp(portugueseWord, (*no)->info2.portugueseWord) == 0)
        {
            insered = (*no);
        }
        else
        {
            if (strcmp(portugueseWord, (*no)->info1.portugueseWord) < 0)
                insered = sourceWords(&(*no)->left, portugueseWord);
            else if ((*no)->infoCount == 1 || strcmp(portugueseWord, (*no)->info2.portugueseWord) < 0)
                insered = sourceWords(&(*no)->middle, portugueseWord);
            else
                insered = sourceWords(&(*no)->right, portugueseWord);
        }
    }

    return (insered);
}

void displayTranslationPortuguese(Portuguese23Tree **root, const char *portugueseWord)
{
    Portuguese23Tree *resultado = NULL;
    if (root != NULL)
    {
        resultado = sourceWords(root, portugueseWord);
        if (resultado)
        {
            printf("Traducoes da palavra: '%s':\n", portugueseWord);

            if (strcmp(portugueseWord, resultado->info1.portugueseWord) == 0)
                printBinaryTree(resultado->info1.englishWord);
            else
                printBinaryTree(resultado->info2.englishWord);
        }
        else
            printf("A palavra '%s' nao foi encontrada na árvore.\n", portugueseWord);
    }
}

void deallocateNode(Portuguese23Tree **no)
{
    free(*no);
    *no = NULL;
}

static TranslationInfo biggerInfoNode23(Portuguese23Tree *root)
{
    return (root->infoCount == 2 ? root->info2 : root->info1);
}

static int isInfo1(Portuguese23Tree no, char *info)
{
    return (strcmp(info, no.info1.portugueseWord) == 0);
}

static int isInfo2(Portuguese23Tree no, char *info)
{
    return (no.infoCount == 2 && strcmp(info, no.info2.portugueseWord) == 0);
}

static Portuguese23Tree *sourceBiggerChild(Portuguese23Tree *root, Portuguese23Tree **father, TranslationInfo *bigger_valor)
{
    Portuguese23Tree *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        if (child->infoCount == 1)
            child = child->middle;
        else
            child = child->right;
    }

    if (child != NULL)
        *bigger_valor = biggerInfoNode23(child);

    return (child);
}

static Portuguese23Tree *sourceMinorChild(Portuguese23Tree *root, Portuguese23Tree **father)
{
    Portuguese23Tree *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        child = child->left;
    }

    return (child);
}

static int waveMoviment(TranslationInfo leaving, TranslationInfo *input, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **root, Portuguese23Tree **bigger, int (*functionRemove)(Portuguese23Tree **, char *, Portuguese23Tree *, Portuguese23Tree **, Portuguese23Tree **))
{
    int removed = functionRemove(root, leaving.portugueseWord, father, origin, bigger);
    *input = leaving;

    return (removed);
}

static Portuguese23Tree *sourceFather(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourceFather(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = sourceFather(root->middle, info);
            else
                father = sourceFather(root->right, info);

            if (father == NULL)
                father = root;
        }
    }

    return (father);
}

static Portuguese23Tree *sourceBiggerFather(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourceBiggerFather(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = sourceBiggerFather(root->middle, info);
            else
                father = sourceBiggerFather(root->right, info);

            if (father == NULL &&
                ((root->infoCount == 1 && strcmp(root->info1.portugueseWord, info) > 0) ||
                 (root->infoCount == 2 && strcmp(root->info2.portugueseWord, info) > 0)))
                father = root;
        }
    }

    return father;
}

static Portuguese23Tree *sourceMinorFather2Infos(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourceMinorFather2Infos(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = sourceMinorFather2Infos(root->middle, info);
            else
                father = sourceMinorFather2Infos(root->right, info);

            if (father == NULL && root->infoCount == 2 && strcmp(root->info2.portugueseWord, info) < 0)
                father = root;
        }
    }

    return father;
}

static Portuguese23Tree *sourcerMinorFather(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourcerMinorFather(root->left, info);
            else if (root->infoCount == 1 || strcmp(info, root->info2.portugueseWord) < 0)
                father = sourcerMinorFather(root->middle, info);
            else
                father = sourcerMinorFather(root->right, info);

            if (father == NULL && strcmp(root->info1.portugueseWord, info) < 0)
                father = root;
        }
    }

    return father;
}

int removeNoLeaf1(Portuguese23Tree **origin, Portuguese23Tree *root, TranslationInfo *info, Portuguese23Tree *child1, Portuguese23Tree *child2, Portuguese23Tree **bigger)
{
    int removed;
    Portuguese23Tree *child, *father;
    TranslationInfo info_child;

    father = root;

    child = sourceBiggerChild(child1, &father, &info_child);

    if (child->infoCount == 2)
    {
        *info = info_child;
        child->infoCount = 1;
        removed = 1;
    }
    else
    {
        child = sourceMinorChild(child2, &father);
        removed = waveMoviment(child->info1, info, father, origin, &child, bigger, tree23Remove1);
    }

    return removed;
}

int removeNoLeaf2(Portuguese23Tree **origin, Portuguese23Tree *root, TranslationInfo *info, Portuguese23Tree *child1, Portuguese23Tree *child2, Portuguese23Tree **bigger)
{
    int removed;
    Portuguese23Tree *child, *father;
    TranslationInfo info_child;

    father = root;

    child = sourceMinorChild(child1, &father);

    if (child->infoCount == 2)
    {
        *info = child->info1;
        child->info1 = child->info2;
        child->infoCount = 1;
        removed = 1;
    }
    else
    {
        child = sourceBiggerChild(child2, &father, &info_child);
        removed = waveMoviment(info_child, info, father, origin, &child, bigger, tree23Remove2);
    }

    return (removed);
}

int tree23Remove1(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **bigger)
{
    int removed = 0;

    if (*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if (info1 || info2)
        {
            removed = 1;
            if (isLeaf(*root))
            {
                if ((*root)->infoCount == 2)
                {
                    if (info1)
                        (*root)->info1 = (*root)->info2;
                    (*root)->infoCount = 1;
                }
                else
                {
                    if (father == NULL)
                        deallocateNode(root);
                    else
                    {
                        Portuguese23Tree *father_aux;
                        TranslationInfo info_father;
                        if (*root == father->left || (father->infoCount == 2 && *root == father->middle))
                        {
                            father_aux = sourceFather(*origin, father->info1.portugueseWord);

                            if (*root == father->left)
                                info_father = father->info1;
                            else
                                info_father = father->info2;

                            removed = waveMoviment(info_father, &((*root)->info1), father_aux, origin, &father, bigger, tree23Remove1);
                        }
                        else
                        {
                            father_aux = sourceBiggerFather(*origin, (*root)->info1.portugueseWord);

                            Portuguese23Tree *menor_father;
                            menor_father = sourceMinorFather2Infos(*origin, (*root)->info1.portugueseWord);

                            if (father_aux == NULL || (father_aux != father && menor_father != NULL))
                            {
                                *bigger = father;
                                (*root)->infoCount = 0;
                                removed = -1;
                            }
                            else
                            {
                                if (strcmp(father_aux->info1.portugueseWord, (*root)->info1.portugueseWord) > 0)
                                    info_father = father_aux->info1;
                                else
                                    info_father = father_aux->info2;

                                Portuguese23Tree *avo;
                                avo = sourceFather(*origin, info_father.portugueseWord);
                                removed = waveMoviment(info_father, &((*root)->info1), avo, origin, &father_aux, bigger, tree23Remove1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removed = removeNoLeaf1(origin, *root, &((*root)->info2), (*root)->middle, (*root)->right, bigger);
            else if (info1)
                removed = removeNoLeaf1(origin, *root, &((*root)->info1), (*root)->left, (*root)->middle, bigger);
        }
        else
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                removed = tree23Remove1(&(*root)->left, info, *root, origin, bigger);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
                removed = tree23Remove1(&(*root)->middle, info, *root, origin, bigger);
            else
                removed = tree23Remove1(&(*root)->right, info, *root, origin, bigger);
        }
    }
    return (removed);
}

int tree23Remove2(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **bigger)
{
    int removed = 0;

    if (*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if (info1 || info2)
        {
            removed = 1;
            if (isLeaf(*root))
            {
                if ((*root)->infoCount == 2)
                {
                    if (info1)
                        (*root)->info1 = (*root)->info2;

                    (*root)->infoCount = 1;
                }
                else
                {
                    if (father == NULL)
                        deallocateNode(root);
                    else
                    {
                        Portuguese23Tree *father_aux;
                        TranslationInfo info_father;
                        if (*root == father->middle || (father->infoCount == 2 && *root == father->right))
                        {
                            father_aux = sourceFather(*origin, father->info1.portugueseWord);

                            if (*root == father->middle)
                                info_father = father->info1;
                            else
                                info_father = father->info2;

                            removed = waveMoviment(info_father, &((*root)->info1), father_aux, origin, &father, bigger, tree23Remove2);
                        }
                        else
                        {
                            father_aux = sourcerMinorFather(*origin, (*root)->info1.portugueseWord);

                            Portuguese23Tree *menor_father;
                            menor_father = sourceMinorFather2Infos(*origin, (*root)->info1.portugueseWord);

                            Portuguese23Tree *avo;
                            if (father_aux == NULL || (father_aux != father && menor_father != NULL))
                            {
                                removed = -1;
                                *bigger = father;
                            }
                            else
                            {
                                if (father_aux->infoCount == 2 && father_aux->info2.portugueseWord < (*root)->info1.portugueseWord)
                                    info_father = father_aux->info2;
                                else
                                    info_father = father_aux->info1;

                                avo = sourceFather(*origin, info_father.portugueseWord);
                                removed = waveMoviment(info_father, &((*root)->info1), avo, origin, &father_aux, bigger, tree23Remove2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removed = removeNoLeaf2(origin, *root, &((*root)->info2), (*root)->right, (*root)->middle, bigger);
            else if (info1)
                removed = removeNoLeaf2(origin, *root, &((*root)->info1), (*root)->middle, (*root)->left, bigger);
        }
        else
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                removed = tree23Remove2(&(*root)->left, info, *root, origin, bigger);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
                removed = tree23Remove2(&(*root)->middle, info, *root, origin, bigger);
            else
                removed = tree23Remove2(&(*root)->right, info, *root, origin, bigger);
        }
    }
    return (removed);
}

int possibleRemove(Portuguese23Tree *root)
{
    int possible = 0;

    if (root != NULL)
    {
        possible = root->infoCount == 2;

        if (!possible)
        {
            possible = possibleRemove(root->middle);

            if (!possible)
                possible = possibleRemove(root->left);
        }
    }

    return (possible);
}

Portuguese23Tree *togetherNode(Portuguese23Tree *child1, TranslationInfo info, Portuguese23Tree *bigger, Portuguese23Tree **root)
{
    addKey(&child1, &info, bigger);

    (*root)->infoCount--;

    if ((*root)->infoCount == 0)
        deallocateNode(root);

    return (child1);
}

static int balance(Portuguese23Tree **root, Portuguese23Tree *child1, Portuguese23Tree **child2, TranslationInfo info, Portuguese23Tree **bigger)
{
    int balanced = 0;
    if (*child2 == NULL || (*child2)->infoCount == 0)
    {
        if (*child2 != NULL)
            deallocateNode(child2);

        *bigger = togetherNode(child1, info, *bigger, root);
        balanced = 1;
    }
    return (balanced);
}

int rebalance(Portuguese23Tree **root, char *info, Portuguese23Tree **bigger)
{
    int balanced = 0;
    if (*root != NULL)
    {
        if (!isLeaf(*root))
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                balanced = rebalance(&((*root)->left), info, bigger);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
            {
                if ((*root)->left->infoCount == 2 && !possibleRemove((*root)->middle))
                    balanced = -1;
                else
                    balanced = rebalance(&((*root)->middle), info, bigger);
            }
            else
            {
                if ((*root)->middle->infoCount == 2 && !possibleRemove((*root)->right))
                    balanced = -1;
                else
                    balanced = rebalance(&((*root)->right), info, bigger);
            }

            if (balanced != -1)
            {
                if ((*root)->infoCount == 1)
                    balanced = balance(root, (*root)->left, &((*root)->middle), (*root)->info1, bigger);
                else if ((*root)->infoCount == 2)
                    balanced = balance(root, (*root)->middle, &((*root)->right), (*root)->info2, bigger);
                balanced = 1;
            }
        }
    }

    return (balanced);
}

int removeTree23(Portuguese23Tree **root, char *info)
{
    Portuguese23Tree *bigger, *posicao_juncao;
    int removed = tree23Remove1(root, info, NULL, root, &posicao_juncao);

    if (removed == -1)
    {
        TranslationInfo valor_juncao = biggerInfoNode23(posicao_juncao);
        bigger = NULL;
        removed = rebalance(root, valor_juncao.portugueseWord, &bigger);

        if (removed == -1)
        {
            Portuguese23Tree *father, *posicao_juncao2;
            TranslationInfo *input;
            father = sourceFather(*root, valor_juncao.portugueseWord);

            if (isInfo1(*posicao_juncao, valor_juncao.portugueseWord))
                input = &(posicao_juncao->middle->info1);
            else
                input = &(posicao_juncao->right->info1);

            removed = waveMoviment(valor_juncao, input, father, root, &posicao_juncao, &posicao_juncao2, tree23Remove2);

            if (removed == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                father = sourceFather(*root, valor_juncao.portugueseWord);
                removed = waveMoviment(valor_juncao, &(posicao_juncao2->left->info1), father, root, &posicao_juncao2, &posicao_juncao, tree23Remove1);

                valor_juncao = biggerInfoNode23(posicao_juncao);
                bigger = NULL;
                removed = rebalance(root, valor_juncao.portugueseWord, &bigger);
            }
        }

        if (*root == NULL)
            *root = bigger;
    }

    return (removed);
}


void freeInfo2_3(TranslationInfo *info)
{
    freeBinaryTree(info->englishWord);
    free(info->portugueseWord);
}

void freeTree(Portuguese23Tree *no)
{
    if (no != NULL)
    {
        freeTree(no->left);
        freeInfo2_3(&no->info1);
        freeTree(no->middle);
        if (no->infoCount == 2)
        {
            freeInfo2_3(&no->info2);
            freeTree(no->right);
        }
        free(no);
    }
}

// #######################################################################################
// BINÁRIA
// #######################################################################################

EnglishBinTree *createNodeBin(const char *englishWord, char *unit)
{
    EnglishBinTree *novoNo = (EnglishBinTree *)malloc(sizeof(EnglishBinTree));
    if (novoNo != NULL)
    {
        novoNo->englishWord = (char *)malloc(strlen(englishWord) + 1);
        strcpy(novoNo->englishWord, englishWord);
        novoNo->units = NULL;
        InsertLinkedList(&(novoNo->units), unit);
        novoNo->left = novoNo->right = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em ingles na tree binaria de busca
int insertenglishWord(EnglishBinTree **root, TranslationInfo *info)
{
    int result = 0;
    if ((*root) == NULL)
    {
        EnglishBinTree *new = createNodeBin(info->englishWord->englishWord, info->englishWord->units->unitName);
        *root = new;
        result = 1;
    }
    else if (strcmp(info->englishWord->englishWord, (*root)->englishWord) < 0)
        result = insertenglishWord(&(*root)->left, info);
    else if (strcmp(info->englishWord->englishWord, (*root)->englishWord) > 0)
        result = insertenglishWord(&(*root)->right, info);
    else
        result = InsertLinkedList(&((*root)->units), info->englishWord->units->unitName);

    return result;
}

void printBinaryTree(EnglishBinTree *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->left); 
        printf("ENG: %s \n", root->englishWord);
        showLinkedList(root->units);
        printBinaryTree(root->right); 
    }
}

int isLeafBin(EnglishBinTree *root)
{
    return (root->left == NULL && root->right == NULL);
}

EnglishBinTree *oneChild(EnglishBinTree **root)
{
    EnglishBinTree *aux;
    aux = NULL;

    if ((*root)->right == NULL)
        aux = (*root)->left;
    else if ((*root)->left == NULL)
        aux = (*root)->right;

    return (aux);
}

EnglishBinTree *minorChild(EnglishBinTree *root)
{
    EnglishBinTree *aux;
    aux = root;

    if (root)
    {
        if (root->left)
            aux = minorChild(root->left);
    }

    return (aux);
}

int removerenglishWord(EnglishBinTree **root, const char *word)
{
    EnglishBinTree *endchild = NULL;
    int exist = 0;

    if (*root)
    {
        if (strcmp(word, (*root)->englishWord) == 0)
        {
            EnglishBinTree *aux = *root;
            exist = 1;
            if (isLeafBin(*root))
            {
                free(aux);
                *root = NULL;
            }
            else if ((endchild = oneChild(root)) != NULL)
            {
                free(aux);
                *root = endchild;
            }
            else
            {
                endchild = minorChild((*root)->right);
                strcpy((*root)->englishWord, endchild->englishWord);
                (*root)->units = endchild->units;

                removerenglishWord(&(*root)->right, endchild->englishWord);
            }
        }
        else if (strcmp(word, (*root)->englishWord) < 0)
            exist = removerenglishWord(&(*root)->left, word);
        else
            exist = removerenglishWord(&(*root)->right, word);
    }

    return (exist);
}

void freeBinaryTree(EnglishBinTree *root)
{
    if (root)
    {
        freeBinaryTree(root->left);
        freeBinaryTree(root->right);
        free(root->englishWord);
        free(root);
    }
}

// #######################################################################################
// list encadeada
// #######################################################################################

void alocateLinkedListUnit(LinkedUnitList **list)
{
    *list = (LinkedUnitList *)malloc(sizeof(LinkedUnitList));
    (*list)->unitName = NULL;
    (*list)->next = NULL;
}

void defineLinkedListUnit(LinkedUnitList *list, const char *unitName)
{
    list->unitName = (char *)malloc((strlen(unitName) + 1) * sizeof(char));
    strcpy(list->unitName, unitName);
}

void showLinkedList(const LinkedUnitList *list)
{
    if (list)
    {
        if (list->next)
            printf("Unidade %s, ", list->unitName);
        else
            printf("Unidade %s", list->unitName);
        showLinkedList(list->next);
    }
    else
        printf("\n");
}

int sourceLinkedList(const LinkedUnitList *list, const char *unitName)
{
    int confirmation = 0;
    if (list)
    {
        if (strcmp(list->unitName, unitName) == 0)
            confirmation = 1;
        else
            confirmation = sourceLinkedList(list->next, unitName);
    }
    return (confirmation);
}

void freeLinkedList(LinkedUnitList *list)
{
    if (list)
    {
        freeLinkedList(list->next);
        free(list->unitName);
        free(list);
        list = NULL;
    }
}

int InsertLinkedList(LinkedUnitList **list, char *name)
{
    int confirmation = 1;
    if (!sourceLinkedList(*list, name))
    {
        LinkedUnitList *new;
        alocateLinkedListUnit(&new);
        defineLinkedListUnit(new, name);
        new->next = *list;
        *list = new;
    }
    else
        confirmation = 0;

    return confirmation;
}

int removeLinkedList(LinkedUnitList **list, const char *name)
{
    int confirmation = 0;
    if (*list)
    {
        if (strcmp((*list)->unitName, name) == 0)
        {
            LinkedUnitList *aux = *list;
            *list = (*list)->next;
            free(aux->unitName);
            free(aux);
            confirmation = 1;
        }
        else
            confirmation = removeLinkedList(&(*list)->next, name);
    }
    return (confirmation);
}

// #######################################################################################
// REMOÇÃO
// #######################################################################################

int removeEnglishFindUnit(Portuguese23Tree **root, const char *palavras_ingles, const char *unit)
{
    int confirm = 1;
    confirm = auxRemoveEnglishFindUnit(*root, palavras_ingles, unit, root);

    return (confirm);
}

int removeUnit(EnglishBinTree **root, const char *englishWord, const char *unit)
{
    int confirm = 1;

    if (*root)
    {
        if (strcmp((*root)->englishWord, englishWord) == 0)
        {
            confirm = removeLinkedList(&(*root)->units, unit);
            if (!(*root)->units)
                confirm = removerenglishWord(root, englishWord);
        }
        else if (strcmp((*root)->englishWord, englishWord) > 0)
            confirm = removeUnit(&(*root)->left, englishWord, unit);
        else
            confirm = removeUnit(&(*root)->right, englishWord, unit);
    }

    return (confirm);
}

int auxRemoveEnglishFindUnit(Portuguese23Tree *root, const char *englishWord, const char *unit, Portuguese23Tree **top)
{
    int confirm = 0;

    if (root)
    {
        confirm = auxRemoveEnglishFindUnit((root)->left, englishWord, unit, top);
        confirm = auxRemoveEnglishFindUnit((root)->middle, englishWord, unit, top) || confirm;

        if ((root)->infoCount == 2)
        {
            confirm = auxRemoveEnglishFindUnit((root)->right, englishWord, unit, top) || confirm;
            confirm = removeUnit(&(root)->info2.englishWord, englishWord, unit);
        }

        confirm = removeUnit(&(root)->info1.englishWord, englishWord, unit);

        if (!(root)->info1.englishWord)
            confirm = removeTree23(top, root->info1.portugueseWord);

        if ((root)->infoCount == 2 && !(root)->info2.englishWord)
            confirm = removeTree23(top, root->info2.portugueseWord);
    }

    return (confirm);
}

int removednitFromEnglishWord(EnglishBinTree **root, const char *portugueseWord, const char *unit)
{
    int confirm = 0;
    if (*root)
    {
        confirm = removednitFromEnglishWord(&(*root)->left, portugueseWord, unit);
        confirm = removednitFromEnglishWord(&(*root)->right, portugueseWord, unit) || confirm;
        confirm = removeLinkedList(&(*root)->units, unit);

        if (!(*root)->units)
            confirm = removerenglishWord(root, (*root)->englishWord);
    }

    return confirm;
}

int auxRemovePortugueseWordFindUnit(Portuguese23Tree *root, char *portugueseWord, const char *unit, Portuguese23Tree **top)
{
    int confirm = 1;

    if (root)
    {
        if (strcmp((root)->info1.portugueseWord, portugueseWord) == 0)
        {
            confirm = removednitFromEnglishWord(&(root)->info1.englishWord, portugueseWord, unit);
            if (!(root)->info1.englishWord)
                confirm = removeTree23(top, portugueseWord);
        }
        else if ((root)->infoCount == 2 && strcmp((root)->info2.portugueseWord, portugueseWord) == 0)
        {
            confirm = removednitFromEnglishWord(&(root)->info2.englishWord, portugueseWord, unit);
            if (!(root)->info2.englishWord)
                confirm = removeTree23(top, portugueseWord);
        }
        else if (strcmp(portugueseWord, (root)->info1.portugueseWord) < 0)
            confirm = auxRemovePortugueseWordFindUnit((root)->left, portugueseWord, unit, top);
        else if ((root)->infoCount == 1 || strcmp(portugueseWord, (root)->info2.portugueseWord) < 0)
            confirm = auxRemovePortugueseWordFindUnit((root)->middle, portugueseWord, unit, top);
        else
            confirm = auxRemovePortugueseWordFindUnit((root)->right, portugueseWord, unit, top);
    }

    return (confirm);
}

int removePortugueseWordFindUnit(Portuguese23Tree **root, char *portugueseWord, const char *unit)
{
    int confirm = 1;
    confirm = auxRemovePortugueseWordFindUnit(*root, portugueseWord, unit, root);

    return (confirm);
}