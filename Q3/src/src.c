#include "src.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ############################################################################################################

/**
 * @brief Cria uma estrutura do tipo Info com os valores fornecidos.
 *
 * @param start Valor inicial da estrutura Info.
 * @param end Valor final da estrutura Info.
 * @param status Status da estrutura Info.
 * @return Ponteiro para a estrutura Info criada.
 */
Info *CreateInfo(int start, int end, int status)
{
    Info *info = (Info *)malloc(sizeof(Info));

    if (info != NULL)
    {
        info->start = start;
        info->end = end;
        info->status = status;
    }
    return (info);
}

/**
 * @brief Cria um novo nó para a estrutura de árvore 2-3.
 *
 * @param info Ponteiro para a estrutura Info a ser armazenada no nó.
 * @param leftChild Ponteiro para o filho esquerdo do nó.
 * @param centerChild Ponteiro para o filho central do nó.
 * @return Ponteiro para o nó criado.
 */
Memory *createNode(const Info *info, Memory *leftChild, Memory *centerChild)
{
    Memory *node = (Memory *)malloc(sizeof(Memory));
    if (node == NULL)
        printf("Erro ao alocar memória para o nó.\n");

    node->info1 = (Info *)malloc(sizeof(Info));
    if (node->info1 == NULL)
    {
        printf("Erro ao alocar memória para info1.\n");
        free(node);
    }
    *(node->info1) = *info;

    node->info2 = NULL;
    node->left = leftChild;
    node->center = centerChild;
    node->right = NULL;
    node->numKeys = 1;

    return (node);
}

/**
 * @brief Verifica se o nó é uma folha na árvore 2-3.
 *
 * @param node Ponteiro para o nó a ser verificado.
 * @return 1 se o nó é uma folha, caso contrário, 0.
 */
int isLeaf(const Memory *node)
{
    int thought = 0;

    if (node->left == NULL)
        thought = 1;

    return (thought);
}

/**
 * @brief Adiciona uma nova chave a um nó da árvore 2-3.
 *
 * @param node Ponteiro para o nó onde a chave será adicionada.
 * @param info Ponteiro para a nova chave a ser adicionada.
 * @param child Ponteiro para o nó filho associado à nova chave.
 * @return Ponteiro atualizado para o nó.
 */
Memory *addKey(Memory *node, const Info *info, Memory *child)
{
    if (info->start > node->info1->start)
    {
        node->info2 = malloc(sizeof(Info));
        *(node->info2) = *info;
        node->right = child;
    }
    else
    {
        node->info2 = node->info1;
        node->right = node->center;
        node->info1 = malloc(sizeof(Info));
        *(node->info1) = *info;
        node->center = child;
    }
    node->numKeys = 2;

    return (node);
}

/**
 * @brief Realiza a divisão de um nó 2-3, promovendo uma chave e criando um novo nó.
 *
 * @param node Nó a ser dividido.
 * @param info Informação a ser inserida durante a divisão.
 * @param promote Ponteiro para armazenar a chave promovida.
 * @param biggerChild Ponteiro para o filho maior após a divisão.
 * @return Memory* Retorna um ponteiro para o novo nó criado durante a divisão.
 */
Memory *nodeBreak(Memory *node, Info info, Info *promote, Memory *biggerChild)
{
    Memory *bigger;

    if (info.start > node->info2->start)
    {
        *promote = *(node->info2);
        bigger = createNode(&info, node->right, biggerChild);
    }
    else if (info.start > node->info1->start)
    {
        *promote = info;
        bigger = createNode(&(*(node->info2)), biggerChild, node->right);
    }
    else
    {
        *promote = *(node->info1);
        bigger = createNode(&(*(node->info2)), node->center, node->right);

        *(node->info1) = info;

        node->center = biggerChild;
    }

    node->numKeys = 1;

    return (bigger);
}

/**
 * @brief Insere uma nova chave na árvore 2-3.
 *
 * @param node Ponteiro duplo para o nó raiz da árvore.
 * @param info Ponteiro para a chave a ser inserida.
 * @param promote Ponteiro para armazenar a chave promovida (se houver).
 * @param father Ponteiro duplo para o nó pai.
 * @return Ponteiro para o nó criado ou modificado durante a inserção.
 */
Memory *insertTree23(Memory **node, Info *info, Info *promote, Memory **father)
{
    Info promote1;
    Memory *biggerNode = NULL;
    if (*node == NULL)
        *node = createNode(info, NULL, NULL);

    else
    {
        if (isLeaf(*node))
        {
            if ((*node)->numKeys == 1)
                *node = addKey(*node, info, NULL);
            else
            {
                biggerNode = nodeBreak(*node, *info, promote, NULL);
                if (father == NULL)
                {
                    *node = createNode(promote, *node, biggerNode);
                    biggerNode = NULL;
                }
            }
        }
        else
        {
            if (info->start < (*node)->info1->start)
                biggerNode = insertTree23(&((*node)->left), info, promote, node);
            else if ((*node)->numKeys == 1 || info->start < (*node)->info2->start)
                biggerNode = insertTree23(&((*node)->center), info, promote, node);
            else
                biggerNode = insertTree23(&((*node)->right), info, promote, node);
            if (biggerNode)
            {
                if ((*node)->numKeys == 1)
                {
                    *node = addKey(*node, promote, biggerNode);
                    biggerNode = NULL;
                }
                else
                {
                    biggerNode = nodeBreak(*node, *promote, &promote1, biggerNode);
                    if (father == NULL)
                    {
                        *node = createNode(&promote1, *node, biggerNode);
                        biggerNode = NULL;
                    }
                }
            }
        }
    }

    return (biggerNode);
}

/**
 * @brief Procura por um espaço livre na árvore que atenda ao tamanho necessário.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param requiredSpace Tamanho do espaço necessário.
 * @return Ponteiro para o nó que contém o espaço livre encontrado, ou NULL se não houver espaço suficiente.
 */
Memory *findSpace(Memory *root, int requiredSpace)
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
                    result = root;
            }
        }

        if (result == NULL)
        {
            result = findSpace(root->left, requiredSpace);

            if (result == NULL)
                result = findSpace(root->center, requiredSpace);

            if (result == NULL && root->numKeys == 2)
                result = findSpace(root->right, requiredSpace);
        }
    }

    return (result);
}

/**
 * @brief Exibe todas as informações armazenadas na árvore.
 *
 * @param root Ponteiro para a raiz da árvore.
 */
void displayInfos(Memory *root)
{
    if (root)
    {
        displayInfos(root->left);
        printf("Começo: %d | Fim %d | Status: %d\n", root->info1->start, root->info1->end, root->info1->status);
        displayInfos(root->center);

        if (root->numKeys == 2)
        {
            printf("Começo: %d | Fim %d | Status: %d\n", root->info2->start, root->info2->end, root->info2->status);
            displayInfos(root->right);
        }
    }
}

/**
 * @brief Aloca espaço na árvore para atender ao tamanho necessário.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param requiredSpace Tamanho do espaço necessário.
 * @param return_start Ponteiro para armazenar o início do espaço alocado.
 * @return Tamanho do espaço alocado.
 */
int allocateSpace(Memory **root, int requiredSpace, int *return_start)
{
    Memory *node = findSpace(*root, requiredSpace);
    int lenspace = 0;

    if (node)
    {
        Info *targetInfo = NULL;
        int availableSpace = 0;

        if (node->info1 && node->info1->status == FREE)
        {
            availableSpace = node->info1->end - node->info1->start + 1;
            if (availableSpace >= requiredSpace)
                targetInfo = node->info1;
        }

        if (!targetInfo && node->info2 && node->info2->status == FREE)
        {
            availableSpace = node->info2->end - node->info2->start + 1;
            if (availableSpace >= requiredSpace)
                targetInfo = node->info2;
        }

        if (targetInfo)
        {
            int newEndOccupied = targetInfo->start + requiredSpace - 1;

            Info *newOccupiedInfo = CreateInfo(targetInfo->start, newEndOccupied, OCCUPIED);
            *return_start = targetInfo->start;
            lenspace = availableSpace;

            if (targetInfo == node->info1)
                node->info1 = newOccupiedInfo;
            else if (targetInfo == node->info2)
                node->info2 = newOccupiedInfo;

            if (availableSpace > requiredSpace)
            {
                int remainingStart = newEndOccupied + 1;
                int remainingEnd = targetInfo->end;
                Info *remainingInfo = CreateInfo(remainingStart, remainingEnd, FREE);
                Info promote;
                insertTree23(root, remainingInfo, &promote, NULL);
            }

            printf("Espaço alocado com sucesso.\n");
        }
        else
            printf("Espaço insuficiente na memória\n");
    }
    else
        printf("Espaço insuficiente na memória\n");

    return (lenspace);
}

/**
 * @brief Libera um espaço específico na memória, marcando-o como livre.
 *
 * @param memory Ponteiro para a raiz da árvore.
 * @param start Início do espaço a ser liberado.
 * @param end Fim do espaço a ser liberado.
 */
void freeSpace(Memory *memory, int start, int end)
{
    if (memory != NULL)
    {
        if (memory->info1 != NULL && memory->info1->start == start && memory->info1->end == end)
            memory->info1->status = 1;

        if (memory->info2 != NULL && memory->info2->start == start && memory->info2->end == end)
            memory->info2->status = 1;

        freeSpace(memory->left, start, end);
        freeSpace(memory->center, start, end);
        freeSpace(memory->right, start, end);
    }
}

/**
 * @brief Une nós consecutivos na árvore 2-3 que possuem status e intervalos contíguos.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param return_start Ponteiro para armazenar o início do intervalo unificado.
 */
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

/*

20-80-0


aux1 = 41
aux2 = 61


*/

// ============================
// HARD CODING TÁ ERRADO NÃO PEGUEM
Info *returnEnd(Memory *root, int valor)
{
    printf("%d\n", valor);
    if (root != NULL)
    {
        if (root->info1->start == valor)
        {
            return root->info1;
        }
        else if (root->numKeys == 2)
        {
            if (root->info2->start == valor)
                return root->info2;
        }

        if (root->left)
            return returnEnd(root->left, valor);
        if (root->center)
            return returnEnd(root->center, valor);
        if (root->right)
            return returnEnd(root->right, valor);
    }
    
    // Return NULL if nothing was found
    return NULL;
}



void mergeNodesMiddle(Memory **root, int *aux1, int *aux2)
{
    if (root != NULL && *root != NULL)
    {
        Memory *current = *root;

        // Verifica se ambos os intervalos (info1 e info2) existem
        if (current->info1 && current->info2)
        {
            // Verifica se os intervalos são contínuos e tem o mesmo status
            if (current->info1->status == current->info2->status &&
                current->info1->end + 1 == current->info2->start)
            {
                *aux1 = current->info1->start;
                *aux2 = current->info2->start;
            }

            // Verifica se o valor de aux1-1 foi encontrado e atualiza o 'end' do nó
            if (current->info1->end < current->info2->end && current->info1->status == current->info2->status)
            {
                // Procura pelo valor de aux1 - 1 para localizar o nó onde devemos inserir o valor de aux2
                Info *found = returnEnd(*root, *aux1 - 1);
                
                // Se o nó for encontrado, substituímos o valor de 'end' do nó com o valor de 'info2->end'
                if (found)
                {
                    found->end = current->info2->end;
                    printf("Substituição realizada: novo 'end' = %d\n", found->end);  // Para depuração
                }
                else
                {
                    printf("Valor %d não encontrado para substituição.\n", *aux1 - 1);  // Para depuração
                }
            }
        }

        // Recursão para os filhos (esquerda, centro, direita)
        if (current->left)
            mergeNodesMiddle(&current->left, aux1, aux2);

        if (current->center)
            mergeNodesMiddle(&current->center, aux1, aux2);

        if (current->right)
            mergeNodesMiddle(&current->right, aux1, aux2);
    }
}



/**
 * @brief Une nós consecutivos na árvore 2-3 que possuem status e intervalos contíguos.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param return_start Ponteiro para armazenar o início do intervalo unificado.
 */
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

        if (current->left)
            mergeNodesEnd(&current->left, return_start);
        else if (current->center)
            mergeNodesEnd(&current->center, return_start);
        else if (current->right)
            mergeNodesEnd(&current->right, return_start);
    }
}

/**
 * @brief Verifica o equilíbrio da árvore 2-3 e ajusta caso necessário.
 *
 * @param root Ponteiro para o nó raiz da árvore.
 * @param child1 Primeiro nó filho.
 * @param child2 Segundo nó filho.
 * @param info Informações a serem usadas no ajuste.
 * @param bigger Ponteiro para o nó maior após o ajuste.
 * @return int Retorna 1 se o nó foi equilibrado, caso contrário, 0.
 */
static int balancing(Memory **root, Memory *child1, Memory **child2, Info info, Memory **bigger)
{
    int balanced = 0;
    if (*child2 == NULL || (*child2)->numKeys == 0)
    {
        if (*child2 != NULL)
            node23Deallocate(child2);

        *bigger = node23Together(child1, info, *bigger, root);
        balanced = 1;
    }
    return (balanced);
}

/**
 * @brief Verifica se o campo info1 do nó contém a informação passada.
 *
 * @param node Nó da árvore.
 * @param info Informação a ser comparada.
 * @return int Retorna 1 se info1 for igual à informação passada, caso contrário, 0.
 */
static int isInfo1(Memory node, int info)
{
    return (info == node.info1->start);
}

/**
 * @brief Verifica se o campo info2 do nó contém a informação passada.
 *
 * @param node Nó da árvore.
 * @param info Informação a ser comparada.
 * @return int Retorna 1 se info1 for igual à informação passada, caso contrário, 0.
 */
static int isInfo2(Memory node, int info)
{
    return (node.numKeys == 2 && info == node.info2->start);
}

/**
 * @brief Aloca memória para um novo nó 2-3.
 *
 * @return Memory* Retorna um ponteiro para o nó alocado.
 * @note Se a alocação falhar, o programa encerra com erro.
 */
Memory *node23Allocate()
{
    Memory *node;
    node = (Memory *)malloc(sizeof(Memory));

    if (!node)
    {
        printf("Erro ao alocar nó da árvore");
        exit(EXIT_FAILURE);
    }

    return node;
}

/**
 * @brief Libera a memória de um nó 2-3 e define o ponteiro como NULL.
 *
 * @param node Ponteiro para o nó a ser desalocado.
 */
void node23Deallocate(Memory **node)
{
    free(*node);
    *node = NULL;
}

/**
 * @brief Adiciona uma nova chave a um nó 2-3 existente.
 *
 * @param node Ponteiro para o nó onde a chave será adicionada.
 * @param info Informação a ser adicionada.
 * @param biggerChild Ponteiro para o filho maior associado à nova chave.
 */
void nodeAddInfo(Memory *node, Info info, Memory *biggerChild)
{
    if (info.start > node->info1->start)
    {
        if (node->info2 == NULL)
        {
            node->info2 = (Info *)malloc(sizeof(Info));
            if (node->info2 == NULL)
            {
                fprintf(stderr, "Erro ao alocar memória para info2\n");
                exit(EXIT_FAILURE);
            }
        }
        *(node->info2) = info;
        node->right = biggerChild;
    }
    else
    {
        if (node->info2 == NULL)
        {
            node->info2 = (Info *)malloc(sizeof(Info));
            if (node->info2 == NULL)
            {
                fprintf(stderr, "Erro ao alocar memória para info2\n");
                exit(EXIT_FAILURE);
            }
        }
        *(node->info2) = *(node->info1);

        node->right = node->center;
        node->center = biggerChild;
        *(node->info1) = info;
    }

    node->numKeys = 2;
}

/**
 * @brief Combina informações e nós em uma estrutura 2-3.
 *
 * Essa função adiciona uma informação ao nó `child1`, ajusta a quantidade de chaves no nó raiz e desaloca a raiz caso ela fique vazia após a operação.
 *
 * @param child1 Ponteiro para o nó que será combinado.
 * @param info Informação que será adicionada ao nó.
 * @param bigger Ponteiro para o nó maior que será combinado.
 * @param root Ponteiro duplo para a raiz da árvore.
 * @return Ponteiro para o nó `child1`, que se torna o nó resultante.
 */
Memory *node23Together(Memory *child1, Info info, Memory *bigger, Memory **root)
{
    nodeAddInfo(child1, info, bigger);

    (*root)->numKeys--;

    if ((*root)->numKeys == 0)
        node23Deallocate(root);

    return (child1);
}

/**
 * @brief Obtém a maior informação armazenada no nó 2-3.
 *
 * Retorna a maior informação do nó com base no número de chaves presentes.
 *
 * @param root Ponteiro para o nó atual.
 * @return Maior informação armazenada no nó.
 */
static Info node23BiggerInfo(Memory *root)
{
    return (root->numKeys == 2 ? *(root->info2) : *(root->info1));
}

/**
 * @brief Localiza o filho menor (à esquerda) de uma subárvore 2-3.
 *
 * Essa função percorre a subárvore à esquerda até encontrar um nó folha e retorna esse nó.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param father Ponteiro para armazenar o pai do nó encontrado.
 * @return Ponteiro para o menor nó folha encontrado.
 */
static Memory *lookMinorChild(Memory *root, Memory **father)
{
    Memory *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        child = child->left;
    }

    return (child);
}

/**
 * @brief Localiza o filho maior (à direita) de uma subárvore 2-3.
 *
 * Essa função percorre a subárvore à direita até encontrar um nó folha e retorna esse nó.
 * A maior informação do nó encontrado também é armazenada.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param father Ponteiro para armazenar o pai do nó encontrado.
 * @param bigger_valor Ponteiro para armazenar a maior informação encontrada.
 * @return Ponteiro para o maior nó folha encontrado.
 */
static Memory *lookBiggerChild(Memory *root, Memory **father, Info *bigger_valor)
{
    Memory *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        if (child->numKeys == 1)
            child = child->center;
        else
            child = child->right;
    }

    if (child != NULL)
        *bigger_valor = node23BiggerInfo(child);

    return (child);
}

/**
 * @brief Encontra o pai de um nó contendo uma informação específica.
 *
 * Essa função percorre a árvore para encontrar o pai de um nó que contém a informação especificada.
 * Caso o pai não seja encontrado, retorna NULL.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param info Informação a ser localizada.
 * @return Ponteiro para o nó pai contendo a informação, ou NULL caso não seja encontrado.
 */
static Memory *sourceFather(Memory *root, int info)
{
    Memory *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (info < root->info1->start)
                father = sourceFather(root->left, info);
            else if (root->numKeys == 1 || info < root->info2->start)
                father = sourceFather(root->center, info);
            else
                father = sourceFather(root->right, info);

            if (father == NULL)
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Localiza o pai do maior nó que contém uma informação maior que um valor dado.
 *
 * Essa função percorre a árvore para encontrar o pai de um nó cujo valor é maior que a informação especificada.
 * Caso o pai não seja encontrado, retorna NULL.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param info Informação a ser localizada.
 * @return Ponteiro para o nó pai contendo a maior informação maior que o valor especificado, ou NULL caso não seja encontrado.
 */
static Memory *sourceBiggerFather(Memory *root, int info)
{
    Memory *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (info < root->info1->start)
                father = sourceBiggerFather(root->left, info);
            else if (root->numKeys == 1 || info < root->info2->start)
                father = sourceBiggerFather(root->center, info);
            else
                father = sourceBiggerFather(root->right, info);

            if (father == NULL && ((root->numKeys == 1 && root->info1->start > info) || (root->numKeys == 2 && root->info2->start > info)))
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Localiza o pai do menor nó que contém uma informação menor que um valor dado.
 *
 * Essa função percorre a árvore para encontrar o pai de um nó cujo valor é menor que a informação especificada.
 * Caso o pai não seja encontrado, retorna NULL.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param info Informação a ser localizada.
 * @return Ponteiro para o nó pai contendo a menor informação menor que o valor especificado, ou NULL caso não seja encontrado.
 */
static Memory *sourceMinorFather(Memory *root, int info)
{
    Memory *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (info < root->info1->start)
                father = sourceMinorFather(root->left, info);
            else if (root->numKeys == 1 || info < root->info2->start)
                father = sourceMinorFather(root->center, info);
            else
                father = sourceMinorFather(root->right, info);

            if (father == NULL && root->info1->start < info)
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Localiza o pai do menor nó que contém duas informações e é menor que um valor dado.
 *
 * Essa função percorre a árvore para encontrar o pai de um nó contendo duas informações, cujo valor é menor que a informação especificada.
 * Caso o pai não seja encontrado, retorna NULL.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param info Informação a ser localizada.
 * @return Ponteiro para o nó pai contendo a menor informação menor que o valor especificado e com duas informações, ou NULL caso não seja encontrado.
 */
static Memory *sourceMinorFather2Infos(Memory *root, int info)
{
    Memory *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (info < root->info1->start)
                father = sourceMinorFather2Infos(root->left, info);
            else if (root->numKeys == 1 || info < root->info2->start)
                father = sourceMinorFather2Infos(root->center, info);
            else
                father = sourceMinorFather2Infos(root->right, info);

            if (father == NULL && root->numKeys == 2 && root->info2->start < info)
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Verifica se é possível remover um nó específico da árvore.
 *
 * Essa função verifica se um nó na árvore pode ser removido com base em suas chaves e filhos.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @return Retorna 1 se for possível remover o nó, caso contrário, retorna 0.
 */
int possibleRemove(Memory *root)
{
    int possible = 0;

    if (root != NULL)
    {
        possible = root->numKeys == 2;

        if (!possible)
        {
            possible = possibleRemove(root->center);

            if (!possible)
                possible = possibleRemove(root->left);
        }
    }

    return (possible);
}

/**
 * @brief Movimenta informações durante a remoção para manter o equilíbrio da árvore.
 *
 * Essa função realiza o movimento de uma informação, transferindo-a entre nós para manter o equilíbrio da árvore 2-3.
 *
 * @param leaving Informação que está saindo.
 * @param input Ponteiro para a nova informação a ser inserida.
 * @param father Ponteiro para o pai do nó afetado.
 * @param origin Ponteiro para a raiz da subárvore de origem.
 * @param root Ponteiro para a raiz da árvore.
 * @param bigger Ponteiro para o maior nó afetado.
 * @param removeFunction Função que será usada para remover a informação.
 * @return Retorna 1 se a operação foi bem-sucedida, ou 0 caso contrário.
 */
static int waveMoviment(Info leaving, Info *input, Memory *father, Memory **origin, Memory **root, Memory **bigger, int (*removeFunction)(Memory **, int, Memory *, Memory **, Memory **))
{
    int removed = removeFunction(root, leaving.start, father, origin, bigger);
    *input = leaving;

    return (removed);
}

/**
 * @brief Cria uma nova árvore de memória, inicialmente vazia.
 *
 * A função simplesmente retorna NULL, indicando que a árvore de memória está vazia.
 *
 * @return Ponteiro NULL, indicando que não há memória alocada.
 */
Memory *createMemory()
{
    return NULL;
}

/**
 * @brief Desaloca recursivamente todos os nós da árvore de memória.
 *
 * A função realiza a desalocação de todos os nós da árvore binária de 2-3,
 * liberando a memória de todos os seus filhos e do próprio nó.
 *
 * @param root Ponteiro duplo para o nó raiz da árvore.
 */
void deallocateMemory(Memory **root)
{
    if (*root != NULL)
    {
        deallocateMemory(&((*root)->left));
        deallocateMemory(&((*root)->center));

        if ((*root)->numKeys == 2)
            deallocateMemory(&((*root)->right));

        node23Deallocate(root);
    }
}

/**
 * @brief Remove um nó de memória em um nó não folha (primeiro caso de remoção).
 *
 * A função remove um nó da árvore binária de 2-3 em uma posição não-folha, fazendo o ajuste de
 * filhos e promovendo informações de outros nós para preencher o espaço.
 *
 * @param origin Ponteiro duplo para o nó raiz da árvore original.
 * @param root Nó atual que contém o valor a ser removido.
 * @param info O valor que será removido.
 * @param child1 Filho 1 da árvore.
 * @param child2 Filho 2 da árvore.
 * @param bigger Ponteiro para o nó de maior valor que será atualizado.
 * @return Inteiro indicando o sucesso ou falha da remoção.
 */
int memoryRemoveNoLeaf1(Memory **origin, Memory *root, Info *info, Memory *child1, Memory *child2, Memory **bigger)
{
    int removed;
    Memory *child, *father;
    Info info_child;

    father = root;

    child = lookBiggerChild(child1, &father, &info_child);

    if (child->numKeys == 2)
    {
        *info = info_child;
        child->numKeys = 1;
    }
    else
    {
        child = lookMinorChild(child2, &father);
        removed = waveMoviment(*(child->info1), info, father, origin, &child, bigger, removeMemory1);
    }

    return (removed);
}

/**
 * @brief Remove um nó de memória em um nó não folha (segundo caso de remoção).
 *
 * A função remove um nó da árvore binária de 2-3 em uma posição não-folha, ajustando filhos
 * e promovendo informações para manter a árvore balanceada.
 *
 * @param origin Ponteiro duplo para o nó raiz da árvore original.
 * @param root Nó atual que contém o valor a ser removido.
 * @param info O valor que será removido.
 * @param child1 Filho 1 da árvore.
 * @param child2 Filho 2 da árvore.
 * @param bigger Ponteiro para o nó de maior valor que será atualizado.
 * @return Inteiro indicando o sucesso ou falha da remoção.
 */
int memoryRemoveNoLeaf2(Memory **origin, Memory *root, Info *info, Memory *child1, Memory *child2, Memory **bigger)
{
    int removed;
    Memory *child, *father;
    Info info_child;

    father = root;

    child = lookMinorChild(child1, &father);

    if (child->numKeys == 2)
    {
        *info = *(child->info1);
        *(child->info1) = *(child->info2);
        child->numKeys = 1;
    }
    else
    {
        child = lookBiggerChild(child2, &father, &info_child);
        removed = waveMoviment(info_child, info, father, origin, &child, bigger, memoryRemove2);
    }

    return removed;
}

/**
 * @brief Remove um nó de memória do nó folha 1.
 *
 * A função realiza a remoção de um nó de memória quando o nó é folha, ajustando
 * os filhos e promovendo valores para garantir a integridade da árvore.
 *
 * @param root Ponteiro duplo para o nó raiz da árvore.
 * @param info Valor a ser removido.
 * @param father Nó pai do nó atual.
 * @param origin Ponteiro duplo para a árvore original.
 * @param bigger Ponteiro para o nó maior que será atualizado.
 * @return Inteiro indicando o sucesso ou falha da remoção.
 */
int removeMemory1(Memory **root, int info, Memory *father, Memory **origin, Memory **bigger)
{
    int removed = 0;

    if (*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if (info1 || info2)
        {
            removed = 1;
            if (isLeaf(*root))
            {
                if ((*root)->numKeys == 2)
                {
                    if (info1)
                        (*root)->info1 = (*root)->info2;

                    (*root)->numKeys = 1;
                }
                else
                {
                    if (father == NULL)
                        node23Deallocate(root);
                    else
                    {
                        Memory *auxFather;
                        Info info_father;
                        if (*root == father->left || (father->numKeys == 2 && *root == father->center))
                        {
                            auxFather = sourceFather(*origin, father->info1->start);

                            if (*root == father->left)
                                info_father = *(father->info1);
                            else
                                info_father = *(father->info2);

                            removed = waveMoviment(info_father, (*root)->info1, auxFather, origin, &father, bigger, removeMemory1);
                        }
                        else
                        {
                            auxFather = sourceBiggerFather(*origin, (*root)->info1->start);

                            Memory *menor_father;
                            menor_father = sourceMinorFather2Infos(*origin, (*root)->info1->start);

                            if (auxFather == NULL || (auxFather != father && menor_father != NULL))
                            {
                                *bigger = father;
                                (*root)->numKeys = 0;
                                removed = -1;
                            }
                            else
                            {
                                if (auxFather->info1->start > (*root)->info1->start)
                                    info_father = *(auxFather->info1);
                                else
                                    info_father = *(auxFather->info2);

                                Memory *avo;
                                avo = sourceFather(*origin, info_father.start);
                                removed = waveMoviment(info_father, (*root)->info1, avo, origin, &auxFather, bigger, removeMemory1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removed = memoryRemoveNoLeaf1(origin, *root, (*root)->info2, (*root)->center, (*root)->right, bigger);
            else if (info1)
                removed = memoryRemoveNoLeaf1(origin, *root, (*root)->info1, (*root)->left, (*root)->center, bigger);
        }
        else
        {
            if (info < (*root)->info1->start)
                removed = removeMemory1(&(*root)->left, info, *root, origin, bigger);
            else if ((*root)->numKeys == 1 || info < (*root)->info2->start)
                removed = removeMemory1(&(*root)->center, info, *root, origin, bigger);
            else
                removed = removeMemory1(&(*root)->right, info, *root, origin, bigger);
        }
    }
    return (removed);
}

/**
 * @brief Remove uma informação específica da árvore de memória. A função verifica a presença da informação e a remove caso seja encontrada.
 * Ela trabalha recursivamente nas subárvores esquerda, central ou direita, dependendo da posição da informação na árvore. Se a remoção
 * resultar em um desequilíbrio, a árvore é rebalanceada.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info A informação a ser removida.
 * @param father O pai do nó atual (usado para balanceamento).
 * @param origin A raiz original da árvore.
 * @param bigger Um ponteiro para armazenar o nó de memória maior, caso seja necessário durante o balanceamento.
 * @return Retorna 1 se a informação foi removida com sucesso, -1 se houve desequilíbrio, e 0 se a informação não foi encontrada.
 */
int memoryRemove2(Memory **root, int info, Memory *father, Memory **origin, Memory **bigger)
{
    int removed = 0;

    if (*root != NULL)
    {
        int info1 = isInfo1(**root, info);
        int info2 = isInfo2(**root, info);

        if (info1 || info2)
        {
            removed = 1;
            if (isLeaf(*root))
            {
                if ((*root)->numKeys == 2)
                {
                    if (info1)
                        (*root)->info1 = (*root)->info2;

                    (*root)->numKeys = 1;
                }
                else
                {
                    if (father == NULL)
                        node23Deallocate(root);
                    else
                    {
                        Memory *auxFather;
                        Info info_father;
                        if (*root == father->center || (father->numKeys == 2 && *root == father->right))
                        {
                            auxFather = sourceFather(*origin, father->info1->start);

                            if (*root == father->center)
                                info_father = *(father->info1);
                            else
                                info_father = *(father->info2);

                            removed = waveMoviment(info_father, (*root)->info1, auxFather, origin, &father, bigger, memoryRemove2);
                        }
                        else
                        {
                            auxFather = sourceMinorFather(*origin, (*root)->info1->start);

                            Memory *menor_father;
                            menor_father = sourceMinorFather2Infos(*origin, (*root)->info1->start);

                            Memory *avo;
                            if (auxFather == NULL || (auxFather != father && menor_father != NULL))
                            {
                                removed = -1;
                                *bigger = father;
                            }
                            else
                            {
                                if (auxFather->numKeys == 2 && auxFather->info2->start < (*root)->info1->start)
                                    info_father = *(auxFather->info2);
                                else
                                    info_father = *(auxFather->info1);

                                avo = sourceFather(*origin, info_father.start);
                                removed = waveMoviment(info_father, (*root)->info1, avo, origin, &auxFather, bigger, memoryRemove2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removed = memoryRemoveNoLeaf2(origin, *root, (*root)->info2, (*root)->right, (*root)->center, bigger);
            else if (info1)
                removed = memoryRemoveNoLeaf2(origin, *root, (*root)->info1, (*root)->center, (*root)->left, bigger);
        }
        else
        {
            if (info < (*root)->info1->start)
                removed = memoryRemove2(&(*root)->left, info, *root, origin, bigger);
            else if ((*root)->numKeys == 1 || info < (*root)->info2->start)
                removed = memoryRemove2(&(*root)->center, info, *root, origin, bigger);
            else
                removed = memoryRemove2(&(*root)->right, info, *root, origin, bigger);
        }
    }
    return (removed);
}

/**
 * @brief Função principal para remover uma informação da árvore de memória. Se a informação não for encontrada, tenta-se rebalancear
 * a árvore e remover a informação novamente. Esta função lida com o rebalanceamento e casos onde a remoção não é trivial.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info A informação a ser removida.
 * @return Retorna 1 se a informação foi removida, -1 se houve um desequilíbrio e foi necessário rebalanceamento, ou 0 se a informação não foi encontrada.
 */
int removeMemory(Memory **root, int info)
{
    Memory *bigger, *junctionPosition;
    int removed = removeMemory1(root, info, NULL, root, &junctionPosition);

    if (removed == -1)
    {
        Info junctionValue = node23BiggerInfo(junctionPosition);
        bigger = NULL;
        removed = memoryRebalance(root, junctionValue.start, &bigger);

        if (removed == -1)
        {
            Memory *father, *junctionPosition2;
            Info *input;
            father = sourceFather(*root, junctionValue.start);

            if (isInfo1(*junctionPosition, junctionValue.start))
                input = junctionPosition->center->info1;
            else
                input = junctionPosition->right->info1;

            removed = waveMoviment(junctionValue, input, father, root, &junctionPosition, &junctionPosition2, memoryRemove2);

            if (removed == -1)
            {
                junctionValue = *(junctionPosition2->info1);
                father = sourceFather(*root, junctionValue.start);
                removed = waveMoviment(junctionValue, junctionPosition2->left->info1, father, root, &junctionPosition2, &junctionPosition, removeMemory1);

                junctionValue = node23BiggerInfo(junctionPosition);
                bigger = NULL;
                removed = memoryRebalance(root, junctionValue.start, &bigger);
            }
        }

        if (*root == NULL)
            *root = bigger;
    }

    return (removed);
}

/**
 * @brief Rebalanceia a árvore se ela ficar desequilibrada após uma remoção. Tenta realizar rotações e redistribuições nos nós.
 * A função verifica recursivamente as subárvores para determinar se é necessário fazer ajustes e realiza os ajustes necessários.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info A informação usada para rebalancear a árvore.
 * @param bigger Um ponteiro para armazenar o nó de memória maior, caso seja necessário durante o balanceamento.
 * @return Retorna 0 se a árvore estiver balanceada, -1 se a árvore ainda estiver desequilibrada e precisar de mais ajustes.
 */
int memoryRebalance(Memory **root, int info, Memory **bigger)
{
    int balanced = 0;
    if (*root != NULL)
    {
        if (!isLeaf(*root))
        {
            if (info < (*root)->info1->start)
                balanced = memoryRebalance(&((*root)->left), info, bigger);
            else if ((*root)->numKeys == 1 || info < (*root)->info2->start)
            {
                if ((*root)->left->numKeys == 2 && !possibleRemove((*root)->center))
                    balanced = -1;
                else
                    balanced = memoryRebalance(&((*root)->center), info, bigger);
            }
            else
            {
                if ((*root)->center->numKeys == 2 && !possibleRemove((*root)->right))
                    balanced = -1;
                else
                    balanced = memoryRebalance(&((*root)->right), info, bigger);
            }

            if (balanced != -1)
            {
                if ((*root)->numKeys == 1)
                    balanced = balancing(root, (*root)->left, &((*root)->center), *(*root)->info1, bigger);
                else if ((*root)->numKeys == 2)
                    balanced = balancing(root, (*root)->center, &((*root)->right), *(*root)->info2, bigger);
            }
        }
    }

    return (balanced);
}
