#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para processar cada linha do arquivo e inserir na estrutura
void processLine(char *line, PortugueseEnglish **root, int *currentUnit) {
    if (line[0] == '%' && strstr(line, "Unidade")) {
        sscanf(line, "%% Unidade %d", currentUnit); // Atualiza a unidade atual
    } else {
        char *token = strtok(line, ":"); // Pega a palavra em inglês
        if (token) {
            char englishWord[100];
            strcpy(englishWord, token);

            token = strtok(NULL, ",;"); // Pega as palavras em português
            while (token) {
                char portugueseWord[100];
                sscanf(token, " %99[^\n]", portugueseWord);

                // Cria a estrutura para armazenar as palavras
                Info info;
                info.unit = *currentUnit;
                info.englishTranslation = (EnglishPortuguese *)malloc(sizeof(EnglishPortuguese));
                info.englishTranslation->word = strdup(englishWord);
                info.englishTranslation->left = info.englishTranslation->right = NULL;
                info.portugueseWord = strdup(portugueseWord);

                Info promote;
                insertPortugueseWord(root, info, &promote, NULL);

                token = strtok(NULL, ",;"); // Continua com a próxima palavra
            }
        }
    }
}

int main() {
    PortugueseEnglish *root = NULL;
    FILE *file = fopen("../../input.txt", "r");

    char line[256];
    int currentUnit = 0;

    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove o caractere de nova linha
        processLine(line, &root, &currentUnit); // Processa cada linha
    }

    fclose(file);

    // Imprime palavras separadas por unidade
    printf("\nPalavras da Unidade 1:\n");
    printWordsByUnit(root, 1);

    printf("\nPalavras da Unidade 2:\n");
    printWordsByUnit(root, 2);

    return 0;
}
