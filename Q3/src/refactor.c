#include "src.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Memory *insertTree23(Memory **node, Info *info, Info *promote, Memory **pai)
{
    Info promote1;
    Memory *biggerNode = NULL;
    if (*node == NULL)
        *node = createnodede(info, NULL, NULL);

    else
    {
        if (isLeaf(*node))
        { 
            if ((*node)->numKeys == 1)
                *node = addInfo(*node, info, NULL);
            else
            {
                biggerNode = quebranode(node, info, promote, NULL);
                if (pai == NULL)
                { 
                    *node = createnodede(promote, *node, biggerNode);
                    biggerNode = NULL;
                }
            }
        }
        else
        { 
            if (info->start < (*node)->info1->start)
                biggerNode = insertTree23(&((*node)->left), info, promote, node);
            else if ((*node)->numKeys == 1 || info->start < (*node)->info2->start)
                biggerNode = insertTree23(&((*node)->center), info, promote, node);
            else
                biggerNode = insertTree23(&((*node)->right), info, promote, node);
            if (biggerNode)
            {
                if ((*node)->numKeys == 1)
                {
                    *node = addInfo(*node, promote, biggerNode);
                    biggerNode = NULL;
                }
                else
                {
                    biggerNode = quebranode(node, promote, &promote1, &biggerNode);
                    if (pai == NULL)
                    {
                        *node = createnodede(&promote1, *node, biggerNode);
                        biggerNode = NULL;
                    }
                }
            }
        }
    }

    return (biggerNode);
}