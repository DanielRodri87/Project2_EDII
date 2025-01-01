#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #######################################################################################

// 2-3

// #######################################################################################
Info criaInfo(char *palavra, char *palavraIngles, char *unidade)
{
    Info info;

    // Aloca e copia a palavra em portugues
    info.palavraPortugues = malloc(strlen(palavra) + 1);
    strcpy(info.palavraPortugues, palavra);

    // Estrutura de palavraIngles
    info.palavraIngles = malloc(sizeof(Inglesbin));
    info.palavraIngles->palavraIngles = malloc(strlen(palavraIngles) + 1);
    strcpy(info.palavraIngles->palavraIngles, palavraIngles);

    info.palavraIngles->unidades = NULL;
    inserir_lista_encadeada_unidade(&(info.palavraIngles->unidades), unidade);

    return info;
}

int inserirPalavraPortugues(Portugues23 **arvore, char *palavraPortugues, char *palavraIngles, char *unidade)
{
    Info promove;
    Portugues23 *Pai = NULL;
    int inseriu;

    Info novoInfo = criaInfo(palavraPortugues, palavraIngles, unidade);
    if (inserirArv23(arvore, &novoInfo, &promove, &Pai))
        inseriu = 1;

    return inseriu;
}



Portugues23 *criaNo(const Info *informacao, Portugues23 *filhoesq, Portugues23 *filhocen)
{
    Portugues23 *no;
    no = (Portugues23 *)malloc(sizeof(Portugues23)); // allocate memory

    // Preenche info1 com os dados da nova disciplina
    no->info1 = *informacao;
    no->esq = filhoesq;
    no->cent = filhocen;
    no->nInfos = 1;

    // Inicializa info2 com nulo
    no->info2.palavraIngles = NULL;
    no->info2.palavraPortugues = NULL;
    no->dir = NULL;

    return no;
}

void adicionaChave(Portugues23 **no, const Info *informacao, Portugues23 *filho)
{
    if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) > 0)
    {
        // Adiciona a nova informação a info2
        (*no)->info2 = *informacao;
        (*no)->dir = filho; // Ajusta o ponteiro do filho direito
    }
    else
    {
        // Move info1 para info2 e coloca a nova informação em info1
        (*no)->info2 = (*no)->info1;
        (*no)->info1 = *informacao;
        (*no)->dir = (*no)->cent;
        (*no)->cent = filho;
    }
    (*no)->nInfos = 2; // Atualiza o número de informações
}

Portugues23 *quebraNo(Portugues23 **no, const Info *informacao, Info *promove, Portugues23 *filho)
{
    Portugues23 *maior;

    if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0)
    {
        // A nova informação é menor que info1, então info1 será promovida
        *promove = (*no)->info1;
        maior = criaNo(&(*no)->info2, (*no)->cent, (*no)->dir);

        // Atualiza info1 com a nova informação
        (*no)->info1 = *informacao;
        (*no)->cent = filho;
    }
    else if (strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)
    {
        // A nova informação é maior que info1 e menor que info2, então ela será promovida
        *promove = *informacao;
        maior = criaNo(&(*no)->info2, filho, (*no)->dir);
    }
    else
    {
        // A nova informação é maior que info1 e info2, então info2 será promovido
        *promove = (*no)->info2;
        maior = criaNo(informacao, (*no)->dir, filho);
    }

    // Limpa info2
    (*no)->info2.palavraIngles = NULL;
    (*no)->info2.palavraPortugues = NULL;

    (*no)->nInfos = 1; // Atualizando a quantidade de informação no nó
    (*no)->dir = NULL; // Ajusta o filho direito

    return maior;
}

int ehFolha(const Portugues23 *no)
{
    int achou = 0;

    if (no->esq == NULL)
    {
        achou = 1; // Se não tem filho esquerdo, é uma folha
    }

    return achou;
}

Portugues23 *inserirArv23(Portugues23 **no, Info *informacao, Info *promove, Portugues23 **Pai)
{
    Info promove1;
    Portugues23 *maiorNo = NULL;

    if (*no == NULL)
    {
        // Cria um novo nó caso seja nulo
        *no = criaNo(informacao, NULL, NULL);
    }
    if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) == 0)
        insertpalavraIngles(&(*no)->info1.palavraIngles, informacao);
    else if ((*no)->nInfos == 2 && strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) == 0)
        insertpalavraIngles(&(*no)->info2.palavraIngles, informacao);
    else
    {
        if (ehFolha(*no))
        { // Verifica se é folha
            if ((*no)->nInfos == 1)
            {
                // O nó tem espaço para a nova chave
                adicionaChave(no, informacao, NULL);
            }
            else
            {
                // O nó precisa ser quebrado
                Portugues23 *novo;
                novo = quebraNo(no, informacao, promove, NULL); // quebra no e sobe a informação
                if (*Pai == NULL)
                {
                    Portugues23 *novaRaiz;
                    novaRaiz = criaNo(promove, *no, novo); // Cria nova raiz se necessário
                    *no = novaRaiz;
                }
                else
                {
                    maiorNo = novo; // Ajusta o novo maior nó
                }
            }
        }
        else
        { // Nó não é folha
            // Navega para o filho apropriado
            if (strcmp(informacao->palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                maiorNo = inserirArv23(&((*no)->esq), informacao, promove, no); // Insere na subárvore à esq
            }
            else if ((*no)->nInfos == 1 || strcmp(informacao->palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                maiorNo = inserirArv23(&((*no)->cent), informacao, promove, no); // Insere na subárvore do cent
            }
            else
            {
                maiorNo = inserirArv23(&((*no)->dir), informacao, promove, no); // Insere na subárvore à dir
            }

            // Após inserir, verifica se houve promoção
            if (maiorNo != NULL)
            {
                if ((*no)->nInfos == 1)
                {
                    adicionaChave(no, promove, maiorNo);
                    maiorNo = NULL;
                }
                else
                { // Quando não tem espaço
                    // O nó precisa ser quebrado
                    Portugues23 *novo;
                    novo = quebraNo(no, promove, &promove1, maiorNo); // Quebra o nó e sobe a informação
                    if (*Pai == NULL)
                    {
                        Portugues23 *novaRaiz;
                        novaRaiz = criaNo(&promove1, *no, novo); // Cria nova raiz se necessário
                        *no = novaRaiz;
                        maiorNo = NULL;
                    }
                    else
                    {
                        maiorNo = novo; // Ajusta o novo maior nó
                        *promove = promove1;
                    }
                }
            }
        }
    }

    return maiorNo;
}

// ############################################## FUNÇOES PARA EXIBIR ##############################################

void exibir_tree23(const Portugues23 *Raiz)
{
    if (Raiz != NULL)
    {
        exibir_tree23(Raiz->esq);
        printf("Palavra (PT): %s\n", Raiz->info1.palavraPortugues);
        printBinaryTree(Raiz->info1.palavraIngles);
        printf("\n");
        exibir_tree23(Raiz->cent);
        // Se houver o segundo elemento (nInfos == 2), exibe o segundo filho
        if (Raiz->nInfos == 2)
        {
            printf("Palavra (PT): %s\n", Raiz->info2.palavraPortugues);
            printBinaryTree(Raiz->info2.palavraIngles);
            printf("\n");
            exibir_tree23(Raiz->dir);
        }
    }
}

void imprimirPalavrasUnidade(Portugues23 *arvore, char *unidade)
{
    if (arvore)
    {
        imprimirPalavrasUnidade(arvore->esq, unidade);
        imprimirTraducoes(arvore->info1.palavraIngles, unidade, arvore->info1.palavraPortugues);

        imprimirPalavrasUnidade(arvore->cent, unidade);
        if (arvore->nInfos == 2)
        {
            imprimirTraducoes(arvore->info2.palavraIngles, unidade, arvore->info2.palavraPortugues);
        }
        imprimirPalavrasUnidade(arvore->dir, unidade);
    }
}

void imprimirTraducoes(Inglesbin *node, char *unidade, const char *palavraPortugues)
{
    if (node)
    {
        if (buscar_lista_encadeada_unidade(node->unidades, unidade))
        {
            printf("Palavra em Portugues: %s\n", palavraPortugues);
            printf("Palavra em ingles: %s\n", node->palavraIngles);
        }
        imprimirTraducoes(node->esq, unidade, palavraPortugues);
        imprimirTraducoes(node->dir, unidade, palavraPortugues);
    }
}

Portugues23 *BuscarPalavra(Portugues23 **no, const char *palavraPortugues)
{
    Portugues23 *inserida = NULL; // Inicializa o retorno como NULL

    if (no != NULL && *no != NULL)
    {
        if (strcmp(palavraPortugues, (*no)->info1.palavraPortugues) == 0)
        {
            inserida = (*no); // Palavra encontrada, retorna o nó
        }
        else if ((*no)->nInfos == 2 && strcmp(palavraPortugues, (*no)->info2.palavraPortugues) == 0)
        {
            inserida = (*no);
        }
        else
        {
            // Continua a busca nos filhos
            if (strcmp(palavraPortugues, (*no)->info1.palavraPortugues) < 0)
            {
                inserida = BuscarPalavra(&(*no)->esq, palavraPortugues);
            }
            else if ((*no)->nInfos == 1 || strcmp(palavraPortugues, (*no)->info2.palavraPortugues) < 0)
            {
                inserida = BuscarPalavra(&(*no)->cent, palavraPortugues);
            }
            else
            {
                inserida = BuscarPalavra(&(*no)->dir, palavraPortugues);
            }
        }
    }

    return inserida;
}

void exibir_traducao_Portugues(Portugues23 **Raiz, const char *palavraPortugues)
{
    Portugues23 *resultado = NULL;
    if (Raiz != NULL)
    {
        resultado = BuscarPalavra(Raiz, palavraPortugues);
        if (resultado)
        {
            printf("Traducoes em ingles para a palavra '%s':\n", palavraPortugues);

            if (strcmp(palavraPortugues, resultado->info1.palavraPortugues) == 0)
            {
                printBinaryTree(resultado->info1.palavraIngles);
            }
            else
            {
                printBinaryTree(resultado->info2.palavraIngles);
            }
        }
        else
        {
            printf("A palavra '%s' nao foi encontrada na arvore.\n", palavraPortugues);
        }
    }
}

// ############################################# REMOÇÃO ############################################

void no23_desalocar(Portugues23 **no)
{
    free(*no);
    *no = NULL;
}

static Info no23_maior_info(Portugues23 *raiz)
{
    return raiz->nInfos == 2 ? raiz->info2 : raiz->info1;
}

static int eh_info1(Portugues23 no, char *info)
{
    return strcmp(info, no.info1.palavraPortugues) == 0;
}

static int eh_info2(Portugues23 no, char *info)
{
    return no.nInfos == 2 && strcmp(info, no.info2.palavraPortugues) == 0;
}

static Portugues23 *arvore23_buscar_maior_filho(Portugues23 *raiz, Portugues23 **pai, Info *maior_valor)
{
    Portugues23 *filho;
    filho = raiz;

    while (!ehFolha(filho))
    {
        *pai = filho;
        if (filho->nInfos == 1)
            filho = filho->cent;
        else
            filho = filho->dir;
    }

    if (filho != NULL)
        *maior_valor = no23_maior_info(filho);

    return filho;
}

static Portugues23 *arvore23_buscar_menor_filho(Portugues23 *raiz, Portugues23 **pai)
{
    Portugues23 *filho;
    filho = raiz;

    while (!ehFolha(filho))
    {
        *pai = filho;
        filho = filho->esq;
    }

    return filho;
}

static int movimento_onda(Info saindo, Info *entrada, Portugues23 *pai, Portugues23 **origem, Portugues23 **raiz, Portugues23 **maior, int (*funcao_remover)(Portugues23 **, char *, Portugues23 *, Portugues23 **, Portugues23 **))
{
    int removeu = funcao_remover(raiz, saindo.palavraPortugues, pai, origem, maior);
    *entrada = saindo;
    return removeu;
}

static Portugues23 *arvore23_buscar_pai(Portugues23 *raiz, char *info)
{
    Portugues23 *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (strcmp(info, raiz->info1.palavraPortugues) < 0)
                pai = arvore23_buscar_pai(raiz->esq, info);
            else if (raiz->nInfos == 1 || (strcmp(info, raiz->info2.palavraPortugues) < 0))
                pai = arvore23_buscar_pai(raiz->cent, info);
            else
                pai = arvore23_buscar_pai(raiz->dir, info);

            if (pai == NULL)
                pai = raiz;
        }
    }

    return pai;
}

static Portugues23 *arvore23_buscar_maior_pai(Portugues23 *raiz, char *info)
{
    Portugues23 *pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (strcmp(info, raiz->info1.palavraPortugues) < 0)
                pai = arvore23_buscar_maior_pai(raiz->esq, info);
            else if (raiz->nInfos == 1 || (strcmp(info, raiz->info2.palavraPortugues) < 0))
                pai = arvore23_buscar_maior_pai(raiz->cent, info);
            else
                pai = arvore23_buscar_maior_pai(raiz->dir, info);

            if (pai == NULL &&
                ((raiz->nInfos == 1 && strcmp(raiz->info1.palavraPortugues, info) > 0) ||
                 (raiz->nInfos == 2 && strcmp(raiz->info2.palavraPortugues, info) > 0)))
                pai = raiz;
        }
    }

    return pai;
}

static Portugues23 *arvore23_buscar_menor_pai_2_infos(Portugues23 *raiz, char *info)
{
    Portugues23 *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (strcmp(info, raiz->info1.palavraPortugues) < 0)
                pai = arvore23_buscar_menor_pai_2_infos(raiz->esq, info);
            else if (raiz->nInfos == 1 || (strcmp(info, raiz->info2.palavraPortugues) < 0))
                pai = arvore23_buscar_menor_pai_2_infos(raiz->cent, info);
            else
                pai = arvore23_buscar_menor_pai_2_infos(raiz->dir, info);

            if (pai == NULL && raiz->nInfos == 2 && strcmp(raiz->info2.palavraPortugues, info) < 0)
                pai = raiz;
        }
    }

    return pai;
}

static Portugues23 *arvore23_buscar_menor_pai(Portugues23 *raiz, char *info)
{
    Portugues23 *pai;
    pai = NULL;

    if (raiz != NULL)
    {
        if (!eh_info1(*raiz, info) && !eh_info2(*raiz, info))
        {
            if (strcmp(info, raiz->info1.palavraPortugues) < 0)
                pai = arvore23_buscar_menor_pai(raiz->esq, info);
            else if (raiz->nInfos == 1 || strcmp(info, raiz->info2.palavraPortugues) < 0)
                pai = arvore23_buscar_menor_pai(raiz->cent, info);
            else
                pai = arvore23_buscar_menor_pai(raiz->dir, info);

            if (pai == NULL && strcmp(raiz->info1.palavraPortugues, info) < 0)
                pai = raiz;
        }
    }

    return pai;
}

int arvore23_remover_nao_folha1(Portugues23 **origem, Portugues23 *raiz, Info *info, Portugues23 *filho1, Portugues23 *filho2, Portugues23 **maior)
{
    int removeu;
    Portugues23 *filho, *pai;
    Info info_filho;

    pai = raiz;

    filho = arvore23_buscar_maior_filho(filho1, &pai, &info_filho);

    if (filho->nInfos == 2)
    {
        *info = info_filho;
        filho->nInfos = 1;
        removeu = 1;
    }
    else
    {
        filho = arvore23_buscar_menor_filho(filho2, &pai);
        removeu = movimento_onda(filho->info1, info, pai, origem, &filho, maior, arvore23_remover1);
    }

    return removeu;
}

int arvore23_remover_nao_folha2(Portugues23 **origem, Portugues23 *raiz, Info *info, Portugues23 *filho1, Portugues23 *filho2, Portugues23 **maior)
{
    int removeu;
    Portugues23 *filho, *pai;
    Info info_filho;

    pai = raiz;

    filho = arvore23_buscar_menor_filho(filho1, &pai);

    if (filho->nInfos == 2)
    {
        *info = filho->info1;
        filho->info1 = filho->info2;
        filho->nInfos = 1;
        removeu = 1;
    }
    else
    {
        filho = arvore23_buscar_maior_filho(filho2, &pai, &info_filho);
        removeu = movimento_onda(info_filho, info, pai, origem, &filho, maior, arvore23_remover2);
    }

    return removeu;
}

int arvore23_remover1(Portugues23 **raiz, char *info, Portugues23 *pai, Portugues23 **origem, Portugues23 **maior)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (ehFolha(*raiz))
            {
                if ((*raiz)->nInfos == 2)
                {
                    if (info1)
                        (*raiz)->info1 = (*raiz)->info2;
                    (*raiz)->nInfos = 1;
                }
                else
                {
                    if (pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        Portugues23 *pai_aux;
                        Info info_pai;
                        if (*raiz == pai->esq || (pai->nInfos == 2 && *raiz == pai->cent))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.palavraPortugues);

                            if (*raiz == pai->esq)
                                info_pai = pai->info1;
                            else
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover1);
                        }
                        else
                        {
                            pai_aux = arvore23_buscar_maior_pai(*origem, (*raiz)->info1.palavraPortugues);

                            Portugues23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.palavraPortugues);

                            if (pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {
                                *maior = pai;
                                (*raiz)->nInfos = 0;
                                removeu = -1;
                            }
                            else
                            {
                                if (strcmp(pai_aux->info1.palavraPortugues, (*raiz)->info1.palavraPortugues) > 0)
                                    info_pai = pai_aux->info1;
                                else
                                    info_pai = pai_aux->info2;

                                Portugues23 *avo;
                                avo = arvore23_buscar_pai(*origem, info_pai.palavraPortugues);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, arvore23_remover1);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = arvore23_remover_nao_folha1(origem, *raiz, &((*raiz)->info2), (*raiz)->cent, (*raiz)->dir, maior);
            else if (info1)
                removeu = arvore23_remover_nao_folha1(origem, *raiz, &((*raiz)->info1), (*raiz)->esq, (*raiz)->cent, maior);
        }
        else
        {
            if (strcmp(info, (*raiz)->info1.palavraPortugues) < 0)
                removeu = arvore23_remover1(&(*raiz)->esq, info, *raiz, origem, maior);
            else if ((*raiz)->nInfos == 1 || (strcmp(info, (*raiz)->info2.palavraPortugues) < 0))
                removeu = arvore23_remover1(&(*raiz)->cent, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover1(&(*raiz)->dir, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int arvore23_remover2(Portugues23 **raiz, char *info, Portugues23 *pai, Portugues23 **origem, Portugues23 **maior)
{
    int removeu = 0;

    if (*raiz != NULL)
    {
        int info1 = eh_info1(**raiz, info);
        int info2 = eh_info2(**raiz, info);

        if (info1 || info2)
        {
            removeu = 1;
            if (ehFolha(*raiz))
            {
                if ((*raiz)->nInfos == 2)
                {
                    if (info1)
                        (*raiz)->info1 = (*raiz)->info2;

                    (*raiz)->nInfos = 1;
                }
                else
                {
                    if (pai == NULL)
                        no23_desalocar(raiz);
                    else
                    {
                        Portugues23 *pai_aux;
                        Info info_pai;
                        if (*raiz == pai->cent || (pai->nInfos == 2 && *raiz == pai->dir))
                        {
                            pai_aux = arvore23_buscar_pai(*origem, pai->info1.palavraPortugues);

                            if (*raiz == pai->cent)
                                info_pai = pai->info1;
                            else
                                info_pai = pai->info2;

                            removeu = movimento_onda(info_pai, &((*raiz)->info1), pai_aux, origem, &pai, maior, arvore23_remover2);
                        }
                        else
                        {
                            pai_aux = arvore23_buscar_menor_pai(*origem, (*raiz)->info1.palavraPortugues);

                            Portugues23 *menor_pai;
                            menor_pai = arvore23_buscar_menor_pai_2_infos(*origem, (*raiz)->info1.palavraPortugues);

                            Portugues23 *avo;
                            if (pai_aux == NULL || (pai_aux != pai && menor_pai != NULL))
                            {
                                removeu = -1;
                                *maior = pai;
                            }
                            else
                            {
                                if (pai_aux->nInfos == 2 && pai_aux->info2.palavraPortugues < (*raiz)->info1.palavraPortugues)
                                    info_pai = pai_aux->info2;
                                else
                                    info_pai = pai_aux->info1;

                                avo = arvore23_buscar_pai(*origem, info_pai.palavraPortugues);
                                removeu = movimento_onda(info_pai, &((*raiz)->info1), avo, origem, &pai_aux, maior, arvore23_remover2);
                            }
                        }
                    }
                }
            }
            else if (info2)
                removeu = arvore23_remover_nao_folha2(origem, *raiz, &((*raiz)->info2), (*raiz)->dir, (*raiz)->cent, maior);
            else if (info1)
                removeu = arvore23_remover_nao_folha2(origem, *raiz, &((*raiz)->info1), (*raiz)->cent, (*raiz)->esq, maior);
        }
        else
        {
            if (strcmp(info, (*raiz)->info1.palavraPortugues) < 0)
                removeu = arvore23_remover2(&(*raiz)->esq, info, *raiz, origem, maior);
            else if ((*raiz)->nInfos == 1 || (strcmp(info, (*raiz)->info2.palavraPortugues) < 0))
                removeu = arvore23_remover2(&(*raiz)->cent, info, *raiz, origem, maior);
            else
                removeu = arvore23_remover2(&(*raiz)->dir, info, *raiz, origem, maior);
        }
    }
    return removeu;
}

int possivel_remover(Portugues23 *raiz)
{
    int possivel = 0;

    if (raiz != NULL)
    {
        possivel = raiz->nInfos == 2;

        if (!possivel)
        {
            possivel = possivel_remover(raiz->cent);

            if (!possivel)
                possivel = possivel_remover(raiz->esq);
        }
    }

    return possivel;
}

Portugues23 *no23_juntar(Portugues23 *filho1, Info info, Portugues23 *maior, Portugues23 **raiz)
{
    adicionaChave(&filho1, &info, maior);

    (*raiz)->nInfos--;

    if ((*raiz)->nInfos == 0)
        no23_desalocar(raiz);

    return filho1;
}

static int balanceamento(Portugues23 **raiz, Portugues23 *filho1, Portugues23 **filho2, Info info, Portugues23 **maior)
{
    int balanceou = 0;
    if (*filho2 == NULL || (*filho2)->nInfos == 0)
    {
        if (*filho2 != NULL)
            no23_desalocar(filho2);

        *maior = no23_juntar(filho1, info, *maior, raiz);
        balanceou = 1;
    }
    return balanceou;
}

int arvore23_rebalancear(Portugues23 **raiz, char *info, Portugues23 **maior)
{
    int balanceou = 0;
    if (*raiz != NULL)
    {
        if (!ehFolha(*raiz))
        {
            if (strcmp(info, (*raiz)->info1.palavraPortugues) < 0)
                balanceou = arvore23_rebalancear(&((*raiz)->esq), info, maior);
            else if ((*raiz)->nInfos == 1 || (strcmp(info, (*raiz)->info2.palavraPortugues) < 0))
            {
                if ((*raiz)->esq->nInfos == 2 && !possivel_remover((*raiz)->cent))
                    balanceou = -1;
                else
                    balanceou = arvore23_rebalancear(&((*raiz)->cent), info, maior);
            }
            else
            {
                if ((*raiz)->cent->nInfos == 2 && !possivel_remover((*raiz)->dir))
                    balanceou = -1;
                else
                    balanceou = arvore23_rebalancear(&((*raiz)->dir), info, maior);
            }

            if (balanceou != -1)
            {
                if ((*raiz)->nInfos == 1)
                    balanceou = balanceamento(raiz, (*raiz)->esq, &((*raiz)->cent), (*raiz)->info1, maior);
                else if ((*raiz)->nInfos == 2)
                    balanceou = balanceamento(raiz, (*raiz)->cent, &((*raiz)->dir), (*raiz)->info2, maior);
                balanceou = 1;
            }
        }
    }

    return balanceou;
}

int arvore_2_3_remover(Portugues23 **raiz, char *info)
{
    Portugues23 *maior, *posicao_juncao;
    int removeu = arvore23_remover1(raiz, info, NULL, raiz, &posicao_juncao);

    if (removeu == -1)
    {
        Info valor_juncao = no23_maior_info(posicao_juncao);
        maior = NULL;
        removeu = arvore23_rebalancear(raiz, valor_juncao.palavraPortugues, &maior);

        if (removeu == -1)
        {
            Portugues23 *pai, *posicao_juncao2;
            Info *entrada;
            pai = arvore23_buscar_pai(*raiz, valor_juncao.palavraPortugues);

            if (eh_info1(*posicao_juncao, valor_juncao.palavraPortugues))
                entrada = &(posicao_juncao->cent->info1);
            else
                entrada = &(posicao_juncao->dir->info1);

            removeu = movimento_onda(valor_juncao, entrada, pai, raiz, &posicao_juncao, &posicao_juncao2, arvore23_remover2);

            if (removeu == -1)
            {
                valor_juncao = posicao_juncao2->info1;
                pai = arvore23_buscar_pai(*raiz, valor_juncao.palavraPortugues);
                removeu = movimento_onda(valor_juncao, &(posicao_juncao2->esq->info1), pai, raiz, &posicao_juncao2, &posicao_juncao, arvore23_remover1);

                valor_juncao = no23_maior_info(posicao_juncao);
                maior = NULL;
                removeu = arvore23_rebalancear(raiz, valor_juncao.palavraPortugues, &maior);
            }
        }

        if (*raiz == NULL)
            *raiz = maior;
    }

    return removeu;
}

/*#########################################FREE#######################################################*/

void freeInfo2_3(Info *info)
{
    free_arvore_binaria(info->palavraIngles);
    free(info->palavraPortugues);
}

void freeTree(Portugues23 *no)
{
    if (no != NULL)
    {
        freeTree(no->esq);
        freeInfo2_3(&no->info1);
        freeTree(no->cent);
        if (no->nInfos == 2)
        {
            freeInfo2_3(&no->info2);
            freeTree(no->dir);
        }
        free(no);
    }
}

// #######################################################################################

// BINÁRIA

// #######################################################################################

Inglesbin *createNode(const char *palavraIngles, char *unidade)
{
    Inglesbin *novoNo = (Inglesbin *)malloc(sizeof(Inglesbin));
    if (novoNo != NULL)
    {
        novoNo->palavraIngles = (char *)malloc(strlen(palavraIngles) + 1);
        strcpy(novoNo->palavraIngles, palavraIngles);
        novoNo->unidades = NULL;
        inserir_lista_encadeada_unidade(&(novoNo->unidades), unidade);
        novoNo->esq = novoNo->dir = NULL;
    }
    return novoNo;
}

// Função para inserir uma palavra em ingles na arvore binaria de busca
int insertpalavraIngles(Inglesbin **root, Info *informacao)
{
    int result = 0;
    if ((*root) == NULL)
    {
        Inglesbin *new = createNode(informacao->palavraIngles->palavraIngles, informacao->palavraIngles->unidades->nome_unidade);
        *root = new;
        result = 1;
    }
    else if (strcmp(informacao->palavraIngles->palavraIngles, (*root)->palavraIngles) < 0)
        result = insertpalavraIngles(&(*root)->esq, informacao);
    else if (strcmp(informacao->palavraIngles->palavraIngles, (*root)->palavraIngles) > 0)
        result = insertpalavraIngles(&(*root)->dir, informacao);
    else
        result = inserir_lista_encadeada_unidade(&((*root)->unidades), informacao->palavraIngles->unidades->nome_unidade);

    return result;
}

void printBinaryTree(Inglesbin *root)
{
    if (root != NULL)
    {
        printBinaryTree(root->esq); // Percorre a árvore à esquerda
        // Imprime a tradução de ingles associada à palavra em portugues
        printf("Palavra em Ingles: %s \n", root->palavraIngles);
        show_lista_encadeada_unidade(root->unidades);
        printBinaryTree(root->dir); // Percorre a árvore à direita
    }
}

int eh_Folha(Inglesbin *raiz)
{
    return (raiz->esq == NULL && raiz->dir == NULL);
}

Inglesbin *soUmFilho(Inglesbin **raiz)
{
    Inglesbin *aux;
    aux = NULL;

    if ((*raiz)->dir == NULL)
    {
        aux = (*raiz)->esq;
    }
    else if ((*raiz)->esq == NULL)
    {
        aux = (*raiz)->dir;
    }

    return aux;
}

Inglesbin *menorFilho(Inglesbin *raiz)
{
    Inglesbin *aux;
    aux = raiz;

    if (raiz)
    {
        if (raiz->esq)
            aux = menorFilho(raiz->esq);
    }

    return aux;
}

int removerPalavraIngles(Inglesbin **raiz, const char *palavra)
{
    Inglesbin *endFilho = NULL;
    int existe = 0;

    if (*raiz)
    {
        if (strcmp(palavra, (*raiz)->palavraIngles) == 0)
        {
            Inglesbin *aux = *raiz;
            existe = 1;
            if (eh_Folha(*raiz))
            {
                free(aux);
                *raiz = NULL;
            }
            else if ((endFilho = soUmFilho(raiz)) != NULL)
            {
                free(aux);
                *raiz = endFilho;
            }
            else
            {
                endFilho = menorFilho((*raiz)->dir);
                strcpy((*raiz)->palavraIngles, endFilho->palavraIngles);
                (*raiz)->unidades = endFilho->unidades;

                removerPalavraIngles(&(*raiz)->dir, endFilho->palavraIngles);
            }
        }
        else if (strcmp(palavra, (*raiz)->palavraIngles) < 0)
        {
            existe = removerPalavraIngles(&(*raiz)->esq, palavra);
        }
        else
        {
            existe = removerPalavraIngles(&(*raiz)->dir, palavra);
        }
    }

    return existe;
}

void free_arvore_binaria(Inglesbin *raiz)
{
    if (raiz)
    {
        free_arvore_binaria(raiz->esq);
        free_arvore_binaria(raiz->dir);
        free(raiz->palavraIngles);
        free(raiz);
    }
}

// #######################################################################################

// Lista encadeada

// #######################################################################################

void aloca_lista_encadeada_unidade(ListaEncadeadaUnidade **lista)
{
    *lista = (ListaEncadeadaUnidade *)malloc(sizeof(ListaEncadeadaUnidade));
    (*lista)->nome_unidade = NULL;
    (*lista)->prox = NULL;
}

void def_lista_encadeada_unidade(ListaEncadeadaUnidade *lista, const char *nome_unidade)
{
    lista->nome_unidade = (char *)malloc((strlen(nome_unidade) + 1) * sizeof(char));
    strcpy(lista->nome_unidade, nome_unidade);
}

void show_lista_encadeada_unidade(const ListaEncadeadaUnidade *lista)
{
    if (lista)
    {
        if (lista->prox)
            printf("Unidade %s, ", lista->nome_unidade);
        else
            printf("Unidade %s", lista->nome_unidade);
        show_lista_encadeada_unidade(lista->prox);
    }
    else
        printf("\n");
}

int buscar_lista_encadeada_unidade(const ListaEncadeadaUnidade *lista, const char *nome_unidade)
{
    int confirmacao = 0;
    if (lista)
    {
        if (strcmp(lista->nome_unidade, nome_unidade) == 0)
            confirmacao = 1;
        else
            confirmacao = buscar_lista_encadeada_unidade(lista->prox, nome_unidade);
    }
    return confirmacao;
}

void free_lista_encadeada_unidade(ListaEncadeadaUnidade *lista)
{
    if (lista)
    {
        free_lista_encadeada_unidade(lista->prox);
        free(lista->nome_unidade);
        free(lista);
        lista = NULL;
    }
}

int inserir_lista_encadeada_unidade(ListaEncadeadaUnidade **lista, char *nome)
{
    int confirmacao = 1;
    if (!buscar_lista_encadeada_unidade(*lista, nome))
    {
        ListaEncadeadaUnidade *new;
        aloca_lista_encadeada_unidade(&new);
        def_lista_encadeada_unidade(new, nome);
        new->prox = *lista;
        *lista = new;
    }
    else
        confirmacao = 0;

    return confirmacao;
}

int remover_lista_encadeada_unidade(ListaEncadeadaUnidade **lista, const char *nome)
{
    int confirmacao = 0;
    if (*lista)
    {
        if (strcmp((*lista)->nome_unidade, nome) == 0)
        {
            ListaEncadeadaUnidade *aux = *lista;
            *lista = (*lista)->prox;
            free(aux->nome_unidade);
            free(aux);
            confirmacao = 1;
        }
        else
            confirmacao = remover_lista_encadeada_unidade(&(*lista)->prox, nome);
    }
    return confirmacao;
}

// #######################################################################################

// REMOÇÃO

// #######################################################################################

int Remove_palavra_ingles_unidade(Portugues23 **raiz, const char *palavras_ingles, const char *unidade)
{
    int confirm = 1;
    confirm = remove_palavra_ingles_unidade(*raiz, palavras_ingles, unidade, raiz);
    return confirm;
}

int remove_unidade(Inglesbin **raiz, const char *palavraIngles, const char *unidade)
{
    int confirm = 1;

    if (*raiz)
    {
        if (strcmp((*raiz)->palavraIngles, palavraIngles) == 0)
        {
            confirm = remover_lista_encadeada_unidade(&(*raiz)->unidades, unidade);
            if (!(*raiz)->unidades)
                confirm = removerPalavraIngles(raiz, palavraIngles);
        }
        else if (strcmp((*raiz)->palavraIngles, palavraIngles) > 0)
            confirm = remove_unidade(&(*raiz)->esq, palavraIngles, unidade);
        else
            confirm = remove_unidade(&(*raiz)->dir, palavraIngles, unidade);
    }

    return confirm;
}

int remove_palavra_ingles_unidade(Portugues23 *raiz, const char *palavraIngles, const char *unidade, Portugues23 **top)
{
    int confirm = 0;

    if (raiz)
    {
        confirm = remove_palavra_ingles_unidade((raiz)->esq, palavraIngles, unidade, top);
        confirm = remove_palavra_ingles_unidade((raiz)->cent, palavraIngles, unidade, top) || confirm;

        if ((raiz)->nInfos == 2)
        {
            confirm = remove_palavra_ingles_unidade((raiz)->dir, palavraIngles, unidade, top) || confirm;
            confirm = remove_unidade(&(raiz)->info2.palavraIngles, palavraIngles, unidade);
        }

        confirm = remove_unidade(&(raiz)->info1.palavraIngles, palavraIngles, unidade);

        if (!(raiz)->info1.palavraIngles)
            confirm = arvore_2_3_remover(top, raiz->info1.palavraPortugues);

        if ((raiz)->nInfos == 2 && !(raiz)->info2.palavraIngles)
            confirm = arvore_2_3_remover(top, raiz->info2.palavraPortugues);
    }

    return confirm;
}

// ##########################################REMOVER A UNIDADE DAS PALAVRAS EM PORTUGUÊS##########################################

int _remove_palavra_portugues_unidade(Inglesbin **raiz, const char *palavraPortugues, const char *unidade)
{
    int confirm = 0;
    if (*raiz)
    {
        confirm = _remove_palavra_portugues_unidade(&(*raiz)->esq, palavraPortugues, unidade);
        confirm = _remove_palavra_portugues_unidade(&(*raiz)->dir, palavraPortugues, unidade) || confirm;
        confirm = remover_lista_encadeada_unidade(&(*raiz)->unidades, unidade);

        if (!(*raiz)->unidades)
            confirm = removerPalavraIngles(raiz, (*raiz)->palavraIngles);
    }

    return confirm;
}

int remove_palavra_portugues_unidade(Portugues23 *raiz, char *palavraPortugues, const char *unidade, Portugues23 **top)
{
    int confirm = 1;

    if (raiz)
    {
        if (strcmp((raiz)->info1.palavraPortugues, palavraPortugues) == 0)
        {
            confirm = _remove_palavra_portugues_unidade(&(raiz)->info1.palavraIngles, palavraPortugues, unidade);
            if (!(raiz)->info1.palavraIngles)
                confirm = arvore_2_3_remover(top, palavraPortugues);
        }
        else if ((raiz)->nInfos == 2 && strcmp((raiz)->info2.palavraPortugues, palavraPortugues) == 0)
        {
            confirm = _remove_palavra_portugues_unidade(&(raiz)->info2.palavraIngles, palavraPortugues, unidade);
            if (!(raiz)->info2.palavraIngles)
                confirm = arvore_2_3_remover(top, palavraPortugues);
        }
        else if (strcmp(palavraPortugues, (raiz)->info1.palavraPortugues) < 0)
            confirm = remove_palavra_portugues_unidade((raiz)->esq, palavraPortugues, unidade, top);
        else if ((raiz)->nInfos == 1 || strcmp(palavraPortugues, (raiz)->info2.palavraPortugues) < 0)
            confirm = remove_palavra_portugues_unidade((raiz)->cent, palavraPortugues, unidade, top);
        else
            confirm = remove_palavra_portugues_unidade((raiz)->dir, palavraPortugues, unidade, top);
    }

    return confirm;
}

int Remove_palavra_portugues_unidade(Portugues23 **raiz, char *palavraPortugues, const char *unidade)
{
    int confirm = 1;
    confirm = remove_palavra_portugues_unidade(*raiz, palavraPortugues, unidade, raiz);
    return confirm;
}