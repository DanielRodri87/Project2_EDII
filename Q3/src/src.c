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
    return (info);
}

Memory *CreateNode(Info *info, Memory *leftNode, Memory *leftCenterNode)
{
    Memory *node = (Memory *)malloc(sizeof(Memory));
    
    node->info1 = info;
    node->numKeys = 1;
    node->left = leftNode;
    node->leftCenter = leftCenterNode;
    node->center = NULL;
    node->rightCenter = NULL;
    node->right = NULL;

    return (node);
}

int IsLeaf(Memory *node)
{
    int status;
    if (node->left == NULL)
        status = 1;
    else
        status = 0;

    return (status);
}


void AddInfo(Memory **node, Info *info, Memory *child)
{
    if ((*node)->numKeys == 1)
    {
        if (info->status == (*node)->info1->status)
        {
            (*node)->info1->end = info->end;
        }
        else if (info->start > (*node)->info1->start)
        {
            (*node)->info2 = info;
            (*node)->center = child;
            (*node)->numKeys = 2;
        }
        else
        {
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            (*node)->center = (*node)->leftCenter;
            (*node)->leftCenter = child;
            (*node)->numKeys = 2;
        }
    }
    else if ((*node)->numKeys == 2)
    {
        if (info->status == (*node)->info2->status)
        {
            (*node)->info2->end = info->end;
        }
        else if (info->start > (*node)->info2->start)
        {
            (*node)->info3 = info;
            (*node)->rightCenter = child;
            (*node)->numKeys = 3;
        }
        else if (info->start > (*node)->info1->start && info->start < (*node)->info2->start)
        {
            (*node)->info3 = (*node)->info2;
            (*node)->info2 = info;
            (*node)->rightCenter = (*node)->center;
            (*node)->center = child;
            (*node)->numKeys = 3;
        }
        else
        {
            (*node)->info3 = (*node)->info2;
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            (*node)->rightCenter = (*node)->center;
            (*node)->center = (*node)->leftCenter;
            (*node)->leftCenter = child;
            (*node)->numKeys = 3;
        }
    }
    else
    {
        if (info->status == (*node)->info3->status)
        {
            (*node)->info3->end = info->end;
        }
        else if (info->start > (*node)->info3->start)
        {
            (*node)->info4 = info;
            (*node)->right = child;
            (*node)->numKeys = 4;
        }
        else if (info->start > (*node)->info2->start && info->start < (*node)->info3->start)
        {
            (*node)->info4 = (*node)->info3;
            (*node)->info3 = info;
            (*node)->right = (*node)->rightCenter;
            (*node)->rightCenter = child;
            (*node)->numKeys = 4;
        }
        else if (info->start > (*node)->info1->start && info->start < (*node)->info2->start)
        {
            (*node)->info4 = (*node)->info3;
            (*node)->info3 = (*node)->info2;
            (*node)->info2 = info;
            (*node)->right = (*node)->rightCenter;
            (*node)->rightCenter = (*node)->center;
            (*node)->center = child;
            (*node)->numKeys = 4;
        }
        else
        {
            (*node)->info4 = (*node)->info3;
            (*node)->info3 = (*node)->info2;
            (*node)->info2 = (*node)->info1;
            (*node)->info1 = info;
            (*node)->right = (*node)->rightCenter;
            (*node)->rightCenter = (*node)->center;
            (*node)->center = (*node)->leftCenter;
            (*node)->leftCenter = child;
            (*node)->numKeys = 4;
        }
    }
}

Split SplitNode(Memory **root, Info *info, Memory *child)
{
    Memory *largestNode;
    Info *promote = NULL;
    Split itBroke;

    if (info->start > (*root)->info4->start && (*root)->info4 != NULL)
    {
        promote = CreateInfo((*root)->info3->start, (*root)->info3->end, (*root)->info3->status);
        largestNode = CreateNode((*root)->info4, (*root)->rightCenter, (*root)->right);
        largestNode->info2 = info;
        largestNode->center = child;
        largestNode->numKeys = 2;
    }
    else if (info->start > (*root)->info3->start)
    {
        promote = (*root)->info3;
        largestNode = CreateNode(info, (*root)->rightCenter, child);
        largestNode->info2 = (*root)->info4;
        largestNode->center = (*root)->right;
        largestNode->numKeys = 2;
    }
    else if (info->start > (*root)->info2->start)
    {
        promote = info;
        largestNode = CreateNode((*root)->info3, (*root)->center, (*root)->rightCenter);
        largestNode->info2 = (*root)->info4;
        largestNode->center = (*root)->right;
        largestNode->numKeys = 2;
    }
    else if (info->start > (*root)->info1->start)
    {
        promote = (*root)->info2;
        largestNode = CreateNode((*root)->info3, (*root)->center, (*root)->rightCenter);
        largestNode->info2 = (*root)->info4;
        largestNode->center = (*root)->right;
        largestNode->numKeys = 2;
        (*root)->info2 = info;
        (*root)->center = child;
    }
    else
    {
        promote = (*root)->info2;
        largestNode = CreateNode((*root)->info3, (*root)->center, (*root)->rightCenter);
        largestNode->info2 = (*root)->info4;
        largestNode->center = (*root)->right;
        largestNode->numKeys = 2;
        (*root)->info2 = (*root)->info1;
        (*root)->center = (*root)->leftCenter;
        (*root)->info1 = info;
        (*root)->leftCenter = child;
    }

    (*root)->numKeys = 2;
    (*root)->rightCenter = NULL;
    (*root)->right = NULL;

    (*root)->info3 = NULL;
    (*root)->info4 = NULL;

    itBroke.largestNode = largestNode;
    itBroke.promote = promote;

    return (itBroke);
}

void Insert45(Memory **root, Memory *parent, Info **promote, int start, int end, int status, int *flag)
{
    if (*root == NULL)
    {
        Info *nova_info = CreateInfo(start, end, status);
        *root = CreateNode(nova_info, NULL, NULL);
        *flag = 1; 
    }
    else
    {
        if (IsLeaf(*root))
        {
            *flag = 1; 
            if ((*root)->numKeys < 4)
            {
                Info *nova_info = CreateInfo(start, end, status);
                AddInfo(root, nova_info, NULL);
            }
            else
            {
                Info *nova_info = CreateInfo(start, end, status);
                Split resultado = SplitNode(root, nova_info, NULL);

                if (!parent)
                {
                    *root = CreateNode(resultado.promote, *root, resultado.largestNode);
                }
                else
                {
                    if (parent->numKeys < 4)
                    {
                        AddInfo(&parent, resultado.promote, resultado.largestNode);
                    }
                    else
                    {
                        resultado = SplitNode(&parent, resultado.promote, resultado.largestNode);
                        parent = CreateNode(resultado.promote, parent, resultado.largestNode);
                    }
                }
            }
        }
        else
        {
            if (start < (*root)->info1->start)
                Insert45(&(*root)->left, *root, promote, start, end, status, flag);

            else if ((*root)->numKeys == 1 || ((*root)->numKeys <= 2 && start < (*root)->info2->start))
                Insert45(&((*root)->leftCenter), *root, promote, start, end, status, flag);

            else if (((*root)->numKeys == 2) || ((*root)->numKeys <= 2 && start < (*root)->info3->start))
                Insert45(&((*root)->center), *root, promote, start, end, status, flag);

            else
                Insert45(&((*root)->right), *root, promote, start, end, status, flag);
        }
    }
}

Memory *FindSpace(Memory *root, int requiredSpace)
{
    Memory *found = NULL;
    if (root != NULL)
    {
        if (IsLeaf(root))
        {
            int espacoDisponivel1 = root->info1->end - root->info1->start;
            int espacoDisponivel2 = (root->numKeys >= 2) ? root->info2->end - root->info2->start : 0;
            int espacoDisponivel3 = (root->numKeys >= 3) ? root->info3->end - root->info3->start : 0;
            int espacoDisponivel4 = (root->numKeys == 4) ? root->info4->end - root->info4->start : 0;

            if (root->info1->status == FREE && espacoDisponivel1 >= requiredSpace)
                found = root;
            else if (root->numKeys >= 2 && root->info2->status == FREE && espacoDisponivel2 >= requiredSpace)
                found = root;
            else if (root->numKeys >= 3 && root->info3->status == FREE && espacoDisponivel3 >= requiredSpace)
                found = root;
            else if (root->numKeys == 4 && root->info4->status == FREE && espacoDisponivel4 >= requiredSpace)
                found = root;
            
        }
        else
        {
            found = FindSpace(root->left, requiredSpace);
            if (root->left != NULL)
                FindSpace(root->left, requiredSpace);
        }
    }
    return (found);
}

Memory *SourceSpace(Memory *root, int RequiredSpace)
{
    return FindSpace(root, RequiredSpace);
}