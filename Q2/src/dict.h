#ifndef DICTIONARY_H
#define DICTIONARY_H

typedef struct BinaryTreeInfo
{
    char *englishWord;
    char *unitName;
} BinaryTreeInfo;

typedef struct BinaryTree
{
    BinaryTreeInfo info;
    struct BinaryTree *left;
    struct BinaryTree *right;
} BinaryTree;

typedef enum Color
{
    Red = 0,
    Black = 1
} Color;

typedef struct MainInfo
{
    BinaryTree *binaryTreeEnglishWord;
    char *portugueseWord;
} MainInfo;

typedef struct RedBlackTree
{
    MainInfo info;
    Color color;
    struct RedBlackTree *left;
    struct RedBlackTree *right;
} RedBlackTree;

typedef enum InputType
{
    UnitName,
    EnglishPortugueseWord,
} InputType;

// File Handling
InputType ReadIdentifyLine(const char *line);
int PopulateTreeFromLine(RedBlackTree **root, const char *line, char *unitName);
int ReadBlocks(RedBlackTree **root);

// Red-black tree
void AllocateRedBlackTree(RedBlackTree **root);
void FreeRedBlackTree(RedBlackTree **root);
void DefineRedBlackTree(RedBlackTree *root, char *engWord, char *portWord, char *unitName);
void DisplayRedBlackTree(const RedBlackTree *root);
int InsertRedBlackTree(RedBlackTree **root, RedBlackTree *newNode);
int RemoveRedBlackTree(RedBlackTree **root, char *engWord);

// Display Functions
void DisplayRedBlackTreeByUnit(const RedBlackTree *root, const char *unit);
void DisplayRedBlackTreeByPortugueseWord(const RedBlackTree *root, const char *word);
void ShowBinaryTree(BinaryTree *root);

// Binary Tree Functions
void AllocateBinaryTree(BinaryTree **root);
void DefineBinaryTree(BinaryTree *root, char *englishWord, char *unitName);
void FreeBinaryTreeInfo(BinaryTreeInfo *info);
void FreeBinaryTree(BinaryTree **root);
int UpdateUnitName(BinaryTree *root, char *unitName);
int InsertBinaryTree(BinaryTree **root, BinaryTree *newNode);
BinaryTree *OnlyOneChild(BinaryTree **root);
int IsLeaf(BinaryTree **root);
BinaryTree *SmallestChild(BinaryTree **root);
void FreeValue(BinaryTree **root);
int RemoveBinaryTree(BinaryTree **root, const char *englishWord);

// Auxiliary Functions
int StringLength(char *string);
int AllocateString(char **string, int size);
int NamePresent(const char *nameToFind, const char *names);
void JoinNameWithComma(char *oldName, char *newName, char **merged);
void RemoveComma(char *word);

#endif // DICTIONARY_H
