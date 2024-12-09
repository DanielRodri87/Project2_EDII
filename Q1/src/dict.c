#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ------------------------------------------ INSERIR ---------------------------------------------------------
int isLeaf(PortugueseEnglish *node)
{
    return (node->left == NULL);
}

PortugueseEnglish *createNode(const Info information, PortugueseEnglish *leftChild, PortugueseEnglish *centerChild)
{
    PortugueseEnglish *node = (PortugueseEnglish *)malloc(sizeof(PortugueseEnglish));

    if (!node)
        printf("Failed to create node\n");

    if (node)
    {
        node->info1 = information;
        node->left = leftChild;
        node->center = centerChild;
        node->infoCount = 1;
    }

    return (node);
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
    return (node);
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

    return (greater);
}

void addEnglishTranslation(EnglishPortuguese **translations, const char *englishWord)
{
    EnglishPortuguese *current = *translations;
    EnglishPortuguese *parent = NULL; 

    while (current)
    {
        if (strcmp(current->word, englishWord) == 0)
        {
            current = NULL; 
        }
        else
        {
            parent = current; 

            if (strcmp(current->word, englishWord) > 0)
                current = current->left; 
            else
                current = current->right; 
        }
    }

    if (current == NULL && (!parent || strcmp(parent->word, englishWord) != 0))
    {
        EnglishPortuguese *newTranslation = (EnglishPortuguese *)malloc(sizeof(EnglishPortuguese));
        newTranslation->word = strdup(englishWord);
        newTranslation->left = newTranslation->right = NULL;

        if (!parent)
            *translations = newTranslation; 
        else if (strcmp(parent->word, englishWord) > 0)
            parent->left = newTranslation; 
        else
            parent->right = newTranslation; 
    }
}


int updateExistingTranslation(PortugueseEnglish *node, Info info)
{
    int controle = 0;
    if (node != NULL)
    {
        if (strcmp(node->info1.portugueseWord, info.portugueseWord) == 0)
        {
            addEnglishTranslation(&(node->info1.englishTranslation), info.englishTranslation->word);
            controle = 1;
        }

        if (node->infoCount == 2 && strcmp(node->info2.portugueseWord, info.portugueseWord) == 0)
        {
            addEnglishTranslation(&(node->info2.englishTranslation), info.englishTranslation->word);
            controle = 1;
        }
    }
    return (controle);
}

PortugueseEnglish *insertPortugueseWord(PortugueseEnglish **node, Info info, Info *promote, PortugueseEnglish **parent)
{
    PortugueseEnglish *greaterNode;
    Info promote1;
    greaterNode = NULL;

    if (*node == NULL)
        *node = createNode(info, NULL, NULL);
    else
    {
        if (!(updateExistingTranslation(*node, info)))
        {
            if (isLeaf(*node))
            {
                if ((*node)->infoCount == 1)
                    *node = addKey(*node, info, NULL);
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
                    greaterNode = insertPortugueseWord(&((*node)->left), info, promote, node);
                else
                {
                    if (((*node)->infoCount == 1) || (strcmp(info.portugueseWord, (*node)->info2.portugueseWord) < 0))
                        greaterNode = insertPortugueseWord(&((*node)->center), info, promote, node);
                    else
                        greaterNode = insertPortugueseWord(&((*node)->right), info, promote, node);
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
    }

    return (greaterNode);
}

// ------------------------------------------ EXIBIR ---------------------------------------------------------

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
            displayWords(root->right);
    }
}

int LargestUnit(PortugueseEnglish *root)
{
    if (root != NULL)
    {
        int currentMax = (root->infoCount == 2) ? (root->info1.unit > root->info2.unit ? root->info1.unit : root->info2.unit) : root->info1.unit;

        int leftMax = LargestUnit(root->left);
        int centerMax = LargestUnit(root->center);
        int rightMax = (root->infoCount == 2) ? LargestUnit(root->right) : 0;

        int bigger = currentMax;
        if (leftMax > bigger)
            bigger = leftMax;
        if (centerMax > bigger)
            bigger = centerMax;
        if (rightMax > bigger)
            bigger = rightMax;

        return (bigger);
    }
}

// ------------------------------------------ REMOÇÃO ---------------------------------------------------------
void ShortestInfoRight(PortugueseEnglish *root, PortugueseEnglish **node, PortugueseEnglish **father)
{
    if (root->left != NULL)
    {
        *father = root;
        ShortestInfoRight(root->left, node, father);
    }
    else
        *node = root;
}

void GreaterInfoLeft(PortugueseEnglish *root, PortugueseEnglish **node, PortugueseEnglish **father)
{
    if (root->right != NULL)
    {
        *father = root;
        GreaterInfoLeft(root->right, node, father);
    }
    else
        *node = root;
}

int remover23(PortugueseEnglish **father, PortugueseEnglish **root, char *value)
{
    int removed = 0;
    PortugueseEnglish *no = NULL, *no1, *fatherNo = NULL, *fatherNo1 = NULL, **aux;
    aux = (PortugueseEnglish **)malloc(sizeof(PortugueseEnglish *));
    no1 = (PortugueseEnglish *)malloc(sizeof(PortugueseEnglish));

    if (*root != NULL)
    {
        if (isLeaf(*root) == 1)
        {
            if ((*root)->infoCount == 2)
            {
                if (strcmp(value, (*root)->info2.portugueseWord) == 0)
                { 
                    (*root)->info2.englishTranslation = NULL;
                    (*root)->info2.portugueseWord = NULL;
                    (*root)->infoCount = 1;
                    removed = 1;
                }
                else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
                { 
                    (*root)->info1 = (*root)->info2;
                    (*root)->info2.englishTranslation = NULL;
                    (*root)->info2.portugueseWord = NULL;
                    (*root)->infoCount = 1;
                    removed = 1;
                }
            }
            else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
            {
                if (*father == NULL)
                {
                    free(*root);
                    *root = NULL;
                    removed = 1;
                }
                else if (*root == (*father)->left)
                {
                    (*root)->info1 = (*father)->info1;
                    fatherNo = *father;
                    ShortestInfoRight((*father)->center, &no, &fatherNo);
                    (*father)->info1 = no->info1;
                    removed = 1;

                    if (no->infoCount == 2)
                    {
                        no->info1 = no->info2;
                        (*root)->info2.englishTranslation = NULL;
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
                            ShortestInfoRight(fatherNo->right, &no1, &fatherNo1);
                            fatherNo->info2 = no1->info1;

                            if (no1->infoCount == 2)
                            {
                                no1->info1 = no1->info2;
                                no1->info2.englishTranslation = NULL;
                                no1->info2.portugueseWord = NULL;
                                no1->infoCount = 1;
                            }
                            else
                            {
                                no->info2 = fatherNo->info2;
                                no->infoCount = 2;
                                fatherNo->info2.englishTranslation = NULL;
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
                    removed = 1;
                    if ((*father)->infoCount == 1)
                    {
                        if (((*father)->left)->infoCount == 2)
                        {
                            (*root)->info1 = (*father)->info1;
                            (*father)->info1 = ((*father)->left)->info2;
                            ((*father)->left)->info2.englishTranslation = NULL;
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
                        ShortestInfoRight((*father)->right, &no, &fatherNo);
                        (*father)->info2 = no->info1;

                        if (no->infoCount == 2)
                        {
                            no->info1 = no->info2;
                            no->info2.englishTranslation = NULL;
                            no->info2.portugueseWord = NULL;
                            no->infoCount = 1;
                        }
                        else
                        {
                            (*root)->infoCount = 2;
                            (*root)->info2 = (*father)->info2;
                            (*father)->info2.englishTranslation = NULL;
                            (*father)->info2.portugueseWord = NULL;
                            (*father)->infoCount = 1;
                            free(no);
                            (*father)->right = NULL;
                        }
                    }
                }
                else
                {
                    removed = 1;
                    fatherNo = *father;
                    GreaterInfoLeft((*father)->center, &no, &fatherNo);

                    if (no->infoCount == 1)
                    {
                        no->info2 = (*father)->info2;
                        (*father)->info2.englishTranslation = NULL;
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
                        no->info2.englishTranslation = NULL;
                        no->info2.portugueseWord = NULL;
                        no->infoCount = 1;
                    }
                }
            }
        }
        else
        { 
            if (strcmp(value, (*root)->info1.portugueseWord) < 0)
                removed = remover23(root, &(*root)->left, value);
            else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
            {
                fatherNo = *root;
                ShortestInfoRight((*root)->center, &no, &fatherNo);
                (*root)->info1 = no->info1;
                remover23(root, &(*root)->center, (*root)->info1.portugueseWord);
                removed = 1;
            }
            else if (((*root)->infoCount == 1) || (strcmp(value, (*root)->info1.portugueseWord) < 0))
                removed = remover23(root, &(*root)->center, value);
        
            else if (strcmp(value, (*root)->info1.portugueseWord) == 0)
            {
                fatherNo = *father;
                ShortestInfoRight((*father)->right, &no, &fatherNo);
                (*root)->info2 = no->info1;
                remover23(root, &(*root)->right, (*root)->info2.portugueseWord);
                removed = 1;
            }
            else
                removed = remover23(root, &(*root)->right, value);
        }
    }
    return (removed);
}

int isLeafBin(EnglishPortuguese *root) {
    return (root->left == NULL && root->right == NULL);
}

EnglishPortuguese *singleChild(EnglishPortuguese *root) {
    EnglishPortuguese *aux = NULL;

    if (root->right == NULL) {
        aux = root->left;
    } else if (root->left == NULL) {
        aux = root->right;
    }

    return aux;
}

EnglishPortuguese *smallestChild(EnglishPortuguese *root) {
    EnglishPortuguese *aux = root;

    if (root) {
        if (root->left)
            aux = smallestChild(root->left);
    }

    return aux;
}

int removeEnglishWord(EnglishPortuguese **root, char *word) {
    EnglishPortuguese *childNode;
    int exists = 0;

    if (*root) {
        if (strcmp(word, (*root)->word) == 0) {
            exists = 1;
            printf("Removing word: %s\n", word);
            EnglishPortuguese *aux = *root;
            if (isLeafBin(*root)) {
                free(aux);
                *root = NULL;
            } else if ((childNode = singleChild(*root)) != NULL) {
                free(aux);
                *root = childNode;
            } else {
                childNode = smallestChild((*root)->right);
                strcpy((*root)->word, childNode->word);

                removeEnglishWord(&(*root)->right, childNode->word);
            }
        } else if (strcmp(word, (*root)->word) < 0) {
            exists = removeEnglishWord(&(*root)->left, word);
        } else {
            exists = removeEnglishWord(&(*root)->right, word);
        }
    }

    return exists;
}



void SearchEnglishWord(PortugueseEnglish **root, char *englishWord, int unit, PortugueseEnglish **parent) {
    int removed;
    if (*root != NULL) {
        SearchEnglishWord(&(*root)->left, englishWord, unit, parent);

        if ((*root)->info1.englishTranslation != NULL && (*root)->info1.unit == unit) {
            removed = removeEnglishWord(&(*root)->info1.englishTranslation, englishWord);
            if (removed) 
                printf("The word %s was successfully removed!\n\n", englishWord);
            if ((*root)->info1.englishTranslation == NULL) {
                removed = remover23(parent, root, (*root)->info1.portugueseWord);
                if (removed) 
                    printf("Node removed\n\n");
            }
        }

        SearchEnglishWord(&(*root)->center, englishWord, unit, root);

        if ((*root)->infoCount == 2 && (*root)->info2.englishTranslation != NULL && (*root)->info2.unit == unit) {
            removed = removeEnglishWord(&(*root)->info2.englishTranslation, englishWord);
            if (removed) 
                printf("The word %s was successfully removed!\n\n", englishWord);
            if ((*root)->info2.englishTranslation == NULL) {
                removed = remover23(parent, root, (*root)->info2.portugueseWord);
                if (removed) 
                    printf("Node removed\n\n");
            }
        }
        if ((*root)->infoCount == 2 && (*root)->info2.englishTranslation != NULL) {
            SearchEnglishWord(&(*root)->right, englishWord, unit, root);
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
            printf("%s: %s;\n",
                   root->info1.englishTranslation->word,
                   root->info1.portugueseWord);
        
        if (root->infoCount == 2 && root->info2.unit == unit)
            printf("%s: %s;\n",
                   root->info2.englishTranslation->word,
                   root->info2.portugueseWord);

        printWordsByUnit(root->left, unit);
        printWordsByUnit(root->center, unit);

        if (root->infoCount == 2)
            printWordsByUnit(root->right, unit);
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
