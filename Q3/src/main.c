#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "src.h"

#define BLOCO 100

int main()
{
	Memory *tree = NULL;
	int start = 0, end = 0, status = 2, flag = 0, requiridSpace;
	char opc;
	srand(time(NULL)); 

	/*
		Faça um programa em C que cadastra os nós da árvore, onde o usuário deve informar se o primeiro nó é 
		livre ou ocupado, o endereço inicial e final do nó. Os demais nós serão contabilizados pelo sistema se 
		são livres ou ocupados e o usuário deve apenas informar o endereço final de cada um. O cadastro 
		termina quando o usuário informar como endereço final de um nó o último endereço da memória. 
	*/

	do
	{
		printf("Indique o inicio e o fim:\n");
		printf("Inicio: ");
		scanf("%d", &start);

		printf("Fim: ");
		scanf("%d", &end);

	} while (start < 1 || end > BLOCO);

	do
	{
		printf("O bloco esta livre ou ocupado? (L/O): \n");
		scanf(" %c", &opc);

		switch (opc)
		{
		case 'L':
			status = FREE;
			break;
		case 'O':
			status = OCCUPIED;
			break;
		default:
			printf("Opcao invalida\n");
		}

	} 
	while (status != FREE && status != OCCUPIED);

	Insert23(&tree, NULL,NULL, start, end, status, &flag);
	start = end + 1;

	while (end != BLOCO)
	{
		printf("Informe o valor de fim (entre %d à %d): ", start, BLOCO);
		scanf("%d", &end);

		if (end >= start && end <= BLOCO)
		{
			if (status == FREE)
				status = OCCUPIED;
			else
				status = FREE;

			flag = 0;
			Insert23(&tree, NULL, NULL, start, end, status, &flag);
			start = end + 1;
		}
		else
			printf("Valor de fim inválido. Informe um valor entre 1 e %d: \n", BLOCO);
	}

	printf("Antes: \n");
	DisplayInfos(tree);


	/*
		Faça uma função que o usuário informe a quantidade de nós que ele precisa alocar e retorne as 
		informações do nó que atenda as necessidades do usuário e então modifique a situação do referido nó de 
		Livre para Ocupado.  
			(i) Lembre-se que a árvore deve manter nós intercalados de acordo com a situação do nó, ou 
			seja, se a situação de um nó muda então os nós adjacentes a ele deve ser concatenados.  
			Consequentemente os nós da árvore serão modificados. 

			(ii)Lembre-se  também se o nó escolhido possui uma quantidade maior de blocos do que o 
			solicitado pelo usuário os nós árvore devem ser atualizados de forma que mantenha blocos 
			adjacentes livres ou ocupados em um mesmo nó.  
	*/
	printf("Informe quanto espaço é necessário: ");
	scanf("%d", &requiridSpace);
	AllocateSpace(&tree, requiridSpace);
	// MergeAdjacentBlocks(tree);
	
	printf("Depois: \n");
	DisplayInfos(tree);

	return 0;
}