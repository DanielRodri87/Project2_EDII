#ifndef DICT_H
#define DICT_H

typedef struct listaEncadeadaUnidade
{
  char *nome_unidade;
  struct listaEncadeadaUnidade *prox;
} ListaEncadeadaUnidade;

typedef struct Inglesbin{
    ListaEncadeadaUnidade *unidades;
    char *palavraIngles;
    struct Inglesbin *dir,*esq;
} Inglesbin;

typedef struct info {
    char *palavraPortugues;
    Inglesbin *palavraIngles;
} Info;

typedef struct Portugues23{
    Info info1;
    Info info2;
    struct Portugues23 *cent;
    struct Portugues23 *dir;
    struct Portugues23 *esq;
    int nInfos;
} Portugues23;


// Main.c
int inserirPalavraPortugues(Portugues23 **arvore, char *palavraPortugues, char *palavraIngles, char *unidade);
void imprimirPalavrasUnidade(Portugues23 *arvore, char *unidade);
void exibir_traducao_Portugues(Portugues23 **Raiz, const char *palavraPortugues);
int Remove_palavra_ingles_unidade(Portugues23 **raiz, const char *palavras_ingles, const char *unidade);
int Remove_palavra_portugues_unidade(Portugues23 **raiz, char *palavraPortugues, const char *unidade);
void exibir_tree23(const Portugues23 *Raiz);
void freeTree(Portugues23 *no);

// Auxiliares
int insertpalavraIngles(Inglesbin **root, Info *informacao);
Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **Pai);
int inserir_lista_encadeada_unidade(ListaEncadeadaUnidade **lista, char *nome);
void printBinaryTree(Inglesbin *root);
void imprimirTraducoes(Inglesbin *node, char *unidade, const char *palavraPortugues);
int buscar_lista_encadeada_unidade(const ListaEncadeadaUnidade *lista, const char *nome_unidade);
void show_lista_encadeada_unidade(const ListaEncadeadaUnidade *lista);
void free_arvore_binaria(Inglesbin *raiz);
int remove_palavra_ingles_unidade(Portugues23 *raiz, const char *palavraIngles, const char *unidade, Portugues23 **top);
int arvore23_remover1(Portugues23 **raiz, char *info, Portugues23 *pai, Portugues23 **origem, Portugues23 **maior);
int arvore23_remover2(Portugues23 **raiz, char *info, Portugues23 *pai, Portugues23 **origem, Portugues23 **maior);

#endif
