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

#define TAMANHO_COMANDO 255
#define TAMANHO_BUSCA 5 // Quantidade de elementos na busca de semelhantes

int arvoreVazia(No *raiz);
No* criarNo(char *nome, int ehUmArquivo);
void inserirFilho(No *pai, No *filho);
No* buscarFilho(No *pai, char *nome);
void removerFilho(No *pai, No *filho);
void carregarArquivo(No *raiz, char *arquivo);
void processarLinha(No *raiz, char *linha);
void comandoSearch(No *raiz, char *nome);
No* buscarRecursivo(No *atual, char *nome);
void comandoList(No *atual);
void comandoHelp();
void comandoClear();
void executarComandos(No** raiz, char* comando, int sizeComando);
void liberarNo(No *no);
void liberarArvore(No *raiz);
No* navegarEndereco(No** raiz, char* caminho);

#endif
