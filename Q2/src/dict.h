#ifndef Q2_H
#define Q2_H

typedef enum color
{
  RED = 0,
  BLACK = 1
} Color;

typedef struct treeNodeInfo
{
  char *englishWord;
  int unit;
} TreeNodeInfo;

typedef struct binaryTreeNode
{
  TreeNodeInfo info;
  struct binaryTreeNode *left;
  struct binaryTreeNode *right;
} BinaryTreeNode;


typedef struct rbTreeNodeInfo
{
  BinaryTreeNode *binaryTreeEnglish;
  char *portugueseWord;
} RBTreeNodeInfo;

typedef struct redBlackTreeNode
{
  RBTreeNodeInfo info;

  Color color;
  struct redBlackTreeNode *left;
  struct redBlackTreeNode *right;
} RedBlackTreeNode;

/*
=====================================================================================================
*/

int insertRedBlackTreeNode(RedBlackTreeNode **root, RBTreeNodeInfo info);

void displayBinaryTree(BinaryTreeNode *root);

void removeRBTreeNodeByEN(RedBlackTreeNode **root, const char *englishWord, int unit);

void removeRBTreeNodeByPT(RedBlackTreeNode **root, char *portugueseWord, int unit);

#endif
