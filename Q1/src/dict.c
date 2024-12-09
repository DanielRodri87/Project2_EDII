#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ------------------------------------------ Arv 2-3 ---------------------------------------------------------
Portuguese23 *searchWord(Portuguese23 **no, const char *portugueseWord)
{
    Portuguese23 *inserida = NULL; // Inicializa o retorno como NULL

    if (no != NULL && *no != NULL)
    {
        if (strcmp(portugueseWord, (*no)->info1.portugueseWord) == 0)
        {
            inserida = (*no); // Palavra encontrada, retorna o nó
        }
        else if ((*no)->infoCount == 2 && strcmp(portugueseWord, (*no)->info2.portugueseWord) == 0)
        {
            inserida = (*no);
        }
        else
        {
            // Continua a busca nos filhos
            if (strcmp(portugueseWord, (*no)->info1.portugueseWord) < 0)
            {
                inserida = searchWord(&(*no)->left, portugueseWord);
            }
            else if ((*no)->infoCount == 1 || strcmp(portugueseWord, (*no)->info2.portugueseWord) < 0)
            {
                inserida = searchWord(&(*no)->center, portugueseWord);
            }
            else
            {
                inserida = searchWord(&(*no)->right, portugueseWord);
            }
        }
    }

    return inserida;
}

void addTranslation(Portuguese23 *no, const char *portugueseWord, const char *englishWord, int unit)
{
    if (strcmp(portugueseWord, (no)->info1.portugueseWord) == 0)
    {
        addEnglishTranslation(&(no)->info1, englishWord, unit);
    }
    else if (no->infoCount == 2 && strcmp(portugueseWord, no->info2.portugueseWord) == 0)
    {
        addEnglishTranslation(&(no)->info2, englishWord, unit);
    }
}

int insertPortugueseWord(Portuguese23 **arvore, char *portugueseWord, char *englishWord, int unit) {
    Info promove;
    Portuguese23 *father = NULL;
    int inseriu;

    // Busca a palavra na árvore
    Portuguese23 *noExistente = NULL;
    noExistente =  searchWord(arvore, portugueseWord);

    if (noExistente != NULL) {
        printf("A palavra já existe. Adicionando tradução...\n");
        addTranslation(noExistente, portugueseWord, englishWord, unit);
        inseriu = 1;
    } else {
        Info novoInfo = createInfo(portugueseWord, englishWord, unit);
        insertTree23(arvore, &novoInfo, &promove, &father);
        inseriu = 0;
    }
    return inseriu;
}

Info createInfo(char *palavra, char *englishWord, int unit)
{
    Info info;

    info.portugueseWord = malloc(strlen(palavra) + 1);
    strcpy(info.portugueseWord, palavra);

    info.englishWord = NULL;
    info.englishWord = insertEnglishWord(info.englishWord, englishWord, unit);
    return info;
}

Portuguese23 *createNode(const Info *info, Portuguese23 *leftChild, Portuguese23 *centerChild)
{
    Portuguese23 *no = (Portuguese23 *)malloc(sizeof(Portuguese23));
    no->info1 = *info;
    no->info2.englishWord = NULL;
    no->info2.portugueseWord = NULL;
    no->left = leftChild;
    no->center = centerChild;
    no->right = NULL;
    no->infoCount = 1;

    return no;
}

Portuguese23 *addKey(Portuguese23 *no, const Info *informacao, Portuguese23 *filho)
{
    if (strcmp(informacao->portugueseWord, no->info1.portugueseWord) > 0)
    {
        no->info2 = *informacao;
        no->right = filho;
    }
    else
    {
        no->info2 = no->info1;
        no->right = no->center;
        no->info1 = *informacao;
        no->center = filho;
    }
    no->infoCount = 2;
    return no;
}

Portuguese23 *splitNode(Portuguese23 **no, const Info *informacao, Info *promove, Portuguese23 **filho)
{
    Portuguese23 *maior;

    if (strcmp(informacao->portugueseWord, (*no)->info2.portugueseWord) > 0)
    {
        *promove = (*no)->info2;
        if(filho)
            maior = createNode(informacao, (*no)->right, *filho);
        else
            maior = createNode(informacao, (*no)->right, NULL);
    }
    else if (strcmp(informacao->portugueseWord, (*no)->info1.portugueseWord) > 0)
    {
        *promove = *informacao;
        if(filho)
            maior = createNode(&(*no)->info2, *filho, (*no)->right);
        else
            maior = createNode(&(*no)->info2, NULL, (*no)->right);
    }
    else
    {
        *promove = (*no)->info1;
        maior = createNode(&(*no)->info2, (*no)->center, (*no)->right);
        (*no)->info1 = *informacao;
        if(filho)
            (*no)->center = *filho;
        else
            (*no)->center = NULL;
    }

    (*no)->infoCount = 1;
    return maior;
}

int isLeaf(const Portuguese23 *no)
{
    return (no->left == NULL);
}


Portuguese23 *insertTree23(Portuguese23 **no, Info *informacao, Info *promove, Portuguese23 **father)
{
    Info promove1;
    Portuguese23 *maiorNo = NULL;
    if (*no == NULL)
    {
        // Cria um novo nó caso seja nulo
        *no = createNode(informacao, NULL, NULL);
    }
    else
    {
        if (isLeaf(*no))
        { // Caso seja folha
            if ((*no)->infoCount == 1)
            {
                // O nó tem espaço para a nova chave
                *no = addKey(*no, informacao, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                maiorNo = splitNode(no, informacao, promove, NULL);
                if (*father == NULL)
                { // Se não há father, criar nova root
                    *no = createNode(promove, *no, maiorNo);
                    maiorNo = NULL;
                }
            }
        }
        else
        { // Nó não e folha
            // Navega para o filho apropriado
            if (strcmp(informacao->portugueseWord, (*no)->info1.portugueseWord) < 0)
            {
                maiorNo = insertTree23(&((*no)->left), informacao, promove, no);
            }
            else if ((*no)->infoCount == 1 || strcmp(informacao->portugueseWord, (*no)->info2.portugueseWord) < 0)
            {
                maiorNo = insertTree23(&((*no)->center), informacao, promove, no);
            }
            else
            {
                maiorNo = insertTree23(&((*no)->right), informacao, promove, no);
            }

            // Após inserir, verifica se houve promoção
            if (maiorNo)
            {
                if ((*no)->infoCount == 1)
                {
                    // Adiciona chave promovida no nó atual
                    *no = addKey(*no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                {
                    // O nó precisa ser quebrado
                    maiorNo = splitNode(no, promove, &promove1, &maiorNo);
                    if (*father == NULL)
                    {
                        *no = createNode(&promove1, *no, maiorNo);
                        maiorNo = NULL;
                    }
                }
            }
        }
    }

    return maiorNo;
}



// ############################################## FUNÇOES PARA EXIBIR ##############################################

void displayTree23(const Portuguese23 *root)
{
    if (root != NULL)
    {
        displayTree23(root->left);
        printf("Palavra (PT): %s", root->info1.portugueseWord);
        if (root->info1.englishWord != NULL && root->info1.englishWord->englishWord != NULL)
        {
            printBinaryTree(root->info1.englishWord);
            printf("\n");
        }
        displayTree23(root->center);
        // Se houver o segundo elemento (infoCount == 2), exibe o segundo filho
        if (root->infoCount == 2)
        {
            printf("Palavra (PT): %s", root->info2.portugueseWord);

            // Exibir a tradução em inglês, se houver
            if (root->info2.englishWord != NULL && root->info2.englishWord->englishWord != NULL)
                printBinaryTree(root->info2.englishWord);
            printf("\n");
        }

        if (root->infoCount == 2)
        {
            displayTree23(root->right);
        }
    }
}

void printTranslations(EnglishBin *node, int unit, const char *palavraPortuguês)
{
    if (node)
    {
        if (node->unit == unit)
        {
            printf("Palavra em Português: %s\n", palavraPortuguês);
            printf("Palavra em inglês: %s\n", node->englishWord);
        }
        printTranslations(node->left, unit, palavraPortuguês);
        printTranslations(node->right, unit, palavraPortuguês);
    }
}

void displayPortugueseTranslation(Portuguese23 **root, const char *portugueseWord)
{
    Portuguese23 *resultado = NULL;
    if (root != NULL)
    {
        resultado = searchWord(root, portugueseWord);
        if (resultado)
        {
            printf("Traduções em inglês para a palavra '%s':\n", portugueseWord);

            if (strcmp(portugueseWord, resultado->info1.portugueseWord) == 0)
            {
                printBinaryTree(resultado->info1.englishWord);
            }
            else
            {
                printBinaryTree(resultado->info2.englishWord);
            }
        }
        else
        {
            printf("A palavra '%s' não foi encontrada na árvore.\n", portugueseWord);
        }
    }
}



// ############################################# REMOÇÃO ############################################


void smallestInfoRight(Portuguese23 *root, Portuguese23 **no, Portuguese23 **fatherNo)
{
    if (root->left != NULL)
    {
        *fatherNo = root;
        smallestInfoRight(root->left, no, fatherNo);
    }
    else
        *no = root;
}

void largestInfoLeft(Portuguese23 *root, Portuguese23 **no, Portuguese23 **fatherNo)
{
    if (root->right != NULL)
    {
        *fatherNo = root;
        largestInfoLeft(root->right, no, fatherNo);
    }
    else
        *no = root;
}

int remove23(Portuguese23 **father, Portuguese23 **root, char *valor)
{
    int removeu = 0;
    Portuguese23 *no = NULL, *no1, *fatherNo = NULL, *fatherNo1 = NULL, **aux;
    aux = (Portuguese23 **)malloc(sizeof(Portuguese23 *));
    no1 = (Portuguese23 *)malloc(sizeof(Portuguese23));

    if (*root != NULL)
    {
        if (isLeaf(*root) == 1)
        {
            if ((*root)->infoCount == 2)
            {
                if (strcmp(valor, (*root)->info2.portugueseWord) == 0)
                { // quando é folha, tem duas informações e o numero ta na segunda posição
                    (*root)->info2.englishWord = NULL;
                    (*root)->info2.portugueseWord = NULL;
                    (*root)->infoCount = 1;
                    removeu = 1;
                }
                else if (strcmp(valor, (*root)->info1.portugueseWord) == 0)
                { // quando é folha, tem duas informações e o numero ta na primeira posição do nó
                    (*root)->info1 = (*root)->info2;
                    (*root)->info2.englishWord = NULL;
                    (*root)->info2.portugueseWord = NULL;
                    (*root)->infoCount = 1;
                    removeu = 1;
                }
            }
            else if (strcmp(valor, (*root)->info1.portugueseWord) == 0)
            {
                if (*father == NULL)
                {
                    free(*root);
                    *root = NULL;
                    removeu = 1;
                }
                else if (*root == (*father)->left)
                {
                    (*root)->info1 = (*father)->info1;
                    fatherNo = *father;
                    smallestInfoRight((*father)->center, &no, &fatherNo);
                    (*father)->info1 = no->info1;
                    removeu = 1;

                    if (no->infoCount == 2)
                    {
                        no->info1 = no->info2;
                        (*root)->info2.englishWord = NULL;
                        (*root)->info2.portugueseWord = NULL;
                        no->infoCount = 1;
                    }
                    else
                    {
                        if (fatherNo->infoCount == 1)
                        {
                            (*root)->info2 = no->info1;
                            (*root)->infoCount = 2;
                            free(no);
                            *father = *root;
                        }
                        else
                        {
                            no->info1 = fatherNo->info2;
                            fatherNo1 = fatherNo;
                            smallestInfoRight(fatherNo->right, &no1, &fatherNo1);
                            fatherNo->info2 = no1->info1;

                            if (no1->infoCount == 2)
                            {
                                no1->info1 = no1->info2;
                                no1->info2.englishWord = NULL;
                                no1->info2.portugueseWord = NULL;
                                no1->infoCount = 1;
                            }
                            else
                            {
                                no->info2 = fatherNo->info2;
                                no->infoCount = 2;
                                fatherNo->info2.englishWord = NULL;
                                fatherNo->info2.portugueseWord = NULL;
                                fatherNo->infoCount = 1;
                                free(no1);
                                fatherNo1->right = NULL;
                            }
                        }
                    }
                }
                else if ((*root) == (*father)->center)
                {
                    removeu = 1;
                    if ((*father)->infoCount == 1)
                    {
                        if (((*father)->left)->infoCount == 2)
                        {
                            (*root)->info1 = (*father)->info1;
                            (*father)->info1 = ((*father)->left)->info2;
                            ((*father)->left)->info2.englishWord = NULL;
                            ((*father)->left)->info2.portugueseWord = NULL;
                            ((*father)->left)->infoCount = 1;
                        }
                        else
                        {
                            ((*father)->left)->info2 = (*father)->info1;
                            free(*root);
                            ((*father)->left)->infoCount = 2;
                            *aux = (*father)->left;
                            free(*father);
                            *father = *aux;
                        }
                    }
                    else
                    {
                        (*root)->info1 = (*father)->info2;
                        fatherNo = *father;
                        smallestInfoRight((*father)->right, &no, &fatherNo);
                        (*father)->info2 = no->info1;

                        if (no->infoCount == 2)
                        {
                            no->info1 = no->info2;
                            no->info2.englishWord = NULL;
                            no->info2.portugueseWord = NULL;
                            no->infoCount = 1;
                        }
                        else
                        {
                            (*root)->infoCount = 2;
                            (*root)->info2 = (*father)->info2;
                            (*father)->info2.englishWord = NULL;
                            (*father)->info2.portugueseWord = NULL;
                            (*father)->infoCount = 1;
                            free(no);
                            (*father)->right = NULL;
                        }
                    }
                }
                else
                {
                    removeu = 1;
                    fatherNo = *father;
                    largestInfoLeft((*father)->center, &no, &fatherNo);

                    if (no->infoCount == 1)
                    {
                        no->info2 = (*father)->info2;
                        (*father)->info2.englishWord = NULL;
                        (*father)->info2.portugueseWord = NULL;
                        (*father)->infoCount = 1;
                        no->infoCount = 2;
                        free(*root);
                        *root = NULL;
                    }
                    else
                    {
                        (*root)->info1 = (*father)->info2;
                        (*father)->info2 = no->info2;
                        no->info2.englishWord = NULL;
                        no->info2.portugueseWord = NULL;
                        no->infoCount = 1;
                    }
                }
            }
        }
        else
        { // se nao é folha
            if (strcmp(valor, (*root)->info1.portugueseWord) < 0)
                removeu = remove23(root, &(*root)->left, valor);
            else if (strcmp(valor, (*root)->info1.portugueseWord) == 0)
            {
                fatherNo = *root;
                smallestInfoRight((*root)->center, &no, &fatherNo);
                (*root)->info1 = no->info1;
                remove23(root, &(*root)->center, (*root)->info1.portugueseWord);
                removeu = 1;
            }
            else if (((*root)->infoCount == 1) || (strcmp(valor, (*root)->info1.portugueseWord) < 0))
            {
                removeu = remove23(root, &(*root)->center, valor);
            }
            else if (strcmp(valor, (*root)->info1.portugueseWord) == 0)
            {
                fatherNo = *father;
                smallestInfoRight((*father)->right, &no, &fatherNo);
                (*root)->info2 = no->info1;
                remove23(root, &(*root)->right, (*root)->info2.portugueseWord);
                removeu = 1;
            }
            else
            {
                removeu = remove23(root, &(*root)->right, valor);
            }
        }
    }
    return removeu;
}

/*#########################################FREE#######################################################*/

void freeInfo23(Info *info)
{
  FreeTreeBinary(info->englishWord);
  free(info->portugueseWord);
}

void freeTree(Portuguese23 *no)
{
    if (no != NULL)
    {
        freeTree(no->left);
        freeTree(no->center);
        if (no->infoCount == 2)
            freeTree(no->right);
        if (no->infoCount == 2)
            freeInfo23(&no->info2);
        freeInfo23(&no->info1);
        free(no);
    }
}


// ------------------------ Árvore Binária ---------------------
EnglishBin *createBinaryNode(const char *englishWord, int unit)
{
    EnglishBin *novoNo = (EnglishBin *)malloc(sizeof(EnglishBin));
    if (novoNo != NULL)
    {
        novoNo->englishWord = (char *)malloc(strlen(englishWord) + 1);
        strcpy(novoNo->englishWord, englishWord);
        novoNo->unit = unit;
        novoNo->left = novoNo->right = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em inglês na arvore binaria de busca
EnglishBin *insertEnglishWord(EnglishBin *root, const char *englishWord, int unit)
{
    EnglishBin *result;
    if (root == NULL)
    {
        result = createBinaryNode(englishWord, unit);
    }
    else
    {
        if (strcmp(englishWord, root->englishWord) < 0)
        {
            root->left = insertEnglishWord(root->left, englishWord, unit);
        }
        else if (strcmp(englishWord, root->englishWord) > 0)
        {
            root->right = insertEnglishWord(root->right, englishWord, unit);
        }
        result = root;
    }
    return result;
}

void addEnglishTranslation(Info *info, const char *palavraIng, int unit)
{
    info->englishWord = insertEnglishWord(info->englishWord, palavraIng, unit);
}

void printBinaryTree(EnglishBin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->left); // Percorre a árvore à leftuerda
        printf("\n");
        // Imprime a tradução de inglês associada à palavra em português
        printf("Palavra em Inglês: %s = unit: %d\n", root->englishWord, root->unit);
        printBinaryTree(root->right); // Percorre a árvore à righteita
    }
}

int isBinaryLeaf(EnglishBin *root){
    return (root->left == NULL && root->right == NULL);
}

EnglishBin *onlyOneChild(EnglishBin *root){
    EnglishBin *aux;
    aux = NULL;

    if (root->right == NULL)
    {
        aux = root->left;
    }
    else if (root->left == NULL)
    {
        aux = root->right;
    }

    return aux;
}

EnglishBin *smallestChild(EnglishBin *root){
    EnglishBin *aux;
    aux = root;

    if (root)
    {
        if (root->left)
            aux = smallestChild(root->left);
    }

    return aux;
}

int removeEnglishWord(EnglishBin **root, char *palavra)
{
    EnglishBin *endFilho;
    int existe = 0;

    if (*root)
    {
        if (strcmp(palavra, (*root)->englishWord) == 0)
        {
            existe = 1;
            printf("removendo palavra: %s\n", palavra);
            EnglishBin *aux = *root;
            if (isBinaryLeaf(*root))
            {
                free(aux);
                *root = NULL;
            }
            else if ((endFilho = onlyOneChild(*root)) != NULL)
            {
                free(aux);
                *root = endFilho;
            }
            else
            {
                endFilho = smallestChild((*root)->right);
                strcpy((*root)->englishWord, endFilho->englishWord);
                (*root)->unit = endFilho->unit;

                removeEnglishWord(&(*root)->right, endFilho->englishWord);
            }
        }
        else if (strcmp(palavra, (*root)->englishWord) < 0)
        {
            existe = removeEnglishWord(&(*root)->left, palavra);
        }
        else
        {
            existe = removeEnglishWord(&(*root)->right, palavra);
        }
    }

    return existe;
}

void searchEnglishWord(Portuguese23 **root, char *word, int unit, Portuguese23 **father)
{
    int removeu;
    if (*root != NULL)
    {
        searchEnglishWord(&(*root)->left, word, unit, father);

        if ((*root)->info1.englishWord != NULL && (*root)->info1.englishWord->unit == unit)
        {
            removeu = removeEnglishWord(&(*root)->info1.englishWord, word);
            if(removeu) 
                printf("A palavra %s foi removida com sucesso!\n\n", word);
            if ((*root)->info1.englishWord == NULL)
            {
                removeu = remove23(father, root, (*root)->info1.portugueseWord);
                if(removeu) printf("Removido\n\n");
            }
        }

        searchEnglishWord(&(*root)->center, word, unit, root);

        if ((*root)->infoCount == 2 && (*root)->info2.englishWord != NULL && (*root)->info2.englishWord->unit == unit)
        {
            removeu = removeEnglishWord(&(*root)->info2.englishWord, word);
            if(removeu) 
                printf("A palavra %s foi removida com sucesso!\n\n", word);
            if ((*root)->info2.englishWord == NULL)
            {
                removeu = remove23(father, root, (*root)->info2.portugueseWord);
                if(removeu) printf("Removido\n\n");
            }
        }
        if ((*root)->infoCount == 2 && (*root)->info2.englishWord != NULL)
        {
            searchEnglishWord(&(*root)->right, word, unit, root);
        }
    }
}

void FreeTreeBinary(EnglishBin *root)
{
  if (root)
  {
    FreeTreeBinary(root->left);
    FreeTreeBinary(root->right);
    free(root->englishWord);
    free(root);
  }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// I - informar uma unit e então imprima todas as words da unit em português seguida das
// equivalentes em inglês
// ---------------------------------------------------- XXXXXX -------------------------------------------------
void printWordsByUnit(Portuguese23 *root, int unit)
{
    if (root != NULL)
    {
        if (root->infoCount >= 1 && root->info1.englishWord->unit == unit)
            printf("%s: %s;\n",
                   root->info1.englishWord->englishWord,
                   root->info1.portugueseWord);
        
        if (root->infoCount == 2 && root->info2.englishWord->unit == unit)
            printf("%s: %s;\n",
                   root->info2.englishWord->englishWord,
                   root->info2.portugueseWord);

        printWordsByUnit(root->left, unit);
        printWordsByUnit(root->center, unit);

        if (root->infoCount == 2)
            printWordsByUnit(root->right, unit);
    }
}

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// II - informar uma word em português e então imprima todas as words em inglês equivalente à word em
// português dada, independente da unit
// ---------------------------------------------------- XXXXXX -------------------------------------------------
// void findEnglishByPortuguese(PortugueseEnglish *root, const char *portugueseWord)
// {
//     if (root != NULL)
//     {
//         if (root->infoCount >= 1 && strcmp(root->info1.portugueseWord, portugueseWord) == 0)
//             printf("Inglês: %s\n", root->info1.englishTranslation->word);
//         if (root->infoCount == 2 && strcmp(root->info2.portugueseWord, portugueseWord) == 0)
//             printf("Inglês: %s\n", root->info2.englishTranslation->word);

//         findEnglishByPortuguese(root->left, portugueseWord);
//         findEnglishByPortuguese(root->center, portugueseWord);
//         if (root->infoCount == 2)
//             findEnglishByPortuguese(root->right, portugueseWord);
//     }
// }

// ---------------------------------------------------- XXXXXX -------------------------------------------------
// III - informar uma word em inglês e a unit a qual a mesma pertence removê-la das árvores binárias
// das quais ela pertence. Caso ela seja a única word em uma das árvores binárias, remover também da
// árvore 2-3
// ---------------------------------------------------- XXXXXX -------------------------------------------------



// ---------------------------------------------------- XXXXXX -------------------------------------------------
// IV - informar uma word em português e a unit a qual a mesma pertence e então removê-la, para isto
// deve remover a word em inglês da árvore binária correspondente a word em português da mesma
// unit. Caso ela seja a única word na árvore binária, a word em português deve ser removida da
// árvore 2-3 */
// ---------------------------------------------------- XXXXXX -------------------------------------------------