#ifndef DICT_H
#define DICT_H

typedef struct inglesPortugues{
    char *palavra;
    struct inglesPortugues *dir,*esq;
}InglesPortugues;

typedef struct info{
    char *palavraPortugues;
    int unidade;
    InglesPortugues *traducaoIngles;
}Info;

typedef struct portuguesIngles{
    Info info1,info2;

    int qtdInfo;
    struct portuguesIngles *dir,*cen,*esq;
}PortugesIngles;


PortugesIngles* criaNo(Info info,PortugesIngles *filhoEsq,PortugesIngles *filhoCen);
PortugesIngles* quebraNo(PortugesIngles **no,Info valor,Info *promove,PortugesIngles **filho);
// PortugesIngles* adicionaChave(PortugesIngles *no,Info info,PortugesIngles *filho);
int ehFolha(PortugesIngles *no);
PortugesIngles* inserirPalavraPortugues(PortugesIngles **no, Info info, Info *promove, PortugesIngles **Pai);
void test();

#endif 
