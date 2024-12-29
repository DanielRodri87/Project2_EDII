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


void drawTree(const Memory *root);

Info *CreateInfo(int start, int end, int status);
Memory *inserirArv23(Memory **no, Info *informacao, Info *promove, Memory **Pai);

Memory *FindSpace(Memory *root, int requiredSpace);
Memory *SourceSpace(Memory *root, int RequiredSpace);
void DisplayInfos(Memory *root);

int AllocateSpace(Memory **root, int requiredSpace, int *return_start);
void FreeSpace(Memory *memory, int start, int end);
int removeFromMemory(Memory **parent, Memory **node, Info *key);

int mergeNodesStart(Memory **root, int *return_start);
void mergeNodesMiddle(Memory **root, int *remover_inicio_meio1, int *remover_fim_meio1, int *remover_inicio_meio2, int *remover_fim_meio2);
int mergeNodesEnd(Memory **root, int *return_start);


#endif