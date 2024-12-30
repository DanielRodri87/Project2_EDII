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

int allocateSpace(Memory **root, int requiredSpace, int *return_start);
void freeSpace(Memory *memory, int start, int end);
int removeMemory(Memory **root, int info);

void mergeNodesStart(Memory **root, int *return_start);
void mergeNodesMiddle(Memory **root, int *aux1, int *aux2);
void mergeNodesEnd(Memory **root, int *return_start);

int removeMemory1(Memory **root, int info, Memory *father, Memory **origin, Memory **bigger);
int memoryRemove2(Memory **root, int info, Memory *father, Memory **origin, Memory **bigger);

int memoryRebalance(Memory **root, int info, Memory **bigger);
void node23Deallocate(Memory **node);
Memory *node23Together(Memory *child1, Info info, Memory *bigger, Memory **root);

#endif