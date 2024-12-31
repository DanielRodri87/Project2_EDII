#ifndef DICT_H
#define DICT_H

typedef struct units {
    int unit;
    struct units *next;
} Units;

typedef struct InfoBin {
    char *word;
    Units *units;
} InfoBin;

typedef struct engPT{
    InfoBin *info;
    struct engPT *left, *right;
} EngPT;

typedef struct info {
    char *word;
    EngPT *versionEnglish;
} Info;

typedef struct Tree23{
    Info info1;
    Info info2;
    struct tree23 *center;
    struct tree23 *left;
    struct tree23 *right;
    int infoCount;
} Tree23;

Tree23 *createNode(Info info, Tree23 *leftChild, Tree23 *centerChild);
Tree23 *insertTree23(Tree23 **node, Info info, Info *promote, Tree23 **father);
int insereArvBin(EngPT **root, const char *word);

int removerArv23(Tree23 **root, const char *info, const Tree23 *father, Tree23 **ref);
void freeTree(Tree23 *no);
void toupperString(char *string);

void displayBinary(const EngPT *root);
void displayWordForUnit(Tree23 *root, int unit);
void displayWordsEnglishFindPortuguese(Tree23 *root, char *portugueseWord);

#endif
