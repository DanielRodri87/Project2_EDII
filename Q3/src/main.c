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
	printf("  [2] Compactar Memória Ocupada\n");
	printf("  [3] Remover bloco de Memória\n");
	printf("  [4] Exibir alocação e memória\n");
	printf("  [5] Sair\n");
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
				removeFromMemory(NULL, &tree, &key);
				break;

			case 2: 
				int remover_inicio_meio, remover_fim_meio;
				remover_fim_meio = mergeNodesMiddle(&tree, &remover_inicio_meio);
				key.start = remover_inicio_meio;
				key.end = remover_fim_meio;
				removeFromMemory(NULL, &tree, &key);
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
		case 3:
			printf("Digite o endereço de início do bloco de memória: ");
			scanf("%d", &key.start);

			printf("Digite o endereço final do bloco de memória: ");
			scanf("%d", &key.end);

			removeFromMemory(NULL, &tree, &key);
			break;
		case 4:
			DisplayInfos(tree);
			break;
		case 5:
			printf("Saindo da aplicação...\n");
			break;
		default:
			printf("Opção inválida\n");
			break;
		}
	} while (control_menu != 5);
}