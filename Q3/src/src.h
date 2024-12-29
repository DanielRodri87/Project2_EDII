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
int Memory_remover(Memory **raiz, int info);
void mergeNodesStart(Memory **root, int *return_start);
void mergeNodesMiddle(Memory **root, int *aux1, int *aux2);
void mergeNodesEnd(Memory **root, int *return_start);


// Remover
// Memory *no23_criar(Info info, Memory *filho_esquerdo, Memory *filho_centro);
// Memory *Memory_criar();
void Memory_desalocar(Memory **raiz);
// Memory *Memory_inserir(Memory **raiz, Info info, Memory *pai, Info *promove);
int Memory_remover1(Memory **raiz, int info, Memory *pai, Memory **origem, Memory **maior);
int Memory_remover2(Memory **raiz, int info, Memory *pai, Memory **origem, Memory **maior);
int Memory_rebalancear(Memory **raiz, int info, Memory **maior);
Memory *no23_juntar(Memory *filho1, Info info, Memory *maior, Memory **raiz);
// Memory *Memory_buscar();
void no23_desalocar(Memory **no);

#endif