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

	} while (start < 0 || end > BLOCO);

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

	} while (status != FREE && status != OCCUPIED);

	Insert23(&tree, NULL, NULL, start, end, status, &flag);
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

	printf("Informe quanto espaço é necessário: ");
	scanf("%d", &requiridSpace);
	AllocateSpace(&tree, requiridSpace);

	printf("Depois: Sem Merge \n");
	DisplayInfos(tree);

	int remover_inicio, remover_fim;
	remover_fim = mergeNodes(&tree, &remover_inicio);
	Info key;
	key.start = remover_inicio;
	key.end = remover_fim;
	removeFromMemory(NULL, &tree, &key);

	printf("Depois: Com Merge \n");
	DisplayInfos(tree);

  
	return 0;
}