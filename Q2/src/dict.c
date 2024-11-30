#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InsertTreeBin(BinaryTreeNode *node, TreeNodeInfo info)
{
  if (node == NULL)
  {
    node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    node->info = info;
    node->left = NULL;
    node->right = NULL;
  }
  else
  {
    if (strcmp(info.englishWord, node->info.englishWord) < 0)
      InsertTreeBin(node->left, info);
    else
      InsertTreeBin(node->right, info);
    
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
  if (getgetColor((*node)->left) == BLACK && getColor((*node)->right) == RED)
    rotacaoleft(node);
  if ((*node)->left != NULL && (*node)->left->color == RED && (*node)->left->left != NULL && (*node)->left->left->color == RED)
    rotacaoright(node);
  if (getgetColor((*node)->left) == RED && getColor((*node)->right) == RED)
    replacementColor(node);
}

int insertRedBlackTreeNode(RedBlackTreeNode **root, RBTreeNodeInfo info)
{
  int insert = 0;
  if ((*root) == NULL)
  {
    CreateNode(root, info);
    insert = 1;
  }
  else
  {
    if (strcmp((*root)->info.portugueseWord, info.portugueseWord) == 0)
    {
      inserirArvBin(&(*root)->info.binaryTreeEnglish, info.binaryTreeEnglish->info);
      insert = 1;
    }
    else if (strcmp((*root)->info.portugueseWord, info.portugueseWord) > 0)
      insert = insertRedBlackTreeNode(&(*root)->left, info);
    else
      insert = insertRedBlackTreeNode(&(*root)->left, info);

    TreeBalance(root);
  }
  return (insert);
}
