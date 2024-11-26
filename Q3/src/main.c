#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "src.h"
#define BLOCO 100

int main()
{
	Memory *tree = NULL;
	int start = 0, end = 0, status = 2;
	int flag = 0;
	char opc;
	srand(time(NULL)); 

	do
	{
		printf("Indique o inicio e o fim:\n");
		printf("Inicio: ");
		scanf("%d", &start);

		printf("Fim:");
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

	Insert45(&tree, NULL,NULL, start, end, status, &flag);
	start = end + 1;

	while (end != BLOCO)
	{
		printf("Informe o valor de fim (entre %d à %d): ", start, BLOCO);
		scanf("%d", &end);

		if (end >= start && end <= BLOCO)
		{
			status = rand() % 2;

			flag = 0;
			Insert45(&tree, NULL, NULL, start, end, status, &flag);
			start = end + 1;
		}
		else
			printf("Valor de fim inválido. Informe um valor entre 1 e %d: \n", BLOCO);
	}

	return 0;
}