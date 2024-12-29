#include "src.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEBUGANDO
void printNode(const Memory *node, int depth)
{
    if (node == NULL)
        return;

    // Imprime indentação para representar a profundidade
    for (int i = 0; i < depth; i++)
        printf("    ");

    // Imprime as informações do nó
    printf("[");
    if (node->info1)
        printf("(%d, %d, %d)", node->info1->start, node->info1->end, node->info1->status);
    if (node->numKeys == 2 && node->info2)
        printf(" | (%d, %d, %d)", node->info2->start, node->info2->end, node->info2->status);
    printf("]\n");
}

// Função recursiva para imprimir a árvore
void printTree(const Memory *root, int depth)
{
    if (root == NULL)
        return;

    // Imprime o nó atual
    printNode(root, depth);

    // Recursivamente imprime as subárvores
    printTree(root->left, depth + 1);
    printTree(root->center, depth + 1);
    if (root->numKeys == 2) // Só imprime a subárvore direita se houver uma segunda chave
        printTree(root->right, depth + 1);
}

// Função principal para desenhar a árvore
void drawTree(const Memory *root)
{
    printf("Árvore 2-3:\n");
    printTree(root, 0);
}

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
int AllocateSpace(Memory **root, int requiredSpace, int *return_start) {
    Memory *node = FindSpace(*root, requiredSpace);
    int lenspace = 0;

    if (node) {
        Info *targetInfo = NULL;
        int availableSpace = 0;

        // Verifica info1
        if (node->info1 && node->info1->status == FREE) {
            availableSpace = node->info1->end - node->info1->start + 1;
            if (availableSpace >= requiredSpace) {
                targetInfo = node->info1;
            }
        }

        // Verifica info2 se info1 não for suficiente
        if (!targetInfo && node->info2 && node->info2->status == FREE) {
            availableSpace = node->info2->end - node->info2->start + 1;
            if (availableSpace >= requiredSpace) {
                targetInfo = node->info2;
            }
        }

        if (targetInfo) {
            int newEndOccupied = targetInfo->start + requiredSpace - 1;

            Info *newOccupiedInfo = CreateInfo(targetInfo->start, newEndOccupied, OCCUPIED);
            *return_start = targetInfo->start;
            lenspace = availableSpace;

            // Atualiza o espaço ocupado e insere o restante como livre
            if (targetInfo == node->info1) {
                node->info1 = newOccupiedInfo;
            } else if (targetInfo == node->info2) {
                node->info2 = newOccupiedInfo;
            }

            if (availableSpace > requiredSpace) {
                int remainingStart = newEndOccupied + 1;
                int remainingEnd = targetInfo->end;
                Info *remainingInfo = CreateInfo(remainingStart, remainingEnd, FREE);
                Info promove;
                inserirArv23(root, remainingInfo, &promove, NULL);
            }

            printf("Espaço alocado com sucesso.\n");
        } else {
            printf("Espaço insuficiente na memória\n");
        }
    } else {
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
void freeInfo(Info *info)
{
    if (info)
    {
        free(info);
    }
}

void freeMemory(Memory *node)
{
    if (node)
    {
        freeMemory(node->left);
        freeMemory(node->center);
        freeMemory(node->right);

        if (node->info1)
        {
            freeInfo(node->info1);
        }
        if (node->numKeys == 2 && node->info2)
        {
            freeInfo(node->info2);
        }

        free(node);
    }
}

void findSmallestRight(Memory *root, Memory **no, Memory **paiNo)
{
    if (root->left != NULL)
    {
        *paiNo = root;
        findSmallestRight(root->left, no, paiNo);
    }
    else
        *no = root;
}

void findLargestLeft(Memory *root, Memory **no, Memory **paiNo)
{
    if (root->right != NULL)
    {
        *paiNo = root;
        findLargestLeft(root->right, no, paiNo);
    }
    else
        *no = root;
}

int removeFromMemory(Memory **parent, Memory **node, Info *key)
{
    int removeu = 0;
    Memory *no = NULL, *no1, *parentNo = NULL, *parentNo1 = NULL, **aux;
    aux = (Memory **)malloc(sizeof(Memory *));
    no1 = (Memory *)malloc(sizeof(Memory));

    if (*node != NULL)
    {
        if (ehFolha(*node) == 1)
        {
            if ((*node)->numKeys == 2)
            {
                if (key->start == (*node)->info2->start)
                { // quando é folha, tem duas informações e o numero ta na segunda posição
                    (*node)->info2 = NULL;
                    (*node)->numKeys = 1;
                    removeu = 1;
                }
                else if (key->start == (*node)->info1->start)
                { // quando é folha, tem duas informações e o numero ta na primeira posição do nó
                    (*node)->info1 = NULL;
                    (*node)->numKeys = 1;
                    removeu = 1;
                }
            }
            else if (key->start == (*node)->info1->start)
            {
                if (*parent == NULL)
                {
                    free(*node);
                    *node = NULL;
                    removeu = 1;
                }
                else if (*node == (*parent)->left)
                {
                    (*node)->info1 = (*parent)->info1;
                    parentNo = *parent;
                    findSmallestRight((*parent)->center, &no, &parentNo);
                    (*parent)->info1 = no->info1;
                    removeu = 1;

                    if (no->numKeys == 2)
                    {
                        no->info1 = no->info2;
                        (*node)->info1 = NULL;
                        (*node)->numKeys = 1;
                        removeu = 1;
                    }
                    else
                    {
                        if (parentNo->numKeys == 1)
                        {
                            (*node)->info2 = no->info1;
                            (*node)->numKeys = 2;
                            free(no);
                            *parent = *node;
                        }
                        else
                        {
                            no->info1 = parentNo->info2;
                            parentNo1 = parentNo;
                            findSmallestRight(parentNo->right, &no1, &parentNo1);
                            parentNo->info2 = no1->info1;

                            if (no1->numKeys == 2)
                            {
                                no1->info1 = no1->info2;
                                no1->info2 = NULL;
                                no1->numKeys = 1;
                            }
                            else
                            {
                                // entrou aqui
                                no->info2 = parentNo->info2;
                                no->numKeys = 2;
                                parentNo->info2 = NULL;
                                parentNo->numKeys = 1;
                                free(no1);
                                parentNo1->right = NULL;
                            }
                        }
                    }
                }
                else if ((*node) == (*parent)->center)
                {
                    removeu = 1;
                    if ((*parent)->numKeys == 1)
                    {
                        if (((*parent)->left)->numKeys == 2)
                        {
                            (*node)->info1 = (*parent)->info1;
                            (*parent)->info1 = ((*parent)->left)->info2;
                            ((*parent)->left)->info2 = NULL;
                            ((*parent)->left)->numKeys = 1;
                        }
                        else
                        {
                            ((*parent)->left)->info2 = (*parent)->info1;
                            free(*node);
                            ((*parent)->left)->numKeys = 2;
                            *aux = (*parent)->left;
                            free(*parent);
                            *parent = *aux;
                        }
                    }
                    else
                    {
                        (*node)->info1 = (*parent)->info2;
                        parentNo = *parent;
                        findSmallestRight((*parent)->right, &no, &parentNo);
                        (*parent)->info2 = no->info1;

                        if (no->numKeys == 2)
                        {
                            no->info1 = no->info2;
                            no->info2 = NULL;
                            no->numKeys = 1;
                        }
                        else
                        {
                            (*node)->numKeys = 2;
                            (*node)->info2 = (*parent)->info2;
                            (*parent)->info2 = NULL;
                            (*parent)->numKeys = 1;
                            free(no);
                            (*parent)->right = NULL;
                        }
                    }
                }
                else
                {
                    removeu = 1;
                    parentNo = *parent;
                    findLargestLeft((*parent)->center, &no, &parentNo);

                    if (no->numKeys == 1)
                    {
                        no->info2 = (*parent)->info2;
                        (*parent)->info2 = NULL;
                        (*parent)->numKeys = 1;
                        no->numKeys = 2;
                        free(*node);
                        *node = NULL;
                    }
                    else
                    {
                        (*node)->info1 = (*parent)->info2;
                        (*parent)->info2 = no->info2;
                        no->info2 = NULL;
                        no->numKeys = 1;
                    }
                }
            }
        }
        else
        { // se nao é folha
            if (key->start < (*node)->info1->start)
                removeu = removeFromMemory(node, &(*node)->left, key);
            else if (key->start == (*node)->info1->start)
            {
                parentNo = *node;
                findSmallestRight((*node)->center, &no, &parentNo);
                (*node)->info1 = no->info1;
                removeFromMemory(node, &(*node)->center, (*node)->info1);
                removeu = 1;
            }
            else if (((*node)->numKeys == 1) || key->start < (*node)->info1->start)
            {
                removeu = removeFromMemory(node, &(*node)->center, key);
            }
            else if (key->start == (*node)->info1->start)
            {
                parentNo = *parent;
                findSmallestRight((*parent)->right, &no, &parentNo);
                (*node)->info2 = no->info1;
                removeFromMemory(node, &(*node)->right, (*node)->info2);
                removeu = 1;
            }
            else
            {
                removeu = removeFromMemory(node, &(*node)->right, key);
            }
        }
    }
    return removeu;
}

int mergeNodesStart(Memory **root, int *return_start)
{
    int return_end = -1;
    if (root != NULL)
    {
        Memory *current = *root;

        // Verifica se o nó possui ambas as informações
        if (current->info1 && current->info2)
        {
            // Verifica se as informações possuem o mesmo status e são contíguas
            if (current->info1->status == current->info2->status &&
                current->info1->end + 1 == current->info2->start)
            {
                // Atualiza o final da primeira informação
                current->info1->end = current->info2->end;
                current->numKeys--;

                *return_start = current->info2->start;
                return_end = current->info2->end;
            }
        }

        // Verifica e mescla com o nó à esquerda
        if (current->left && current->info1 &&
            current->left->info2 &&
            current->left->info2->status == current->info1->status &&
            current->left->info2->end + 1 == current->info1->start)
        {
            current->left->info2->end = current->info1->end;
            current->numKeys--;

            *return_start = current->info1->start;
            return_end = current->info1->end;
        }

        // Verifica e mescla com o nó à direita
        if (current->right && current->info2 &&
            current->right->info1 &&
            current->info2->status == current->right->info1->status &&
            current->info2->end + 1 == current->right->info1->start)
        {
            current->info2->end = current->right->info1->end;
            current->right->numKeys--;

            *return_start = current->info2->start;
            return_end = current->info2->end;
        }

        // Chamada recursiva para os filhos
        if (current->left)
        {
            int left_end = mergeNodesStart(&current->left, return_start);
            if (left_end > return_end)
                return_end = left_end;
        }
        if (current->center)
        {
            int center_end = mergeNodesStart(&current->center, return_start);
            if (center_end > return_end)
                return_end = center_end;
        }
        if (current->right)
        {
            int right_end = mergeNodesStart(&current->right, return_start);
            if (right_end > return_end)
                return_end = right_end;
        }
    }
    return return_end;
}

// ============================

void mergeNodesMiddle(Memory **root, int *remover_inicio_meio1, int *remover_fim_meio1, int *remover_inicio_meio2, int *remover_fim_meio2)
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
                // Salva os endereços do primeiro nó que será removido
                *remover_inicio_meio1 = current->info1->start;
                *remover_fim_meio1 = current->info1->end;

                // Salva os endereços do segundo nó que será removido
                *remover_inicio_meio2 = current->info2->start;
                *remover_fim_meio2 = current->info2->end;

                return; // Para a busca após encontrar a primeira mescla
            }
        }

        // Recursão para encontrar nós intermediários nos filhos
        if (current->left)
            mergeNodesMiddle(&current->left, remover_inicio_meio1, remover_fim_meio1, remover_inicio_meio2, remover_fim_meio2);

        if (current->center)
            mergeNodesMiddle(&current->center, remover_inicio_meio1, remover_fim_meio1, remover_inicio_meio2, remover_fim_meio2);

        if (current->right)
            mergeNodesMiddle(&current->right, remover_inicio_meio1, remover_fim_meio1, remover_inicio_meio2, remover_fim_meio2);
    }
}

int mergeNodesEnd(Memory **root, int *return_start)
{
    int return_end = -1;
    if (root != NULL)
    {
        Memory *current = *root;

        // Verifica se há contiguidade no último nó
        if (current->info2 && current->right)
        {
            if (current->info2->status == current->right->info1->status &&
                current->info2->end + 1 == current->right->info1->start)
            {
                current->info2->end = current->right->info1->end;
                current->right->numKeys--;

                *return_start = current->right->info1->start;
                return_end = current->right->info1->end;
            }
        }

        // Verifica e mescla com o nó à esquerda
        if (current->left && current->info1)
        {
            if (current->left->info2->status == current->info1->status &&
                current->left->info2->end + 1 == current->info1->start)
            {
                current->left->info2->end = current->info1->end;
                current->numKeys--;

                *return_start = current->info1->start;
                return_end = current->info1->end;
            }
        }

        // Chamada recursiva para os filhos
        if (current->left)
        {
            int left_end = mergeNodesEnd(&current->left, return_start);
            if (left_end > return_end)
                return_end = left_end;
        }
        if (current->center)
        {
            int center_end = mergeNodesEnd(&current->center, return_start);
            if (center_end > return_end)
                return_end = center_end;
        }
        if (current->right)
        {
            int right_end = mergeNodesEnd(&current->right, return_start);
            if (right_end > return_end)
                return_end = right_end;
        }
    }
    return return_end;
}
