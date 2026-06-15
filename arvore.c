#include "arvore.h"

int arvoreVazia(No *raiz)
{
    if(raiz == NULL)
        return 1;

    return 0;
}

No* criarNo(char *nome, int ehUmArquivo){

    // criar um espacinho na memoria para guardar o nó
    No *novo = (No*) malloc(sizeof(No));

    // memoria deu certo?
    if(novo == NULL){
        printf("\nErro de memoria\n");
        return NULL;
    }

    strcpy(novo->nome, nome);

    novo->ehUmArquivo = ehUmArquivo;

    // quando cria o nó ele ainda nao tem ligação com ninguem
    novo->pai = NULL;
    novo->primeiroFilho = NULL;
    novo->proximoIrmao = NULL;

    return novo;
}


void inserirFilho(No *pai, No *filho){

    // existe pai?
    if(pai == NULL || filho == NULL){
        return;
    }

    // guardar quem é o pai do filho
    filho->pai = pai;

    // se não existe filho ainda esse vira o primeiro
    if(pai->primeiroFilho == NULL){

        pai->primeiroFilho = filho;
        return;
    }

    // achar o ultimo irmão
    No *atual = pai->primeiroFilho;

    while(atual->proximoIrmao != NULL){
        atual = atual->proximoIrmao;
    }

    // liga no finalzinho
    atual->proximoIrmao = filho;
}

// Detalhe o remover ela não da o free() não, então se querem remover e apagar chamem os dois remover e liberar.
void removerFilho(No *pai, No *filho){

    if(pai == NULL || filho == NULL){
        return;
    }

    // caso filho for o primeiro da lista
    if(pai->primeiroFilho == filho){
        pai->primeiroFilho = filho->proximoIrmao;
        filho->proximoIrmao = NULL;
        filho->pai = NULL;
        return;
    }

    // procurar o irmão anterior
    No *anterior = pai->primeiroFilho;

    while(anterior != NULL &&
        anterior->proximoIrmao != filho){
        anterior = anterior->proximoIrmao;
    }

    // encontrou?
    if(anterior != NULL){
        anterior->proximoIrmao = filho->proximoIrmao;
        filho->proximoIrmao = NULL;
        filho->pai = NULL;
    }
}

No* buscarFilho(No *pai, char *nome){

    // pai existe?
    if(pai == NULL){
        return NULL;
    }

    No *atual = pai->primeiroFilho;

    // correr pelos filhos diretos
    while(atual != NULL){
        if(strcmp(atual->nome, nome) == 0){
            return atual;
        }
        atual = atual->proximoIrmao;
    }

    // nao encontrou
    return NULL;
}

void liberarNo(No *no){

    // chegou no final?
    if(no == NULL){
        return;
    }

    // primeiro liberar todos os filhos
    No *filho = no->primeiroFilho;

    while(filho != NULL){

        No *prox = filho->proximoIrmao;

        liberarNo(filho);

        filho = prox;
    }

    // depois libera o proprio nó
    free(no);
}

void liberarArvore(No *raiz){

    if(arvoreVazia(raiz)){
        return;
    }

    liberarNo(raiz);
}