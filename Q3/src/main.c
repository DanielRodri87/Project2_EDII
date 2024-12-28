#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src.h"

#define BLOCO 100

void DisplayMenu()
{
	printf("\n===============================================================\n");
	printf("                          MENU PRINCIPAL                      \n");
	printf("===============================================================\n");
	printf("  [1] Ocupar Nós Livres\n");
	printf("  [2] Desocupar Nós\n");
	printf("  [3] Compactar Memória\n");
	printf("  [4] Remover bloco de Memória\n");
	printf("  [5] Exibir alocação e memória\n");
	printf("  [6] Sair\n");
	printf("===============================================================\n");
	printf("Escolha uma opção: ");
}

int main()
{
	Memory *tree = NULL;
	Info key;

	int start = 0, end = 0, status = 2, flag = 0, requiridSpace, control_menu = 0, sub_menu = 0;
	char opc;
	srand(time(NULL));

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

	do
	{
		DisplayMenu();
		scanf("%d", &control_menu);

		switch (control_menu)
		{
		case 1:
			printf("Informe quanto espaço é necessário: ");
			scanf("%d", &requiridSpace);
			AllocateSpace(&tree, requiridSpace);
			break;
		case 2:
			printf("Informe o inicio da memória: ");
			scanf("%d", &start);
			printf("Informe o fim da memória: ");
			scanf("%d", &end);
			FreeSpace(tree, start, end);
			break;
		case 3:
			printf("  [1] Início\n");
			printf("  [2] Meio\n");
			printf("  [3] Fim\n");
			printf("  [4] Cancelar\n");
			printf("Digite a opção desejada: ");
			scanf("%d", &sub_menu);

			switch (sub_menu)
			{
			case 1:
				int remover_inicio, remover_fim;
				remover_fim = mergeNodesStart(&tree, &remover_inicio);
				key.start = remover_inicio;
				key.end = remover_fim;
				printf("Inicio: %d Fim %d\n\n", key.start, key.end);

				removeFromMemory(NULL, &tree, &key);
				break;

			case 2:
				int remover_inicio_meio1, remover_fim_meio1;
				int remover_inicio_meio2, remover_fim_meio2;
				mergeNodesMiddle(&tree, &remover_inicio_meio1, &remover_fim_meio1, &remover_inicio_meio2, &remover_fim_meio2);

				key.start = remover_inicio_meio1;
				key.end = remover_fim_meio1; 
				printf("Inicio: %d Fim %d\n\n", key.start, key.end);
				removeFromMemory(&tree, &tree, &key);

				key.start = remover_inicio_meio2;
				key.end = remover_fim_meio2; 
				removeFromMemory(&tree, &tree, &key);
				printf("Inicio: %d Fim %d\n\n", key.start, key.end);

				break;

			case 3:
				int remover_inicio_fim, remover_fim_fim;
				remover_fim_fim = mergeNodesEnd(&tree, &remover_inicio_fim);
				key.start = remover_inicio_fim;
				key.end = remover_fim_fim;
				removeFromMemory(NULL, &tree, &key);
				break;

			case 4:
				printf("Voltando...\n");
				break;
			default:
				printf("Digite uma opção válida de menu\n");
				break;
			}
			break;

		case 4:
			printf("Digite o endereço de início do bloco de memória: ");
			scanf("%d", &key.start);

			printf("Digite o endereço final do bloco de memória: ");
			scanf("%d", &key.end);

			removeFromMemory(NULL, &tree, &key);
			break;
		case 5:
			DisplayInfos(tree);
			break;
		case 6:
			printf("Saindo da aplicação...\n");
			break;
		default:
			printf("Opção inválida\n");
			break;
		}
	} while (control_menu != 6);
}