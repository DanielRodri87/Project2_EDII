#ifndef DICT_H
#define DICT_H

typedef struct EnglishPortuguese {
    char *word;
    struct EnglishPortuguese *right, *left;
} EnglishPortuguese;

typedef struct Info {
    char *portugueseWord;
    int unit;
    EnglishPortuguese *englishTranslation;
} Info;

typedef struct PortugueseEnglish {
    Info info1, info2;
    int infoCount;
    struct PortugueseEnglish *right, *center, *left;
} PortugueseEnglish;

PortugueseEnglish* createNode(Info info, PortugueseEnglish *leftChild, PortugueseEnglish *centerChild);
int LargestUnit(PortugueseEnglish *root);
void removeWord(PortugueseEnglish **root, const char *word, int unit);

PortugueseEnglish* splitNode(PortugueseEnglish **node, Info value, Info *promote, PortugueseEnglish **child);
PortugueseEnglish* addKey(PortugueseEnglish *node, Info info, PortugueseEnglish *child);
int isLeaf(PortugueseEnglish *node);

PortugueseEnglish* insertPortugueseWord(PortugueseEnglish **node, Info info, Info *promote, PortugueseEnglish **parent);
void displayWords(PortugueseEnglish *root);
void printWordsByUnit(PortugueseEnglish *root, int unit);
void findEnglishByPortuguese(PortugueseEnglish *root, const char *portugueseWord);
int remover23(PortugueseEnglish **father, PortugueseEnglish **root, char *value);
void SearchEnglishWord(PortugueseEnglish **root, char *englishWord, int unit, PortugueseEnglish **parent); 

#endif
