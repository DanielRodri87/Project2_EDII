#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------ Arv 2-3 ---------------------------------------------------------
Portuguese23 *searchWord(Portuguese23 **no, const char *portugueseWord)
{
    Portuguese23 *insert = NULL;

    if (no != NULL && *no != NULL)
    {
        if (strcmp(portugueseWord, (*no)->info1.portugueseWord) == 0)
            insert = (*no);
        else if ((*no)->infoCount == 2 && strcmp(portugueseWord, (*no)->info2.portugueseWord) == 0)
            insert = (*no);
        else
        {
            if (strcmp(portugueseWord, (*no)->info1.portugueseWord) < 0)
                insert = searchWord(&(*no)->left, portugueseWord);
            else if ((*no)->infoCount == 1 || strcmp(portugueseWord, (*no)->info2.portugueseWord) < 0)
                insert = searchWord(&(*no)->center, portugueseWord);
            else
                insert = searchWord(&(*no)->right, portugueseWord);
        }
    }

    return (insert);
}

void addTranslation(Portuguese23 *no, const char *portugueseWord, const char *englishWord, int unit)
{
    if (strcmp(portugueseWord, (no)->info1.portugueseWord) == 0)
        addEnglishTranslation(&(no)->info1, englishWord, unit);
    else if (no->infoCount == 2 && strcmp(portugueseWord, no->info2.portugueseWord) == 0)
        addEnglishTranslation(&(no)->info2, englishWord, unit);
}

int insertPortugueseWord(Portuguese23 **root, char *portugueseWord, char *englishWord, int unit)
{
    Info promote;
    Portuguese23 *father = NULL;
    int insert;

    Portuguese23 *nodeExist = NULL;
    nodeExist = searchWord(root, portugueseWord);

    if (nodeExist != NULL)
    {
        addTranslation(nodeExist, portugueseWord, englishWord, unit);
        insert = 1;
    }
    else
    {
        Info newInfo = createInfo(portugueseWord, englishWord, unit);
        insertTree23(root, &newInfo, &promote, &father);
        insert = 0;
    }
    return (insert);
}

Info createInfo(char *word, char *englishWord, int unit)
{
    Info info;

    info.portugueseWord = malloc(strlen(word) + 1);
    strcpy(info.portugueseWord, word);

    info.englishWord = NULL;
    info.englishWord = insertEnglishWord(info.englishWord, englishWord, unit);
    return info;
}

Portuguese23 *createNode(const Info *info, Portuguese23 *leftChild, Portuguese23 *centerChild)
{
    Portuguese23 *no = (Portuguese23 *)malloc(sizeof(Portuguese23));
    no->info1 = *info;
    no->info2.englishWord = NULL;
    no->info2.portugueseWord = NULL;
    no->left = leftChild;
    no->center = centerChild;
    no->right = NULL;
    no->infoCount = 1;

    return (no);
}

Portuguese23 *addKey(Portuguese23 *no, const Info *info, Portuguese23 *child)
{
    if (strcmp(info->portugueseWord, no->info1.portugueseWord) > 0)
    {
        no->info2 = *info;
        no->right = child;
    }
    else
    {
        no->info2 = no->info1;
        no->right = no->center;
        no->info1 = *info;
        no->center = child;
    }
    no->infoCount = 2;
    return no;
}

Portuguese23 *splitNode(Portuguese23 **no, const Info *info, Info *promote, Portuguese23 **child)
{
    Portuguese23 *bigger;

    if (strcmp(info->portugueseWord, (*no)->info2.portugueseWord) > 0)
    {
        *promote = (*no)->info2;
        if (child)
            bigger = createNode(info, (*no)->right, *child);
        else
            bigger = createNode(info, (*no)->right, NULL);
    }
    else if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) > 0)
    {
        *promote = *info;
        if (child)
            bigger = createNode(&(*no)->info2, *child, (*no)->right);
        else
            bigger = createNode(&(*no)->info2, NULL, (*no)->right);
    }
    else
    {
        *promote = (*no)->info1;
        bigger = createNode(&(*no)->info2, (*no)->center, (*no)->right);
        (*no)->info1 = *info;
        if (child)
            (*no)->center = *child;
        else
            (*no)->center = NULL;
    }

    (*no)->infoCount = 1;

    return (bigger);
}

int isLeaf(const Portuguese23 *no)
{
    return (no->left == NULL);
}

Portuguese23 *insertTree23(Portuguese23 **no, Info *info, Info *promote, Portuguese23 **father)
{
    Info promote1;
    Portuguese23 *biggerNo = NULL;
    if (*no == NULL)
        *no = createNode(info, NULL, NULL);

    else
    {
        if (isLeaf(*no))
        {
            if ((*no)->infoCount == 1)
                *no = addKey(*no, info, NULL);
            else
            {
                biggerNo = splitNode(no, info, promote, NULL);
                if (*father == NULL)
                {
                    *no = createNode(promote, *no, biggerNo);
                    biggerNo = NULL;
                }
            }
        }
        else
        {
            if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
                biggerNo = insertTree23(&((*no)->left), info, promote, no);
            else if ((*no)->infoCount == 1 || strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
                biggerNo = insertTree23(&((*no)->center), info, promote, no);
            else
                biggerNo = insertTree23(&((*no)->right), info, promote, no);

            if (biggerNo)
            {
                if ((*no)->infoCount == 1)
                {
                    *no = addKey(*no, promote, biggerNo);
                    biggerNo = NULL;
                }
                else
                {
                    biggerNo = splitNode(no, promote, &promote1, &biggerNo);
                    if (*father == NULL)
                    {
                        *no = createNode(&promote1, *no, biggerNo);
                        biggerNo = NULL;
                    }
                }
            }
        }
    }

    return (biggerNo);
}

void displayTree23(const Portuguese23 *root)
{
    if (root != NULL)
    {
        displayTree23(root->left);
        printf("word (PT): %s", root->info1.portugueseWord);
        if (root->info1.englishWord != NULL && root->info1.englishWord->englishWord != NULL)
        {
            printBinaryTree(root->info1.englishWord);
            printf("\n");
        }
        displayTree23(root->center);
        if (root->infoCount == 2)
        {
            printf("word (PT): %s", root->info2.portugueseWord);

            if (root->info2.englishWord != NULL && root->info2.englishWord->englishWord != NULL)
                printBinaryTree(root->info2.englishWord);
            printf("\n");
        }

        if (root->infoCount == 2)
        {
            displayTree23(root->right);
        }
    }
}

void printTranslations(EnglishBin *node, int unit, const char *wordPortuguês)
{
    if (node)
    {
        if (node->unit == unit)
        {
            printf("PT: %s\n", wordPortuguês);
            printf("ENG: %s\n", node->englishWord);
        }
        printTranslations(node->left, unit, wordPortuguês);
        printTranslations(node->right, unit, wordPortuguês);
    }
}

void displayPortugueseTranslation(Portuguese23 **root, const char *portugueseWord)
{
    Portuguese23 *resultado = NULL;
    if (root != NULL)
    {
        resultado = searchWord(root, portugueseWord);
        if (resultado)
        {
            printf("Traduções em inglês para a word '%s':\n", portugueseWord);

            if (strcmp(portugueseWord, resultado->info1.portugueseWord) == 0)
                printBinaryTree(resultado->info1.englishWord);
            else
                printBinaryTree(resultado->info2.englishWord);
        }
        else
            printf("A word '%s' não foi encontrada na árvore.\n", portugueseWord);
    }
}

void smallestInfoRight(Portuguese23 *root, Portuguese23 **no, Portuguese23 **fatherNo)
{
    if (root->left != NULL)
    {
        *fatherNo = root;
        smallestInfoRight(root->left, no, fatherNo);
    }
    else
        *no = root;
}

void largestInfoLeft(Portuguese23 *root, Portuguese23 **no, Portuguese23 **fatherNo)
{
    if (root->right != NULL)
    {
        *fatherNo = root;
        largestInfoLeft(root->right, no, fatherNo);
    }
    else
        *no = root;
}

int remove23(Portuguese23 **father, Portuguese23 **root, char *value)
{
    int removeu = 0;
    Portuguese23 *no = NULL, *no1, *fatherNo = NULL, *fatherNo1 = NULL, **aux;
    aux = (Portuguese23 **)malloc(sizeof(Portuguese23 *));
    no1 = (Portuguese23 *)malloc(sizeof(Portuguese23));

    if (*root != NULL)
    {
        if (isLeaf(*root) == 1)
        {
            if ((*root)->infoCount == 2)
            {
                if (strcmp(value, (*root)->info2.portugueseWord) == 0)
                {
                    (*root)->info2.englishWord = NULL;
                    (*root)->info2.portugueseWord = NULL;
                    (*root)->infoCount = 1;
                    removeu = 1;
                }
                else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
                {
                    (*root)->info1 = (*root)->info2;
                    (*root)->info2.englishWord = NULL;
                    (*root)->info2.portugueseWord = NULL;
                    (*root)->infoCount = 1;
                    removeu = 1;
                }
            }
            else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
            {
                if (*father == NULL)
                {
                    free(*root);
                    *root = NULL;
                    removeu = 1;
                }
                else if (*root == (*father)->left)
                {
                    (*root)->info1 = (*father)->info1;
                    fatherNo = *father;
                    smallestInfoRight((*father)->center, &no, &fatherNo);
                    (*father)->info1 = no->info1;
                    removeu = 1;

                    if (no->infoCount == 2)
                    {
                        no->info1 = no->info2;
                        (*root)->info2.englishWord = NULL;
                        (*root)->info2.portugueseWord = NULL;
                        no->infoCount = 1;
                    }
                    else
                    {
                        if (fatherNo->infoCount == 1)
                        {
                            (*root)->info2 = no->info1;
                            (*root)->infoCount = 2;
                            free(no);
                            *father = *root;
                        }
                        else
                        {
                            no->info1 = fatherNo->info2;
                            fatherNo1 = fatherNo;
                            smallestInfoRight(fatherNo->right, &no1, &fatherNo1);
                            fatherNo->info2 = no1->info1;

                            if (no1->infoCount == 2)
                            {
                                no1->info1 = no1->info2;
                                no1->info2.englishWord = NULL;
                                no1->info2.portugueseWord = NULL;
                                no1->infoCount = 1;
                            }
                            else
                            {
                                no->info2 = fatherNo->info2;
                                no->infoCount = 2;
                                fatherNo->info2.englishWord = NULL;
                                fatherNo->info2.portugueseWord = NULL;
                                fatherNo->infoCount = 1;
                                free(no1);
                                fatherNo1->right = NULL;
                            }
                        }
                    }
                }
                else if ((*root) == (*father)->center)
                {
                    removeu = 1;
                    if ((*father)->infoCount == 1)
                    {
                        if (((*father)->left)->infoCount == 2)
                        {
                            (*root)->info1 = (*father)->info1;
                            (*father)->info1 = ((*father)->left)->info2;
                            ((*father)->left)->info2.englishWord = NULL;
                            ((*father)->left)->info2.portugueseWord = NULL;
                            ((*father)->left)->infoCount = 1;
                        }
                        else
                        {
                            ((*father)->left)->info2 = (*father)->info1;
                            free(*root);
                            ((*father)->left)->infoCount = 2;
                            *aux = (*father)->left;
                            free(*father);
                            *father = *aux;
                        }
                    }
                    else
                    {
                        (*root)->info1 = (*father)->info2;
                        fatherNo = *father;
                        smallestInfoRight((*father)->right, &no, &fatherNo);
                        (*father)->info2 = no->info1;

                        if (no->infoCount == 2)
                        {
                            no->info1 = no->info2;
                            no->info2.englishWord = NULL;
                            no->info2.portugueseWord = NULL;
                            no->infoCount = 1;
                        }
                        else
                        {
                            (*root)->infoCount = 2;
                            (*root)->info2 = (*father)->info2;
                            (*father)->info2.englishWord = NULL;
                            (*father)->info2.portugueseWord = NULL;
                            (*father)->infoCount = 1;
                            free(no);
                            (*father)->right = NULL;
                        }
                    }
                }
                else
                {
                    removeu = 1;
                    fatherNo = *father;
                    largestInfoLeft((*father)->center, &no, &fatherNo);

                    if (no->infoCount == 1)
                    {
                        no->info2 = (*father)->info2;
                        (*father)->info2.englishWord = NULL;
                        (*father)->info2.portugueseWord = NULL;
                        (*father)->infoCount = 1;
                        no->infoCount = 2;
                        free(*root);
                        *root = NULL;
                    }
                    else
                    {
                        (*root)->info1 = (*father)->info2;
                        (*father)->info2 = no->info2;
                        no->info2.englishWord = NULL;
                        no->info2.portugueseWord = NULL;
                        no->infoCount = 1;
                    }
                }
            }
        }
        else
        {
            if (strcmp(value, (*root)->info1.portugueseWord) < 0)
                removeu = remove23(root, &(*root)->left, value);
            else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
            {
                fatherNo = *root;
                smallestInfoRight((*root)->center, &no, &fatherNo);
                (*root)->info1 = no->info1;
                remove23(root, &(*root)->center, (*root)->info1.portugueseWord);
                removeu = 1;
            }
            else if (((*root)->infoCount == 1) || (strcmp(value, (*root)->info1.portugueseWord) < 0))
            {
                removeu = remove23(root, &(*root)->center, value);
            }
            else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
            {
                fatherNo = *father;
                smallestInfoRight((*father)->right, &no, &fatherNo);
                (*root)->info2 = no->info1;
                remove23(root, &(*root)->right, (*root)->info2.portugueseWord);
                removeu = 1;
            }
            else
            {
                removeu = remove23(root, &(*root)->right, value);
            }
        }
    }
    return removeu;
}

/*#########################################FREE#######################################################*/

void freeInfo23(Info *info)
{
    FreeTreeBinary(info->englishWord);
    free(info->portugueseWord);
}

void freeTree(Portuguese23 *no)
{
    if (no != NULL)
    {
        freeTree(no->left);
        freeTree(no->center);
        if (no->infoCount == 2)
            freeTree(no->right);
        if (no->infoCount == 2)
            freeInfo23(&no->info2);
        freeInfo23(&no->info1);
        free(no);
    }
}

// ------------------------ Árvore Binária ---------------------
EnglishBin *createBinaryNode(const char *englishWord, int unit)
{
    EnglishBin *novoNo = (EnglishBin *)malloc(sizeof(EnglishBin));
    if (novoNo != NULL)
    {
        novoNo->englishWord = (char *)malloc(strlen(englishWord) + 1);
        strcpy(novoNo->englishWord, englishWord);
        novoNo->unit = unit;
        novoNo->left = novoNo->right = NULL;
    }
    return novoNo;
}

// Função para inserir uma word em inglês na root binaria de busca
EnglishBin *insertEnglishWord(EnglishBin *root, const char *englishWord, int unit)
{
    EnglishBin *result;
    if (root == NULL)
    {
        result = createBinaryNode(englishWord, unit);
    }
    else
    {
        if (strcmp(englishWord, root->englishWord) < 0)
        {
            root->left = insertEnglishWord(root->left, englishWord, unit);
        }
        else if (strcmp(englishWord, root->englishWord) > 0)
        {
            root->right = insertEnglishWord(root->right, englishWord, unit);
        }
        result = root;
    }
    return result;
}

void addEnglishTranslation(Info *info, const char *wordIng, int unit)
{
    info->englishWord = insertEnglishWord(info->englishWord, wordIng, unit);
}

int isBinaryLeaf(EnglishBin *root)
{
    return (root->left == NULL && root->right == NULL);
}

EnglishBin *onlyOneChild(EnglishBin *root)
{
    EnglishBin *aux;
    aux = NULL;

    if (root->right == NULL)
    {
        aux = root->left;
    }
    else if (root->left == NULL)
    {
        aux = root->right;
    }

    return aux;
}

EnglishBin *smallestChild(EnglishBin *root)
{
    EnglishBin *aux;
    aux = root;

    if (root)
    {
        if (root->left)
            aux = smallestChild(root->left);
    }

    return aux;
}

int removeEnglishWord(EnglishBin **root, char *word)
{
    EnglishBin *endchild;
    int existe = 0;

    if (*root)
    {
        if (strcmp(word, (*root)->englishWord) == 0)
        {
            existe = 1;
            printf("removendo word: %s\n", word);
            EnglishBin *aux = *root;
            if (isBinaryLeaf(*root))
            {
                free(aux);
                *root = NULL;
            }
            else if ((endchild = onlyOneChild(*root)) != NULL)
            {
                free(aux);
                *root = endchild;
            }
            else
            {
                endchild = smallestChild((*root)->right);
                strcpy((*root)->englishWord, endchild->englishWord);
                (*root)->unit = endchild->unit;

                removeEnglishWord(&(*root)->right, endchild->englishWord);
            }
        }
        else if (strcmp(word, (*root)->englishWord) < 0)
        {
            existe = removeEnglishWord(&(*root)->left, word);
        }
        else
        {
            existe = removeEnglishWord(&(*root)->right, word);
        }
    }

    return existe;
}

void searchEnglishWord(Portuguese23 **root, char *word, int unit, Portuguese23 **father)
{
    int removeu;
    if (*root != NULL)
    {
        searchEnglishWord(&(*root)->left, word, unit, father);

        if ((*root)->info1.englishWord != NULL && (*root)->info1.englishWord->unit == unit)
        {
            removeu = removeEnglishWord(&(*root)->info1.englishWord, word);
            if (removeu)
                printf("A word %s foi removida com sucesso!\n\n", word);
            if ((*root)->info1.englishWord == NULL)
            {
                removeu = remove23(father, root, (*root)->info1.portugueseWord);
                if (removeu)
                    printf("Removido\n\n");
            }
        }

        searchEnglishWord(&(*root)->center, word, unit, root);

        if ((*root)->infoCount == 2 && (*root)->info2.englishWord != NULL && (*root)->info2.englishWord->unit == unit)
        {
            removeu = removeEnglishWord(&(*root)->info2.englishWord, word);
            if (removeu)
                printf("A word %s foi removida com sucesso!\n\n", word);
            if ((*root)->info2.englishWord == NULL)
            {
                removeu = remove23(father, root, (*root)->info2.portugueseWord);
                if (removeu)
                    printf("Removido\n\n");
            }
        }
        if ((*root)->infoCount == 2 && (*root)->info2.englishWord != NULL)
        {
            searchEnglishWord(&(*root)->right, word, unit, root);
        }
    }
}

void FreeTreeBinary(EnglishBin *root)
{
    if (root)
    {
        FreeTreeBinary(root->left);
        FreeTreeBinary(root->right);
        free(root->englishWord);
        free(root);
    }
}

void exibirInfo(Info info)
{
    printf("Português: %s | Inglês: %s\n", info.portugueseWord, info.englishWord->englishWord);
}

void exibirEmOrdem23(Portuguese23 *raiz)
{
    if (raiz != NULL)
    {
        printf("\n-----------ARVORE-2-3------------\n");
        if (raiz->infoCount == 1)
        {
            exibirEmOrdem23(raiz->left);
            exibirInfo(raiz->info1);
            exibirEmOrdem23(raiz->right);
        }
        else if (raiz->infoCount == 2)
        {
            exibirEmOrdem23(raiz->left);
            exibirInfo(raiz->info1);
            exibirEmOrdem23(raiz->center);
            exibirInfo(raiz->info2);
            exibirEmOrdem23(raiz->right);
        }
    }
}

void printBinaryTree(EnglishBin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->left); // Percorre a árvore à leftuerda
        printf("\n");
        // Imprime a tradução de inglês associada à word em português
        printf("word em Inglês: %s = unit: %d\n", root->englishWord, root->unit);
        printBinaryTree(root->right); // Percorre a árvore à righteita
    }
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - informar uma unit e então imprima todas as words da unit em português seguida das
// equivalentes em inglês
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void printWordsByUnit(Portuguese23 *root, int unit)

{
    if (root)
    {
        printWordsByUnit(root->left, unit);
        printTranslations(root->info1.englishWord, unit, root->info1.portugueseWord);

        printWordsByUnit(root->center, unit);
        if (root->infoCount == 2)
        {
            printTranslations(root->info2.englishWord, unit, root->info2.portugueseWord);
        }
        printWordsByUnit(root->right, unit);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - informar uma word em português e então imprima todas as words em inglês equivalente à word em
// português dada, independente da unit
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// void findEnglishByPortuguese(PortugueseEnglish *root, const char *portugueseWord)
// {
//     if (root != NULL)
//     {
//         if (root->infoCount >= 1 && strcmp(root->info1.portugueseWord, portugueseWord) == 0)
//             printf("Inglês: %s\n", root->info1.englishTranslation->word);
//         if (root->infoCount == 2 && strcmp(root->info2.portugueseWord, portugueseWord) == 0)
//             printf("Inglês: %s\n", root->info2.englishTranslation->word);

//         findEnglishByPortuguese(root->left, portugueseWord);
//         findEnglishByPortuguese(root->center, portugueseWord);
//         if (root->infoCount == 2)
//             findEnglishByPortuguese(root->right, portugueseWord);
//     }
// }

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III - informar uma word em inglês e a unit a qual a mesma pertence removê-la das árvores binárias
// das quais ela pertence. Caso ela seja a única word em uma das árvores binárias, remover também da
// árvore 2-3
// ---------------------------------------------------- XXXXXX -------------------------------------------------

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - informar uma word em português e a unit a qual a mesma pertence e então removê-la, para isto
// deve remover a word em inglês da árvore binária correspondente a word em português da mesma
// unit. Caso ela seja a única word na árvore binária, a word em português deve ser removida da
// árvore 2-3 */
// ---------------------------------------------------- XXXXXX -------------------------------------------------