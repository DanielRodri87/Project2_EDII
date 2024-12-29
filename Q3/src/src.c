#include "src.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ############################################################################################################

Info *CreateInfo(int start, int end, int status)
{
    Info *info = (Info *)malloc(sizeof(Info));

    if (info != NULL)
    {
        info->start = start;
        info->end = end;
        info->status = status;
    }
    return info;
}

Memory *criaNo(const Info *informacao, Memory *filhoesq, Memory *filhocen)
{
    Memory *no = (Memory *)malloc(sizeof(Memory));
    if (no == NULL)
        printf("Erro ao alocar memória para o nó.\n");

    no->info1 = (Info *)malloc(sizeof(Info));
    if (no->info1 == NULL)
    {
        printf("Erro ao alocar memória para info1.\n");
        free(no);
    }
    *(no->info1) = *informacao;

    no->info2 = NULL;
    no->left = filhoesq;
    no->center = filhocen;
    no->right = NULL;
    no->numKeys = 1;

    return no;
}

int ehFolha(const Memory *no)
{
    int achou = 0;

    if (no->left == NULL)
        achou = 1;

    return achou;
}

Memory *addInfo(Memory *no, const Info *informacao, Memory *filho)
{
    if (informacao->start > no->info1->start)
    {
        no->info2 = malloc(sizeof(Info));
        *(no->info2) = *informacao;
        no->right = filho;
    }
    else
    {
        no->info2 = no->info1;
        no->right = no->center;
        no->info1 = malloc(sizeof(Info));
        *(no->info1) = *informacao;
        no->center = filho;
    }
    no->numKeys = 2;
    return no;
}

Memory *quebraNo(Memory **no, const Info *informacao, Info *promove, Memory **filho)
{
    Memory *maior;

    if (informacao->start > (*no)->info1->start)
    {
        *promove = *(*no)->info2;
        if (filho)
            maior = criaNo(informacao, (*no)->right, *filho);
        else
            maior = criaNo(informacao, (*no)->right, NULL);
    }
    else if (informacao->start > (*no)->info2->start)
    {
        *promove = *informacao;
        if (filho)
            maior = criaNo((*no)->info2, *filho, (*no)->right);
        else
            maior = criaNo((*no)->info2, NULL, (*no)->right);
    }
    else
    {
        *promove = *(*no)->info1;

        maior = criaNo((*no)->info2, (*no)->center, (*no)->right);

        if ((*no)->info1 == NULL)
        {
            (*no)->info1 = (Info *)malloc(sizeof(Info));
            if ((*no)->info1 == NULL)
                printf("Erro ao alocar memória para info1.\n");
        }

        *(*no)->info1 = *informacao;

        if (filho)
            (*no)->center = *filho;
        else
            (*no)->center = NULL;
    }

    (*no)->numKeys = 1;
    return maior;
}

Memory *inserirArv23(Memory **no, Info *informacao, Info *promove, Memory **pai)
{
    Info promove1;
    Memory *maiorNo = NULL;
    if (*no == NULL)
        *no = criaNo(informacao, NULL, NULL);

    else
    {
        if (ehFolha(*no))
        { // Caso seja folha
            if ((*no)->numKeys == 1)
                *no = addInfo(*no, informacao, NULL);
            else
            {
                maiorNo = quebraNo(no, informacao, promove, NULL);
                if (pai == NULL)
                { // Se não há pai, criar nova raiz
                    *no = criaNo(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        { // Nó não e folha
          // Navega para o filho apropriado
            if (informacao->start < (*no)->info1->start)
                maiorNo = inserirArv23(&((*no)->left), informacao, promove, no);
            else if ((*no)->numKeys == 1 || informacao->start < (*no)->info2->start)
                maiorNo = inserirArv23(&((*no)->center), informacao, promove, no);
            else
                maiorNo = inserirArv23(&((*no)->right), informacao, promove, no);
            if (maiorNo)
            {
                if ((*no)->numKeys == 1)
                {
                    *no = addInfo(*no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                {
                    maiorNo = quebraNo(no, promove, &promove1, &maiorNo);
                    if (pai == NULL)
                    {
                        *no = criaNo(&promove1, *no, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }

    return maiorNo;
}

Memory *FindSpace(Memory *root, int requiredSpace)
{
    Memory *result = NULL;

    if (root != NULL)
    {
        for (int i = 0; i < root->numKeys; i++)
        {
            Info *currentInfo = (i == 0) ? root->info1 : root->info2;

            if (currentInfo->status == FREE)
            {
                int availableSpace = currentInfo->end - currentInfo->start + 1;

                if (availableSpace >= requiredSpace)
                {
                    result = root;
                }
            }
        }

        if (result == NULL)
        {
            result = FindSpace(root->left, requiredSpace);

            if (result == NULL)
                result = FindSpace(root->center, requiredSpace);

            if (result == NULL && root->numKeys == 2)
                result = FindSpace(root->right, requiredSpace);
        }
    }

    return result;
}

Memory *SourceSpace(Memory *root, int requiredSpace)
{
    return FindSpace(root, requiredSpace);
}

void DisplayInfos(Memory *root)
{
    if (root)
    {
        DisplayInfos(root->left);
        printf("Começo: %d | Fim %d | Status: %d\n", root->info1->start, root->info1->end, root->info1->status);
        DisplayInfos(root->center);

        if (root->numKeys == 2)
        {
            printf("Começo: %d | Fim %d | Status: %d\n", root->info2->start, root->info2->end, root->info2->status);
            DisplayInfos(root->right);
        }
    }
}

// ----------------------------------------------------------------
int AllocateSpace(Memory **root, int requiredSpace, int *return_start)
{
    Memory *node = FindSpace(*root, requiredSpace);
    int lenspace = 0;

    if (node)
    {
        Info *targetInfo = NULL;
        int availableSpace = 0;

        // Verifica info1
        if (node->info1 && node->info1->status == FREE)
        {
            availableSpace = node->info1->end - node->info1->start + 1;
            if (availableSpace >= requiredSpace)
            {
                targetInfo = node->info1;
            }
        }

        // Verifica info2 se info1 não for suficiente
        if (!targetInfo && node->info2 && node->info2->status == FREE)
        {
            availableSpace = node->info2->end - node->info2->start + 1;
            if (availableSpace >= requiredSpace)
            {
                targetInfo = node->info2;
            }
        }

        if (targetInfo)
        {
            int newEndOccupied = targetInfo->start + requiredSpace - 1;

            Info *newOccupiedInfo = CreateInfo(targetInfo->start, newEndOccupied, OCCUPIED);
            *return_start = targetInfo->start;
            lenspace = availableSpace;

            // Atualiza o espaço ocupado e insere o restante como livre
            if (targetInfo == node->info1)
            {
                node->info1 = newOccupiedInfo;
            }
            else if (targetInfo == node->info2)
            {
                node->info2 = newOccupiedInfo;
            }

            if (availableSpace > requiredSpace)
            {
                int remainingStart = newEndOccupied + 1;
                int remainingEnd = targetInfo->end;
                Info *remainingInfo = CreateInfo(remainingStart, remainingEnd, FREE);
                Info promove;
                inserirArv23(root, remainingInfo, &promove, NULL);
            }

            printf("Espaço alocado com sucesso.\n");
        }
        else
        {
            printf("Espaço insuficiente na memória\n");
        }
    }
    else
    {
        printf("Espaço insuficiente na memória\n");
    }

    return lenspace;
}

void FreeSpace(Memory *memory, int start, int end)
{
    if (memory != NULL)
    {
        if (memory->info1 != NULL && memory->info1->start == start && memory->info1->end == end)
            memory->info1->status = 1;

        if (memory->info2 != NULL && memory->info2->start == start && memory->info2->end == end)
            memory->info2->status = 1;

        FreeSpace(memory->left, start, end);
        FreeSpace(memory->center, start, end);
        FreeSpace(memory->right, start, end);
    }
}

// ------------------------------------- REMOVER -------------------------------------
// void freeInfo(Info *info)
// {
//     if (info)
//     {
//         free(info);
//     }
// }

// void freeMemory(Memory *node)
// {
//     if (node)
//     {
//         freeMemory(node->left);
//         freeMemory(node->center);
//         freeMemory(node->right);

//         if (node->info1)
//         {
//             freeInfo(node->info1);
//         }
//         if (node->numKeys == 2 && node->info2)
//         {
//             freeInfo(node->info2);
//         }

//         free(node);
//     }
// }

// void findSmallestRight(Memory *root, Memory **no, Memory **paiNo)
// {
//     if (root->left != NULL)
//     {
//         *paiNo = root;
//         findSmallestRight(root->left, no, paiNo);
//     }
//     else
//         *no = root;
// }

// void findLargestLeft(Memory *root, Memory **no, Memory **paiNo)
// {
//     if (root->right != NULL)
//     {
//         *paiNo = root;
//         findLargestLeft(root->right, no, paiNo);
//     }
//     else
//         *no = root;
// }

// int removeFromMemory(Memory **parent, Memory **node, Info *key)
// {
//     int removeu = 0;
//     Memory *no = NULL, *no1, *parentNo = NULL, *parentNo1 = NULL, **aux;
//     aux = (Memory **)malloc(sizeof(Memory *));
//     no1 = (Memory *)malloc(sizeof(Memory));

//     if (*node != NULL)
//     {
//         if (ehFolha(*node) == 1)
//         {
//             if ((*node)->numKeys == 2)
//             {
//                 if (key->start == (*node)->info2->start)
//                 { // quando é folha, tem duas informações e o numero ta na segunda posição
//                     (*node)->info2 = NULL;
//                     (*node)->numKeys = 1;
//                     removeu = 1;
//                 }
//                 else if (key->start == (*node)->info1->start)
//                 { // quando é folha, tem duas informações e o numero ta na primeira posição do nó
//                     (*node)->info1 = NULL;
//                     (*node)->numKeys = 1;
//                     removeu = 1;
//                 }
//             }
//             else if (key->start == (*node)->info1->start)
//             {
//                 if (*parent == NULL)
//                 {
//                     free(*node);
//                     *node = NULL;
//                     removeu = 1;
//                 }
//                 else if (*node == (*parent)->left)
//                 {
//                     (*node)->info1 = (*parent)->info1;
//                     parentNo = *parent;
//                     findSmallestRight((*parent)->center, &no, &parentNo);
//                     (*parent)->info1 = no->info1;
//                     removeu = 1;

//                     if (no->numKeys == 2)
//                     {
//                         no->info1 = no->info2;
//                         (*node)->info1 = NULL;
//                         (*node)->numKeys = 1;
//                         removeu = 1;
//                     }
//                     else
//                     {
//                         if (parentNo->numKeys == 1)
//                         {
//                             (*node)->info2 = no->info1;
//                             (*node)->numKeys = 2;
//                             free(no);
//                             *parent = *node;
//                         }
//                         else
//                         {
//                             no->info1 = parentNo->info2;
//                             parentNo1 = parentNo;
//                             findSmallestRight(parentNo->right, &no1, &parentNo1);
//                             parentNo->info2 = no1->info1;

//                             if (no1->numKeys == 2)
//                             {
//                                 no1->info1 = no1->info2;
//                                 no1->info2 = NULL;
//                                 no1->numKeys = 1;
//                             }
//                             else
//                             {
//                                 // entrou aqui
//                                 no->info2 = parentNo->info2;
//                                 no->numKeys = 2;
//                                 parentNo->info2 = NULL;
//                                 parentNo->numKeys = 1;
//                                 free(no1);
//                                 parentNo1->right = NULL;
//                             }
//                         }
//                     }
//                 }
//                 else if ((*node) == (*parent)->center)
//                 {
//                     removeu = 1;
//                     if ((*parent)->numKeys == 1)
//                     {
//                         if (((*parent)->left)->numKeys == 2)
//                         {
//                             (*node)->info1 = (*parent)->info1;
//                             (*parent)->info1 = ((*parent)->left)->info2;
//                             ((*parent)->left)->info2 = NULL;
//                             ((*parent)->left)->numKeys = 1;
//                         }
//                         else
//                         {
//                             ((*parent)->left)->info2 = (*parent)->info1;
//                             free(*node);
//                             ((*parent)->left)->numKeys = 2;
//                             *aux = (*parent)->left;
//                             free(*parent);
//                             *parent = *aux;
//                         }
//                     }
//                     else
//                     {
//                         (*node)->info1 = (*parent)->info2;
//                         parentNo = *parent;
//                         findSmallestRight((*parent)->right, &no, &parentNo);
//                         (*parent)->info2 = no->info1;

//                         if (no->numKeys == 2)
//                         {
//                             no->info1 = no->info2;
//                             no->info2 = NULL;
//                             no->numKeys = 1;
//                         }
//                         else
//                         {
//                             (*node)->numKeys = 2;
//                             (*node)->info2 = (*parent)->info2;
//                             (*parent)->info2 = NULL;
//                             (*parent)->numKeys = 1;
//                             free(no);
//                             (*parent)->right = NULL;
//                         }
//                     }
//                 }
//                 else
//                 {
//                     removeu = 1;
//                     parentNo = *parent;
//                     findLargestLeft((*parent)->center, &no, &parentNo);

//                     if (no->numKeys == 1)
//                     {
//                         no->info2 = (*parent)->info2;
//                         (*parent)->info2 = NULL;
//                         (*parent)->numKeys = 1;
//                         no->numKeys = 2;
//                         free(*node);
//                         *node = NULL;
//                     }
//                     else
//                     {
//                         (*node)->info1 = (*parent)->info2;
//                         (*parent)->info2 = no->info2;
//                         no->info2 = NULL;
//                         no->numKeys = 1;
//                     }
//                 }
//             }
//         }
//         else
//         { // se nao é folha
//             if (key->start < (*node)->info1->start)
//                 removeu = removeFromMemory(node, &(*node)->left, key);
//             else if (key->start == (*node)->info1->start)
//             {
//                 parentNo = *node;
//                 findSmallestRight((*node)->center, &no, &parentNo);
//                 (*node)->info1 = no->info1;
//                 removeFromMemory(node, &(*node)->center, (*node)->info1);
//                 removeu = 1;
//             }
//             else if (((*node)->numKeys == 1) || key->start < (*node)->info1->start)
//             {
//                 removeu = removeFromMemory(node, &(*node)->center, key);
//             }
//             else if (key->start == (*node)->info1->start)
//             {
//                 parentNo = *parent;
//                 findSmallestRight((*parent)->right, &no, &parentNo);
//                 (*node)->info2 = no->info1;
//                 removeFromMemory(node, &(*node)->right, (*node)->info2);
//                 removeu = 1;
//             }
//             else
//             {
//                 removeu = removeFromMemory(node, &(*node)->right, key);
//             }
//         }
//     }
//     return removeu;
// }

void mergeNodesStart(Memory **root, int *return_start)
{
    if (root != NULL)
    {
        Memory *current = *root;

        if (current->info1 && current->info2)
        {
            if (current->info1->status == current->info2->status &&
                current->info1->end + 1 == current->info2->start)
            {
                current->info1->end = current->info2->end;
                current->numKeys--;

                *return_start = current->info2->start;
            }
        }

        if (current->left && current->info1 &&
            current->left->info2 &&
            current->left->info2->status == current->info1->status &&
            current->left->info2->end + 1 == current->info1->start)
        {
            current->left->info2->end = current->info1->end;
            current->numKeys--;

            *return_start = current->info1->start;
        }

        if (current->right && current->info2 &&
            current->right->info1 &&
            current->info2->status == current->right->info1->status &&
            current->info2->end + 1 == current->right->info1->start)
        {
            current->info2->end = current->right->info1->end;
            current->right->numKeys--;

            *return_start = current->info2->start;
        }

        if (current->left)
            mergeNodesStart(&current->left, return_start);
        else if (current->center)
            mergeNodesStart(&current->center, return_start);
        else if (current->right)
            mergeNodesStart(&current->right, return_start);
    }
}

// ============================
// HARD CODING
void mergeNodesMiddle(Memory **root, int *aux1, int *aux2)
{
    if (root != NULL && *root != NULL)
    {
        Memory *current = *root;

        // Verifica se existem nós intermediários que podem ser mesclados
        if (current->info1 && current->info2)
        {
            // Verifica contiguidade entre info1 e info2
            if (current->info1->status == current->info2->status &&
                current->info1->end + 1 == current->info2->start)
            {
                // Salvando os valores start dos dois nós
                *aux1 = current->info1->start;
                *aux2 = current->info2->start;
            }
            if (current->info1->end < current->info2->end && current->info1->status == current->info2->status)
                current->info1->end = current->info2->end;
        }

        // Recursão para encontrar nós intermediários nos filhos
        if (current->left)
            mergeNodesMiddle(&current->left, aux1, aux2);

        if (current->center)
            mergeNodesMiddle(&current->center, aux1, aux2);

        if (current->right)
            mergeNodesMiddle(&current->right, aux1, aux2);
    }
}


void mergeNodesEnd(Memory **root, int *return_start)
{
    if (root != NULL)
    {
        Memory *current = *root;

        if (current->info2 && current->right)
        {
            if (current->info2->status == current->right->info1->status &&
                current->info2->end + 1 == current->right->info1->start)
            {
                current->info2->end = current->right->info1->end;
                current->right->numKeys--;
                *return_start = current->right->info1->start;
            }
        }

        if (current->left && current->info1)
        {
            if (current->left->info2->status == current->info1->status &&
                current->left->info2->end + 1 == current->info1->start)
            {
                current->left->info2->end = current->info1->end;
                current->numKeys--;

                *return_start = current->info1->start;
            }
        }

        // Chamada recursiva para os filhos
        if (current->left)
            mergeNodesEnd(&current->left, return_start);
        else if (current->center)
            mergeNodesEnd(&current->center, return_start);
        else if (current->right)
            mergeNodesEnd(&current->right, return_start);
    }
}

// ###################################################################################################################
// Remover do
//         Mateus
// ###################################################################################################################
static int balanceamento(Memory **raiz, Memory *filho1, Memory **filho2, Info info, Memory **maior)
{
    int balanceou = 0;
    if (*filho2 == NULL || (*filho2)->numKeys == 0)
    {
        if (*filho2 != NULL)
            no23_desalocar(filho2);

        *maior = no23_juntar(filho1, info, *maior, raiz);
        balanceou = 1;
    }
    return balanceou;
}

static int eh_folha(Memory no)
{
    return no.left == NULL;
}

static int eh_info1(Memory no, int info)
{
    return info == no.info1->start;
}

static int eh_info2(Memory no, int info)
{
    return no.numKeys == 2 && info == no.info2->start;
}

Memory *no23_alocar()
{
    Memory *no;
    no = (Memory *)malloc(sizeof(Memory));

    if (!no)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return no;
}

void no23_desalocar(Memory **no)
{
    free(*no);
    *no = NULL;
}

Memory *no23_criar(Info info, Memory *filho_left, Memory *filho_center)
{
    Memory *no;
    no = no23_alocar();

    no->info1 = (Info *)malloc(sizeof(Info));
    if (no->info1 == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para info1\n");
        exit(EXIT_FAILURE);
    }

    *(no->info1) = info;

    no->left = filho_left;
    no->center = filho_center;
    no->right = NULL;
    no->numKeys = 1;
    return no;
}

Memory *no23_quebrar(Memory *no, Info info, Info *promove, Memory *filho_maior)
{
    Memory *maior;

    if (info.start > no->info2->start)
    {
        *promove = *(no->info2);
        maior = no23_criar(info, no->right, filho_maior);
    }
    else if (info.start > no->info1->start)
    {
        *promove = info;
        maior = no23_criar(*(no->info2), filho_maior, no->right);
    }
    else
    {
        *promove = *(no->info1);
        maior = no23_criar(*(no->info2), no->center, no->right);

        *(no->info1) = info;

        no->center = filho_maior;
    }

    no->numKeys = 1;

    return maior;
}
void no23_adicionar_info(Memory *no, Info info, Memory *filho_maior)
{
    if (info.start > no->info1->start)
    {
        if (no->info2 == NULL)
        {
            no->info2 = (Info *)malloc(sizeof(Info));
            if (no->info2 == NULL)
            {
                fprintf(stderr, "Erro ao alocar memória para info2\n");
                exit(EXIT_FAILURE);
            }
        }
        *(no->info2) = info;
        no->right = filho_maior;
    }
    else
    {
        if (no->info2 == NULL)
        {
            no->info2 = (Info *)malloc(sizeof(Info));
            if (no->info2 == NULL)
            {
                fprintf(stderr, "Erro ao alocar memória para info2\n");
                exit(EXIT_FAILURE);
            }
        }
        *(no->info2) = *(no->info1);

        no->right = no->center;
        no->center = filho_maior;
        *(no->info1) = info;
    }

    no->numKeys = 2;
}

Memory *no23_juntar(Memory *filho1, Info info, Memory *maior, Memory **raiz)
{
    no23_adicionar_info(filho1, info, maior);

    (*raiz)->numKeys--;

    if ((*raiz)->numKeys == 0)
        no23_desalocar(raiz);

    return filho1;
}

static Info no23_maior_info(Memory *raiz)
{
    return raiz->numKeys == 2 ? *(raiz->info2) : *(raiz->info1);
}

static Memory *Memory_buscar_menor_filho(Memory *raiz, Memory **pai)
{
    Memory *filho;
    filho = raiz;

    while (!eh_folha(*filho))
    {
        *pai = filho;
        filho = filho->left;
    }

    return filho;
}

static Memory *Memory_buscar_maior_filho(Memory *raiz, Memory **pai, Info *maior_valor)
{
    Memory *filho;
    filho = raiz;

    while (!eh_folha(*filho))
    {
        *pai = filho;
        if (filho->numKeys == 1)
            filho = filho->center;
        else
            filho = filho->right;
    }

    if (filho != NULL)
        *maior_valor = no23_maior_info(filho);

    return filho;
}

static Memory *Memory_buscar_pai(Memory *raiz, int info)
{
    Memory *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < raiz->info1->start)
                pai = Memory_buscar_pai(raiz->left, info);
            else if (raiz->numKeys == 1 || info < raiz->info2->start)
                pai = Memory_buscar_pai(raiz->center, info);
            else
                pai = Memory_buscar_pai(raiz->right, info);

            if (pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

static Memory *Memory_buscar_maior_pai(Memory *raiz, int info)
{
    Memory *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < raiz->info1->start)
                pai = Memory_buscar_maior_pai(raiz->left, info);
            else if (raiz->numKeys == 1 || info < raiz->info2->start)
                pai = Memory_buscar_maior_pai(raiz->center, info);
            else
                pai = Memory_buscar_maior_pai(raiz->right, info);

            if (pai == NULL && ((raiz->numKeys == 1 && raiz->info1->start > info) || (raiz->numKeys == 2 && raiz->info2->start > info)))
                pai = raiz;
        }
    }

    return pai;
}

// static Memory *Memory_buscar_maior_pai_2_infos(Memory *raiz, int info)
// {
//     Memory *pai;
//     pai = NULL;

//     if (raiz != NULL)
//     {
//         if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
//         {
//             if (info < raiz->info1->start)
//                 pai = Memory_buscar_maior_pai_2_infos(raiz->left, info);
//             else if (raiz->numKeys == 1 || info < raiz->info2->start)
//                 pai = Memory_buscar_maior_pai_2_infos(raiz->center, info);
//             else
//                 pai = Memory_buscar_maior_pai_2_infos(raiz->right, info);

//             if (pai == NULL && raiz->numKeys == 2 && raiz->info1->start > info)
//                 pai = raiz;
//         }
//     }

//     return pai;
// }

static Memory *Memory_buscar_menor_pai(Memory *raiz, int info)
{
    Memory *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < raiz->info1->start)
                pai = Memory_buscar_menor_pai(raiz->left, info);
            else if (raiz->numKeys == 1 || info < raiz->info2->start)
                pai = Memory_buscar_menor_pai(raiz->center, info);
            else
                pai = Memory_buscar_menor_pai(raiz->right, info);

            if (pai == NULL && raiz->info1->start < info)
                pai = raiz;
        }
    }

    return pai;
}

static Memory *Memory_buscar_menor_pai_2_infos(Memory *raiz, int info)
{
    Memory *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (info < raiz->info1->start)
                pai = Memory_buscar_menor_pai_2_infos(raiz->left, info);
            else if (raiz->numKeys == 1 || info < raiz->info2->start)
                pai = Memory_buscar_menor_pai_2_infos(raiz->center, info);
            else
                pai = Memory_buscar_menor_pai_2_infos(raiz->right, info);

            if (pai == NULL && raiz->numKeys == 2 && raiz->info2->start < info)
                pai = raiz;
        }
    }

    return pai;
}

int possivel_remover(Memory *raiz)
{
    int possivel = 0;

    if (raiz != NULL)
    {
        possivel = raiz->numKeys == 2;

        if (!possivel)
        {
            possivel = possivel_remover(raiz->center);

            if (!possivel)
                possivel = possivel_remover(raiz->left);
        }
    }

    return possivel;
}

static int movimento_onda(Info saindo, Info *entrada, Memory *pai, Memory **origem, Memory **raiz, Memory **maior, int (*funcao_remover)(Memory **, int, Memory *, Memory **, Memory **))
{
    int removeu = funcao_remover(raiz, saindo.start, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}

Memory *Memory_criar()
{
    return NULL;
}

void Memory_desalocar(Memory **raiz)
{
    if (*raiz != NULL)
    {
        Memory_desalocar(&((*raiz)->left));
        Memory_desalocar(&((*raiz)->center));

        if ((*raiz)->numKeys == 2)
            Memory_desalocar(&((*raiz)->right));

        no23_desalocar(raiz);
    }
}

Memory *Memory_inserir(Memory **raiz, Info info, Memory *pai, Info *promove)
{
    Memory *maior;
    maior = NULL;

    if (*raiz == NULL)
        *raiz = no23_criar(info, NULL, NULL);
    else
    {
        if (eh_folha(**raiz))
        {
            if ((*raiz)->numKeys == 1)
                no23_adicionar_info(*raiz, info, NULL);
            else
            {
                maior = no23_quebrar(*raiz, info, promove, NULL);
                if (pai == NULL)
                {
                    *raiz = no23_criar(*promove, *raiz, maior);
                    maior = NULL;
                }
            }
        }
        else
        {
            if (info.start < (*raiz)->info1->start)
                maior = Memory_inserir(&((*raiz)->left), info, *raiz, promove);
            else if ((*raiz)->numKeys == 1 || info.start < (*raiz)->info2->start)
                maior = Memory_inserir(&((*raiz)->center), info, *raiz, promove);
            else
                maior = Memory_inserir(&((*raiz)->right), info, *raiz, promove);

            if (maior != NULL)
            {
                if ((*raiz)->numKeys == 1)
                {
                    no23_adicionar_info(*raiz, *promove, maior);
                    maior = NULL;
                }
                else
                {
                    Info promove_aux;
                    maior = no23_quebrar(*raiz, *promove, &promove_aux, maior);
                    *promove = promove_aux;
                    if (pai == NULL)
                    {
                        *raiz = no23_criar(promove_aux, *raiz, maior);
                        maior = NULL;
                    }
                }
            }
        }
    }

    return maior;
}

int Memory_remover_nao_folha1(Memory **origem, Memory *raiz, Info *info, Memory *filho1, Memory *filho2, Memory **maior)
{
    int removeu;
    Memory *filho, *pai;
    Info info_filho;

    pai = raiz;

    filho = Memory_buscar_maior_filho(filho1, &pai, &info_filho);

    if (filho->numKeys == 2)
    {
        *info = info_filho;
        filho->numKeys = 1;
    }
    else
    {
        filho = Memory_buscar_menor_filho(filho2, &pai);
        removeu = movimento_onda(*(filho->info1), info, pai, origem, &filho, maior, Memory_remover1);
    }

    return removeu;
}

int Memory_remover_nao_folha2(Memory **origem, Memory *raiz, Info *info, Memory *filho1, Memory *filho2, Memory **maior)
{
    int removeu;
    Memory *filho, *pai;
    Info info_filho;

    pai = raiz;

    filho = Memory_buscar_menor_filho(filho1, &pai);

    if (filho->numKeys == 2)
    {
        *info = *(filho->info1);
        *(filho->info1) = *(filho->info2);
        filho->numKeys = 1;
    }
    else
    {
        filho = Memory_buscar_maior_filho(filho2, &pai, &info_filho);
        removeu = movimento_onda(info_filho, info, pai, origem, &filho, maior, Memory_remover2);
    }

    return removeu;
}

int Memory_remover1(Memory **raiz, int info, Memory *pai, Memory **origem, Memory **maior)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (eh_folha(**raiz))
            {
                if ((*raiz)->numKeys == 2)
                {
                    if (info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->numKeys = 1;
                }
                else
                {
                    if (pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        Memory *pai_aux;
                        Info info_pai;
                        if (*raiz == pai->left || (pai->numKeys == 2 && *raiz == pai->center))
                        {
                            pai_aux = Memory_buscar_pai(*origem, pai->info1->start);

                            if (*raiz == pai->left)
                                info_pai = *(pai->info1);
                            else
                                info_pai = *(pai->info2);

                            removeu = movimento_onda(info_pai, (*raiz)->info1, pai_aux, origem, &pai, maior, Memory_remover1);
                        }
                        else // Filho do center (com pai de 1 info) ou da direita
                        {
                            pai_aux = Memory_buscar_maior_pai(*origem, (*raiz)->info1->start);

                            Memory *menor_pai;
                            menor_pai = Memory_buscar_menor_pai_2_infos(*origem, (*raiz)->info1->start);

                            if (pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {
                                *maior = pai;
                                (*raiz)->numKeys = 0;
                                removeu = -1;
                            }
                            else
                            {
                                if (pai_aux->info1->start > (*raiz)->info1->start)
                                    info_pai = *(pai_aux->info1);
                                else
                                    info_pai = *(pai_aux->info2);

                                Memory *avo;
                                avo = Memory_buscar_pai(*origem, info_pai.start);
                                removeu = movimento_onda(info_pai, (*raiz)->info1, avo, origem, &pai_aux, maior, Memory_remover1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = Memory_remover_nao_folha1(origem, *raiz, (*raiz)->info2, (*raiz)->center, (*raiz)->right, maior);
            else if (info1)
                removeu = Memory_remover_nao_folha1(origem, *raiz, (*raiz)->info1, (*raiz)->left, (*raiz)->center, maior);
        }
        else
        {
            if (info < (*raiz)->info1->start)
                removeu = Memory_remover1(&(*raiz)->left, info, *raiz, origem, maior);
            else if ((*raiz)->numKeys == 1 || info < (*raiz)->info2->start)
                removeu = Memory_remover1(&(*raiz)->center, info, *raiz, origem, maior);
            else
                removeu = Memory_remover1(&(*raiz)->right, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int Memory_remover2(Memory **raiz, int info, Memory *pai, Memory **origem, Memory **maior)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (eh_folha(**raiz))
            {
                if ((*raiz)->numKeys == 2)
                {
                    if (info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->numKeys = 1;
                }
                else
                {
                    if (pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        Memory *pai_aux;
                        Info info_pai;
                        if (*raiz == pai->center || (pai->numKeys == 2 && *raiz == pai->right))
                        {
                            pai_aux = Memory_buscar_pai(*origem, pai->info1->start);

                            if (*raiz == pai->center)
                                info_pai = *(pai->info1);
                            else
                                info_pai = *(pai->info2);

                            removeu = movimento_onda(info_pai, (*raiz)->info1, pai_aux, origem, &pai, maior, Memory_remover2);
                        }
                        else // Filho da esquerda
                        {
                            pai_aux = Memory_buscar_menor_pai(*origem, (*raiz)->info1->start);

                            Memory *menor_pai;
                            menor_pai = Memory_buscar_menor_pai_2_infos(*origem, (*raiz)->info1->start);

                            Memory *avo;
                            if (pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if (pai_aux->numKeys == 2 && pai_aux->info2->start < (*raiz)->info1->start)
                                    info_pai = *(pai_aux->info2);
                                else
                                    info_pai = *(pai_aux->info1);

                                avo = Memory_buscar_pai(*origem, info_pai.start);
                                removeu = movimento_onda(info_pai, (*raiz)->info1, avo, origem, &pai_aux, maior, Memory_remover2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = Memory_remover_nao_folha2(origem, *raiz, (*raiz)->info2, (*raiz)->right, (*raiz)->center, maior);
            else if (info1)
                removeu = Memory_remover_nao_folha2(origem, *raiz, (*raiz)->info1, (*raiz)->center, (*raiz)->left, maior);
        }
        else
        {
            if (info < (*raiz)->info1->start)
                removeu = Memory_remover2(&(*raiz)->left, info, *raiz, origem, maior);
            else if ((*raiz)->numKeys == 1 || info < (*raiz)->info2->start)
                removeu = Memory_remover2(&(*raiz)->center, info, *raiz, origem, maior);
            else
                removeu = Memory_remover2(&(*raiz)->right, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int Memory_remover(Memory **raiz, int info)
{
    Memory *maior, *posicao_juncao;
    int removeu = Memory_remover1(raiz, info, NULL, raiz, &posicao_juncao);

    if (removeu == -1)
    {
        Info valor_juncao = no23_maior_info(posicao_juncao);
        maior = NULL;
        removeu = Memory_rebalancear(raiz, valor_juncao.start, &maior);

        if (removeu == -1)
        {
            Memory *pai, *posicao_juncao2;
            Info *entrada;
            pai = Memory_buscar_pai(*raiz, valor_juncao.start);

            if (eh_info1(*posicao_juncao, valor_juncao.start))
                entrada = posicao_juncao->center->info1;
            else
                entrada = posicao_juncao->right->info1;

            removeu = movimento_onda(valor_juncao, entrada, pai, raiz, &posicao_juncao, &posicao_juncao2, Memory_remover2);

            if (removeu == -1)
            {
                valor_juncao = *(posicao_juncao2->info1);
                pai = Memory_buscar_pai(*raiz, valor_juncao.start);
                removeu = movimento_onda(valor_juncao, posicao_juncao2->left->info1, pai, raiz, &posicao_juncao2, &posicao_juncao, Memory_remover1);

                valor_juncao = no23_maior_info(posicao_juncao);
                maior = NULL;
                removeu = Memory_rebalancear(raiz, valor_juncao.start, &maior);
            }
        }

        if (*raiz == NULL)
            *raiz = maior;
    }

    return removeu;
}

int Memory_rebalancear(Memory **raiz, int info, Memory **maior)
{
    int balanceou = 0;
    if (*raiz != NULL)
    {
        if (!eh_folha(**raiz))
        {
            if (info < (*raiz)->info1->start)
                balanceou = Memory_rebalancear(&((*raiz)->left), info, maior);
            else if ((*raiz)->numKeys == 1 || info < (*raiz)->info2->start)
            {
                if ((*raiz)->left->numKeys == 2 && !possivel_remover((*raiz)->center))
                    balanceou = -1;
                else
                    balanceou = Memory_rebalancear(&((*raiz)->center), info, maior);
            }
            else
            {
                if ((*raiz)->center->numKeys == 2 && !possivel_remover((*raiz)->right))
                    balanceou = -1;
                else
                    balanceou = Memory_rebalancear(&((*raiz)->right), info, maior);
            }

            if (balanceou != -1)
            {
                if ((*raiz)->numKeys == 1)
                    balanceou = balanceamento(raiz, (*raiz)->left, &((*raiz)->center), *(*raiz)->info1, maior);
                else if ((*raiz)->numKeys == 2)
                    balanceou = balanceamento(raiz, (*raiz)->center, &((*raiz)->right), *(*raiz)->info2, maior);
            }
        }
    }

    return balanceou;
}
