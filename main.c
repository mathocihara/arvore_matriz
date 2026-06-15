/*
    PARA RODAR
gcc main.c arvore.c -o programa
./programa
*/

#include "arvore.h"

int main(){

    No *raiz = criarNo("Raiz", 0);

    No *documentos = criarNo("Documentos", 0);
    No *downloads = criarNo("Downloads", 0);
    inserirFilho(raiz, documentos);
    inserirFilho(raiz, downloads);

    No *teste = buscarFilho(raiz, "Downloads");

    if(teste != NULL){
        printf("Encontrou: %s\n", teste->nome);
    }

    liberarArvore(raiz);

    return 0;
}