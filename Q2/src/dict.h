#ifndef DICT_H
#define DICT_H

typedef enum color
{
  RED = 0,
  BLACK = 1
} Color;

typedef struct units
{
  int unit;
  struct units *next;
} Units;

typedef struct infoEngPT
{
  char *englishWord;
  Units *units;
} InfoEngPT;

typedef struct engPT
{
  InfoEngPT info;
  struct engPT *left;
  struct engPT *right;
} EngPT;

typedef struct infoBin
{
  EngPT *binaryTreeEnglish;
  char *portugueseWord;
} InfoBin;

typedef struct rbtree
{
  InfoBin info;
  struct rbtree *left;
  struct rbtree *right;
  Color color;

} RBTree;

// Funções chamadas no main
void displayUnit(RBTree *root, int unit, int *found);
void findEnglishByPortuguese(RBTree *node, const char *portugueseWord);
void removeFromRBTreeEN(RBTree **root, const char *englishWord, int unit);

int insertIntoRBTree(RBTree **root, InfoBin info);
void removeFromRBTreePT(RBTree **root, char *portugueseWord, int unit);

// Auxiliares
void insertUnit(Units **units, int unit);
void insertBinaryTree(EngPT **root, InfoEngPT info);
void createNode(RBTree **node, InfoBin info);

void flipColors(RBTree **root);
void rotateLeft(RBTree **root);
void rotateRight(RBTree **root);

int getColor(RBTree *root);
void balanceTree(RBTree **root);
int insertIntoRBTree(RBTree **root, InfoBin info);

int isUnitPresent(Units *units, int unit);
void displayBinaryTree(EngPT *root, int unit);
int searchUnit(EngPT *root, int unit);
void displayUnit(RBTree *root, int unit, int *found);

#endif
