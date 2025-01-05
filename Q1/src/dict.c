#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #######################################################################################
// Árvore 2-3
// #######################################################################################

/**
 * @brief Cria uma estrutura TranslationInfo com a palavra em português, palavra em inglês e unidade fornecidas.
 *
 * @param palavra Ponteiro para a palavra em português.
 * @param englishWord Ponteiro para a palavra em inglês.
 * @param unit Ponteiro para a unidade associada à tradução.
 * @return TranslationInfo Estrutura TranslationInfo criada.
 */
TranslationInfo createInfo(char *word, char *englishWord, char *unit)
{
    TranslationInfo info;

    info.portugueseWord = malloc(strlen(word) + 1);
    strcpy(info.portugueseWord, word);

    info.englishWord = malloc(sizeof(EnglishBinTree));
    info.englishWord->englishWord = malloc(strlen(englishWord) + 1);
    strcpy(info.englishWord->englishWord, englishWord);

    info.englishWord->units = NULL;
    InsertLinkedList(&(info.englishWord->units), unit);

    return (info);
}

/**
 * @brief Insere uma palavra em português e sua respectiva tradução e unidade na árvore 2-3.
 *
 * @param tree Ponteiro para a raiz da árvore 2-3 em português.
 * @param portugueseWord Ponteiro para a palavra em português a ser inserida.
 * @param englishWord Ponteiro para a palavra em inglês associada à palavra em português.
 * @param unit Ponteiro para a unidade associada à tradução.
 * @return int Retorna 1 se a palavra foi inserida com sucesso, 0 caso contrário.
 */
int insertPortugueseWord(Portuguese23Tree **tree, char *portugueseWord, char *englishWord, char *unit)
{
    TranslationInfo promote;
    Portuguese23Tree *father = NULL;
    int insered;

    TranslationInfo novoInfo = createInfo(portugueseWord, englishWord, unit);
    if (InsertTree23(tree, &novoInfo, &promote, &father))
        insered = 1;

    return (insered);
}

/**
 * @brief Cria um novo nó para a árvore 2-3 com o TranslationInfo fornecido e os ponteiros para os filhos.
 *
 * @param info Ponteiro para a estrutura TranslationInfo.
 * @param leftChild Ponteiro para o filho esquerdo do novo nó.
 * @param centerChild Ponteiro para o filho central do novo nó.
 * @return Portuguese23Tree* Ponteiro para o novo nó criado.
 */
Portuguese23Tree *createNode(const TranslationInfo *info, Portuguese23Tree *leftChild, Portuguese23Tree *centerChild)
{
    Portuguese23Tree *no;
    no = (Portuguese23Tree *)malloc(sizeof(Portuguese23Tree));

    no->info1 = *info;
    no->left = leftChild;
    no->middle = centerChild;
    no->infoCount = 1;

    no->info2.englishWord = NULL;
    no->info2.portugueseWord = NULL;
    no->right = NULL;

    return (no);
}

/**
 * @brief Adiciona uma chave a um nó da árvore 2-3 com uma única chave, atualizando seus filhos conforme necessário.
 *
 * @param no Ponteiro para o nó a ser atualizado.
 * @param info Ponteiro para o TranslationInfo a ser adicionado.
 * @param child Ponteiro para o nó filho associado à nova chave.
 */
void addKey(Portuguese23Tree **no, const TranslationInfo *info, Portuguese23Tree *child)
{
    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) > 0)
    {
        (*no)->info2 = *info;
        (*no)->right = child;
    }
    else
    {
        (*no)->info2 = (*no)->info1;
        (*no)->info1 = *info;
        (*no)->right = (*no)->middle;
        (*no)->middle = child;
    }
    (*no)->infoCount = 2;
}

/**
 * @brief Divide um nó da árvore 2-3 que contém duas chaves e insere uma nova chave, promovendo uma delas.
 *
 * @param no Ponteiro para o nó a ser dividido.
 * @param info Ponteiro para o TranslationInfo a ser inserido.
 * @param promote Ponteiro para armazenar a chave promovida.
 * @param child Ponteiro para o nó filho associado à nova chave.
 * @return Portuguese23Tree* Ponteiro para o novo nó criado após a divisão.
 */
Portuguese23Tree *breakNode(Portuguese23Tree **no, const TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree *child)
{
    Portuguese23Tree *bigger;

    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
    {
        *promote = (*no)->info1;
        bigger = createNode(&(*no)->info2, (*no)->middle, (*no)->right);

        (*no)->info1 = *info;
        (*no)->middle = child;
    }
    else if (strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
    {
        *promote = *info;
        bigger = createNode(&(*no)->info2, child, (*no)->right);
    }
    else
    {
        *promote = (*no)->info2;
        bigger = createNode(info, (*no)->right, child);
    }

    (*no)->info2.englishWord = NULL;
    (*no)->info2.portugueseWord = NULL;

    (*no)->infoCount = 1;
    (*no)->right = NULL;

    return (bigger);
}

/**
 * @brief Verifica se um nó da árvore 2-3 é uma folha.
 *
 * @param no Ponteiro para o nó a ser verificado.
 * @return int Retorna 1 se o nó é uma folha, 0 caso contrário.
 */
int isLeaf(const Portuguese23Tree *no)
{
    int troughet = 0;

    if (no->left == NULL)
        troughet = 1;

    return (troughet);
}

/**
 * @brief Insere um TranslationInfo na árvore 2-3.
 *
 * @param no Ponteiro para o nó raiz da árvore 2-3.
 * @param info Ponteiro para o TranslationInfo a ser inserido.
 * @param promote Ponteiro para armazenar a chave promovida em caso de divisão.
 * @param father Ponteiro para o pai do nó atual.
 * @return Portuguese23Tree* Ponteiro para o nó criado após a inserção, se aplicável.
 */
Portuguese23Tree *InsertTree23(Portuguese23Tree **no, TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree **father)
{
    TranslationInfo promote1;
    Portuguese23Tree *biggerNode = NULL;

    if (*no == NULL)
        *no = createNode(info, NULL, NULL);
    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) == 0)
        insertenglishWord(&(*no)->info1.englishWord, info);
    else if ((*no)->infoCount == 2 && strcmp(info->portugueseWord, (*no)->info2.portugueseWord) == 0)
        insertenglishWord(&(*no)->info2.englishWord, info);
    else
    {
        if (isLeaf(*no))
        {
            if ((*no)->infoCount == 1)
                addKey(no, info, NULL);
            else
            {
                Portuguese23Tree *novo;
                novo = breakNode(no, info, promote, NULL);
                if (*father == NULL)
                {
                    Portuguese23Tree *newroot;
                    newroot = createNode(promote, *no, novo);
                    *no = newroot;
                }
                else
                    biggerNode = novo;
            }
        }
        else
        {
            if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
                biggerNode = InsertTree23(&((*no)->left), info, promote, no);
            else if ((*no)->infoCount == 1 || strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
                biggerNode = InsertTree23(&((*no)->middle), info, promote, no);
            else
                biggerNode = InsertTree23(&((*no)->right), info, promote, no);

            if (biggerNode != NULL)
            {
                if ((*no)->infoCount == 1)
                {
                    addKey(no, promote, biggerNode);
                    biggerNode = NULL;
                }
                else
                {
                    Portuguese23Tree *novo;
                    novo = breakNode(no, promote, &promote1, biggerNode);
                    if (*father == NULL)
                    {
                        Portuguese23Tree *newroot;
                        newroot = createNode(&promote1, *no, novo);
                        *no = newroot;
                        biggerNode = NULL;
                    }
                    else
                    {
                        biggerNode = novo;
                        *promote = promote1;
                    }
                }
            }
        }
    }

    return (biggerNode);
}

/**
 * @brief Exibe o conteúdo da árvore 2-3 em ordem, mostrando as palavras em português e suas traduções.
 *
 * @param root Ponteiro para o nó raiz da árvore.
 */
void displayTree23(const Portuguese23Tree *root)
{
    if (root != NULL)
    {
        displayTree23(root->left);
        printf("PT: %s\n", root->info1.portugueseWord);
        printBinaryTree(root->info1.englishWord);
        printf("\n");
        displayTree23(root->middle);
        if (root->infoCount == 2)
        {
            printf("PT: %s\n", root->info2.portugueseWord);
            printBinaryTree(root->info2.englishWord);
            printf("\n");
            displayTree23(root->right);
        }
    }
}

/**
 * @brief Exibe as palavras em português associadas a uma unidade específica.
 *
 * @param tree Ponteiro para a raiz da árvore 2-3.
 * @param unit Ponteiro para a unidade a ser buscada.
 */
void displayWordsForUnit(Portuguese23Tree *tree, char *unit)
{
    if (tree)
    {
        displayWordsForUnit(tree->left, unit);
        displayTranslacions(tree->info1.englishWord, unit, tree->info1.portugueseWord);

        displayWordsForUnit(tree->middle, unit);
        if (tree->infoCount == 2)
            displayTranslacions(tree->info2.englishWord, unit, tree->info2.portugueseWord);

        displayWordsForUnit(tree->right, unit);
    }
}
/**
 * @brief Exibe as traduções associadas a uma palavra em português específica.
 *
 * @param root Ponteiro para a raiz da árvore 2-3.
 * @param portugueseWord Ponteiro para a palavra em português a ser buscada.
 */
void displayTranslacions(EnglishBinTree *node, char *unit, const char *portugueseWord)
{
    if (node)
    {
        if (sourceLinkedList(node->units, unit))
        {
            printf("PT: %s\n", portugueseWord);
            printf("ENG: %s\n", node->englishWord);
        }
        displayTranslacions(node->left, unit, portugueseWord);
        displayTranslacions(node->right, unit, portugueseWord);
    }
}

/**
 * @brief Procura uma palavra em português na árvore 2-3 e retorna o nó onde ela está localizada.
 *
 * @param no Ponteiro para o nó raiz da árvore.
 * @param portugueseWord Palavra em português a ser buscada.
 * @return Portuguese23Tree* Ponteiro para o nó contendo a palavra buscada, ou NULL se não for encontrada.
 */
Portuguese23Tree *sourceWords(Portuguese23Tree **no, const char *portugueseWord)
{
    Portuguese23Tree *insered = NULL;

    if (no != NULL && *no != NULL) 
    {
        if (strcmp(portugueseWord, (*no)->info1.portugueseWord) == 0)
            insered = (*no);
        else if ((*no)->infoCount == 2 && strcmp(portugueseWord, (*no)->info2.portugueseWord) == 0)
            insered = (*no);
        else
        {
            if (strcmp(portugueseWord, (*no)->info1.portugueseWord) < 0)
                insered = sourceWords(&(*no)->left, portugueseWord);
            else if ((*no)->infoCount == 1 || strcmp(portugueseWord, (*no)->info2.portugueseWord) < 0)
                insered = sourceWords(&(*no)->middle, portugueseWord);
            else
                insered = sourceWords(&(*no)->right, portugueseWord);
        }
    }
    else
        printf("Palavra não encontrada\n");
    
    return (insered);
}


/**
 * @brief Exibe as traduções associadas a uma palavra em português específica.
 *
 * @param root Ponteiro para a raiz da árvore 2-3.
 * @param portugueseWord Palavra em português cujas traduções serão exibidas.
 */
void displayTranslationPortuguese(Portuguese23Tree **root, const char *portugueseWord)
{
    Portuguese23Tree *resultado = NULL;
    if (root != NULL)
    {
        resultado = sourceWords(root, portugueseWord);
        if (resultado)
        {
            printf("Traducoes da palavra: '%s':\n", portugueseWord);

            if (strcmp(portugueseWord, resultado->info1.portugueseWord) == 0)
                printBinaryTree(resultado->info1.englishWord);
            else
                printBinaryTree(resultado->info2.englishWord);
        }
        else
            printf("A palavra '%s' nao foi encontrada na árvore.\n", portugueseWord);
    }
}

void displayTranslationPortuguesePath(Portuguese23Tree *root, int nivel)
{
    if (root != NULL)
    {
        for (int i = 0; i < nivel; i++)
            printf("    ");

        if (root->infoCount == 1)
            printf("Palavra: %s\n", root->info1.portugueseWord);
        else if (root->infoCount == 2)
            printf("Palavra: %s | %s\n", root->info1.portugueseWord, root->info2.portugueseWord);

        for (int i = 0; i < nivel; i++)
            printf("    ");
        printf("Esquerda ->\n");
        if (root->left != NULL)
            displayTranslationPortuguesePath(root->left, nivel + 1);
        else
        {
            for (int i = 0; i <= nivel; i++)
                printf("    ");
            printf("NULL\n");
        }

        if (root->middle != NULL)
        {
            for (int i = 0; i < nivel; i++)
                printf("    ");
            printf("Centro ->\n");
            displayTranslationPortuguesePath(root->middle, nivel + 1);
        }

        if (root->infoCount == 2)
        {
            for (int i = 0; i < nivel; i++)
                printf("    ");
            printf("Direita ->\n");
            if (root->right != NULL)
                displayTranslationPortuguesePath(root->right, nivel + 1);
            else
            {
                for (int i = 0; i <= nivel; i++)
                    printf("    ");
                printf("NULL\n");
            }
        }
    }
}

/**
 * @brief Libera a memória alocada para um nó da árvore 2-3.
 *
 * @param no Ponteiro para o nó a ser desalocado.
 */
void deallocateNode(Portuguese23Tree **no)
{
    free(*no);
    *no = NULL;
}

static TranslationInfo biggerInfoNode23(Portuguese23Tree *root)
{
    return (root->infoCount == 2 ? root->info2 : root->info1);
}

static int isInfo1(Portuguese23Tree no, char *info)
{
    return (strcmp(info, no.info1.portugueseWord) == 0);
}

static int isInfo2(Portuguese23Tree no, char *info)
{
    return (no.infoCount == 2 && strcmp(info, no.info2.portugueseWord) == 0);
}

/**
 * @brief Localiza o maior filho de um nó e armazena o valor promovido.
 *
 * @param root Ponteiro para o nó raiz.
 * @param father Ponteiro para armazenar o pai do maior filho.
 * @param bigger_valor Ponteiro para armazenar a maior chave encontrada.
 * @return Portuguese23Tree* Ponteiro para o maior filho encontrado.
 */
static Portuguese23Tree *sourceBiggerChild(Portuguese23Tree *root, Portuguese23Tree **father, TranslationInfo *bigger_valor)
{
    Portuguese23Tree *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        if (child->infoCount == 1)
            child = child->middle;
        else
            child = child->right;
    }

    if (child != NULL)
        *bigger_valor = biggerInfoNode23(child);

    return (child);
}

/**
 * @brief Localiza o menor filho de um nó.
 *
 * @param root Ponteiro para o nó raiz.
 * @param father Ponteiro para armazenar o pai do menor filho.
 * @return Portuguese23Tree* Ponteiro para o menor filho encontrado.
 */
static Portuguese23Tree *sourceMinorChild(Portuguese23Tree *root, Portuguese23Tree **father)
{
    Portuguese23Tree *child;
    child = root;

    while (!isLeaf(child))
    {
        *father = child;
        child = child->left;
    }

    return (child);
}

/**
 * @brief Executa o movimento de onda para reorganizar os nós ao remover uma chave.
 *
 * @param leaving Chave a ser removida.
 * @param input Ponteiro para armazenar a nova chave promovida.
 * @param father Ponteiro para o pai do nó atual.
 * @param origin Ponteiro para o nó de origem.
 * @param root Ponteiro para a raiz da árvore.
 * @param bigger Ponteiro para o maior nó afetado.
 * @param functionRemove Função auxiliar de remoção.
 * @return int Retorna 1 se a remoção foi bem-sucedida, 0 caso contrário.
 */
static int waveMoviment(TranslationInfo leaving, TranslationInfo *input, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **root, Portuguese23Tree **bigger, int (*functionRemove)(Portuguese23Tree **, char *, Portuguese23Tree *, Portuguese23Tree **, Portuguese23Tree **))
{
    int removed = functionRemove(root, leaving.portugueseWord, father, origin, bigger);
    *input = leaving;

    return (removed);
}

/**
 * @brief Localiza o pai de um nó que contém uma chave específica.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info Palavra a ser buscada.
 * @return Portuguese23Tree* Ponteiro para o nó pai, ou NULL se não for encontrado.
 */
static Portuguese23Tree *sourceFather(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourceFather(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = sourceFather(root->middle, info);
            else
                father = sourceFather(root->right, info);

            if (father == NULL)
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Localiza o pai do maior nó com base em uma palavra específica.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info Palavra a ser buscada.
 * @return Portuguese23Tree* Ponteiro para o nó pai, ou NULL se não for encontrado.
 */
static Portuguese23Tree *sourceBiggerFather(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourceBiggerFather(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = sourceBiggerFather(root->middle, info);
            else
                father = sourceBiggerFather(root->right, info);

            if (father == NULL &&
                ((root->infoCount == 1 && strcmp(root->info1.portugueseWord, info) > 0) ||
                 (root->infoCount == 2 && strcmp(root->info2.portugueseWord, info) > 0)))
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Localiza o pai do menor nó contendo duas informações, com base em uma palavra específica.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info Palavra a ser buscada.
 * @return Portuguese23Tree* Ponteiro para o nó pai, ou NULL se não for encontrado.
 */
static Portuguese23Tree *sourceMinorFather2Infos(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourceMinorFather2Infos(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = sourceMinorFather2Infos(root->middle, info);
            else
                father = sourceMinorFather2Infos(root->right, info);

            if (father == NULL && root->infoCount == 2 && strcmp(root->info2.portugueseWord, info) < 0)
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Localiza o menor pai de um nó com base em uma palavra específica.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param info Palavra a ser buscada.
 * @return Portuguese23Tree* Ponteiro para o nó pai, ou NULL se não for encontrado.
 */
static Portuguese23Tree *sourcerMinorFather(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!isInfo1(*root, info) && !isInfo2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = sourcerMinorFather(root->left, info);
            else if (root->infoCount == 1 || strcmp(info, root->info2.portugueseWord) < 0)
                father = sourcerMinorFather(root->middle, info);
            else
                father = sourcerMinorFather(root->right, info);

            if (father == NULL && strcmp(root->info1.portugueseWord, info) < 0)
                father = root;
        }
    }

    return (father);
}

/**
 * @brief Remove uma chave de um nó folha e reorganiza a árvore.
 *
 * @param origin Ponteiro para o nó de origem.
 * @param root Ponteiro para o nó raiz.
 * @param info Ponteiro para armazenar a chave removida.
 * @param child1 Ponteiro para o primeiro filho do nó afetado.
 * @param child2 Ponteiro para o segundo filho do nó afetado.
 * @param bigger Ponteiro para o maior nó afetado.
 * @return int Retorna 1 se a remoção foi bem-sucedida, 0 caso contrário.
 */
int removeNoLeaf1(Portuguese23Tree **origin, Portuguese23Tree *root, TranslationInfo *info, Portuguese23Tree *child1, Portuguese23Tree *child2, Portuguese23Tree **bigger)
{
    int removed;
    Portuguese23Tree *child, *father;
    TranslationInfo info_child;

    father = root;

    child = sourceBiggerChild(child1, &father, &info_child);

    if (child->infoCount == 2)
    {
        *info = info_child;
        child->infoCount = 1;
        removed = 1;
    }
    else
    {
        child = sourceMinorChild(child2, &father);
        removed = waveMoviment(child->info1, info, father, origin, &child, bigger, tree23Remove1);
    }

    return (removed);
}

/**
 * @brief Remove um nó folha da árvore 2-3 (caso 2).
 *
 * @param origin Endereço da raiz da árvore.
 * @param root Nó atual da árvore.
 * @param info Informação removida do nó.
 * @param child1 Filho à esquerda ou ao meio.
 * @param child2 Filho à direita ou ao meio.
 * @param bigger Ponteiro para armazenar o maior nó afetado.
 *
 * @return Retorna 1 se a remoção foi bem-sucedida, ou -1 se uma operação especial foi requerida.
 */
int removeNoLeaf2(Portuguese23Tree **origin, Portuguese23Tree *root, TranslationInfo *info, Portuguese23Tree *child1, Portuguese23Tree *child2, Portuguese23Tree **bigger)
{
    int removed;
    Portuguese23Tree *child, *father;
    TranslationInfo info_child;

    father = root;

    child = sourceMinorChild(child1, &father);

    if (child->infoCount == 2)
    {
        *info = child->info1;
        child->info1 = child->info2;
        child->infoCount = 1;
        removed = 1;
    }
    else
    {
        child = sourceBiggerChild(child2, &father, &info_child);
        removed = waveMoviment(info_child, info, father, origin, &child, bigger, tree23Remove2);
    }

    return (removed);
}

/**
 * @brief Remove uma informação de um nó da árvore 2-3 (caso geral 1).
 *
 * @param root Endereço do nó raiz da árvore.
 * @param info Palavra em português a ser removida.
 * @param father Nó pai do nó atual.
 * @param origin Endereço da raiz original da árvore.
 * @param bigger Ponteiro para armazenar o maior nó afetado.
 *
 * @return Retorna 1 se a remoção foi bem-sucedida, ou -1 se uma operação especial foi requerida.
 */
int tree23Remove1(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **bigger)
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
                if ((*root)->infoCount == 2)
                {
                    if (info1)
                        (*root)->info1 = (*root)->info2;
                    (*root)->infoCount = 1;
                }
                else
                {
                    if (father == NULL)
                        deallocateNode(root);
                    else
                    {
                        Portuguese23Tree *father_aux;
                        TranslationInfo info_father;
                        if (*root == father->left || (father->infoCount == 2 && *root == father->middle))
                        {
                            father_aux = sourceFather(*origin, father->info1.portugueseWord);

                            if (*root == father->left)
                                info_father = father->info1;
                            else
                                info_father = father->info2;

                            removed = waveMoviment(info_father, &((*root)->info1), father_aux, origin, &father, bigger, tree23Remove1);
                        }
                        else
                        {
                            father_aux = sourceBiggerFather(*origin, (*root)->info1.portugueseWord);

                            Portuguese23Tree *minorFather;
                            minorFather = sourceMinorFather2Infos(*origin, (*root)->info1.portugueseWord);

                            if (father_aux == NULL || (father_aux != father && minorFather != NULL))
                            {
                                *bigger = father;
                                (*root)->infoCount = 0;
                                removed = -1;
                            }
                            else
                            {
                                if (strcmp(father_aux->info1.portugueseWord, (*root)->info1.portugueseWord) > 0)
                                    info_father = father_aux->info1;
                                else
                                    info_father = father_aux->info2;

                                Portuguese23Tree *grandparent;
                                grandparent = sourceFather(*origin, info_father.portugueseWord);
                                removed = waveMoviment(info_father, &((*root)->info1), grandparent, origin, &father_aux, bigger, tree23Remove1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removed = removeNoLeaf1(origin, *root, &((*root)->info2), (*root)->middle, (*root)->right, bigger);
            else if (info1)
                removed = removeNoLeaf1(origin, *root, &((*root)->info1), (*root)->left, (*root)->middle, bigger);
        }
        else
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                removed = tree23Remove1(&(*root)->left, info, *root, origin, bigger);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
                removed = tree23Remove1(&(*root)->middle, info, *root, origin, bigger);
            else
                removed = tree23Remove1(&(*root)->right, info, *root, origin, bigger);
        }
    }
    return (removed);
}

/**
 * @brief Remove uma informação de um nó da árvore 2-3 (caso geral 2).
 *
 * @param root Endereço do nó raiz da árvore.
 * @param info Palavra em português a ser removida.
 * @param father Nó pai do nó atual.
 * @param origin Endereço da raiz original da árvore.
 * @param bigger Ponteiro para armazenar o maior nó afetado.
 *
 * @return Retorna 1 se a remoção foi bem-sucedida, ou -1 se uma operação especial foi requerida.
 */
int tree23Remove2(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origin, Portuguese23Tree **bigger)
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
                if ((*root)->infoCount == 2)
                {
                    if (info1)
                        (*root)->info1 = (*root)->info2;

                    (*root)->infoCount = 1;
                }
                else
                {
                    if (father == NULL)
                        deallocateNode(root);
                    else
                    {
                        Portuguese23Tree *father_aux;
                        TranslationInfo info_father;
                        if (*root == father->middle || (father->infoCount == 2 && *root == father->right))
                        {
                            father_aux = sourceFather(*origin, father->info1.portugueseWord);

                            if (*root == father->middle)
                                info_father = father->info1;
                            else
                                info_father = father->info2;

                            removed = waveMoviment(info_father, &((*root)->info1), father_aux, origin, &father, bigger, tree23Remove2);
                        }
                        else
                        {
                            father_aux = sourcerMinorFather(*origin, (*root)->info1.portugueseWord);

                            Portuguese23Tree *minorFather;
                            minorFather = sourceMinorFather2Infos(*origin, (*root)->info1.portugueseWord);

                            Portuguese23Tree *grandparent;
                            if (father_aux == NULL || (father_aux != father && minorFather != NULL))
                            {
                                removed = -1;
                                *bigger = father;
                            }
                            else
                            {
                                if (father_aux->infoCount == 2 && father_aux->info2.portugueseWord < (*root)->info1.portugueseWord)
                                    info_father = father_aux->info2;
                                else
                                    info_father = father_aux->info1;

                                grandparent = sourceFather(*origin, info_father.portugueseWord);
                                removed = waveMoviment(info_father, &((*root)->info1), grandparent, origin, &father_aux, bigger, tree23Remove2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removed = removeNoLeaf2(origin, *root, &((*root)->info2), (*root)->right, (*root)->middle, bigger);
            else if (info1)
                removed = removeNoLeaf2(origin, *root, &((*root)->info1), (*root)->middle, (*root)->left, bigger);
        }
        else
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                removed = tree23Remove2(&(*root)->left, info, *root, origin, bigger);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
                removed = tree23Remove2(&(*root)->middle, info, *root, origin, bigger);
            else
                removed = tree23Remove2(&(*root)->right, info, *root, origin, bigger);
        }
    }
    return (removed);
}

/**
 * @brief Verifica se é possível realizar uma remoção em uma subárvore.
 *
 * @param root Ponteiro para a raiz da subárvore.
 *
 * @return Retorna 1 se for possível realizar uma remoção, ou 0 caso contrário.
 */
int possibleRemove(Portuguese23Tree *root)
{
    int possible = 0;

    if (root != NULL)
    {
        possible = root->infoCount == 2;

        if (!possible)
        {
            possible = possibleRemove(root->middle);

            if (!possible)
                possible = possibleRemove(root->left);
        }
    }

    return (possible);
}

/**
 * @brief Junta dois nós da árvore 2-3 e reduz a contagem de chaves do nó raiz.
 *
 * @param child1 Primeiro nó filho.
 * @param info Informação de tradução a ser adicionada.
 * @param bigger Maior nó afetado pela operação.
 * @param root Endereço do nó raiz da árvore.
 *
 * @return Retorna o ponteiro para o nó combinado.
 */
Portuguese23Tree *togetherNode(Portuguese23Tree *child1, TranslationInfo info, Portuguese23Tree *bigger, Portuguese23Tree **root)
{
    addKey(&child1, &info, bigger);

    (*root)->infoCount--;

    if ((*root)->infoCount == 0)
        deallocateNode(root);

    return (child1);
}

/**
 * @brief Equilibra a árvore 2-3 após uma remoção.
 *
 * @param root Endereço do nó raiz da árvore.
 * @param child1 Primeiro nó filho.
 * @param child2 Segundo nó filho.
 * @param info Informação de tradução a ser utilizada no equilíbrio.
 * @param bigger Maior nó afetado pela operação.
 *
 * @return Retorna 1 se o equilíbrio foi realizado, ou 0 se não foi necessário.
 */
static int balance(Portuguese23Tree **root, Portuguese23Tree *child1, Portuguese23Tree **child2, TranslationInfo info, Portuguese23Tree **bigger)
{
    int balanced = 0;
    if (*child2 == NULL || (*child2)->infoCount == 0)
    {
        if (*child2 != NULL)
            deallocateNode(child2);

        *bigger = togetherNode(child1, info, *bigger, root);
        balanced = 1;
    }
    return (balanced);
}

/**
 * @brief Reequilibra a árvore 2-3 após a remoção de uma chave.
 *
 * @param root Endereço do nó raiz da árvore.
 * @param info Palavra em português a ser reequilibrada.
 * @param bigger Maior nó afetado pela operação.
 *
 * @return Retorna 1 se o reequilíbrio foi bem-sucedido, ou -1 em caso de falha.
 */
int rebalance(Portuguese23Tree **root, char *info, Portuguese23Tree **bigger)
{
    int balanced = 0;
    if (*root != NULL)
    {
        if (!isLeaf(*root))
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                balanced = rebalance(&((*root)->left), info, bigger);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
            {
                if ((*root)->left->infoCount == 2 && !possibleRemove((*root)->middle))
                    balanced = -1;
                else
                    balanced = rebalance(&((*root)->middle), info, bigger);
            }
            else
            {
                if ((*root)->middle->infoCount == 2 && !possibleRemove((*root)->right))
                    balanced = -1;
                else
                    balanced = rebalance(&((*root)->right), info, bigger);
            }

            if (balanced != -1)
            {
                if ((*root)->infoCount == 1)
                    balanced = balance(root, (*root)->left, &((*root)->middle), (*root)->info1, bigger);
                else if ((*root)->infoCount == 2)
                    balanced = balance(root, (*root)->middle, &((*root)->right), (*root)->info2, bigger);
                balanced = 1;
            }
        }
    }

    return (balanced);
}

/**
 * @brief Remove uma chave da árvore 2-3.
 *
 * @param root Endereço do nó raiz da árvore.
 * @param info Palavra em português a ser removida.
 *
 * @return Retorna 1 se a remoção foi bem-sucedida, ou -1 em caso de falha.
 */
int removeTree23(Portuguese23Tree **root, char *info)
{
    Portuguese23Tree *bigger, *posicao_juncao;
    int removed = tree23Remove1(root, info, NULL, root, &posicao_juncao);

    if (removed == -1)
    {
        TranslationInfo valor_juncao = biggerInfoNode23(posicao_juncao);
        bigger = NULL;
        removed = rebalance(root, valor_juncao.portugueseWord, &bigger);

        if (removed == -1)
        {
            Portuguese23Tree *father, *posicao_juncao2;
            TranslationInfo *input;
            father = sourceFather(*root, valor_juncao.portugueseWord);

            if (isInfo1(*posicao_juncao, valor_juncao.portugueseWord))
                input = &(posicao_juncao->middle->info1);
            else
                input = &(posicao_juncao->right->info1);

            removed = waveMoviment(valor_juncao, input, father, root, &posicao_juncao, &posicao_juncao2, tree23Remove2);

            if (removed == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                father = sourceFather(*root, valor_juncao.portugueseWord);
                removed = waveMoviment(valor_juncao, &(posicao_juncao2->left->info1), father, root, &posicao_juncao2, &posicao_juncao, tree23Remove1);

                valor_juncao = biggerInfoNode23(posicao_juncao);
                bigger = NULL;
                removed = rebalance(root, valor_juncao.portugueseWord, &bigger);
            }
        }

        if (*root == NULL)
            *root = bigger;
    }

    return (removed);
}

/**
 * @brief Libera os recursos alocados para uma estrutura de informações de tradução.
 *
 * @param info Estrutura de informações de tradução a ser liberada.
 */
void freeInfo2_3(TranslationInfo *info)
{
    freeBinaryTree(info->englishWord);
    free(info->portugueseWord);
}

/**
 * @brief Libera todos os nós da árvore 2-3.
 *
 * @param no Ponteiro para o nó da árvore.
 */
void freeTree(Portuguese23Tree *no)
{
    if (no != NULL)
    {
        freeTree(no->left);
        freeInfo2_3(&no->info1);
        freeTree(no->middle);
        if (no->infoCount == 2)
        {
            freeInfo2_3(&no->info2);
            freeTree(no->right);
        }
        free(no);
    }
}

// #######################################################################################
// BINÁRIA
// #######################################################################################

/**
 * @brief Cria um nó na árvore binária de busca.
 *
 * @param englishWord Palavra em inglês para o nó.
 * @param unit Unidade associada à palavra.
 *
 * @return Retorna o ponteiro para o nó criado, ou NULL em caso de falha.
 */
EnglishBinTree *createNodeBin(const char *englishWord, char *unit)
{
    EnglishBinTree *newNode = (EnglishBinTree *)malloc(sizeof(EnglishBinTree));
    if (newNode != NULL)
    {
        newNode->englishWord = (char *)malloc(strlen(englishWord) + 1);
        strcpy(newNode->englishWord, englishWord);
        newNode->units = NULL;
        InsertLinkedList(&(newNode->units), unit);
        newNode->left = newNode->right = NULL;
    }
    return (newNode);
}

/**
 * @brief Insere uma palavra em inglês na árvore binária de busca.
 *
 * @param root Endereço do nó raiz da árvore.
 * @param info Estrutura de informações de tradução.
 *
 * @return Retorna 1 se a inserção foi bem-sucedida, ou 0 caso contrário.
 */
int insertenglishWord(EnglishBinTree **root, TranslationInfo *info)
{
    int result = 0;
    if ((*root) == NULL)
    {
        EnglishBinTree *new = createNodeBin(info->englishWord->englishWord, info->englishWord->units->unitName);
        *root = new;
        result = 1;
    }
    else if (strcmp(info->englishWord->englishWord, (*root)->englishWord) < 0)
        result = insertenglishWord(&(*root)->left, info);
    else if (strcmp(info->englishWord->englishWord, (*root)->englishWord) > 0)
        result = insertenglishWord(&(*root)->right, info);
    else
        result = InsertLinkedList(&((*root)->units), info->englishWord->units->unitName);

    return (result);
}

/**
 * @brief Imprime todas as palavras e unidades associadas na árvore binária de busca.
 *
 * @param root Ponteiro para o nó raiz da árvore.
 */
void printBinaryTree(EnglishBinTree *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->left);
        printf("ENG: %s \n", root->englishWord);
        showLinkedList(root->units);
        printBinaryTree(root->right);
    }
}

/**
 * @brief Verifica se um nó da árvore binária de busca é folha.
 *
 * @param root Ponteiro para o nó.
 *
 * @return Retorna 1 se o nó é folha, ou 0 caso contrário.
 */
int isLeafBin(EnglishBinTree *root)
{
    return (root->left == NULL && root->right == NULL);
}

/**
 * @brief Obtém o único filho de um nó na árvore binária de busca.
 *
 * @param root Endereço do nó.
 *
 * @return Retorna o ponteiro para o filho, ou NULL se o nó não tiver exatamente um filho.
 */
EnglishBinTree *oneChild(EnglishBinTree **root)
{
    EnglishBinTree *aux;
    aux = NULL;

    if ((*root)->right == NULL)
        aux = (*root)->left;
    else if ((*root)->left == NULL)
        aux = (*root)->right;

    return (aux);
}

/**
 * @brief Encontra o menor filho em uma subárvore binária.
 *
 * @param root Ponteiro para o nó raiz da subárvore.
 *
 * @return Retorna o ponteiro para o menor filho.
 */
EnglishBinTree *minorChild(EnglishBinTree *root)
{
    EnglishBinTree *aux;
    aux = root;

    if (root)
    {
        if (root->left)
            aux = minorChild(root->left);
    }

    return (aux);
}

/**
 * @brief Remove uma palavra em inglês da árvore binária de busca.
 *
 * @param root Endereço do nó raiz da árvore.
 * @param word Palavra a ser removida.
 *
 * @return Retorna 1 se a palavra foi encontrada e removida, ou 0 caso contrário.
 */
int removerenglishWord(EnglishBinTree **root, const char *word)
{
    EnglishBinTree *endchild = NULL;
    int exist = 0;

    if (*root)
    {
        if (strcmp(word, (*root)->englishWord) == 0)
        {
            EnglishBinTree *aux = *root;
            exist = 1;
            if (isLeafBin(*root))
            {
                free(aux);
                *root = NULL;
            }
            else if ((endchild = oneChild(root)) != NULL)
            {
                free(aux);
                *root = endchild;
            }
            else
            {
                endchild = minorChild((*root)->right);
                strcpy((*root)->englishWord, endchild->englishWord);
                (*root)->units = endchild->units;

                removerenglishWord(&(*root)->right, endchild->englishWord);
            }
        }
        else if (strcmp(word, (*root)->englishWord) < 0)
            exist = removerenglishWord(&(*root)->left, word);
        else
            exist = removerenglishWord(&(*root)->right, word);
    }

    return (exist);
}

/**
 * @brief Libera todos os nós de uma árvore binária de busca.
 *
 * @param root Ponteiro para o nó raiz da árvore.
 */
void freeBinaryTree(EnglishBinTree *root)
{
    if (root)
    {
        freeBinaryTree(root->left);
        freeBinaryTree(root->right);
        free(root->englishWord);
        free(root);
    }
}

// #######################################################################################
// list encadeada
// #######################################################################################

/**
 * @brief Aloca memória para uma lista encadeada de unidades.
 *
 * @param list Endereço do ponteiro para a lista.
 */
void alocateLinkedListUnit(LinkedUnitList **list)
{
    *list = (LinkedUnitList *)malloc(sizeof(LinkedUnitList));
    (*list)->unitName = NULL;
    (*list)->next = NULL;
}

/**
 * @brief Define o nome de uma unidade em uma lista encadeada.
 *
 * @param list Ponteiro para a lista encadeada onde o nome será definido.
 * @param unitName Nome da unidade a ser atribuída.
 */
void defineLinkedListUnit(LinkedUnitList *list, const char *unitName)
{
    list->unitName = (char *)malloc((strlen(unitName) + 1) * sizeof(char));
    strcpy(list->unitName, unitName);
}

/**
 * @brief Verifica se uma unidade existe na lista encadeada.
 *
 * @param list Ponteiro constante para a lista encadeada.
 * @param unitName Nome da unidade a ser procurada.
 * @return int 1 se a unidade existe, 0 caso contrário.
 */
void showLinkedList(const LinkedUnitList *list)
{
    if (list)
    {
        if (list->next)
            printf("Unidade %s, ", list->unitName);
        else
            printf("Unidade %s", list->unitName);
        showLinkedList(list->next);
    }
    else
        printf("\n");
}

/**
 * @brief Libera a memória ocupada por uma lista encadeada.
 *
 * @param list Ponteiro para a lista encadeada a ser liberada.
 */
int sourceLinkedList(const LinkedUnitList *list, const char *unitName)
{
    int confirmation = 0;
    if (list)
    {
        if (strcmp(list->unitName, unitName) == 0)
            confirmation = 1;
        else
            confirmation = sourceLinkedList(list->next, unitName);
    }
    return (confirmation);
}

/**
 * @brief Insere um novo elemento na lista encadeada.
 *
 * @param list Ponteiro para o ponteiro da lista encadeada.
 * @param name Nome do elemento a ser inserido.
 * @return int 1 se a inserção for bem-sucedida, 0 caso o elemento já exista.
 */
void freeLinkedList(LinkedUnitList *list)
{
    if (list)
    {
        freeLinkedList(list->next);
        free(list->unitName);
        free(list);
        list = NULL;
    }
}

/**
 * @brief Remove um elemento da lista encadeada.
 *
 * @param list Ponteiro para o ponteiro da lista encadeada.
 * @param name Nome do elemento a ser removido.
 * @return int 1 se a remoção for bem-sucedida, 0 caso o elemento não exista.
 */
int InsertLinkedList(LinkedUnitList **list, char *name)
{
    int confirmation = 1;
    if (!sourceLinkedList(*list, name))
    {
        LinkedUnitList *new;
        alocateLinkedListUnit(&new);
        defineLinkedListUnit(new, name);
        new->next = *list;
        *list = new;
    }
    else
        confirmation = 0;

    return (confirmation);
}

/**
 * @brief Remove uma unidade associada a uma palavra em inglês.
 *
 * @param root Ponteiro para o nó raiz da árvore 2-3 de palavras em português.
 * @param palavras_ingles Palavra em inglês a ser removida.
 * @param unit Nome da unidade a ser removida.
 * @return int 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int removeLinkedList(LinkedUnitList **list, const char *name)
{
    int confirmation = 0;
    if (*list)
    {
        if (strcmp((*list)->unitName, name) == 0)
        {
            LinkedUnitList *aux = *list;
            *list = (*list)->next;
            free(aux->unitName);
            free(aux);
            confirmation = 1;
        }
        else
            confirmation = removeLinkedList(&(*list)->next, name);
    }
    return (confirmation);
}

// #######################################################################################
// REMOÇÃO
// #######################################################################################

/**
 * @brief Remove uma unidade de uma palavra em inglês da árvore binária.
 *
 * @param root Ponteiro para o nó raiz da árvore binária de palavras em inglês.
 * @param englishWord Palavra em inglês associada à unidade.
 * @param unit Nome da unidade a ser removida.
 * @return int 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int removeEnglishFindUnit(Portuguese23Tree **root, const char *palavras_ingles, const char *unit)
{
    int confirm = 1;
    confirm = auxRemoveEnglishFindUnit(*root, palavras_ingles, unit, root);

    return (confirm);
}

int removeUnit(EnglishBinTree **root, const char *englishWord, const char *unit)
{
    int confirm = 1;

    if (*root)
    {
        if (strcmp((*root)->englishWord, englishWord) == 0)
        {
            confirm = removeLinkedList(&(*root)->units, unit);
            if (!(*root)->units)
                confirm = removerenglishWord(root, englishWord);
        }
        else if (strcmp((*root)->englishWord, englishWord) > 0)
            confirm = removeUnit(&(*root)->left, englishWord, unit);
        else
            confirm = removeUnit(&(*root)->right, englishWord, unit);
    }

    return (confirm);
}

/**
 * @brief Função auxiliar para remover uma unidade associada a uma palavra em inglês.
 *
 * @param root Ponteiro para o nó raiz da árvore 2-3.
 * @param englishWord Palavra em inglês a ser removida.
 * @param unit Nome da unidade a ser removida.
 * @param top Ponteiro para o nó raiz da árvore.
 * @return int 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int auxRemoveEnglishFindUnit(Portuguese23Tree *root, const char *englishWord, const char *unit, Portuguese23Tree **top)
{
    int confirm = 0;

    if (root)
    {
        confirm = auxRemoveEnglishFindUnit((root)->left, englishWord, unit, top);
        confirm = auxRemoveEnglishFindUnit((root)->middle, englishWord, unit, top) || confirm;

        if ((root)->infoCount == 2)
        {
            confirm = auxRemoveEnglishFindUnit((root)->right, englishWord, unit, top) || confirm;
            confirm = removeUnit(&(root)->info2.englishWord, englishWord, unit);
        }

        confirm = removeUnit(&(root)->info1.englishWord, englishWord, unit);

        if (!(root)->info1.englishWord)
            confirm = removeTree23(top, root->info1.portugueseWord);

        if ((root)->infoCount == 2 && !(root)->info2.englishWord)
            confirm = removeTree23(top, root->info2.portugueseWord);
    }

    return (confirm);
}

/**
 * @brief Remove unidades associadas a palavras em inglês em uma árvore binária.
 *
 * @param root Ponteiro para o nó raiz da árvore binária.
 * @param portugueseWord Palavra em português associada à unidade.
 * @param unit Nome da unidade a ser removida.
 * @return int 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int removednitFromEnglishWord(EnglishBinTree **root, const char *portugueseWord, const char *unit)
{
    int confirm = 0;
    if (*root)
    {
        confirm = removednitFromEnglishWord(&(*root)->left, portugueseWord, unit);
        confirm = removednitFromEnglishWord(&(*root)->right, portugueseWord, unit) || confirm;
        confirm = removeLinkedList(&(*root)->units, unit);

        if (!(*root)->units)
            confirm = removerenglishWord(root, (*root)->englishWord);
    }

    return confirm;
}

/**
 * @brief Função auxiliar para remover palavras em português associadas a uma unidade.
 *
 * @param root Ponteiro para o nó raiz da árvore 2-3.
 * @param portugueseWord Palavra em português a ser removida.
 * @param unit Nome da unidade a ser removida.
 * @param top Ponteiro para o nó raiz da árvore.
 * @return int 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int auxRemovePortugueseWordFindUnit(Portuguese23Tree *root, char *portugueseWord, const char *unit, Portuguese23Tree **top)
{
    int confirm = 1;

    if (root)
    {
        if (strcmp((root)->info1.portugueseWord, portugueseWord) == 0)
        {
            confirm = removednitFromEnglishWord(&(root)->info1.englishWord, portugueseWord, unit);
            if (!(root)->info1.englishWord)
                confirm = removeTree23(top, portugueseWord);
        }
        else if ((root)->infoCount == 2 && strcmp((root)->info2.portugueseWord, portugueseWord) == 0)
        {
            confirm = removednitFromEnglishWord(&(root)->info2.englishWord, portugueseWord, unit);
            if (!(root)->info2.englishWord)
                confirm = removeTree23(top, portugueseWord);
        }
        else if (strcmp(portugueseWord, (root)->info1.portugueseWord) < 0)
            confirm = auxRemovePortugueseWordFindUnit((root)->left, portugueseWord, unit, top);
        else if ((root)->infoCount == 1 || strcmp(portugueseWord, (root)->info2.portugueseWord) < 0)
            confirm = auxRemovePortugueseWordFindUnit((root)->middle, portugueseWord, unit, top);
        else
            confirm = auxRemovePortugueseWordFindUnit((root)->right, portugueseWord, unit, top);
    }

    return (confirm);
}

/**
 * @brief Remove uma palavra em português associada a uma unidade da árvore 2-3.
 *
 * @param root Ponteiro para o nó raiz da árvore 2-3.
 * @param portugueseWord Palavra em português a ser removida.
 * @param unit Nome da unidade a ser removida.
 * @return int 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int removePortugueseWordFindUnit(Portuguese23Tree **root, char *portugueseWord, const char *unit)
{
    int confirm = 1;
    confirm = auxRemovePortugueseWordFindUnit(*root, portugueseWord, unit, root);

    return (confirm);
}