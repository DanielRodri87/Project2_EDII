#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include <time.h>

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - informar uma unidade e então imprima todas as palavras da unidade em português seguida das
// equivalentes em inglês
// ---------------------------------------------------- XXXXXX -------------------------------------------------
/**
 * @brief Exibe as palavras em inglês contidas na árvore binária em ordem.
 *
 * @param root Ponteiro para a raiz da árvore binária de palavras em inglês.
 */
void DisplayEngWords(EngPT *root)
{
    if (root != NULL)
    {
        DisplayEngWords(root->left);
        printf("%s\n", root->info.englishWord);
        DisplayEngWords(root->right);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - informar uma palavra em português e então imprima todas as palavras em inglês equivalente à palavra em
// português dada, independente da unidade
// ---------------------------------------------------- XXXXXX -------------------------------------------------
/**
 * @brief Imprime todas as palavras em inglês equivalentes à palavra em português dada.
 *
 * @param node Ponteiro para a árvore rubro-negra.
 * @param unit Unidade a ser buscada.
 */
void printWordsByUnit(RBTree *node, int unit)
{
    if (node != NULL)
    {
        Units *current = node->info.binaryTreeEnglish->info.units;
        while (current != NULL)
        {
            if (current->unit == unit)
                printf("%s:%s\n", node->info.binaryTreeEnglish->info.englishWord, node->info.portugueseWord);
            current = current->next;
        }

        printWordsByUnit(node->left, unit);
        printWordsByUnit(node->right, unit);
    }
}

/**
 * @brief Busca e imprime todas as palavras em inglês equivalentes à palavra em português dada.
 *
 * @param node Ponteiro para a árvore rubro-negra.
 * @param portugueseWord Palavra em português a ser buscada.
 */
void findEnglishByPortuguese(RBTree *node, const char *portugueseWord)
{
    if (node != NULL)
    {
        if (strcmp(node->info.portugueseWord, portugueseWord) == 0)
        {
            Units *current = node->info.binaryTreeEnglish->info.units;
            while (current != NULL)
            {
                printWordsByUnit(node, current->unit);
                current = current->next;
            }
        }

        // Continua recursivamente para os filhos
        findEnglishByPortuguese(node->left, portugueseWord);
        findEnglishByPortuguese(node->right, portugueseWord);
    }
}

void findEnglishByPortuguesePath(RBTree *node, const char *portugueseWord, int nivel)
{
    if (node != NULL)
    {
        for (int i = 0; i < nivel; i++)
        {
            printf("    ");
        }

        const char *cor = (node->color == RED) ? "Vermelho" : "Preto";
        printf("Palavra: %s, Cor: %s\n", node->info.portugueseWord, cor);

        if (strcmp(node->info.portugueseWord, portugueseWord) == 0)
        {
            printf("\n=== Palavra encontrada: %s ===\n", portugueseWord);

            Units *current = node->info.binaryTreeEnglish->info.units;
            printf("Unidades associadas:\n");
            while (current != NULL)
                current = current->next;
        }

        for (int i = 0; i < nivel + 1; i++)
        {
            printf("    ");
        }
        printf("Esquerda ->\n");
        findEnglishByPortuguesePath(node->left, portugueseWord, nivel + 1);

        for (int i = 0; i < nivel + 1; i++)
        {
            printf("    ");
        }
        printf("Direita ->\n");
        findEnglishByPortuguesePath(node->right, portugueseWord, nivel + 1);
    }
    else
    {
        for (int i = 0; i < nivel; i++)
        {
            printf("    ");
        }
        printf("NULL\n");
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III - informar uma palavra em inglês e a unidade a qual a mesma pertence removê-la das árvores binárias
// das quais ela pertence. Caso ela seja a única palavra em uma das árvores binárias, remover também da
// árvore 2-3
// ---------------------------------------------------- XXXXXX -------------------------------------------------


/**
 * @brief Verifica se o nó é uma folha.
 * 
 * @param root Ponteiro para o nó da árvore binária de busca em inglês.
 * @return int Retorna 1 se o nó for folha, 0 caso contrário.
 */
int isLeaf(EngPT *root)
{
    return (root->left == NULL && root->right == NULL);
}

/**
 * @brief Verifica se o nó tem apenas um filho e retorna esse filho.
 * 
 * @param root Ponteiro para o nó da árvore binária de busca em inglês.
 * @return EngPT* Ponteiro para o único filho do nó ou NULL se não tiver apenas um filho.
 */
EngPT *oneChild(EngPT *root)
{
    EngPT *aux;
    aux = NULL;
    if (root->left == NULL)
        aux = root->right;
    else if (root->right == NULL)
        aux = root->left;
    return (aux);
}

/**
 * @brief Encontra o menor filho à esquerda na subárvore.
 * 
 * @param root Ponteiro para o nó raiz da subárvore.
 * @return EngPT* Ponteiro para o nó com a menor chave à esquerda.
 */
EngPT *minorLeftChild(EngPT *root)
{
    EngPT *aux;
    aux = NULL;
    if (root)
    {
        aux = minorLeftChild(root->left);
        if (!aux)
            aux = root;
    }
    return (aux);
}

/**
 * @brief Remove recursivamente uma unidade da lista encadeada de unidades.
 * 
 * @param units Ponteiro para a cabeça da lista encadeada.
 * @param unit Unidade a ser removida.
 * @param removed Ponteiro para uma flag que indica se a unidade foi removida (1 para sucesso, 0 para falha).
 * @return Units* Ponteiro para a lista atualizada.
 */
Units *removeRecursive(Units *units, int unit, int *removed)
{
    Units *next = units;
    if (units != NULL)
    {
        if (units->unit == unit)
        {
            next = units->next;
            free(units);
            *removed = 1;
        }
        else
            units->next = removeRecursive(units->next, unit, removed);
    }
    return (next);
}

/**
 * @brief Remove uma palavra em inglês da árvore binária de busca com base em uma unidade específica.
 * 
 * @param root Ponteiro para o nó raiz da árvore binária de busca em inglês.
 * @param englishWord Palavra em inglês a ser removida.
 * @param unit Unidade associada à palavra a ser removida.
 * @return int Retorna 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int removeWordEnglish(EngPT **root, const char *englishWord, int unit)
{
    int find = 0;
    if ((*root) != NULL)
    {
        if (strcmp((*root)->info.englishWord, englishWord) == 0)
        {
            int removedUnit = 0;
            (*root)->info.units = removeRecursive((*root)->info.units, unit, &removedUnit);
            if (removedUnit)
            {
                find = 1;
                if ((*root)->info.units == NULL)
                {
                    EngPT *aux = *root;
                    if (isLeaf(*root))
                    {
                        free(aux);
                        *root = NULL;
                    }
                    else if (oneChild(*root))
                    {
                        EngPT *aux = *root;
                        *root = oneChild(*root);
                        free(aux);
                    }
                    else
                    {
                        EngPT *aux = minorLeftChild((*root)->right);
                        (*root)->info = aux->info;
                        removeWordEnglish(&(*root)->right, aux->info.englishWord, unit);
                    }
                }
            }
        }
        else if (strcmp((*root)->info.englishWord, englishWord) > 0)
            find = removeWordEnglish(&(*root)->left, englishWord, unit);
        else
            find = removeWordEnglish(&(*root)->right, englishWord, unit);
    }
    return (find);
}

/**
 * @brief Percorre a árvore Rubro-Negra para remover uma palavra em inglês e rastrear palavras associadas em português.
 * 
 * @param root Ponteiro para a raiz da árvore Rubro-Negra.
 * @param wordEnglish Palavra em inglês a ser removida.
 * @param unit Unidade associada à palavra a ser removida.
 * @param wordPT Matriz para armazenar palavras em português associadas.
 * @param remove Ponteiro para uma flag indicando se a palavra em português foi encontrada e marcada para remoção.
 * @param removeCount Ponteiro para o contador de palavras em português marcadas para remoção.
 */
void traverseRedBlack(RBTree **root, const char *wordEnglish, int unit, char wordPT[][50], int *remove, int *removeCount)
{
    if ((*root) != NULL)
    {
        traverseRedBlack(&(*root)->left, wordEnglish, unit, wordPT, remove, removeCount);
        int encontrou = removeWordEnglish(&(*root)->info.binaryTreeEnglish, wordEnglish, unit);
        if (encontrou && (*root)->info.binaryTreeEnglish == NULL)
        {
            *remove = 1;
            strcpy(wordPT[*removeCount], (*root)->info.portugueseWord);
            (*removeCount)++;
        }
        traverseRedBlack(&(*root)->right, wordEnglish, unit, wordPT, remove, removeCount);
    }
}

/**
 * @brief Move um nó da árvore Rubro-Negra para a esquerda em um cenário de remoção.
 * 
 * @param root Ponteiro para a raiz da árvore Rubro-Negra.
 */
void moveLeftRed(RBTree **root)
{
    flipColors(root);
    if (getColor((*root)->right->left) == RED)
    {
        rotateRight(&(*root)->right);
        rotateLeft(root);
        flipColors(root);
    }
}

/**
 * @brief Move um nó da árvore Rubro-Negra para a direita em um cenário de remoção.
 * 
 * @param root Ponteiro para a raiz da árvore Rubro-Negra.
 */
void moveRightRed(RBTree **root)
{
    flipColors(root);
    if (getColor((*root)->left->left) == RED)
    {
        rotateRight(root);
        flipColors(root);
    }
}

/**
 * @brief Encontra o menor nó na subárvore esquerda de um nó da árvore Rubro-Negra.
 * 
 * @param root Ponteiro para o nó raiz da subárvore.
 * @return RBTree* Ponteiro para o menor nó da subárvore.
 */
RBTree *sourceMinor(RBTree *root)
{
    RBTree *no1 = root;
    RBTree *no2 = root->left;
    while (no2)
    {
        no1 = no2;
        no2 = no2->left;
    }
    return (no1);
}


/**
 * @brief Remove o menor nó de uma subárvore Rubro-Negra.
 * 
 * @param root Ponteiro para o nó raiz da subárvore.
 * @return int Retorna 1 se o nó foi removido com sucesso.
 */
int removeMinor(RBTree **root)
{
    if (!(*root)->left)
    {
        free(*root);
        *root = NULL;
    }
    else
    {
        if (getColor((*root)->left) == BLACK && getColor((*root)->left->left) == BLACK)
            moveLeftRed(root);
        
        removeMinor(&(*root)->left);
    }
    if (*root)
        balanceTree(root);
}

/**
 * @brief Remove uma palavra em português da árvore Rubro-Negra.
 * 
 * @param root Ponteiro para a raiz da árvore Rubro-Negra.
 * @param wordPT Palavra em português a ser removida.
 * @return int Retorna 1 se a remoção for bem-sucedida, 0 caso contrário.
 */
int removeRBTree(RBTree **root, char *wordPT)
{
    int removed = 0;
    if (*root)
    {
        if (strcmp(wordPT, (*root)->info.portugueseWord) < 0)
        {
            if (getColor((*root)->left) == BLACK && getColor((*root)->left->left) == BLACK)
                moveLeftRed(root);
            removed = removeRBTree(&(*root)->left, wordPT);
        }
        else
        {
            if (getColor((*root)->left) == RED)
                rotateRight(root);
            if (!strcmp(wordPT, (*root)->info.portugueseWord) && !(*root)->right)
            {
                free(*root);
                *root = NULL;
                removed = 1;
            }
            else
            {
                if (getColor((*root)->right) == BLACK && getColor((*root)->right->left) == BLACK)
                    moveRightRed(root);
                if (!strcmp(wordPT, (*root)->info.portugueseWord))
                {
                    RBTree *aux = sourceMinor((*root)->right);
                    (*root)->info = aux->info;
                    removeMinor(&(*root)->right);
                    removed = 1;
                }
                else
                    removed = removeRBTree(&(*root)->right, wordPT);
            }
        }
    }
    if (*root)
        balanceTree(root);

    return (removed);
}

/**
 * @brief Remove uma palavra em inglês e suas associações na árvore Rubro-Negra.
 * 
 * @param root Ponteiro para a raiz da árvore Rubro-Negra.
 * @param englishWord Palavra em inglês a ser removida.
 * @param unit Unidade associada à palavra a ser removida.
 */
void removeFromRBTreeEN(RBTree **root, const char *englishWord, int unit)
{
    char wordPT[100][50];
    int remove = 0;
    int removeCount = 0;
    traverseRedBlack(root, englishWord, unit, wordPT, &remove, &removeCount);
    if (remove)
    {
        for (int i = 0; i < removeCount; i++)
        {
            int sucesso = 0;
            sucesso = removeRBTree(root, wordPT[i]);
            if (!sucesso)
                printf("Erro ao remover.\n");
            else
                printf("Remocao bem-sucedida!\n");
        }
    }
    if ((*root) != NULL)
        (*root)->color = BLACK;
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - informar uma palavra em português e a unidade a qual a mesma pertence e então remove-la, para isto
// deve remove a palavra em inglês da árvore binária correspondente a palavra em português da mesma
// unidade. Caso ela seja a única palavra na árvore binária, a palavra em português deve ser removida da
// árvore Rubro Negra.
// ---------------------------------------------------- XXXXXX -------------------------------------------------
/**
 * @brief Verifica se uma unidade está presente na lista de unidades.
 *
 * @param Units Ponteiro para a lista de unidades.
 * @param unit Unidade a ser verificada.
 * @return 1 se a unidade for encontrada, 0 caso contrário.
 */
int findunit(Units *Units, int unit)
{
    int find = 0;
    if (Units != NULL)
    {
        if (Units->unit == unit)
            find = 1;
        else
            find = findunit(Units->next, unit);
    }
    return find;
}

/**
 * @brief Busca uma palavra em inglês correspondente a uma palavra em português em uma unidade específica.
 *
 * @param root Ponteiro para a raiz da árvore binária de palavras em inglês.
 * @param unit Unidade a qual a palavra em inglês pertence.
 * @param wordEN Array onde a palavra em inglês encontrada será armazenada.
 * @param find Ponteiro para variável que indica se a palavra foi encontrada.
 */
void sourcewordEN(EngPT *root, int unit, char *wordEN, int *find)
{
    if (root != NULL && *find != 1)
    {
        sourcewordEN(root->left, unit, wordEN, find);

        if (*find != 1 && findunit(root->info.units, unit))
        {
            strcpy(wordEN, root->info.englishWord);
            *find = 1;
        }

        if (*find != 1)
            sourcewordEN(root->right, unit, wordEN, find);
    }
}

/**
 * @brief Percorre a árvore rubro-negra para remover uma palavra em português e sua correspondente em inglês.
 *
 * @param root Ponteiro duplo para a raiz da árvore rubro-negra.
 * @param wordPT Palavra em português a ser removida.
 * @param unit Unidade da palavra em português.
 * @param remove Ponteiro para indicar se a remoção ocorreu.
 * @return 1 se a palavra foi removida, 0 caso contrário.
 */
int traverseRBTreePTBR(RBTree **root, char *wordPT, int unit, int *remove)
{
    if ((*root) != NULL)
    {
        if (strcmp(wordPT, (*root)->info.portugueseWord) == 0)
        {
            char wordEN[50];
            int find = 0;

            sourcewordEN((*root)->info.binaryTreeEnglish, unit, wordEN, &find);

            if (find == 1)
            {
                find = removeWordEnglish(&(*root)->info.binaryTreeEnglish, wordEN, unit);
                if (find == 1 && (*root)->info.binaryTreeEnglish == NULL)
                    *remove = 1;
            }
        }
        else
        {
            if (strcmp(wordPT, (*root)->info.portugueseWord) < 0)
                traverseRBTreePTBR(&(*root)->left, wordPT, unit, remove);
            else
                traverseRBTreePTBR(&(*root)->right, wordPT, unit, remove);
        }
    }
    return (*remove);
}

/**
 * @brief Remove uma palavra em português da árvore rubro-negra, caso sua correspondente em inglês também seja removida.
 *
 * @param root Ponteiro duplo para a raiz da árvore rubro-negra.
 * @param wordPT Palavra em português a ser removida.
 * @param unit Unidade da palavra em português.
 */
void removeFromRBTreePT(RBTree **root, char *wordPT, int unit)
{
    int remove = 0;
    traverseRBTreePTBR(root, wordPT, unit, &remove);
    printf("remove: %d\n", remove);
    if (remove == 1)
    {
        int sucesso = 0;
        sucesso = removeRBTree(root, wordPT);
        if (!sucesso)
            printf("Erro ao remove.\n");
        else
            printf("Remocao bem-sucedida!\n");
    }

    if ((*root) != NULL)
        (*root)->color = BLACK;
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
//                                                    AUXILIARES
// ---------------------------------------------------- XXXXXX -------------------------------------------------

/**
 * @brief Insere uma unidade na lista encadeada de unidades.
 *
 * @param units Ponteiro duplo para a cabeça da lista de unidades.
 * @param unit Valor da unidade a ser inserida.
 */
void insertUnit(Units **units, int unit)
{
    if (*units == NULL)
    {
        *units = (Units *)malloc(sizeof(Units));
        (*units)->unit = unit;
        (*units)->next = NULL;
    }
    else
        insertUnit(&(*units)->next, unit);
}

/**
 * @brief Insere um nó na árvore binária.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param info Informações do nó a ser inserido.
 */
void insertBinaryTree(EngPT **root, InfoEngPT info)
{
    if (*root == NULL)
    {
        *root = (EngPT *)malloc(sizeof(EngPT));
        (*root)->info = info;
        (*root)->left = NULL;
        (*root)->right = NULL;
    }
    else
    {
        if (strcmp((*root)->info.englishWord, info.englishWord) == 0)
            insertUnit(&(*root)->info.units, info.units->unit);
        else if (strcmp((*root)->info.englishWord, info.englishWord) > 0)
            insertBinaryTree(&(*root)->left, info);
        else
            insertBinaryTree(&(*root)->right, info);
    }
}

/**
 * @brief Cria um novo nó na árvore rubro-negra.
 *
 * @param node Ponteiro duplo para o nó a ser criado.
 * @param info Informações do nó.
 */
void createNode(RBTree **node, InfoBin info)
{
    *node = (RBTree *)malloc(sizeof(RBTree));
    (*node)->info = info;
    (*node)->info.portugueseWord = strdup(info.portugueseWord);
    (*node)->color = RED;
    (*node)->left = NULL;
    (*node)->right = NULL;
}

/**
 * @brief Inverte as cores do nó e de seus filhos.
 *
 * @param root Ponteiro duplo para a raiz da árvore rubro-negra.
 */
void flipColors(RBTree **root)
{
    (*root)->color = !(*root)->color;
    if ((*root)->left != NULL)
        (*root)->left->color = !(*root)->left->color;

    if ((*root)->right != NULL)
        (*root)->right->color = !(*root)->right->color;
}

/**
 * @brief Realiza uma rotação à esquerda na árvore rubro-negra.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 */
void rotateLeft(RBTree **root)
{
    RBTree *aux = (*root)->right;
    (*root)->right = aux->left;
    aux->left = *root;
    aux->color = (*root)->color;
    (*root)->color = RED;
    *root = aux;
}

/**
 * @brief Realiza uma rotação à direita na árvore rubro-negra.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 */
void rotateRight(RBTree **root)
{
    RBTree *aux = (*root)->left;
    (*root)->left = aux->right;
    aux->right = *root;
    aux->color = (*root)->color;
    (*root)->color = RED;
    *root = aux;
}

/**
 * @brief Obtém a cor de um nó da árvore rubro-negra.
 *
 * @param root Ponteiro para o nó da árvore.
 * @return Cor do nó (1 para vermelho, 0 para preto).
 */
int getColor(RBTree *root)
{
    return (root == NULL) ? 1 : root->color;
}

/**
 * @brief Balanceia a árvore rubro-negra.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 */
void balanceTree(RBTree **root)
{
    if (getColor((*root)->left) == BLACK && getColor((*root)->right) == RED)
        rotateLeft(root);
    if ((*root)->left != NULL && (*root)->left->color == RED && (*root)->left->left != NULL && (*root)->left->left->color == RED)
        rotateRight(root);
    if (getColor((*root)->left) == RED && getColor((*root)->right) == RED)
        flipColors(root);
}

/**
 * @brief Insere informações na árvore rubro-negra.
 *
 * @param root Ponteiro duplo para a raiz da árvore.
 * @param info Informações a serem inseridas.
 * @return 1 se inserido, 0 se não.
 */
int insertIntoRBTree(RBTree **root, InfoBin info)
{
    int inserted = 0;
    if (*root == NULL)
    {
        createNode(root, info);
        inserted = 1;
    }
    else
    {
        if (strcmp((*root)->info.portugueseWord, info.portugueseWord) == 0)
        {
            insertBinaryTree(&(*root)->info.binaryTreeEnglish, info.binaryTreeEnglish->info);
            inserted = 1;
        }
        else if (strcmp((*root)->info.portugueseWord, info.portugueseWord) > 0)
            inserted = insertIntoRBTree(&(*root)->left, info);
        else
            inserted = insertIntoRBTree(&(*root)->right, info);
        balanceTree(root);
    }
    return (inserted);
}

/**
 * @brief Verifica se uma unidade está presente na lista de unidades.
 *
 * @param units Ponteiro para a lista de unidades.
 * @param unit Unidade a ser verificada.
 * @return 1 se presente, 0 caso contrário.
 */
int isUnitPresent(Units *units, int unit)
{
    return (units != NULL) && (units->unit == unit || isUnitPresent(units->next, unit));
}

/**
 * @brief Exibe a árvore binária cujos nós contêm a unidade especificada.
 *
 * @param root Ponteiro para a raiz da árvore binária.
 * @param unit Unidade a ser exibida.
 */
void displayBinaryTree(EngPT *root, int unit)
{
    if (root != NULL)
    {
        displayBinaryTree(root->left, unit);
        if (isUnitPresent(root->info.units, unit))
        {
            printf("%s\n", root->info.englishWord);
            printf("Unidade: %d\n", unit);
        }
        displayBinaryTree(root->right, unit);
    }
}

/**
 * @brief Busca uma unidade na árvore binária.
 *
 * @param root Ponteiro para a raiz da árvore.
 * @param unit Unidade a ser buscada.
 * @return 1 se encontrada, 0 caso contrário.
 */
int searchUnit(EngPT *root, int unit)
{
    int found = 0;

    if (root != NULL)
    {
        if (isUnitPresent(root->info.units, unit))
            found = 1;
        found = found || searchUnit(root->left, unit) || searchUnit(root->right, unit);
    }
    return (found);
}

/**
 * @brief Exibe unidades na árvore rubro-negra que correspondem à unidade especificada.
 *
 * @param root Ponteiro para a raiz da árvore rubro-negra.
 * @param unit Unidade a ser exibida.
 * @param found Ponteiro para flag que indica se a unidade foi encontrada.
 */
void displayUnit(RBTree *root, int unit, int *found)
{
    if (root != NULL)
    {
        displayUnit(root->left, unit, found);
        if (searchUnit(root->info.binaryTreeEnglish, unit))
        {
            *found = 1;
            printf("%s\n", root->info.portugueseWord);
            displayBinaryTree(root->info.binaryTreeEnglish, unit);
            printf("\n");
        }
        displayUnit(root->right, unit, found);
    }
}