#ifndef DICT_H
#define DICT_H

#include <stdbool.h>

typedef struct EnglishNode {
    char *englishWord;              
    char *unit;                      
    struct EnglishNode *left;
    struct EnglishNode *right;
} EnglishNode;

typedef struct PortugueseNode {
    char *portugueseWord;            
    EnglishNode *englishWords;       
    struct PortugueseNode *left;
    struct PortugueseNode *middle;
    struct PortugueseNode *right;
    int nInfos;               
} PortugueseNode;

typedef struct Dictionary {
    PortugueseNode *root;          
} Dictionary;


// Funções Daniel
void addWord(Dictionary *dict, const char *portuguese_Word, const char *english_word, const char *unit);
bool deleteEnglishWord(Dictionary *dict, const char *english_word, const char *unit);
bool deletePortugueseWord(Dictionary *dict, const char *portuguese_word, const char *unit);

// Funções Cristina
void searchUnit(Dictionary *dict, const char *unit);
void searchPortuguese(Dictionary *dict, const char *portuguese_word);
void searchEnglish(Dictionary *dict, const char *english_word);


// Pendentes
EnglishNode *insertEnglishNode(EnglishNode *root, const char *english_word, const char *unit);
PortugueseNode *insertPortugueseNode(PortugueseNode *root, const char *portuguese_word, const char *english_word, const char *unit);
EnglishNode *removeEnglishNode(EnglishNode *root, const char *english_word, const char *unit);
PortugueseNode *removePortugueseNode(PortugueseNode *root, const char *portuguese_word, const char *unit);
void freeEnglishTree(EnglishNode *root);
void freePortugueseTree(PortugueseNode *root);


// Feitas

#endif 
