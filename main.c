#include "src/dict.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Dictionary *dict = (Dictionary *)malloc(sizeof(Dictionary));  
    dict->root = NULL;  
    
    exibir_mensagem();
    
    addWord(dict, "Carro", "Car", "Unidade 1");
    
    viewWordsTest(dict->root);
    
    free(dict);

    return 0;
}
