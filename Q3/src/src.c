#include "src.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * @param leftChild Ponteiro para o filho a esquerda do nó.
 * @param centerChild Ponteiro para o filho do centro do nó.
 * @return Ponteiro para o nó criado.
 */
Memory *createNode(const Info *info, Memory *leftChild, Memory *centerChild)
{
    Memory *node = (Memory *)malloc(sizeof(Memory));
    if (node == NULL)
    {
        printf("Erro ao alocar memória para o nó.\n");
        return NULL;
    }

    node->info1 = (Info *)malloc(sizeof(Info));
    if (node->info1 == NULL)
    {
        printf("Erro ao alocar memória para info1.\n");
        free(node);
        return NULL;
    }
    *(node->info1) = *info;

    node->info2 = NULL;
    node->left = leftChild;
    node->center = centerChild;
    node->right = NULL;
    node->numKeys = 1;

    return node;
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
 * Adiciona informações a um nó da árvore 2-3.
 * 
 * @param no Ponteiro para o nó onde as informações serão adicionadas.
 * @param info Estrutura Info que contém as informações a serem adicionadas.
 * @param biggerChild Ponteiro para o nó filho que será maior, se necessário.
 */
void nodeAddInfo(Memory *no, Info info, Memory *biggerChild)
{
    Info *newInfo = (Info *)malloc(sizeof(Info));
    if (newInfo != NULL)
    {
        *newInfo = info;

        if (info.start > no->info1->start)
        {
            no->info2 = newInfo;
            no->right = biggerChild;
        }
        else
        {
            no->info2 = no->info1;
            no->right = no->center;
            no->center = biggerChild;
            no->info1 = newInfo;
        }
        no->numKeys = 2;
    }
}

/**
 * @brief Realiza a divisão de um nó 2-3, promotendo uma chave e criando um novo nó.
 *
 * @param node Nó a ser dividido.
 * @param info Informação a ser inserida durante a divisão.
 * @param promote Ponteiro para armazenar a chave promovida.
 * @param biggerChild Ponteiro para o maior filho após a divisão.
 * @return Memory* Retorna um ponteiro para o novo nó criado durante a divisão.
 */
Memory *nodeBreak(Memory *no, Info info, Info *promote, Memory *biggerChild)
{
    Memory *bigger;
    if (info.start > no->info2->start)
    {
        *promote = *(no->info2);
        bigger = createNode(&info, no->right, biggerChild);
    }
    else if (info.start > no->info1->start)
    {
        *promote = info;
        bigger = createNode(no->info2, biggerChild, no->right);
    }
    else
    {
        *promote = *(no->info1);
        bigger = createNode(no->info2, no->center, no->right);
        no->info1 = &info;
        no->center = biggerChild;
    }
    no->numKeys = 1;

    return (bigger);
}

/**
 * Insere um novo nó na árvore 2-3.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param info Estrutura Info a ser inserida.
 * @param father Ponteiro para o pai do nó atual.
 * @param promote Ponteiro para a informação que será promovida, se necessário.
 * @return Ponteiro para o nó maior resultante da inserção, se houver.
 */
Memory *insertTree23(Memory **root, Info info, Memory *father, Info *promote)
{
    Memory *bigger;
    bigger = NULL;

    if (*root == NULL)
        *root = createNode(&info, NULL, NULL);
    else
    {
        if (isLeaf(*root))
        {
            if ((*root)->numKeys == 1)
                nodeAddInfo(*root, info, NULL);
            else
            {
                bigger = nodeBreak(*root, info, promote, NULL);
                if (father == NULL)
                {
                    *root = createNode(promote, *root, bigger);
                    bigger = NULL;
                }
            }
        }
        else
        {
            if (info.start < (*root)->info1->start)
                bigger = insertTree23(&((*root)->left), info, *root, promote);
            else if ((*root)->numKeys == 1 || info.start < (*root)->info2->start)
                bigger = insertTree23(&((*root)->center), info, *root, promote);
            else
                bigger = insertTree23(&((*root)->right), info, *root, promote);

            if (bigger != NULL)
            {
                if ((*root)->numKeys == 1)
                {
                    nodeAddInfo(*root, *promote, bigger);
                    bigger = NULL;
                }
                else
                {
                    Info promote_aux;
                    bigger = nodeBreak(*root, *promote, &promote_aux, bigger);
                    *promote = promote_aux;
                    if (father == NULL)
                    {
                        *root = createNode(&promote_aux, *root, bigger);
                        bigger = NULL;
                    }
                }
            }
        }
    }

    return (bigger);
}

/**
 * Calcula a quantidade disponível em um intervalo de informações.
 * 
 * @param info Estrutura Info a ser verificada.
 * @return A quantidade disponível no intervalo.
 */
int availableQuant(Info info)
{
    return (info.end - info.start + 1);
}

/**
 * Encontra um espaço disponível na árvore para alocar um determinado número de unidades.
 * 
 * @param tree Ponteiro duplo para a árvore onde a busca será realizada.
 * @param quant Quantidade de unidades a serem alocadas.
 * @param status Status da informação a ser alocada.
 * @param info Ponteiro para um ponteiro de Info onde será armazenada a informação encontrada.
 * @return Ponteiro para o nó onde o espaço foi encontrado, ou NULL se não houver espaço.
 */
Memory *findSpace(Memory **tree, int quant, int status, Info **info)
{
    Memory *no;
    if (*tree != NULL)
    {
        no = findSpace(&((*tree)->left), quant, status, info);

        if (*info == NULL)
        {
            if ((*tree)->info1->status == status && availableQuant(*((*tree)->info1)) >= quant)
            {
                *info = (*tree)->info1;
                no = *tree;
            }
            else
            {
                no = findSpace(&((*tree)->center), quant, status, info);
                if ((*tree)->numKeys == 2)
                {
                    if ((*tree)->info2->status == status && availableQuant(*((*tree)->info2)) >= quant)
                    {
                        *info = (*tree)->info2;
                        no = *tree;
                    }
                    else if (*info == NULL)
                        no = findSpace(&((*tree)->right), quant, status, info);
                }
            }
        }
    }
    else
        *info = NULL;

    return (no);
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
 * Insere uma nova informação na árvore de memória.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param info Estrutura Info a ser inserida.
 * @param father recebe o pai com parâmetro
 * @param promote Informação que sobe ao quebrar nó.
 * @return Maior valor.
 */
Memory *MemoryInsertNode(Memory **root, Info info, Memory *father, Info *promote)
{
    Memory *bigger;
    bigger = NULL;

    if (*root == NULL)
        *root = createNode(&info, NULL, NULL);
    else
    {
        if (isLeaf(*root))
        {
            if ((*root)->numKeys == 1)
                nodeAddInfo(*root, info, NULL);
            else
            {
                bigger = nodeBreak(*root, info, promote, NULL);
                if (father == NULL)
                {
                    *root = createNode(&(*promote), *root, bigger);
                    bigger = NULL;
                }
            }
        }
        else
        {
            if (info.start < (*root)->info1->start)
                bigger = MemoryInsertNode(&((*root)->left), info, *root, promote);
            else if ((*root)->numKeys == 1 || info.start < (*root)->info2->start)
                bigger = MemoryInsertNode(&((*root)->center), info, *root, promote);
            else
                bigger = MemoryInsertNode(&((*root)->right), info, *root, promote);

            if (bigger != NULL)
            {
                if ((*root)->numKeys == 1)
                {
                    nodeAddInfo(*root, *promote, bigger);
                    bigger = NULL;
                }
                else
                {
                    Info promote_aux;
                    bigger = nodeBreak(*root, *promote, &promote_aux, bigger);
                    *promote = promote_aux;
                    if (father == NULL)
                    {
                        *root = createNode(&promote_aux, *root, bigger);
                        bigger = NULL;
                    }
                }
            }
        }
    }

    return (bigger);
}

/**
 * Insere uma nova informação na árvore de memória.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param info Estrutura Info a ser inserida.
 * @return Ponteiro para o nó resultante da inserção.
 */
Memory *insertMemory(Memory **root, Info info)
{
    Info promote;
    return MemoryInsertNode(root, info, NULL, &promote);
}

/**
 * Encontra o menor bloco de memória a partir de um nó.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param no Ponteiro para o nó a partir do qual a busca será realizada.
 * @param info Ponteiro para a informação a ser verificada.
 * @param minorValue Ponteiro para um ponteiro de Info onde será armazenada a informação menor encontrada.
 * @return Ponteiro para o nó onde o menor bloco foi encontrado.
 */
Memory *sourceMinorBlock(Memory **root, Memory *no, Info *info, Info **minorValue)
{
    Memory *minor, *father;
    *minorValue = NULL;

    if (isLeaf(no))
    {
        if (no->info1->start != info->start)
            minor = no;
        else
            minor = sourceMinorFather(*root, info->start);

        if (minor != NULL)
        {
            if (minor->numKeys == 2 && minor->info2->start < info->start)
                *minorValue = minor->info2;
            else
                *minorValue = minor->info1;
        }
    }
    else if (no->info1->start == info->start)
        minor = lookBiggerChild(no->left, &father, minorValue);
    else
        minor = lookBiggerChild(no->center, &father, minorValue);

    return minor;
}

/**
 * Encontra o maior bloco de memória a partir de um nó.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param no Ponteiro para o nó a partir do qual a busca será realizada.
 * @param info Ponteiro para a informação a ser verificada.
 * @param biggerValue Ponteiro para um ponteiro de Info onde será armazenada a informação maior encontrada.
 * @return Ponteiro para o nó onde o maior bloco foi encontrado.
 */
Memory *sourceBiggerBlock(Memory **root, Memory *no, Info *info, Info **biggerValue)
{
    Memory *bigger;
    Memory *father;
    *biggerValue = NULL;

    if(isLeaf(no))
    {
        if(no->numKeys == 2 && no->info1->start == info->start)
            bigger = no;
        else
            bigger = sourceBiggerFather(*root, info->start);

        if(bigger != NULL)
        {
            if(bigger->info1->start > info->start)
                *biggerValue = bigger->info1;
            else
                *biggerValue = bigger->info2;
        }
    }
    else
    {
        if(no->info1->start == info->start)
            bigger = lookMinorChild(no->center, &father);
        else
            bigger = lookMinorChild(no->right, &father);

        if(bigger != NULL)
            *biggerValue = bigger->info1;
    }

    return (bigger);
}

/**
 * Concatena um nó na árvore de memória.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param lastNumber Ponteiro para o último número armazenado.
 * @param limit Limite de concatenação.
 * @param removeValue Valor a ser removido da memória.
 */
void concatenateNode(Memory **root, int *lastNumber, int limit, int removeValue)
{
    *lastNumber = limit;
    removeMemory(root, removeValue);
}

/**
 * Modifica um nó na árvore de memória.
 * 
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param no Ponteiro para o nó a ser modificado.
 * @param info Ponteiro para a informação a ser modificada.
 * @param quant Quantidade a ser ajustada.
 */
void modifyNode(Memory **root, Memory *no, Info *info, int quant)
{
    Memory *minor;
    Info *minorValue;

    minor = sourceMinorBlock(root, no, info, &minorValue);

    if (quant < availableQuant(*info))
    {
        if (minor == NULL)
        {
            Info Info;
            Info.start = info->start;
            Info.end = info->start + quant - 1;
            Info.status = !(info->status);

            info->start += quant;
            insertMemory(root, Info);
        }
        else
        {
            minorValue->end += quant;
            info->start += quant;
        }
    }
    else
    {
        Memory *bigger;
        Info *biggerValue;

        bigger = sourceBiggerBlock(root, no, info, &biggerValue);

        if (minor == NULL && bigger == NULL)
            info->status = !(info->status);
        else
        {
            if (minor == NULL)
            {
                info->status = !(info->status);
                concatenateNode(root, &(info->end), biggerValue->end, biggerValue->start);
            }
            else if (bigger == NULL)
                concatenateNode(root, &(minorValue->end), info->end, info->start);
            else
            {
                int numero = biggerValue->start;
                concatenateNode(root, &(minorValue->end), biggerValue->end, info->start);
                removeMemory(root, numero);
            }
        }
    }
}

/**
 * Aloca e desaloca espaço na árvore de memória.
 * 
 * @param tree Ponteiro duplo para a árvore onde a alocação será realizada.
 * @param quantNodes Quantidade de nós a serem alocados.
 * @param status Status da informação a ser alocada.
 */
void allocateAndDesallocate(Memory **tree, int quantNodes, int status)
{
    Info *info;
    info = NULL;
    Memory *node;
    node = findSpace(tree, quantNodes, status, &info);

    if (info != NULL)
        modifyNode(tree, node, info, quantNodes);
    else
        printf("\nNão há espaço disponível\n");
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
 * @return maior informação armazenada no nó.
 */
Info *node23BiggerInfo(Memory *root)
{
    return (root->numKeys == 2 ? root->info2 : root->info1);
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
Memory *lookMinorChild(Memory *root, Memory **father)
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
 * @param biggerValue Ponteiro para armazenar a maior informação encontrada.
 * @return Ponteiro para o maior nó folha encontrado.
 */
Memory *lookBiggerChild(Memory *root, Memory **father, Info **biggerValue)
{
    Memory *child;
    child = root;

    while(!isLeaf(child))
    {
        *father = child;
        if(child->numKeys == 1)
            child = child->center;
        else
            child = child->right;
    }

    if(child != NULL)
        *biggerValue = node23BiggerInfo(child);

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
Memory *sourceFather(Memory *root, int info)
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
Memory *sourceBiggerFather(Memory *root, int info)
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
 * Caso o father não seja encontrado, retorna NULL.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 * @param info Informação a ser localizada.
 * @return Ponteiro para o nó pai contendo a menor informação menor que o valor especificado, ou NULL caso não seja encontrado.
 */
Memory *sourceMinorFather(Memory *root, int info)
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
 * filhos e promotendo informações de outros nós para preencher o espaço.
 *
 * @param origin Ponteiro duplo para o nó raiz da árvore original.
 * @param root Nó atual que contém o valor a ser removido.
 * @param info O valor que será removido.
 * @param child1 filho 1 da árvore.
 * @param child2 filho 2 da árvore.
 * @param bigger Ponteiro para o nó de maior valor que será atualizado.
 * @return Inteiro indicando o sucesso ou falha da remoção.
 */
int memoryRemoveNoLeaf1(Memory **origin, Memory *root, Info *info, Memory *child1, Memory *child2, Memory **bigger)
{
    int removed;
    Memory *child, *father;
    Info *info_child;

    father = root;
    child = lookBiggerChild(child1, &father, &info_child);

    if (child->numKeys == 2)
    {
        info = info_child;
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
 * e promotendo informações para manter a árvore balanceada.
 *
 * @param origin Ponteiro duplo para o nó raiz da árvore original.
 * @param root Nó atual que contém o valor a ser removido.
 * @param info O valor que será removido.
 * @param child1 filho 1 da árvore.
 * @param child2 filho 2 da árvore.
 * @param bigger Ponteiro para o nó de maior valor que será atualizado.
 * @return Inteiro indicando o sucesso ou falha da remoção.
 */
int memoryRemoveNoLeaf2(Memory **origin, Memory *root, Info *info, Memory *child1, Memory *child2, Memory **bigger)
{
    int removed;
    Memory *child, *father;
    Info *info_child;

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
        removed = waveMoviment(*info_child, info, father, origin, &child, bigger, memoryRemove2);
    }

    return (removed);
}

/**
 * @brief Remove um nó de memória do nó folha 1.
 *
 * A função realiza a remoção de um nó de memória quando o nó é folha, ajustando
 * os filhos e promotendo valores para garantir a integridade da árvore.
 *
 * @param root Ponteiro duplo para o nó root da árvore.
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

                            Memory *minor_father;
                            minor_father = sourceMinorFather2Infos(*origin, (*root)->info1->start);

                            if (auxFather == NULL || (auxFather != father && minor_father != NULL))
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

                            Memory *minor_father;
                            minor_father = sourceMinorFather2Infos(*origin, (*root)->info1->start);

                            Memory *avo;
                            if (auxFather == NULL || (auxFather != father && minor_father != NULL))
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
        Info junctionValue = *node23BiggerInfo(junctionPosition);
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

                junctionValue = *node23BiggerInfo(junctionPosition);
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
