#include "arvore.h"
#include <stdio.h>
#include <string.h>

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

void carregarArquivo(No *raiz, char *arquivo){

    FILE *fp = fopen(arquivo, "r");

    if(fp == NULL){
        printf("Erro ao abrir arquivo\n");
        return;
    }

    char linha[256];

    while(fgets(linha, sizeof(linha), fp) != NULL){

        // remover \n
        linha[strcspn(linha, "\n")] = '\0';

        processarLinha(raiz, linha);
    }

    fclose(fp);
}

void processarLinha(No *raiz, char *linha){

    char *token = strtok(linha, "/");

    No *atual = raiz;

    while(token != NULL){

        No *encontrado = buscarFilho(atual, token);

        // se não existe, cria
        if(encontrado == NULL){

            int ehArquivo = (strchr(token, '.') != NULL);

            No *novo = criarNo(token, ehArquivo);

            inserirFilho(atual, novo);

            atual = novo;
        }
        else{
            atual = encontrado;
        }

        token = strtok(NULL, "/");
    }
}

void caminhoNo(No* raiz) {
    if (raiz == NULL) {
        return;
    }
    caminhoNo(raiz -> pai);
    if (raiz -> pai != NULL) { // Precisamos disso pois o nome da raiz é 'Raiz', daí fica estranho
        printf("%s/", raiz -> nome);
    } else {
        printf("/");
    }
}

void comandoMkdir(No**raiz, int ehArquivo) {
    char* argumento = strtok(NULL, "");

    if (argumento == NULL) {
        return;
    }
    char* caminho = (char*) malloc(TAMANHO_COMANDO * sizeof(char));
    char* nomeArquivo = strrchr(argumento, '/');
    No* destino = *raiz;

    if (nomeArquivo == NULL) {
        nomeArquivo = argumento; // Caso for só o nome sem o endereço
    } else {
        nomeArquivo += 1;
        strncpy(caminho, argumento, nomeArquivo - argumento);  // Copiar todo o taminho menos a última barra
        caminho[nomeArquivo - argumento] = '\0'; // Adicionar fim da string para não ler o lixo do malloc
        destino = navegarEndereco(raiz, caminho);
        if (destino == NULL) {
            free(caminho);
            return;
        }
    }

    No* novoNo = criarNo(nomeArquivo, ehArquivo);
    inserirFilho(destino, novoNo);
    free(caminho);
}

void comandoSearch(No *raiz, char *nome){
    No* resultado = buscarRecursivo(raiz, nome);

    if (resultado == NULL) {
        printf("Nao encontrado\n");
        return;
    };

    printf("Encontrado: ");
    caminhoNo(resultado);
    printf("\n");
}

void comandoClear() {
    system("cls"); // Windows
    system("clear"); // Linux
}

void comandoHelp() {
    printf("--------------- Ajuda ---------------\n");
    printf("* cd - Entrar em uma pasta\n");
    printf("* search - Busca uma pasta ou arquivo\n");
    printf("* rm - Remover uma pasta\n");
    printf("* list - Lista os componentes dentro de uma pasta em questão\n");
    printf("* mkdir - Cria uma nova pasta\n");
    printf("* touch - Cria um novo arquivo\n");
    printf("* pwd - Mostra a localização atual\n");
    printf("* whoami - Mostra seu usuario no sistema\n");
    printf("* clear - Limpa o conteúdo da tela\n");
    printf("* help - Exibe a relação completa dos commandos\n");
    printf("* exit - Fechar o programa\n");
    printf("-------------------------------------\n");
}

No** pesquisarFilhos(No* raiz, char* consulta) {
    // Poderiamos usar uma lista linkada, mas seria muito complicado
    No** similares = (No**) malloc(TAMANHO_BUSCA * sizeof(No*));
    int contadorSimilares = 0;
    int tamanhoConsulta;

    for (int i=0; i < TAMANHO_COMANDO; i++) {
        if (consulta[i] == '\0') {
            tamanhoConsulta = i;
            break;
        }
    }

    for (int i=0; i < TAMANHO_BUSCA; i++) {
        similares[i] = NULL;
    }


    No* filho = raiz -> primeiroFilho;
    while (filho != NULL && contadorSimilares < TAMANHO_BUSCA) {
        if (filho == NULL) {
            break;
        }

        int similar = 1;
        for (int j=0; j < tamanhoConsulta; j++) {
            if (filho -> nome[j] != consulta[j]) {
                similar = 0;
            }
        }

        if (similar == 1) {
            similares[contadorSimilares] = filho;
            contadorSimilares++;
        }
        filho = filho -> proximoIrmao;
    }

    return similares;
}

void mostrarSimilares(No **raiz, char* argumento) {
    int existemSimilares = 0;
    No** similares = pesquisarFilhos(*raiz, argumento);
    for (int i=0; i < TAMANHO_BUSCA; i++) {
        if (similares[i] != NULL) {
            existemSimilares = 1;
        }
    }
    if (existemSimilares == 1) {
        printf("Pastas similares: \n");
    }
    for (int i=0; i < TAMANHO_BUSCA; i++) {
        if (similares[i] != NULL) {
            printf(" - %s\n", similares[i] -> nome);
        }
    }
}

No* navegarEndereco(No** raiz, char* caminho) {
    // Caso começar com '/' vamos fazer ele ser absoluto
    if (caminho != NULL && caminho[0] == '/') {
        while ((*raiz) -> pai != NULL) {
            raiz = &((*raiz) -> pai);
        }
        caminho += 1; //Pulamos o /
    }

    char* argumento = strtok(caminho, "/");
    if (argumento == NULL) {// Caso for nulo, verificamos se o argumento está em barra
        argumento = strtok(caminho, "");
    }

    while(caminho != NULL && strcmp(caminho, "") != 0) {
        if (strcmp(caminho, "..") == 0) {
            if ((*raiz) -> pai != NULL) {
                raiz = &((*raiz) -> pai);
            }
        }else if (caminho != NULL) {
            No *filho = buscarFilho(*raiz, caminho);
            if (filho != NULL) {
                raiz = &filho;
            } else {
                printf("Diretório não encontrado\n");
                mostrarSimilares(raiz, caminho);
                return NULL;
            }
        }

        caminho = strtok(NULL, "/");
    }
    return *raiz;
}

// Precisamos de um ponteiro de ponteiro para conseguir alterar o valor da raiz global
void executarComandos(No** raiz, char* comando, int sizeComando) {
    char* token = strtok(comando, " ");

    // Coloquei ls para deixar mais fácil digitar
    if (strcmp(token, "list") == 0 || strcmp(token, "ls") == 0) {
        char* argumento = strtok(NULL, "");
        No* destino = navegarEndereco(raiz, argumento);
        if (destino != NULL) {
            comandoList(destino);
        }
    } else if (strcmp(token, "search") == 0) {
        char* argumento = strtok(NULL, "");
        if (argumento == NULL) {
            return;
        }
        comandoSearch(*raiz, argumento);
    } else if (strcmp(token, "cd") == 0) {
        char* argumento = strtok(NULL, "");
        No* destino = navegarEndereco(raiz, argumento);
        if (destino != NULL) {
            *raiz = destino;
        }
    } else if (strcmp(token, "rm") == 0) {
        char* argumento = strtok(NULL, "");
        if (argumento == NULL) {
            return;
        }

        if (strcmp(argumento, "../") == 0 || strcmp(argumento, "/") == 0) { // Nem pensa nisso
            printf("Eh serio?\n");
            return;
        }

        No* destino = navegarEndereco(raiz, argumento);
        if (destino != NULL) {
            removerFilho(destino -> pai, destino);
            liberarNo(destino);
        }
    } else if (strcmp(token, "mkdir") == 0) {
        comandoMkdir(raiz, 0);
    } else if (strcmp(token, "touch") == 0) {
        comandoMkdir(raiz, 1);
    } else if (strcmp(token, "help") == 0) {
        comandoHelp();
    } else if (strcmp(token, "clear") == 0) {
        comandoClear();
    } else if (strcmp(token, "pwd") == 0) {
        caminhoNo(*raiz);
        printf("\n");
    } else if (strcmp(token, "whoami") == 0) {
        printf("O melhor professor do mundo! S2\n");
    } else if (strcmp(token, "exit") == 0) {
        // Não precisamos fazer nada pois isso vai parar o while que chama essa função
    } else {
        printf("Comando nao encontrado\n");
    }
}

No* buscarRecursivo(No *atual, char *nome){
    if(atual == NULL){
        return NULL;
    }

    if(strcmp(atual->nome, nome) == 0){
        return atual;
    }

    No *filho = atual->primeiroFilho;

    while(filho != NULL){
        No* resultado = buscarRecursivo(filho, nome);
        if (resultado != NULL) {
            return resultado;
        }
        filho = filho->proximoIrmao;
    }
    return NULL;
}

void comandoList(No *atual){

    if(atual == NULL){
        return;
    }

    No *filho = atual->primeiroFilho;

    if(filho == NULL){
        printf("Diretorio vazio\n");
        return;
    }

    while(filho != NULL){

        if(filho->ehUmArquivo)
            printf("[ARQ] %s\n", filho->nome);
        else
            printf("[DIR] %s\n", filho->nome);

        filho = filho->proximoIrmao;
    }
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
