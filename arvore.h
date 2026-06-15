/*
    PRECISO FAZER

criarNo();
inserirFilho();
buscarFilho();
removerFilho();
liberarArvore();

*/

#ifndef ARVORE_H
#define ARVORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura do nó da árvore.
// Cada nó vai poder ser uma pasta ou um arquivo.
// Vai ficar assim: pai -> primeiroFilho -> proximoIrmao

typedef struct NO{
    char nome[100];
    int ehUmArquivo;
    struct NO *pai;
    struct NO *primeiroFilho;
    struct NO *proximoIrmao;
}No;

int arvoreVazia(No *raiz);
No* criarNo(char *nome, int ehUmArquivo);
void inserirFilho(No *pai, No *filho);
No* buscarFilho(No *pai, char *nome);
void removerFilho(No *pai, No *filho);
void liberarNo(No *no);
void liberarArvore(No *raiz);

#endif