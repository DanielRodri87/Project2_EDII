#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #######################################################################################

// 2-3

// #######################################################################################
TranslationInfo criaInfo(char *palavra, char *englishWord, char *unit)
{
    TranslationInfo info;

    // Aloca e copia a palavra em portugues
    info.portugueseWord = malloc(strlen(palavra) + 1);
    strcpy(info.portugueseWord, palavra);

    // Estrutura de englishWord
    info.englishWord = malloc(sizeof(EnglishBinTree));
    info.englishWord->englishWord = malloc(strlen(englishWord) + 1);
    strcpy(info.englishWord->englishWord, englishWord);

    info.englishWord->units = NULL;
    InsertLinkedList(&(info.englishWord->units), unit);

    return info;
}

int insertPortugueseWord(Portuguese23Tree **tree, char *portugueseWord, char *englishWord, char *unit)
{
    TranslationInfo promote;
    Portuguese23Tree *father = NULL;
    int insered;

    TranslationInfo novoInfo = criaInfo(portugueseWord, englishWord, unit);
    if (InsertTree23(tree, &novoInfo, &promote, &father))
        insered = 1;

    return (insered);
}



Portuguese23Tree *criaNo(const TranslationInfo *info, Portuguese23Tree *filholeft, Portuguese23Tree *filhocen)
{
    Portuguese23Tree *no;
    no = (Portuguese23Tree *)malloc(sizeof(Portuguese23Tree)); // allocate memory

    // Preenche info1 com os dados da nova disciplina
    no->info1 = *info;
    no->left = filholeft;
    no->middle = filhocen;
    no->infoCount = 1;

    // Inicializa info2 com nulo
    no->info2.englishWord = NULL;
    no->info2.portugueseWord = NULL;
    no->right = NULL;

    return no;
}

void adicionaChave(Portuguese23Tree **no, const TranslationInfo *info, Portuguese23Tree *filho)
{
    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) > 0)
    {
        // Adiciona a nova informação a info2
        (*no)->info2 = *info;
        (*no)->right = filho; // Ajusta o ponteiro do filho righteito
    }
    else
    {
        // Move info1 para info2 e coloca a nova informação em info1
        (*no)->info2 = (*no)->info1;
        (*no)->info1 = *info;
        (*no)->right = (*no)->middle;
        (*no)->middle = filho;
    }
    (*no)->infoCount = 2; // Atualiza o número de informações
}

Portuguese23Tree *quebraNo(Portuguese23Tree **no, const TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree *filho)
{
    Portuguese23Tree *maior;

    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
    {
        // A nova informação é menor que info1, então info1 será promovida
        *promote = (*no)->info1;
        maior = criaNo(&(*no)->info2, (*no)->middle, (*no)->right);

        // Atualiza info1 com a nova informação
        (*no)->info1 = *info;
        (*no)->middle = filho;
    }
    else if (strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
    {
        // A nova informação é maior que info1 e menor que info2, então ela será promovida
        *promote = *info;
        maior = criaNo(&(*no)->info2, filho, (*no)->right);
    }
    else
    {
        // A nova informação é maior que info1 e info2, então info2 será promovido
        *promote = (*no)->info2;
        maior = criaNo(info, (*no)->right, filho);
    }

    // Limpa info2
    (*no)->info2.englishWord = NULL;
    (*no)->info2.portugueseWord = NULL;

    (*no)->infoCount = 1; // Atualizando a quantidade de informação no nó
    (*no)->right = NULL; // Ajusta o filho righteito

    return maior;
}

int ehFolha(const Portuguese23Tree *no)
{
    int achou = 0;

    if (no->left == NULL)
    {
        achou = 1; // Se não tem filho leftuerdo, é uma folha
    }

    return achou;
}

Portuguese23Tree *InsertTree23(Portuguese23Tree **no, TranslationInfo *info, TranslationInfo *promote, Portuguese23Tree **father)
{
    TranslationInfo promote1;
    Portuguese23Tree *biggerNode = NULL;

    if (*no == NULL)
    {
        // Cria um novo nó caso seja nulo
        *no = criaNo(info, NULL, NULL);
    }
    if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) == 0)
        insertenglishWord(&(*no)->info1.englishWord, info);
    else if ((*no)->infoCount == 2 && strcmp(info->portugueseWord, (*no)->info2.portugueseWord) == 0)
        insertenglishWord(&(*no)->info2.englishWord, info);
    else
    {
        if (ehFolha(*no))
        { // Verifica se é folha
            if ((*no)->infoCount == 1)
            {
                // O nó tem espaço para a nova chave
                adicionaChave(no, info, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                Portuguese23Tree *novo;
                novo = quebraNo(no, info, promote, NULL); // quebra no e sobe a informação
                if (*father == NULL)
                {
                    Portuguese23Tree *newroot;
                    newroot = criaNo(promote, *no, novo); // Cria nova root se necessário
                    *no = newroot;
                }
                else
                {
                    biggerNode = novo; // Ajusta o novo maior nó
                }
            }
        }
        else
        { // Nó não é folha
            // Navega para o filho apropriado
            if (strcmp(info->portugueseWord, (*no)->info1.portugueseWord) < 0)
            {
                biggerNode = InsertTree23(&((*no)->left), info, promote, no); // Insere na subárvore à left
            }
            else if ((*no)->infoCount == 1 || strcmp(info->portugueseWord, (*no)->info2.portugueseWord) < 0)
            {
                biggerNode = InsertTree23(&((*no)->middle), info, promote, no); // Insere na subárvore do middle
            }
            else
            {
                biggerNode = InsertTree23(&((*no)->right), info, promote, no); // Insere na subárvore à right
            }

            // Após inserir, verifica se houve promoção
            if (biggerNode != NULL)
            {
                if ((*no)->infoCount == 1)
                {
                    adicionaChave(no, promote, biggerNode);
                    biggerNode = NULL;
                }
                else
                { // Quando não tem espaço
                    // O nó precisa ser quebrado
                    Portuguese23Tree *novo;
                    novo = quebraNo(no, promote, &promote1, biggerNode); // Quebra o nó e sobe a informação
                    if (*father == NULL)
                    {
                        Portuguese23Tree *newroot;
                        newroot = criaNo(&promote1, *no, novo); // Cria nova root se necessário
                        *no = newroot;
                        biggerNode = NULL;
                    }
                    else
                    {
                        biggerNode = novo; // Ajusta o novo maior nó
                        *promote = promote1;
                    }
                }
            }
        }
    }

    return biggerNode;
}

// ############################################## FUNÇOES PARA EXIBIR ##############################################

void displayTree23(const Portuguese23Tree *root)
{
    if (root != NULL)
    {
        displayTree23(root->left);
        printf("Palavra (PT): %s\n", root->info1.portugueseWord);
        printBinaryTree(root->info1.englishWord);
        printf("\n");
        displayTree23(root->middle);
        // Se houver o segundo elemento (infoCount == 2), exibe o segundo filho
        if (root->infoCount == 2)
        {
            printf("Palavra (PT): %s\n", root->info2.portugueseWord);
            printBinaryTree(root->info2.englishWord);
            printf("\n");
            displayTree23(root->right);
        }
    }
}

void displayWordsForUnit(Portuguese23Tree *tree, char *unit)
{
    if (tree)
    {
        displayWordsForUnit(tree->left, unit);
        displayTranslacions(tree->info1.englishWord, unit, tree->info1.portugueseWord);

        displayWordsForUnit(tree->middle, unit);
        if (tree->infoCount == 2)
        {
            displayTranslacions(tree->info2.englishWord, unit, tree->info2.portugueseWord);
        }
        displayWordsForUnit(tree->right, unit);
    }
}

void displayTranslacions(EnglishBinTree *node, char *unit, const char *portugueseWord)
{
    if (node)
    {
        if (sourceLinkedList(node->units, unit))
        {
            printf("Palavra em Portugues: %s\n", portugueseWord);
            printf("Palavra em ingles: %s\n", node->englishWord);
        }
        displayTranslacions(node->left, unit, portugueseWord);
        displayTranslacions(node->right, unit, portugueseWord);
    }
}

Portuguese23Tree *BuscarPalavra(Portuguese23Tree **no, const char *portugueseWord)
{
    Portuguese23Tree *inserida = NULL; // Inicializa o retorno como NULL

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
                inserida = BuscarPalavra(&(*no)->left, portugueseWord);
            }
            else if ((*no)->infoCount == 1 || strcmp(portugueseWord, (*no)->info2.portugueseWord) < 0)
            {
                inserida = BuscarPalavra(&(*no)->middle, portugueseWord);
            }
            else
            {
                inserida = BuscarPalavra(&(*no)->right, portugueseWord);
            }
        }
    }

    return inserida;
}

void displayTranslationPortuguese(Portuguese23Tree **root, const char *portugueseWord)
{
    Portuguese23Tree *resultado = NULL;
    if (root != NULL)
    {
        resultado = BuscarPalavra(root, portugueseWord);
        if (resultado)
        {
            printf("Traducoes em ingles para a palavra '%s':\n", portugueseWord);

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
            printf("A palavra '%s' nao foi encontrada na tree.\n", portugueseWord);
        }
    }
}

// ############################################# REMOÇÃO ############################################

void no23_desalocar(Portuguese23Tree **no)
{
    free(*no);
    *no = NULL;
}

static TranslationInfo no23_maior_info(Portuguese23Tree *root)
{
    return root->infoCount == 2 ? root->info2 : root->info1;
}

static int eh_info1(Portuguese23Tree no, char *info)
{
    return strcmp(info, no.info1.portugueseWord) == 0;
}

static int eh_info2(Portuguese23Tree no, char *info)
{
    return no.infoCount == 2 && strcmp(info, no.info2.portugueseWord) == 0;
}

static Portuguese23Tree *tree23_buscar_maior_filho(Portuguese23Tree *root, Portuguese23Tree **father, TranslationInfo *maior_valor)
{
    Portuguese23Tree *filho;
    filho = root;

    while (!ehFolha(filho))
    {
        *father = filho;
        if (filho->infoCount == 1)
            filho = filho->middle;
        else
            filho = filho->right;
    }

    if (filho != NULL)
        *maior_valor = no23_maior_info(filho);

    return filho;
}

static Portuguese23Tree *tree23_buscar_menor_filho(Portuguese23Tree *root, Portuguese23Tree **father)
{
    Portuguese23Tree *filho;
    filho = root;

    while (!ehFolha(filho))
    {
        *father = filho;
        filho = filho->left;
    }

    return filho;
}

static int movimento_onda(TranslationInfo saindo, TranslationInfo *entrada, Portuguese23Tree *father, Portuguese23Tree **origem, Portuguese23Tree **root, Portuguese23Tree **maior, int (*funcao_remover)(Portuguese23Tree **, char *, Portuguese23Tree *, Portuguese23Tree **, Portuguese23Tree **))
{
    int removeu = funcao_remover(root, saindo.portugueseWord, father, origem, maior);
    *entrada = saindo;
    return removeu;
}

static Portuguese23Tree *tree23_buscar_father(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!eh_info1(*root, info) && !eh_info2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = tree23_buscar_father(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = tree23_buscar_father(root->middle, info);
            else
                father = tree23_buscar_father(root->right, info);

            if (father == NULL)
                father = root;
        }
    }

    return father;
}

static Portuguese23Tree *tree23_buscar_maior_father(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father = NULL;

    if (root != NULL)
    {
        if (!eh_info1(*root, info) && !eh_info2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = tree23_buscar_maior_father(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = tree23_buscar_maior_father(root->middle, info);
            else
                father = tree23_buscar_maior_father(root->right, info);

            if (father == NULL &&
                ((root->infoCount == 1 && strcmp(root->info1.portugueseWord, info) > 0) ||
                 (root->infoCount == 2 && strcmp(root->info2.portugueseWord, info) > 0)))
                father = root;
        }
    }

    return father;
}

static Portuguese23Tree *tree23_buscar_menor_father_2_infos(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!eh_info1(*root, info) && !eh_info2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = tree23_buscar_menor_father_2_infos(root->left, info);
            else if (root->infoCount == 1 || (strcmp(info, root->info2.portugueseWord) < 0))
                father = tree23_buscar_menor_father_2_infos(root->middle, info);
            else
                father = tree23_buscar_menor_father_2_infos(root->right, info);

            if (father == NULL && root->infoCount == 2 && strcmp(root->info2.portugueseWord, info) < 0)
                father = root;
        }
    }

    return father;
}

static Portuguese23Tree *tree23_buscar_menor_father(Portuguese23Tree *root, char *info)
{
    Portuguese23Tree *father;
    father = NULL;

    if (root != NULL)
    {
        if (!eh_info1(*root, info) && !eh_info2(*root, info))
        {
            if (strcmp(info, root->info1.portugueseWord) < 0)
                father = tree23_buscar_menor_father(root->left, info);
            else if (root->infoCount == 1 || strcmp(info, root->info2.portugueseWord) < 0)
                father = tree23_buscar_menor_father(root->middle, info);
            else
                father = tree23_buscar_menor_father(root->right, info);

            if (father == NULL && strcmp(root->info1.portugueseWord, info) < 0)
                father = root;
        }
    }

    return father;
}

int tree23_remover_nao_folha1(Portuguese23Tree **origem, Portuguese23Tree *root, TranslationInfo *info, Portuguese23Tree *filho1, Portuguese23Tree *filho2, Portuguese23Tree **maior)
{
    int removeu;
    Portuguese23Tree *filho, *father;
    TranslationInfo info_filho;

    father = root;

    filho = tree23_buscar_maior_filho(filho1, &father, &info_filho);

    if (filho->infoCount == 2)
    {
        *info = info_filho;
        filho->infoCount = 1;
        removeu = 1;
    }
    else
    {
        filho = tree23_buscar_menor_filho(filho2, &father);
        removeu = movimento_onda(filho->info1, info, father, origem, &filho, maior, tree23Remove1);
    }

    return removeu;
}

int tree23_remover_nao_folha2(Portuguese23Tree **origem, Portuguese23Tree *root, TranslationInfo *info, Portuguese23Tree *filho1, Portuguese23Tree *filho2, Portuguese23Tree **maior)
{
    int removeu;
    Portuguese23Tree *filho, *father;
    TranslationInfo info_filho;

    father = root;

    filho = tree23_buscar_menor_filho(filho1, &father);

    if (filho->infoCount == 2)
    {
        *info = filho->info1;
        filho->info1 = filho->info2;
        filho->infoCount = 1;
        removeu = 1;
    }
    else
    {
        filho = tree23_buscar_maior_filho(filho2, &father, &info_filho);
        removeu = movimento_onda(info_filho, info, father, origem, &filho, maior, tree23Remove2);
    }

    return removeu;
}

int tree23Remove1(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origem, Portuguese23Tree **maior)
{
    int removeu = 0;

    if (*root != NULL)
    {
        int info1 = eh_info1(**root, info);
        int info2 = eh_info2(**root, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (ehFolha(*root))
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
                        no23_desalocar(root);
                    else
                    {
                        Portuguese23Tree *father_aux;
                        TranslationInfo info_father;
                        if (*root == father->left || (father->infoCount == 2 && *root == father->middle))
                        {
                            father_aux = tree23_buscar_father(*origem, father->info1.portugueseWord);

                            if (*root == father->left)
                                info_father = father->info1;
                            else
                                info_father = father->info2;

                            removeu = movimento_onda(info_father, &((*root)->info1), father_aux, origem, &father, maior, tree23Remove1);
                        }
                        else
                        {
                            father_aux = tree23_buscar_maior_father(*origem, (*root)->info1.portugueseWord);

                            Portuguese23Tree *menor_father;
                            menor_father = tree23_buscar_menor_father_2_infos(*origem, (*root)->info1.portugueseWord);

                            if (father_aux == NULL || (father_aux != father && menor_father != NULL))
                            {
                                *maior = father;
                                (*root)->infoCount = 0;
                                removeu = -1;
                            }
                            else
                            {
                                if (strcmp(father_aux->info1.portugueseWord, (*root)->info1.portugueseWord) > 0)
                                    info_father = father_aux->info1;
                                else
                                    info_father = father_aux->info2;

                                Portuguese23Tree *avo;
                                avo = tree23_buscar_father(*origem, info_father.portugueseWord);
                                removeu = movimento_onda(info_father, &((*root)->info1), avo, origem, &father_aux, maior, tree23Remove1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = tree23_remover_nao_folha1(origem, *root, &((*root)->info2), (*root)->middle, (*root)->right, maior);
            else if (info1)
                removeu = tree23_remover_nao_folha1(origem, *root, &((*root)->info1), (*root)->left, (*root)->middle, maior);
        }
        else
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                removeu = tree23Remove1(&(*root)->left, info, *root, origem, maior);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
                removeu = tree23Remove1(&(*root)->middle, info, *root, origem, maior);
            else
                removeu = tree23Remove1(&(*root)->right, info, *root, origem, maior);
        }
    }
    return removeu;
}

int tree23Remove2(Portuguese23Tree **root, char *info, Portuguese23Tree *father, Portuguese23Tree **origem, Portuguese23Tree **maior)
{
    int removeu = 0;

    if (*root != NULL)
    {
        int info1 = eh_info1(**root, info);
        int info2 = eh_info2(**root, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (ehFolha(*root))
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
                        no23_desalocar(root);
                    else
                    {
                        Portuguese23Tree *father_aux;
                        TranslationInfo info_father;
                        if (*root == father->middle || (father->infoCount == 2 && *root == father->right))
                        {
                            father_aux = tree23_buscar_father(*origem, father->info1.portugueseWord);

                            if (*root == father->middle)
                                info_father = father->info1;
                            else
                                info_father = father->info2;

                            removeu = movimento_onda(info_father, &((*root)->info1), father_aux, origem, &father, maior, tree23Remove2);
                        }
                        else
                        {
                            father_aux = tree23_buscar_menor_father(*origem, (*root)->info1.portugueseWord);

                            Portuguese23Tree *menor_father;
                            menor_father = tree23_buscar_menor_father_2_infos(*origem, (*root)->info1.portugueseWord);

                            Portuguese23Tree *avo;
                            if (father_aux == NULL || (father_aux != father && menor_father != NULL))
                            {
                                removeu = -1;
                                *maior = father;
                            }
                            else
                            {
                                if (father_aux->infoCount == 2 && father_aux->info2.portugueseWord < (*root)->info1.portugueseWord)
                                    info_father = father_aux->info2;
                                else
                                    info_father = father_aux->info1;

                                avo = tree23_buscar_father(*origem, info_father.portugueseWord);
                                removeu = movimento_onda(info_father, &((*root)->info1), avo, origem, &father_aux, maior, tree23Remove2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = tree23_remover_nao_folha2(origem, *root, &((*root)->info2), (*root)->right, (*root)->middle, maior);
            else if (info1)
                removeu = tree23_remover_nao_folha2(origem, *root, &((*root)->info1), (*root)->middle, (*root)->left, maior);
        }
        else
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                removeu = tree23Remove2(&(*root)->left, info, *root, origem, maior);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
                removeu = tree23Remove2(&(*root)->middle, info, *root, origem, maior);
            else
                removeu = tree23Remove2(&(*root)->right, info, *root, origem, maior);
        }
    }
    return removeu;
}

int possivel_remover(Portuguese23Tree *root)
{
    int possivel = 0;

    if (root != NULL)
    {
        possivel = root->infoCount == 2;

        if (!possivel)
        {
            possivel = possivel_remover(root->middle);

            if (!possivel)
                possivel = possivel_remover(root->left);
        }
    }

    return possivel;
}

Portuguese23Tree *no23_juntar(Portuguese23Tree *filho1, TranslationInfo info, Portuguese23Tree *maior, Portuguese23Tree **root)
{
    adicionaChave(&filho1, &info, maior);

    (*root)->infoCount--;

    if ((*root)->infoCount == 0)
        no23_desalocar(root);

    return filho1;
}

static int balanceamento(Portuguese23Tree **root, Portuguese23Tree *filho1, Portuguese23Tree **filho2, TranslationInfo info, Portuguese23Tree **maior)
{
    int balanceou = 0;
    if (*filho2 == NULL || (*filho2)->infoCount == 0)
    {
        if (*filho2 != NULL)
            no23_desalocar(filho2);

        *maior = no23_juntar(filho1, info, *maior, root);
        balanceou = 1;
    }
    return balanceou;
}

int tree23_rebalancear(Portuguese23Tree **root, char *info, Portuguese23Tree **maior)
{
    int balanceou = 0;
    if (*root != NULL)
    {
        if (!ehFolha(*root))
        {
            if (strcmp(info, (*root)->info1.portugueseWord) < 0)
                balanceou = tree23_rebalancear(&((*root)->left), info, maior);
            else if ((*root)->infoCount == 1 || (strcmp(info, (*root)->info2.portugueseWord) < 0))
            {
                if ((*root)->left->infoCount == 2 && !possivel_remover((*root)->middle))
                    balanceou = -1;
                else
                    balanceou = tree23_rebalancear(&((*root)->middle), info, maior);
            }
            else
            {
                if ((*root)->middle->infoCount == 2 && !possivel_remover((*root)->right))
                    balanceou = -1;
                else
                    balanceou = tree23_rebalancear(&((*root)->right), info, maior);
            }

            if (balanceou != -1)
            {
                if ((*root)->infoCount == 1)
                    balanceou = balanceamento(root, (*root)->left, &((*root)->middle), (*root)->info1, maior);
                else if ((*root)->infoCount == 2)
                    balanceou = balanceamento(root, (*root)->middle, &((*root)->right), (*root)->info2, maior);
                balanceou = 1;
            }
        }
    }

    return balanceou;
}

int tree_2_3_remover(Portuguese23Tree **root, char *info)
{
    Portuguese23Tree *maior, *posicao_juncao;
    int removeu = tree23Remove1(root, info, NULL, root, &posicao_juncao);

    if (removeu == -1)
    {
        TranslationInfo valor_juncao = no23_maior_info(posicao_juncao);
        maior = NULL;
        removeu = tree23_rebalancear(root, valor_juncao.portugueseWord, &maior);

        if (removeu == -1)
        {
            Portuguese23Tree *father, *posicao_juncao2;
            TranslationInfo *entrada;
            father = tree23_buscar_father(*root, valor_juncao.portugueseWord);

            if (eh_info1(*posicao_juncao, valor_juncao.portugueseWord))
                entrada = &(posicao_juncao->middle->info1);
            else
                entrada = &(posicao_juncao->right->info1);

            removeu = movimento_onda(valor_juncao, entrada, father, root, &posicao_juncao, &posicao_juncao2, tree23Remove2);

            if (removeu == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                father = tree23_buscar_father(*root, valor_juncao.portugueseWord);
                removeu = movimento_onda(valor_juncao, &(posicao_juncao2->left->info1), father, root, &posicao_juncao2, &posicao_juncao, tree23Remove1);

                valor_juncao = no23_maior_info(posicao_juncao);
                maior = NULL;
                removeu = tree23_rebalancear(root, valor_juncao.portugueseWord, &maior);
            }
        }

        if (*root == NULL)
            *root = maior;
    }

    return removeu;
}

/*#########################################FREE#######################################################*/

void freeInfo2_3(TranslationInfo *info)
{
    freeBinaryTree(info->englishWord);
    free(info->portugueseWord);
}

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

EnglishBinTree *createNode(const char *englishWord, char *unit)
{
    EnglishBinTree *novoNo = (EnglishBinTree *)malloc(sizeof(EnglishBinTree));
    if (novoNo != NULL)
    {
        novoNo->englishWord = (char *)malloc(strlen(englishWord) + 1);
        strcpy(novoNo->englishWord, englishWord);
        novoNo->units = NULL;
        InsertLinkedList(&(novoNo->units), unit);
        novoNo->left = novoNo->right = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em ingles na tree binaria de busca
int insertenglishWord(EnglishBinTree **root, TranslationInfo *info)
{
    int result = 0;
    if ((*root) == NULL)
    {
        EnglishBinTree *new = createNode(info->englishWord->englishWord, info->englishWord->units->unitName);
        *root = new;
        result = 1;
    }
    else if (strcmp(info->englishWord->englishWord, (*root)->englishWord) < 0)
        result = insertenglishWord(&(*root)->left, info);
    else if (strcmp(info->englishWord->englishWord, (*root)->englishWord) > 0)
        result = insertenglishWord(&(*root)->right, info);
    else
        result = InsertLinkedList(&((*root)->units), info->englishWord->units->unitName);

    return result;
}

void printBinaryTree(EnglishBinTree *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->left); // Percorre a árvore à leftuerda
        // Imprime a tradução de ingles associada à palavra em portugues
        printf("Palavra em Ingles: %s \n", root->englishWord);
        showLinkedList(root->units);
        printBinaryTree(root->right); // Percorre a árvore à righteita
    }
}

int eh_Folha(EnglishBinTree *root)
{
    return (root->left == NULL && root->right == NULL);
}

EnglishBinTree *soUmFilho(EnglishBinTree **root)
{
    EnglishBinTree *aux;
    aux = NULL;

    if ((*root)->right == NULL)
    {
        aux = (*root)->left;
    }
    else if ((*root)->left == NULL)
    {
        aux = (*root)->right;
    }

    return aux;
}

EnglishBinTree *menorFilho(EnglishBinTree *root)
{
    EnglishBinTree *aux;
    aux = root;

    if (root)
    {
        if (root->left)
            aux = menorFilho(root->left);
    }

    return aux;
}

int removerenglishWord(EnglishBinTree **root, const char *palavra)
{
    EnglishBinTree *endFilho = NULL;
    int existe = 0;

    if (*root)
    {
        if (strcmp(palavra, (*root)->englishWord) == 0)
        {
            EnglishBinTree *aux = *root;
            existe = 1;
            if (eh_Folha(*root))
            {
                free(aux);
                *root = NULL;
            }
            else if ((endFilho = soUmFilho(root)) != NULL)
            {
                free(aux);
                *root = endFilho;
            }
            else
            {
                endFilho = menorFilho((*root)->right);
                strcpy((*root)->englishWord, endFilho->englishWord);
                (*root)->units = endFilho->units;

                removerenglishWord(&(*root)->right, endFilho->englishWord);
            }
        }
        else if (strcmp(palavra, (*root)->englishWord) < 0)
        {
            existe = removerenglishWord(&(*root)->left, palavra);
        }
        else
        {
            existe = removerenglishWord(&(*root)->right, palavra);
        }
    }

    return existe;
}

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

void aloca_list_encadeada_unit(LinkedUnitList **list)
{
    *list = (LinkedUnitList *)malloc(sizeof(LinkedUnitList));
    (*list)->unitName = NULL;
    (*list)->next = NULL;
}

void def_list_encadeada_unit(LinkedUnitList *list, const char *unitName)
{
    list->unitName = (char *)malloc((strlen(unitName) + 1) * sizeof(char));
    strcpy(list->unitName, unitName);
}

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

int sourceLinkedList(const LinkedUnitList *list, const char *unitName)
{
    int confirmacion = 0;
    if (list)
    {
        if (strcmp(list->unitName, unitName) == 0)
            confirmacion = 1;
        else
            confirmacion = sourceLinkedList(list->next, unitName);
    }
    return confirmacion;
}

void free_list_encadeada_unit(LinkedUnitList *list)
{
    if (list)
    {
        free_list_encadeada_unit(list->next);
        free(list->unitName);
        free(list);
        list = NULL;
    }
}

int InsertLinkedList(LinkedUnitList **list, char *name)
{
    int confirmacion = 1;
    if (!sourceLinkedList(*list, name))
    {
        LinkedUnitList *new;
        aloca_list_encadeada_unit(&new);
        def_list_encadeada_unit(new, name);
        new->next = *list;
        *list = new;
    }
    else
        confirmacion = 0;

    return confirmacion;
}

int remover_list_encadeada_unit(LinkedUnitList **list, const char *name)
{
    int confirmacion = 0;
    if (*list)
    {
        if (strcmp((*list)->unitName, name) == 0)
        {
            LinkedUnitList *aux = *list;
            *list = (*list)->next;
            free(aux->unitName);
            free(aux);
            confirmacion = 1;
        }
        else
            confirmacion = remover_list_encadeada_unit(&(*list)->next, name);
    }
    return confirmacion;
}

// #######################################################################################

// REMOÇÃO

// #######################################################################################

int removeEnglishFindUnit(Portuguese23Tree **root, const char *palavras_ingles, const char *unit)
{
    int confirm = 1;
    confirm = auxRemoveEnglishFindUnit(*root, palavras_ingles, unit, root);
    return confirm;
}

int remove_unit(EnglishBinTree **root, const char *englishWord, const char *unit)
{
    int confirm = 1;

    if (*root)
    {
        if (strcmp((*root)->englishWord, englishWord) == 0)
        {
            confirm = remover_list_encadeada_unit(&(*root)->units, unit);
            if (!(*root)->units)
                confirm = removerenglishWord(root, englishWord);
        }
        else if (strcmp((*root)->englishWord, englishWord) > 0)
            confirm = remove_unit(&(*root)->left, englishWord, unit);
        else
            confirm = remove_unit(&(*root)->right, englishWord, unit);
    }

    return confirm;
}

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
            confirm = remove_unit(&(root)->info2.englishWord, englishWord, unit);
        }

        confirm = remove_unit(&(root)->info1.englishWord, englishWord, unit);

        if (!(root)->info1.englishWord)
            confirm = tree_2_3_remover(top, root->info1.portugueseWord);

        if ((root)->infoCount == 2 && !(root)->info2.englishWord)
            confirm = tree_2_3_remover(top, root->info2.portugueseWord);
    }

    return confirm;
}

// ##########################################REMOVER A unit DAS PALAVRAS EM PORTUGUÊS##########################################

int removeUnitFromEnglishWord(EnglishBinTree **root, const char *portugueseWord, const char *unit)
{
    int confirm = 0;
    if (*root)
    {
        confirm = removeUnitFromEnglishWord(&(*root)->left, portugueseWord, unit);
        confirm = removeUnitFromEnglishWord(&(*root)->right, portugueseWord, unit) || confirm;
        confirm = remover_list_encadeada_unit(&(*root)->units, unit);

        if (!(*root)->units)
            confirm = removerenglishWord(root, (*root)->englishWord);
    }

    return confirm;
}

int auxRemovePortugueseWordFindUnit(Portuguese23Tree *root, char *portugueseWord, const char *unit, Portuguese23Tree **top)
{
    int confirm = 1;

    if (root)
    {
        if (strcmp((root)->info1.portugueseWord, portugueseWord) == 0)
        {
            confirm = removeUnitFromEnglishWord(&(root)->info1.englishWord, portugueseWord, unit);
            if (!(root)->info1.englishWord)
                confirm = tree_2_3_remover(top, portugueseWord);
        }
        else if ((root)->infoCount == 2 && strcmp((root)->info2.portugueseWord, portugueseWord) == 0)
        {
            confirm = removeUnitFromEnglishWord(&(root)->info2.englishWord, portugueseWord, unit);
            if (!(root)->info2.englishWord)
                confirm = tree_2_3_remover(top, portugueseWord);
        }
        else if (strcmp(portugueseWord, (root)->info1.portugueseWord) < 0)
            confirm = auxRemovePortugueseWordFindUnit((root)->left, portugueseWord, unit, top);
        else if ((root)->infoCount == 1 || strcmp(portugueseWord, (root)->info2.portugueseWord) < 0)
            confirm = auxRemovePortugueseWordFindUnit((root)->middle, portugueseWord, unit, top);
        else
            confirm = auxRemovePortugueseWordFindUnit((root)->right, portugueseWord, unit, top);
    }

    return confirm;
}

int removePortugueseWordFindUnit(Portuguese23Tree **root, char *portugueseWord, const char *unit)
{
    int confirm = 1;
    confirm = auxRemovePortugueseWordFindUnit(*root, portugueseWord, unit, root);
    return confirm;
}