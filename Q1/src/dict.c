#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test()
{
    printf("oi mundo");
}

int ehFolha(PortugesIngles *no){
	return no->esq == NULL;
}

PortugesIngles* criaNo(Info info, PortugesIngles *filhoEsq, PortugesIngles *filhoCen){
	PortugesIngles *no;
	no = (PortugesIngles*)malloc(sizeof(PortugesIngles));
	
	no->info1 = info;
	no->esq = filhoEsq;
	no->cen = filhoCen;
	no->qtdInfo = 1;

	return no;
}

PortugesIngles* adicionaChave(PortugesIngles *no,Info info,PortugesIngles *filho){
	
	if(strcmp(info.palavraPortugues, no->info1.palavraPortugues) > 0){
		no->info2 = info;
		no->dir = filho;
	}else{
		no->info2 = no->info1;
		no->dir = no->cen;
		no->info1 = info;
		no->cen = filho;
	}

	no->qtdInfo = 2;
	return no;
}

PortugesIngles* quebraNo(PortugesIngles **no,Info valor,Info *promove,PortugesIngles **filho){
	PortugesIngles *maior;

	if(strcmp(valor.palavraPortugues, (*no)->info2.palavraPortugues) > 0){
		*promove = (*no)->info2;
		maior = criaNo(valor, (*no)->dir, *filho);
	}else if(strcmp(valor.palavraPortugues, (*no)->info1.palavraPortugues) > 0){
		*promove = valor;
		maior = criaNo((*no)->info2,*filho,(*no)->dir);
	}else{
		*promove = (*no)->info1;
		maior = criaNo((*no)->info2,(*no)->cen,(*no)->dir);
		(*no)->info1 = valor;
		(*no)->cen = *filho;
	}

	(*no)->qtdInfo = 1;
	return maior;
}

PortugesIngles* inserirPalavraPortugues(PortugesIngles **no, Info info, Info *promove, PortugesIngles **pai){
	PortugesIngles *maiorNo;
	Info promove1;
	maiorNo = NULL;

	if(*no == NULL){
		*no = criaNo(info,NULL,NULL);
	}else{
		if(ehFolha(*no)){
			if((*no)->qtdInfo == 1){
				*no = adicionaChave(*no, info, NULL);
			}else{
				maiorNo = quebraNo(no, info, promove,NULL);
				if(*pai == NULL){
					*no = criaNo(*promove, *no, maiorNo);
					maiorNo = NULL;
				}
			}
		}else{
			if(strcmp(info.palavraPortugues, (*no)->info1.palavraPortugues) < 0){
				maiorNo = inserirPalavraPortugues(&((*no)->esq), info, promove, no);
			}else{
				if(((*no)->qtdInfo == 1) || (strcmp(info.palavraPortugues, (*no)->info2.palavraPortugues) < 0)){
					maiorNo = inserirPalavraPortugues(&((*no)->cen), info, promove,no);
				}else{
					maiorNo = inserirPalavraPortugues(&((*no)->dir), info, promove,no);
				}
			}
		}

		if(maiorNo){
			if((*no)->qtdInfo == 1){
				*no = adicionaChave(*no, *promove, maiorNo);
				maiorNo = NULL;
			}else{
				maiorNo = quebraNo(no, *promove, &promove1, &maiorNo);
				if(*pai){
					*no = criaNo(promove1, *no,maiorNo);
					maiorNo = NULL;
				}
			}
		}
	}

	return maiorNo;
}