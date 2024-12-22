#include "src.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Info *CreateInfo(int start, int end, int status)
{
    Info *info = (Info *)malloc(sizeof(Info));

    if (info != NULL)
    {
        info->start = start;
        info->end = end;
        info->status = status;
    }
    return info;
}

Memory *createNode(Info *information, Memory *leftChild, Memory *centerChild)
{
    Memory *node = (Memory *)malloc(sizeof(Memory));

    if (node != NULL)
    {
        node->info1 = information;
        node->left = leftChild;
        node->center = centerChild;
        node->numKeys = 1;
    }
    else
    {
        printf("Falha ao criar o Nó!\n");
    }

    return node;
}

int isLeaf(Memory *node)
{
    return node->left == NULL;
}

void AddInfo(Memory **node, Info *info, Memory *child)
{
    if ((*node)->numKeys == 1)
    {
        if (info->start > (*node)->info1->start)
        {
            (*node)->info2 = info;
            (*node)->right = child;
        }
        else
        {
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            (*node)->right = (*node)->center;
            (*node)->center = child;
        }
        (*node)->numKeys = 2;
    }
    else if ((*node)->numKeys == 2)
    {
        Memory *newNode = (Memory *)malloc(sizeof(Memory));
        newNode->numKeys = 1;

        if (info->start > (*node)->info2->start)
        {
            newNode->info1 = info;
            newNode->left = (*node)->right;
            newNode->center = child;
        }
        else if (info->start > (*node)->info1->start)
        {
            newNode->info1 = (*node)->info2;
            newNode->left = child;
            newNode->center = (*node)->right;
        }
        else
        {
            newNode->info1 = (*node)->info2;
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            newNode->left = (*node)->center;
            newNode->center = (*node)->right;
            (*node)->center = child;
        }
        (*node)->numKeys = 1;
    }
}

Split SplitNode(Memory **root, Info *info, Memory *child)
{
    Memory *largestNode = (Memory *)malloc(sizeof(Memory));
    Info *promote = NULL;
    Split itBroke;

    largestNode->numKeys = 1;

    if (info->start > (*root)->info2->start)
    {
        promote = (*root)->info2;
        (*root)->numKeys = 1;

        largestNode->info1 = info;
        largestNode->left = (*root)->center;
        largestNode->center = child;
    }
    else if (info->start > (*root)->info1->start)
    {
        promote = info;
        largestNode->info1 = (*root)->info2;
        largestNode->left = child;
        largestNode->center = (*root)->right;

        (*root)->numKeys = 1;
    }
    else
    {
        promote = (*root)->info1;
        (*root)->info1 = info;

        largestNode->info1 = (*root)->info2;
        largestNode->left = (*root)->center;
        largestNode->center = (*root)->right;

        (*root)->numKeys = 1;
    }

    itBroke.largestNode = largestNode;
    itBroke.promote = promote;

    return itBroke;
}

void Insert23(Memory **root, Memory *parent, Info **promote, int start, int end, int status, int *flag)
{
    if (*root == NULL)
    {
        Info *nova_info = CreateInfo(start, end, status);
        *root = createNode(nova_info, NULL, NULL);
        *flag = 1;
    }
    else if (isLeaf(*root))
    {
        *flag = 1;
        if ((*root)->numKeys < 2)
        {
            Info *nova_info = CreateInfo(start, end, status);
            AddInfo(root, nova_info, NULL);
        }
        else
        {
            Info *nova_info = CreateInfo(start, end, status);
            Split resultado = SplitNode(root, nova_info, NULL);

            if (!parent)
                *root = createNode(resultado.promote, *root, resultado.largestNode);
            else if (parent->numKeys < 2)
                AddInfo(&parent, resultado.promote, resultado.largestNode);
            else
            {
                resultado = SplitNode(&parent, resultado.promote, resultado.largestNode);
                parent = createNode(resultado.promote, parent, resultado.largestNode);
            }
        }
    }
    else
    {
        if (start < (*root)->info1->start)
            Insert23(&(*root)->left, *root, promote, start, end, status, flag);
        else if ((*root)->numKeys == 1 || start < (*root)->info2->start)
            Insert23(&(*root)->center, *root, promote, start, end, status, flag);
        else
            Insert23(&(*root)->right, *root, promote, start, end, status, flag);
    }
}

Memory *FindSpace(Memory *root, int requiredSpace)
{
    Memory *result = NULL;

    if (root != NULL)
    {
        for (int i = 0; i < root->numKeys; i++)
        {
            Info *currentInfo = (i == 0) ? root->info1 : root->info2;

            if (currentInfo->status == FREE)
            {
                int availableSpace = currentInfo->end - currentInfo->start + 1;

                if (availableSpace >= requiredSpace)
                {
                    result = root;
                }
            }
        }

        if (result == NULL)
        {
            result = FindSpace(root->left, requiredSpace);

            if (result == NULL)
                result = FindSpace(root->center, requiredSpace);

            if (result == NULL && root->numKeys == 2)
                result = FindSpace(root->right, requiredSpace);
        }
    }

    return result;
}

Memory *SourceSpace(Memory *root, int requiredSpace)
{
    return FindSpace(root, requiredSpace);
}

void DisplayInfos(Memory *root)
{
    if (root)
    {
        DisplayInfos(root->left);
        printf("Começo: %d | Fim %d | Status: %d\n", root->info1->start, root->info1->end, root->info1->status);
        DisplayInfos(root->center);

        if (root->numKeys == 2)
        {
            printf("Começo: %d | Fim %d | Status: %d\n", root->info2->start, root->info2->end, root->info2->status);
            DisplayInfos(root->right);
        }
    }
}

// ----------------------------------------------------------------
void AllocateSpace(Memory **root, int requiredSpace)
{
    Memory *node = FindSpace(*root, requiredSpace);

    if (node)
    {
        int availableSpace = node->info1->end - node->info1->start + 1;

        if (availableSpace >= requiredSpace)
        {
            int newEndOccupied = node->info1->start + requiredSpace - 1;

            Info *newOccupiedInfo = CreateInfo(node->info1->start, newEndOccupied, OCCUPIED);

            node->info1 = newOccupiedInfo;

            if (availableSpace > requiredSpace)
            {
                int remainingStart = newEndOccupied + 1;
                int remainingEnd = node->info1->end;
                Info *remainingInfo = CreateInfo(remainingStart, remainingEnd, FREE);

                int flag = 0;
                Insert23(root, NULL, NULL, remainingInfo->start, remainingInfo->end, FREE, &flag);
            }

            printf("Espaço alocado com sucesso.\n");
        }
        else
        {
            printf("Espaço insuficiente na memória\n");
        }
    }
    else
    {
        printf("Espaço insuficiente na memória\n");
    }
}

// ------------------------------------- REMOVER -------------------------------------
void freeInfo(Info *info)
{
    if (info)
    {
        free(info);
    }
}

void freeMemory(Memory *node)
{
    if (node)
    {
        freeMemory(node->left);
        freeMemory(node->center);
        freeMemory(node->right);

        if (node->info1)
        {
            freeInfo(node->info1);
        }
        if (node->numKeys == 2 && node->info2)
        {
            freeInfo(node->info2);
        }

        free(node);
    }
}

void findSmallestRight(Memory *root, Memory **no, Memory **paiNo)
{
    if (root->left != NULL)
    {
        *paiNo = root;
        findSmallestRight(root->left, no, paiNo);
    }
    else
        *no = root;
}

void findLargestLeft(Memory *root, Memory **no, Memory **paiNo)
{
    if (root->right != NULL)
    {
        *paiNo = root;
        findLargestLeft(root->right, no, paiNo);
    }
    else
        *no = root;
}

int removeFromMemory(Memory **parent, Memory **node, Info *key)
{
    int removeu = 0;
    Memory *no = NULL, *no1, *parentNo = NULL, *parentNo1 = NULL, **aux;
    aux = (Memory **)malloc(sizeof(Memory *));
    no1 = (Memory *)malloc(sizeof(Memory));

    if (*node != NULL)
    {
        if (isLeaf(*node) == 1)
        {
            if ((*node)->numKeys == 2)
            {
                if (key->start == (*node)->info2->start)
                { // quando é folha, tem duas informações e o numero ta na segunda posição
                    (*node)->info2 = NULL;
                    (*node)->numKeys = 1;
                    removeu = 1;
                }
                else if (key->start == (*node)->info1->start)
                { // quando é folha, tem duas informações e o numero ta na primeira posição do nó
                    (*node)->info1 = NULL;
                    (*node)->numKeys = 1;
                    removeu = 1;
                } 
            }
            else if (key->start == (*node)->info1->start)
            {
                if (*parent == NULL)
                {
                    free(*node);
                    *node = NULL;
                    removeu = 1;
                }
                else if (*node == (*parent)->left)
                {
                    (*node)->info1 = (*parent)->info1;
                    parentNo = *parent;
                    findSmallestRight((*parent)->center, &no, &parentNo);
                    (*parent)->info1 = no->info1;
                    removeu = 1;

                    if (no->numKeys == 2)
                    {
                        no->info1 = no->info2;
                        (*node)->info1 = NULL;
                        (*node)->numKeys = 1;
                        removeu = 1;
                    }
                    else
                    {
                        if (parentNo->numKeys == 1)
                        {
                            (*node)->info2 = no->info1;
                            (*node)->numKeys = 2;
                            free(no);
                            *parent = *node;
                        }
                        else
                        {
                            no->info1 = parentNo->info2;
                            parentNo1 = parentNo;
                            findSmallestRight(parentNo->right, &no1, &parentNo1);
                            parentNo->info2 = no1->info1;

                            if (no1->numKeys == 2)
                            {
                                no1->info1 = no1->info2;
                                no1->info2 = NULL;
                                no1->numKeys = 1;
                            }
                            else
                            {
                                // entrou aqui
                                no->info2 = parentNo->info2;
                                no->numKeys = 2;
                                parentNo->info2 = NULL;
                                parentNo->numKeys = 1;
                                free(no1);
                                parentNo1->right = NULL;
                            }
                        }
                    }
                }
                else if ((*node) == (*parent)->center)
                {
                    removeu = 1;
                    if ((*parent)->numKeys == 1)
                    {
                        if (((*parent)->left)->numKeys == 2)
                        {
                            (*node)->info1 = (*parent)->info1;
                            (*parent)->info1 = ((*parent)->left)->info2;
                            ((*parent)->left)->info2 = NULL;
                            ((*parent)->left)->numKeys = 1;
                        }
                        else
                        {
                            ((*parent)->left)->info2 = (*parent)->info1;
                            free(*node);
                            ((*parent)->left)->numKeys = 2;
                            *aux = (*parent)->left;
                            free(*parent);
                            *parent = *aux;
                        }
                    }
                    else
                    {
                        (*node)->info1 = (*parent)->info2;
                        parentNo = *parent;
                        findSmallestRight((*parent)->right, &no, &parentNo);
                        (*parent)->info2 = no->info1;

                        if (no->numKeys == 2)
                        {
                            no->info1 = no->info2;
                            no->info2 = NULL;
                            no->numKeys = 1;
                        }
                        else
                        {
                            (*node)->numKeys = 2;
                            (*node)->info2 = (*parent)->info2;
                            (*parent)->info2 = NULL;
                            (*parent)->numKeys = 1;
                            free(no);
                            (*parent)->right = NULL;
                        }
                    }
                }
                else
                {
                    removeu = 1;
                    parentNo = *parent;
                    findLargestLeft((*parent)->center, &no, &parentNo);

                    if (no->numKeys == 1)
                    {
                        no->info2 = (*parent)->info2;
                        (*parent)->info2 = NULL;
                        (*parent)->numKeys = 1;
                        no->numKeys = 2;
                        free(*node);
                        *node = NULL;
                    }
                    else
                    {
                        (*node)->info1 = (*parent)->info2;
                        (*parent)->info2 = no->info2;
                        no->info2 = NULL;
                        no->numKeys = 1;
                    }
                }
            } 
        }
        else
        { // se nao é folha
            if (key->start < (*node)->info1->start)
                removeu = removeFromMemory(node, &(*node)->left, key);
            else if (key->start == (*node)->info1->start)
            {
                parentNo = *node;
                findSmallestRight((*node)->center, &no, &parentNo);
                (*node)->info1 = no->info1;
                removeFromMemory(node, &(*node)->center, (*node)->info1);
                removeu = 1;
            }
            else if (((*node)->numKeys == 1) || key->start < (*node)->info1->start)
            {
                removeu = removeFromMemory(node, &(*node)->center, key);
            }
            else if (key->start == (*node)->info1->start)
            {
                parentNo = *parent;
                findSmallestRight((*parent)->right, &no, &parentNo);
                (*node)->info2 = no->info1;
                removeFromMemory(node, &(*node)->right, (*node)->info2);
                removeu = 1;
            }
            else
            {
                removeu = removeFromMemory(node, &(*node)->right, key);
            }
        }
    }
    return removeu;
}

int mergeNodes(Memory **root, int *return_start)
{
    int return_end = -1;
    if (root != NULL)
    {
        Memory *current = *root;

        // Verifica se o nó possui ambas as informações
        if (current->info1 && current->info2)
        {
            // Verifica se as informações possuem o mesmo status e são contíguas
            if (current->info1->status == current->info2->status &&
                current->info1->end + 1 == current->info2->start)
            {
                // Atualiza o final da primeira informação
                current->info1->end = current->info2->end;
                current->numKeys--;

                *return_start = current->info2->start;
                return_end = current->info2->end;
                printf("%d\n", return_end);
            }
        }

        // Verifica e mescla com o nó à esquerda
        if (current->left && current->info1 &&
            current->left->info2 &&
            current->left->info2->status == current->info1->status &&
            current->left->info2->end + 1 == current->info1->start)
        {
            current->left->info2->end = current->info1->end;
            current->numKeys--;

            *return_start = current->info1->start;
            return_end = current->info1->end;
        }

        // Verifica e mescla com o nó à direita
        if (current->right && current->info2 &&
            current->right->info1 &&
            current->info2->status == current->right->info1->status &&
            current->info2->end + 1 == current->right->info1->start)
        {
            current->info2->end = current->right->info1->end;
            current->right->numKeys--;

            *return_start = current->info2->start;
            return_end = current->info2->end;
        }

        // Chamada recursiva para os filhos
        if (current->left)
        {
            int left_end = mergeNodes(&current->left, return_start);
            if (left_end > return_end)
                return_end = left_end;
        }
        if (current->center)
        {
            int center_end = mergeNodes(&current->center, return_start);
            if (center_end > return_end)
                return_end = center_end;
        }
        if (current->right)
        {
            int right_end = mergeNodes(&current->right, return_start);
            if (right_end > return_end)
                return_end = right_end;
        }
    }
    return return_end;
}