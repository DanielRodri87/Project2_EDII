#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test() {
    int wait;
    printf("Hello World");
    scanf("%d", &wait);
}

int isLeaf(PortugueseEnglish *node) {
    return node->left == NULL;
}

PortugueseEnglish* createNode(const Info information, PortugueseEnglish *leftChild, PortugueseEnglish *centerChild) {
    PortugueseEnglish *node = (PortugueseEnglish*)malloc(sizeof(PortugueseEnglish));

    if (!node) {
        printf("Failed to create node\n");
    }

    if (node) {
        node->info1 = information;
        node->left = leftChild;
        node->center = centerChild;
        node->infoCount = 1;
    }

    return node;
}

PortugueseEnglish* addKey(PortugueseEnglish *node, Info info, PortugueseEnglish *child) {
    if (strcmp(info.portugueseWord, node->info1.portugueseWord) > 0) {
        node->info2 = info;
        node->right = child;
    } else {
        node->info2 = node->info1;
        node->right = node->center;
        node->info1 = info;
        node->center = child;
    }

    node->infoCount = 2;
    return node;
}

PortugueseEnglish* splitNode(PortugueseEnglish **node, const Info information, Info *promote, PortugueseEnglish **child) {
    PortugueseEnglish *greater;

    if (strcmp(information.portugueseWord, (*node)->info2.portugueseWord) > 0) {
        *promote = (*node)->info2;
        if (child)
            greater = createNode(information, (*node)->right, *child);
        else
            greater = createNode(information, (*node)->right, NULL);
    } else if (strcmp(information.portugueseWord, (*node)->info1.portugueseWord) > 0) {
        *promote = information;
        if (child)
            greater = createNode((*node)->info2, *child, (*node)->right);
        else
            greater = createNode((*node)->info2, NULL, (*node)->right);
    } else {
        *promote = (*node)->info1;
        greater = createNode((*node)->info2, (*node)->center, (*node)->center);
        (*node)->info1 = information;
        (*node)->center = (child ? *child : NULL);
    }
    (*node)->infoCount = 1;

    return greater;
}

PortugueseEnglish* insertPortugueseWord(PortugueseEnglish **node, Info info, Info *promote, PortugueseEnglish **parent) {
    PortugueseEnglish *greaterNode;
    Info promote1;
    greaterNode = NULL;

    if (*node == NULL) {
        *node = createNode(info, NULL, NULL);
    } else {
        if (isLeaf(*node)) {
            if ((*node)->infoCount == 1) {
                *node = addKey(*node, info, NULL);
            } else {
                greaterNode = splitNode(node, info, promote, NULL);
                if (parent != NULL && *parent == NULL) {
                    *node = createNode(*promote, *node, greaterNode);
                    greaterNode = NULL;
                }
            }
        } else {
            if (strcmp(info.portugueseWord, (*node)->info1.portugueseWord) < 0) {
                greaterNode = insertPortugueseWord(&((*node)->left), info, promote, node);
            } else {
                if (((*node)->infoCount == 1) || (strcmp(info.portugueseWord, (*node)->info2.portugueseWord) < 0)) {
                    greaterNode = insertPortugueseWord(&((*node)->center), info, promote, node);
                } else {
                    greaterNode = insertPortugueseWord(&((*node)->right), info, promote, node);
                }
            }
        }

        if (greaterNode) {
            if ((*node)->infoCount == 1) {
                *node = addKey(*node, *promote, greaterNode);
                greaterNode = NULL;
            } else {
                greaterNode = splitNode(node, *promote, &promote1, &greaterNode);
                if (*parent) {
                    *node = createNode(promote1, *node, greaterNode);
                    greaterNode = NULL;
                }
            }
        }
    }

    return greaterNode;
}

void displayWords(PortugueseEnglish *root) {
    if (root != NULL) {
        if (root->infoCount != 2) {
            printf("\n\n%% Unit %d:\n", root->info1.unit);
            printf("English: %s | Portuguese: %s\n", root->info1.englishTranslation->word, root->info1.portugueseWord);
            displayWords(root->left);
            displayWords(root->center);
        } else {
            printf("English: %s | Portuguese: %s\n", root->info2.englishTranslation->word, root->info2.portugueseWord);
            displayWords(root->right);
        }
    }
}

// PortugueseEnglish eh_info1(PortugueseEnglish node, PortugueseEnglish *info)
// {
//     return info == node.info1.portugueseWord;
// }

// PortugueseEnglish eh_info2(PortugueseEnglish node, int info)
// {
//     return no.n_infos == 2 && info == no.info2.numero;
// }

// int possivel_remover(PortugueseEnglish *raiz)
// {
//     int possivel = 0;

//     if(raiz != NULL)
//     {
//         possivel = raiz->infoCount == 2;

//         if(!possivel)
//         {
//             possivel = possivel_remover(raiz->center);

//             if(!possivel)
//                 possivel = possivel_remover(raiz->left);
//         }
//     }

//     return possivel;
// }

// int arvore23_remover(PortugueseEnglish **raiz, int info, PortugueseEnglish *pai, PortugueseEnglish **origem)
// {
//     int removeu = 0;

//     if(*raiz != NULL)
//     {
//         int info1 = eh_info1(**raiz, info);
//         int info2 = eh_info2(**raiz, info);

//         if(info1 || info2)
//         {
//             removeu = 1;
//             if(eh_folha(**raiz))
//             {
//                 if((*raiz)->infoCount == 2)
//                 {
//                     if(info1)
//                         troca_infos(&((*raiz)->info1), &((*raiz)->info2));

//                     (*raiz)->infoCount = 1;
//                 }
//                 else
//                 {
//                     if(pai != NULL)
//                     {
//                         if(*raiz == pai->left)
//                             movimento_onda(pai->info1, &((*raiz)->info1), NULL, origem, origem);
//                         else
//                         {
//                             if(pai->infoCount == 2)
//                             {
//                                 if(*raiz == pai->center)
//                                     movimento_onda(pai->info2, &((*raiz)->info1), NULL, origem, origem);
//                                 else
//                                     movimento_onda(pai->info2, &(pai->center->info2), NULL, origem, origem);
//                             }
//                             else
//                                 movimento_onda(pai->info1, &(pai->left->info2), NULL, origem, origem);
//                         }
//                     }
//                     else
//                     {
//                         free(*raiz);
//                         *raiz = NULL;
//                     }
//                 }
//             }
//             else
//             {
//                 PortugueseEnglish *pai_aux, *filho;
//                 pai_aux = *raiz;
//                 Info info_aux;
//                 int juntar = 0;

//                 if(info2)
//                 {
//                     if(possivel_remover((*raiz)->right))
//                         filho = buscar_menor_filho((*raiz)->right, &pai_aux, &info_aux);
//                     else if(possivel_remover((*raiz)->center))
//                         filho = buscar_maior_filho((*raiz)->center, &pai_aux, &info_aux);
//                     else
//                     {
//                         no23_juntar((*raiz)->center, (*raiz)->center, &(*raiz)->center);
//                         (*raiz)->infoCount = 1;
//                         juntar = 1;
//                     }

//                     if(!juntar)
//                         movimento_onda(info_aux, &((*raiz)->info2), pai_aux, origem, &filho);
//                 }
//                 else if(info1)
//                 {
//                     if(possivel_remover((*raiz)->left))
//                         filho = buscar_maior_filho((*raiz)->left, &pai_aux, &info_aux);
//                     else if(possivel_remover((*raiz)->center))
//                         filho = buscar_menor_filho((*raiz)->center, &pai_aux, &info_aux);
//                     else if((*raiz)->infoCount == 1)
//                     {
//                         if(pai != NULL)
//                         {
//                             if(*raiz == pai->left || (pai->infoCount == 2 && (*raiz == pai->center)))
//                             {
//                                 filho = buscar_menor_filho((*raiz)->center, &pai_aux, &info_aux);
//                                 pai_aux = buscar_pai(*origem, pai->info1.portugueseWord);

//                                 if(*raiz == pai->left)
//                                     movimento_onda(pai->info1, &(filho->info2), pai_aux, origem, origem);
//                                 else
//                                     movimento_onda(pai->info2, &(filho->info2), pai_aux, origem, origem);
//                             }
//                             else
//                             {
//                                 filho = buscar_maior_filho((*raiz)->left, &pai_aux, &info_aux);
//                                 pai_aux = buscar_pai(*origem, pai->info1.portugueseWord);

//                                 filho->info2 = filho->info1;
//                                 if(pai->infoCount == 2 && (*raiz == pai->right))
//                                     movimento_onda(pai->info2, &(filho->info1), pai_aux, origem, origem);
//                                 else
//                                     movimento_onda(pai->info1, &(filho->info1), pai_aux, origem, origem);
//                             }
//                         }
//                         else
//                         {
//                             PortugueseEnglish *aux;
//                             aux = *raiz;

//                             no23_juntar((*raiz)->left, (*raiz)->center, raiz);
//                             juntar = 1;
                            
//                             no23_desalocar(&aux);
//                         }
//                     }

//                     if(pai != NULL && !juntar)
//                         movimento_onda(info_aux, &((*raiz)->info1), pai_aux, origem, &filho);
//                 }
//             }
//         }
//         else
//         {
//             if(info < (*raiz)->info1.portugueseWord)
//                 removeu = arvore23_remover(&(*raiz)->left, info, *raiz, origem);
//             else if((*raiz)->infoCount == 1 || info < (*raiz)->info2.portugueseWord)
//                 removeu = arvore23_remover(&(*raiz)->center, info, *raiz, origem);
//             else
//                 removeu = arvore23_remover(&(*raiz)->right, info, *raiz, origem);
//         }
//     }

//     return removeu;
// }