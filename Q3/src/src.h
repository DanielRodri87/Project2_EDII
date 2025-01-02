#ifndef SRC_H
#define SRC_H

#define FREE 1
#define OCCUPIED 0

typedef struct Info
{
    int start;
    int end;
    int status;
} Info;

typedef struct Memory
{
    Info *info1;
    Info *info2;

    struct Memory *left;
    struct Memory *center;
    struct Memory *right;

    int numKeys;

} Memory;

Info *CreateInfo(int start, int end, int status);
Memory *insertTree23(Memory **node, Info *info, Info *promote, Memory **father);
void displayInfos(Memory *root);

int removeMemory(Memory **root, int info);

int removeMemory1(Memory **root, int info, Memory *father, Memory **origin, Memory **bigger);
int memoryRemove2(Memory **root, int info, Memory *father, Memory **origin, Memory **bigger);
Memory *sourceMinorFather(Memory *root, int info);

int memoryRebalance(Memory **root, int info, Memory **bigger);
void node23Deallocate(Memory **node);
Memory *node23Together(Memory *child1, Info info, Memory *bigger, Memory **root);


// update
void allocateAndDesallocate(Memory **tree, int quantNodes, int status);
Memory *sourceFather(Memory *root, int info);
void nodeAddInfo(Memory *node, Info info, Memory *biggerChild);
Memory *lookBiggerChild(Memory *root, Memory **father, Info *bigger_valor);
Memory *lookMinorChild(Memory *root, Memory **father);
Memory *sourceBiggerFather(Memory *root, int info);
Info *node23BiggerInfo(Memory *root);
#endif