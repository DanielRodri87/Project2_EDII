#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//erro encontrado: o node tem que ser passado por referência para que a árvore seja atualizada de forma correta
void InsertTreeBin(BinaryTreeNode **node, TreeNodeInfo info)
{
  if (*node == NULL)
  {
    *node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    (*node)->info = info;
    (*node)->left = NULL;
    (*node)->right = NULL;
  }
  else
  {
    if (strcmp(info.englishWord, (*node)->info.englishWord) < 0)
      InsertTreeBin(&(*node)->left,info);
    else
      InsertTreeBin(&(*node)->right,info);
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

//adicionei uma verificação para ver se o nó é null antes de acessar ele, concorda?
void replacementColor(RedBlackTreeNode **node)
{

  if (node == NULL || *node == NULL)
    return;

  (*node)->color = !(*node)->color;
  if ((*node)->left != NULL)
    (*node)->left->color = !(*node)->left->color;
  if ((*node)->right != NULL)
    (*node)->right->color = !(*node)->right->color;
}

//adicionei 3 verificações no inicio para evitar erros se o nó ou o seu filho direito forem nulos.
void leftRotate(RedBlackTreeNode **node)
{
    if (node == NULL || *node == NULL || (*node)->right == NULL)
        return; 

    RedBlackTreeNode *temp = (*node)->right;

    (*node)->right = temp->left;
    temp->left = *node;

    temp->color = (*node)->color; 
    (*node)->color = RED;      

    *node = temp;
}

//alterei para fazer 3 verificações no inicio e para ajustar a cor após a rotação
void rightRotate(RedBlackTreeNode **node)
{

  if (node == NULL || *node == NULL || (*node)->left == NULL)
  return;


  RedBlackTreeNode *temp = (*node)->left;
  (*node)->left = temp->right;
  temp->right = *node;
  //(*node)->color = temp->color;
  //temp->color = RED;
  *node = temp;

  (*node)->right->color = RED;
  (*node)->color = BLACK;
}

int getColor(RedBlackTreeNode *node)
{
  int result = 1;
  if (node != NULL)
    result = node->color;
  return (result);
}

//alterei muita coisa aqui: funções estão sendo chamadas de nomes errados
void TreeBalance(RedBlackTreeNode **node)
{
  if (getColor((*node)->left) == BLACK && getColor((*node)->right) == RED)
    leftRotate(node);
  if ((*node)->left != NULL && (*node)->left->color == RED && (*node)->left->left != NULL && (*node)->left->left->color == RED)
    rightRotate(node);
  if (getColor((*node)->left) == RED && getColor((*node)->right) == RED)
    replacementColor(node);
}

//alterei no final de left para right
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
      insert = insertRedBlackTreeNode(&(*root)->right, info);

    TreeBalance(root);
  }
  return (insert);
}
