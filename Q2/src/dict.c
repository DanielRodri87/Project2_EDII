#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InsertTreeBin(BinaryTreeNode **node, TreeNodeInfo info)
{
  if (node == NULL)
  {
    *node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    (*node)->info = info;
    (*node)->left = NULL;
    (*node)->right = NULL;
  }
  else
  {
      if (strcmp((*node)->info.englishWord, info.englishWord) > 0)
      InsertTreeBin(&(*node)->left, info);
    else
      InsertTreeBin(&(*node)->right, info);
  }
}

void CreateNode(RedBlackTreeNode **node, RBTreeNodeInfo info)
{
  *node = (RedBlackTreeNode *)malloc(sizeof(RedBlackTreeNode));
  (*node)->info = info;
  (*node)->info.portugueseWord = strdup(info.portugueseWord);
  (*node)->color = RED;
  (*node)->left = NULL;
  (*node)->right = NULL;
}

void replacementColor(RedBlackTreeNode **node)
{
  (*node)->color = !(*node)->color;
  if ((*node)->left != NULL)
    (*node)->left->color = !(*node)->left->color;
  if ((*node)->right != NULL)
    (*node)->right->color = !(*node)->right->color;
}

void leftRotate(RedBlackTreeNode **node)
{
  RedBlackTreeNode *temp = (*node)->right;
  (*node)->right = temp->left;
  temp->left = *node;
  (*node)->color = temp->color;
  temp->color = RED;
  *node = temp;
}

void rightRotate(RedBlackTreeNode **node)
{
  RedBlackTreeNode *temp = (*node)->left;
  (*node)->left = temp->right;
  temp->right = *node;
  (*node)->color = temp->color;
  temp->color = RED;
  *node = temp;
}

int getColor(RedBlackTreeNode *node)
{
  int result = 1;
  if (node != NULL)
    result = node->color;
  return (result);
}

void TreeBalance(RedBlackTreeNode **node)
{
  if (getColor((*node)->left) == BLACK && getColor((*node)->right) == RED)
    leftRotate(node);
  if ((*node)->left != NULL && (*node)->left->color == RED && (*node)->left->left != NULL && (*node)->left->left->color == RED)
    rightRotate(node);
  if (getColor((*node)->left) == RED && getColor((*node)->right) == RED)
    replacementColor(node);
}

int insertRedBlackTreeNode(RedBlackTreeNode **node, RBTreeNodeInfo info)
{
  int insert = 0;
  if ((*node) == NULL)
  {
    CreateNode(node, info);
    insert = 1;
  }
  else
  {
    if (strcmp((*node)->info.portugueseWord, info.portugueseWord) == 0)
    {
      InsertTreeBin(&(*node)->info.binaryTreeEnglish, info.binaryTreeEnglish->info);
      insert = 1;
    }
    else if (strcmp((*node)->info.portugueseWord, info.portugueseWord) > 0)
      insert = insertRedBlackTreeNode(&(*node)->left, info);
    else
      insert = insertRedBlackTreeNode(&(*node)->left, info);

    TreeBalance(node);
  }
  return (insert);
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - informar uma unidade e então imprima todas as palavras da unidade em português seguida das
// equivalentes em inglês
// ---------------------------------------------------- XXXXXX -------------------------------------------------

void printWordsByUnit(RedBlackTreeNode *node, int unit)
{
    if (node != NULL)
    {
        if (node->info.binaryTreeEnglish->info.unit == unit)
        {
            printf("PT: %s\n", node->info.portugueseWord);
            printf("ENG: %s\n", node->info.binaryTreeEnglish->info.englishWord);
        }

        printWordsByUnit(node->left, unit);
        printWordsByUnit(node->right, unit);
    }
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - informar uma palavra em português e então imprima todas as palavras em inglês equivalente à palavra em
// português dada, independente da unidade
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void findEnglishByPortuguese(RedBlackTreeNode *node, const char *portugueseWord)
{
  if (node != NULL)
  {
    if (strcmp(node->info.portugueseWord, portugueseWord) == 0)
      printWordsByUnit(node, node->info.binaryTreeEnglish->info.unit);

    findEnglishByPortuguese(node->left, portugueseWord);
    findEnglishByPortuguese(node->right, portugueseWord);
  }
}


// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III - informar uma palavra em inglês e a unidade a qual a mesma pertence removê-la das árvores binárias
// das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da
// árvore 2-3
// ---------------------------------------------------- XXXXXX -------------------------------------------------
int isLeaf(RedBlackTreeNode *node)
{
  return (node->left == NULL && node->right == NULL);
}

RedBlackTreeNode *soumfilho(RedBlackTreeNode *node)
{
  RedBlackTreeNode *aux;
  aux = NULL;
  if (node->left == NULL)
    aux = node->right;
  else if (node->right == NULL)
    aux = node->left;

  return (aux);
}

RedBlackTreeNode *SmallChild(RedBlackTreeNode *raiz)
{
  RedBlackTreeNode *aux;
  aux = NULL;
  if (raiz)
  {
    aux = SmallChild(raiz->left);
    if (!aux)
      aux = raiz;
  }
  return aux;
}