/*
    PARA RODAR
gcc main.c arvore.c -o programa
./programa
*/

#include "arvore.h"

int main(){

    No *raiz = criarNo("Raiz", 0);
    char *comando = (char*) malloc(TAMANHO_COMANDO * sizeof(char));

    carregarArquivo(raiz, "in.txt");
    comandoHelp(); // Mostrar a ajuda no começo do programa

    while (strcmp(comando, "exit") != 0) {
        printf("Thiago@projeto> ");
        scanf(" %[^\n]", comando);
        executarComandos(&raiz, comando, TAMANHO_COMANDO);
    }

    liberarArvore(raiz);
    return 0;
}
