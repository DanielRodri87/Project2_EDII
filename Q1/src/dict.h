#ifndef DICT_H
#define DICT_H

typedef struct EnglishBin {
    char *englishWord;
    struct EnglishBin *right, *left;
    int unit;
} EnglishBin;

typedef struct Info {
    char *portugueseWord;
    EnglishBin *englishWord;
} Info;

typedef struct Portuguese23 {
    Info info1;
    Info info2;
    struct Portuguese23 *center;
    struct Portuguese23 *right;
    struct Portuguese23 *left;
    int infoCount;
} Portuguese23;



// ------------------------------------ 2-3 -------------------------------------------
void exibirEmOrdem23(Portuguese23 *raiz);
void printBinaryTree(EnglishBin *root);
Portuguese23 *searchWord(Portuguese23 **node, const char *portugueseWord);
int insertPortugueseWord(Portuguese23 **tree, char *portugueseWord, char *englishWord, int unit);
void addEnglishTranslation(Info *info, const char *englishWord, int unit);

Info createInfo(char *word, char *englishWord, int unit);
Portuguese23 *createNode(const Info *info, Portuguese23 *leftChild, Portuguese23 *centerChild);
Portuguese23 *addKey(Portuguese23 *node, const Info *info, Portuguese23 *child);

Portuguese23 *splitNode(Portuguese23 **node, const Info *info, Info *promote, Portuguese23 **child);
int isLeaf(const Portuguese23 *node);
Portuguese23 *insertTree23(Portuguese23 **node, Info *info, Info *promote, Portuguese23 **parent);

void freeInfo23(Info *info);
void freeTree(Portuguese23 *node);
void displayTree23(const Portuguese23 *root);

void printWordsByUnit(Portuguese23 *tree, int unit);
void displayPortugueseTranslation(Portuguese23 **root, const char *portugueseWord);
void printTranslations(EnglishBin *node, int unit, const char *portugueseWord);

void smallestInfoRight(Portuguese23 *root, Portuguese23 **node, Portuguese23 **parentNode);
void largestInfoLeft(Portuguese23 *root, Portuguese23 **node, Portuguese23 **parentNode);
int remove23(Portuguese23 **parent, Portuguese23 **root, char *value);



// ------------------------------------ Binary -------------------------------------------
EnglishBin* createBinaryNode(const char* word, int unit);
EnglishBin* insertEnglishWord(EnglishBin* root, const char* word, int unit);
void addTranslation(Portuguese23 *node, const char *portugueseWord, const char *englishWord, int unit);

int isBinaryLeaf(EnglishBin *root);
EnglishBin *onlyOneChild(EnglishBin *root);
EnglishBin *smallestChild(EnglishBin *root);

void printBinaryTree(EnglishBin *root);
int removeEnglishWord(EnglishBin **root, char *word);
void searchEnglishWord(Portuguese23 **root, char *englishWord, int unit, Portuguese23 **parent);

void FreeTreeBinary(EnglishBin *root);

#endif
