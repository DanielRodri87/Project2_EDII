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

typedef struct Split
{
    Memory *largestNode;
    Info *promote;
} Split;

Info *CreateInfo(int start, int end, int status);
Memory *createNode(Info *information, Memory *leftChild, Memory *centerChild);
int isLeaf(Memory *node);

void AddInfo(Memory **node, Info *info, Memory *child);
Split SplitNode(Memory **root, Info *info, Memory *child);
void Insert23(Memory **root, Memory *parent, Info **promote, int start, int end, int status, int *flag);

Memory *FindSpace(Memory *root, int requiredSpace);
Memory *SourceSpace(Memory *root, int RequiredSpace);
void DisplayInfos(Memory *root);

void AllocateSpace(Memory **root, int requiredSpace);
void FreeSpace(Memory *memory, int start, int end);
int removeFromMemory(Memory **parent, Memory **node, Info *key);

int mergeNodesStart(Memory **root, int *return_start);
int mergeNodesMiddle(Memory **root, int *return_start);
int mergeNodesEnd(Memory **root, int *return_start);

#endif