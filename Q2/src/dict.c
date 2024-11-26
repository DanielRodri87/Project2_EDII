#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AllocateBinaryTree(BinaryTree **root)
{
  *root = (BinaryTree *)malloc(sizeof(BinaryTree));
  (*root)->left = NULL;
  (*root)->right = NULL;
}

void DefineBinaryTree(BinaryTree *root, char *englishWord, char *unitName)
{
  AllocateString(&root->info.englishWord, StringLength(englishWord));
  AllocateString(&root->info.unitName, StringLength(unitName));
  strcpy(root->info.englishWord, englishWord);
  strcpy(root->info.unitName, unitName);
}

void FreeBinaryTreeInfo(BinaryTreeInfo *info)
{
  free(info->unitName);
  free(info->englishWord);
}

void FreeBinaryTree(BinaryTree **root)
{
  if (*root)
  {
    FreeBinaryTree(&(*root)->left);
    FreeBinaryTree(&(*root)->right);
    FreeBinaryTreeInfo(&(*root)->info);
    free(*root);
  }
}

int UpdateUnitName(BinaryTree *root, char *unitName)
{
  int confirmed = 1;

  if (root)
    if (!NamePresent(unitName, root->info.unitName))
    {
      char *newName = NULL;
      JoinNameWithComma(root->info.unitName, unitName, &newName);
      free(root->info.unitName);
      root->info.unitName = newName;
    }
    else
      confirmed = 0;

  return confirmed;
}

int InsertBinaryTree(BinaryTree **root, BinaryTree *newNode)
{
  int success = 1;

  if (!*root)
    *root = newNode;
  else if (strcmp((*root)->info.englishWord, newNode->info.englishWord) > 0)
    success = InsertBinaryTree(&(*root)->left, newNode);
  else if (strcmp((*root)->info.englishWord, newNode->info.englishWord) < 0)
    success = InsertBinaryTree(&(*root)->right, newNode);
  else
    // If the word already exists, only update the units where it appeared.
    success = UpdateUnitName(*root, newNode->info.unitName);

  return success;
}

BinaryTree *OnlyOneChild(BinaryTree **root)
{
  BinaryTree *aux = NULL;
  if ((*root)->left == NULL && (*root)->right != NULL)
    aux = (*root)->right;
  else if ((*root)->left != NULL && (*root)->right == NULL)
    aux = (*root)->left;
  return aux;
}

int IsLeaf(BinaryTree **root)
{
  int isLeaf = 0;
  if ((*root)->left == NULL && (*root)->right == NULL)
    isLeaf = 1;
  return isLeaf;
}

BinaryTree *SmallestChild(BinaryTree **root)
{
  BinaryTree *aux = NULL;
  if ((*root)->left)
    aux = SmallestChild(&(*root)->left);
  else
    aux = *root;

  return aux;
}

void FreeValue(BinaryTree **root)
{
  FreeBinaryTreeInfo(&(*root)->info);
  free(*root);
}

int RemoveBinaryTree(BinaryTree **root, const char *englishWord)
{
  int confirmed = 1;
  BinaryTree *aux = NULL;

  if (*root)
  {
    if (strcmp((*root)->info.englishWord, englishWord) == 0)
    {
      if (IsLeaf(root))
      {
        FreeValue(root);
        *root = NULL;
      }
      else if ((aux = OnlyOneChild(root)))
      {
        FreeValue(root);
        *root = aux;
      }
      else
      {
        aux = SmallestChild(&(*root)->right);
        FreeBinaryTreeInfo(&(*root)->info);
        DefineBinaryTree(*root, aux->info.englishWord, aux->info.unitName);
        RemoveBinaryTree(&(*root)->right, aux->info.englishWord);
      }
    }
  }
  else
    confirmed = 0;

  return confirmed;
}

void ShowBinaryTree(BinaryTree *root)
{
  if (root)
  {
    ShowBinaryTree(root->left);
    printf("English: %s\n", root->info.englishWord);
    printf("Appeared in: %s\n", root->info.unitName);
    ShowBinaryTree(root->right);
  }
}

void AllocateRedBlackTreeNode(RedBlackTree **root)
{
    *root = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    (*root)->color = Red;
    (*root)->left = NULL;
    (*root)->right = NULL;
}

void DefineRedBlackTreeNode(RedBlackTree *root, char *englishWord, char *portugueseWord, char *unitName)
{
    AllocateBinaryTree(&root->info.binaryTreeEnglishWord);
    DefineBinaryTree(root->info.binaryTreeEnglishWord, englishWord, unitName);
    AllocateString(&root->info.portugueseWord, StringLength(portugueseWord));
    strcpy(root->info.portugueseWord, portugueseWord);
}

void FreeRedBlackTreeNode(RedBlackTree **root)
{
    FreeBinaryTree(&(*root)->info.binaryTreeEnglishWord);
    if ((*root)->info.portugueseWord)
        free((*root)->info.portugueseWord);
    free(*root);
}

void FreeRedBlackTree(RedBlackTree **root)
{
    if (*root)
    {
        FreeRedBlackTree(&(*root)->left);
        FreeRedBlackTree(&(*root)->right);
        FreeRedBlackTreeNode(root);
    }
}

static void RotateLeft(RedBlackTree **root)
{
    RedBlackTree *aux = (*root)->right;
    (*root)->right = aux->left;
    aux->left = *root;
    aux->color = (*root)->color;
    (*root)->color = Red;
    *root = aux;
}

static void RotateRight(RedBlackTree **root)
{
    RedBlackTree *aux = (*root)->left;
    (*root)->left = aux->right;
    aux->right = *root;
    aux->color = (*root)->color;
    (*root)->color = Red;
    *root = aux;
}

static void SwapColor(RedBlackTree **root)
{
    (*root)->color = !(*root)->color;
    if ((*root)->left)
        (*root)->left->color = !(*root)->left->color;
    if ((*root)->right)
        (*root)->right->color = !(*root)->right->color;
}

static Color GetColor(const RedBlackTree *root)
{
    return root ? root->color : Black;
}

static void BalanceTree(RedBlackTree **root)
{
    if (GetColor((*root)->right) == Red)
        RotateLeft(root);
    if ((*root)->left && GetColor((*root)->left) == Red && GetColor((*root)->left->left) == Red)
        RotateRight(root);
    if (GetColor((*root)->left) == Red && GetColor((*root)->right) == Red)
        SwapColor(root);
}

static int InsertRedBlackTreeNode(RedBlackTree **root, RedBlackTree *newNode)
{
    int result = 1;

    if (!*root)
        *root = newNode;
    else if (strcmp((*root)->info.portugueseWord, newNode->info.portugueseWord) > 0)
        result = InsertRedBlackTreeNode(&(*root)->left, newNode);
    else if (strcmp((*root)->info.portugueseWord, newNode->info.portugueseWord) < 0)
        result = InsertRedBlackTreeNode(&(*root)->right, newNode);
    else
        result = InsertBinaryTree(&(*root)->info.binaryTreeEnglishWord, newNode->info.binaryTreeEnglishWord);

    BalanceTree(root);
    return result;
}

int InsertRedBlackTree(RedBlackTree **root, RedBlackTree *newNode)
{
    int result = InsertRedBlackTreeNode(root, newNode);
    (*root)->color = Black;
    return result;
}

static void MoveLeftRed(RedBlackTree **root)
{
    SwapColor(root);
    if (GetColor((*root)->right->left) == Red)
    {
        RotateRight(&(*root)->right);
        RotateLeft(root);
        SwapColor(root);
    }
}

static void MoveRightRed(RedBlackTree **root)
{
    SwapColor(root);
    if (GetColor((*root)->left->left) == Red)
    {
        RotateRight(root);
        SwapColor(root);
    }
}

static RedBlackTree *FindSmallest(RedBlackTree *root)
{
    RedBlackTree *node = root;
    while (node->left)
        node = node->left;
    return node;
}

static int RemoveSmallest(RedBlackTree **root)
{
    if (!(*root)->left)
    {
        FreeRedBlackTree(root);
        *root = NULL;
    }
    else
    {
        if (GetColor((*root)->left) == Black && GetColor((*root)->left->left) == Black)
            MoveLeftRed(root);
        RemoveSmallest(&(*root)->left);
    }

    if (*root)
        BalanceTree(root);
    return 1;
}

static int RemoveRedBlackTreeNode(RedBlackTree **root, char *englishWord)
{
    if (*root)
    {
        if (strcmp(englishWord, (*root)->info.portugueseWord) < 0)
        {
            if (GetColor((*root)->left) == Black && GetColor((*root)->left->left) == Black)
                MoveLeftRed(root);
            RemoveRedBlackTreeNode(&(*root)->left, englishWord);
        }
        else
        {
            if (GetColor((*root)->left) == Red)
                RotateRight(root);
            if (strcmp((*root)->info.portugueseWord, englishWord) == 0 && !(*root)->right)
            {
                FreeRedBlackTreeNode(root);
                *root = NULL;
            }
            else
            {
                if (GetColor((*root)->right) == Black && GetColor((*root)->right->left) == Black)
                    MoveRightRed(root);
                if (strcmp((*root)->info.portugueseWord, englishWord) == 0)
                {
                    RedBlackTree *aux = FindSmallest((*root)->right);
                    DefineRedBlackTreeNode(*root, aux->info.portugueseWord, aux->info.portugueseWord, aux->info.portugueseWord);
                    RemoveSmallest(&(*root)->right);
                }
                else
                    RemoveRedBlackTreeNode(&(*root)->right, englishWord);
            }
        }
    }

    if (*root)
        BalanceTree(root);
    return 1;
}

int RemoveRedBlackTree(RedBlackTree **root, char *englishWord)
{
    int result = RemoveRedBlackTreeNode(root, englishWord);
    if (*root)
        (*root)->color = Black;
    return result;
}

void DisplayRedBlackTree(const RedBlackTree *root)
{
    if (root)
    {
        DisplayRedBlackTree(root->left);

        printf("Color: %s\n", root->color == Red ? "Red" : "Black");
        printf("Portuguese Word: %s\n", root->info.portugueseWord);
        ShowBinaryTree(root->info.binaryTreeEnglishWord);

        DisplayRedBlackTree(root->right);
    }
}
