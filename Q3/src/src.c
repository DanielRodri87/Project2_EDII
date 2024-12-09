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

// void MergeAdjacentBlocks(Memory *root)
// {

// }
