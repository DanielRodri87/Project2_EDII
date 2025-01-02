#ifndef DICT_H
#define DICT_H

typedef struct linkedUnitList {
  char *unitName;
  struct linkedUnitList *next;
} LinkedUnitList;

typedef struct EnglishBinTree {
    LinkedUnitList *units;
    char *englishWord;
    struct EnglishBinTree *right, *left;
} EnglishBinTree;

typedef struct translationInfo {
    char *portugueseWord;
    EnglishBinTree *englishWord;
} TranslationInfo;

typedef struct Portuguese23Tree {
    TranslationInfo info1;
    TranslationInfo info2;
    struct Portuguese23Tree *middle;
    struct Portuguese23Tree *right;
    struct Portuguese23Tree *left;
    int infoCount;
} Portuguese23Tree;


// Main.c
int insertPortugueseWord(Portuguese23Tree **tree, char *portugueseWord, char *englishWord, char *unit);
void displayWordsForUnit(Portuguese23Tree *tree, char *unit);
void displayTranslationPortuguese(Portuguese23Tree **root, const char *portugueseWord);

int removeEnglishFindUnit(Portuguese23Tree **root, const char *englishWord, const char *unit);
int removePortugueseWordFindUnit(Portuguese23Tree **root, char *portugueseWord, const char *unit);
void displayTree23(const Portuguese23Tree *root);

void freeTree(Portuguese23Tree *no);
void displayTranslationPortuguesePath(Portuguese23Tree *root, int nivel);

// Auxiliares
int insertenglishWord(EnglishBinTree **root, TranslationInfo *info);
Portuguese23Tree *InsertTree23(Portuguese23Tree **no, TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree **father);
int InsertLinkedList(LinkedUnitList **list, char *name);

void printBinaryTree(EnglishBinTree *root);
void displayTranslacions(EnglishBinTree *node, char *unit, const char *portugueseWord);
int sourceLinkedList(const LinkedUnitList *list, const char *name_unit);

void showLinkedList(const LinkedUnitList *list);
void freeBinaryTree(EnglishBinTree *root);
int auxRemoveEnglishFindUnit(Portuguese23Tree *root, const char *englishWord, const char *unit, Portuguese23Tree **top);

int tree23Remove1(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **maior);
int tree23Remove2(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **maior);

#endif
