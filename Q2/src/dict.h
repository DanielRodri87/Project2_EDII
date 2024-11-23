#ifndef DICT_H
#define DICT_H

typedef struct st_info_ArvoreBinaria
{
  char *palavra_ingles;
  char *nome_unidade;
} InfoArvoreBinaria;

typedef struct st_ArvoreBinaria
{
  InfoArvoreBinaria info;
  struct st_ArvoreBinaria *esquerda;
  struct st_ArvoreBinaria *direita;
} ArvoreBinaria;


typedef enum en_COR
{
  VERMELHO = 0,
  PRETO = 1
} Cor;

typedef struct st_InfoMain
{
  ArvoreBinaria *arv_binaria_palavra_ingles;
  char *palavra_portugues;
} InfoMain;

typedef struct st_ArvoreVermelhoPreto
{
  InfoMain info;

  Cor cor;
  struct st_ArvoreVermelhoPreto *esq;
  struct st_ArvoreVermelhoPreto *dir;
} ArvoreVermelhoPreto;


void aloca_arvore_vermelho_preto(ArvoreVermelhoPreto **raiz);
void free_arvore_vermelho_preto(ArvoreVermelhoPreto **raiz);
void def_arvore_vermelho_preto(ArvoreVermelhoPreto *raiz, char *plv_ingles, char *plv_portugues, char *nome_unidade);
void show_arvore_vermelho_preto(const ArvoreVermelhoPreto *raiz);
int inserir_arvore_vermelho_preto(ArvoreVermelhoPreto **raiz, ArvoreVermelhoPreto *new_node);
int remover_arvore_vermelho_preto(ArvoreVermelhoPreto **raiz, char *plv_ingles);

void aloca_arvore_binaria(ArvoreBinaria **raiz);
void def_arvore_binaria(ArvoreBinaria *raiz, char *palavra_ingles, char *nome_unidade);
void show_arvore_binaria(ArvoreBinaria *raiz);
void free_arvore_binaria(ArvoreBinaria **raiz);
int atualizar_nome_unidade(ArvoreBinaria *raiz, char *nome_unidade);
int insercao_arvore_binaria(ArvoreBinaria **raiz, ArvoreBinaria *new_node);
ArvoreBinaria *so_um_filho(ArvoreBinaria **raiz);
ArvoreBinaria *menor_filho(ArvoreBinaria **raiz);
int remocao_arvore_binaria(ArvoreBinaria **raiz, const char *palavra_ingles);

#endif