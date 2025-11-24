#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int valor;
    struct No *anterior;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Lista;

void inicializarLista(Lista *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int listaVazia(Lista *lista) {
    return lista->inicio == NULL;
}

No* criarNo(int valor) {
    No *novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    novoNo->valor = valor;
    novoNo->anterior = NULL;
    novoNo->proximo = NULL;
    return novoNo;
}

void inserirInicio(Lista *lista, int valor) {
    No *novoNo = criarNo(valor);
    if (novoNo == NULL) return;
    
    if (listaVazia(lista)) {
        lista->inicio = novoNo;
        lista->fim = novoNo;
    } else {
        novoNo->proximo = lista->inicio;
        lista->inicio->anterior = novoNo;
        lista->inicio = novoNo;
    }
    lista->tamanho++;
    printf("Valor %d inserido no início da lista.\n", valor);
}

void inserirPosicao(Lista *lista, int valor, int posicao) {
    if (posicao < 1 || posicao > lista->tamanho + 1) {
        printf("Erro: Posicao inválida! A lista tem %d elementos.\n", lista->tamanho);
        return;
    }
    
    if (posicao == 1) {
        inserirInicio(lista, valor);
        return;
    }
    
    if (posicao == lista->tamanho + 1) {
        printf("Use a opcao 'Inserir no final' para esta posicao.\n");
        return;
    }
    
    No *novoNo = criarNo(valor);
    if (novoNo == NULL) return;

    No *atual = lista->inicio;
    for (int i = 1; i < posicao; i++) {
        atual = atual->proximo;
    }

    novoNo->anterior = atual->anterior;
    novoNo->proximo = atual;
    atual->anterior->proximo = novoNo;
    atual->anterior = novoNo;
    
    lista->tamanho++;
    printf("Valor %d inserido na posicao %d.\n", valor, posicao);
}

void inserirFinal(Lista *lista, int valor) {
    No *novoNo = criarNo(valor);
    if (novoNo == NULL) return;
    
    if (listaVazia(lista)) {
        lista->inicio = novoNo;
        lista->fim = novoNo;
    } else {
        novoNo->anterior = lista->fim;
        lista->fim->proximo = novoNo;
        lista->fim = novoNo;
    }
    lista->tamanho++;
    printf("Valor %d inserido no final da lista.\n", valor);
}

void removerPosicao(Lista *lista, int posicao) {
    if (listaVazia(lista)) {
        printf("Erro: Lista vazia! Não é possível remover.\n");
        return;
    }
    
    if (posicao < 1 || posicao > lista->tamanho) {
        printf("Erro: Posicao inválida! A lista tem %d elementos.\n", lista->tamanho);
        return;
    }
    
    No *noRemover;
    
    if (posicao == 1) {
        noRemover = lista->inicio;
        lista->inicio = noRemover->proximo;
        
        if (lista->inicio != NULL) {
            lista->inicio->anterior = NULL;
        } else {
            lista->fim = NULL; 
        }
    } else if (posicao == lista->tamanho) {
        noRemover = lista->fim;
        lista->fim = noRemover->anterior;
        lista->fim->proximo = NULL;
    } else {
        noRemover = lista->inicio;
        for (int i = 1; i < posicao; i++) {
            noRemover = noRemover->proximo;
        }
        
        noRemover->anterior->proximo = noRemover->proximo;
        noRemover->proximo->anterior = noRemover->anterior;
    }
    
    int valorRemovido = noRemover->valor;
    free(noRemover);
    lista->tamanho--;
    
    printf("Valor %d removido da posicao %d.\n", valorRemovido, posicao);
}

int buscarValor(Lista *lista, int valor) {
    if (listaVazia(lista)) {
        printf("Lista vazia! Valor não encontrado.\n");
        return -1;
    }
    
    No *atual = lista->inicio;
    int posicao = 1;
    
    while (atual != NULL) {
        if (atual->valor == valor) {
            printf("Valor %d encontrado na posicao %d.\n", valor, posicao);
            return posicao;
        }
        atual = atual->proximo;
        posicao++;
    }
    
    printf("Valor %d não encontrado na lista.\n", valor);
    return -1;
}

void listarElementos(Lista *lista) {
    if (listaVazia(lista)) {
        printf("Lista vazia!\n");
        return;
    }
    
    printf("\n=== ELEMENTOS DA LISTA ===\n");
    printf("Tamanho: %d\n", lista->tamanho);
    printf("Formato: [Valor] (Anterior -> Atual -> Próximo)\n\n");
    
    No *atual = lista->inicio;
    int posicao = 1;
    
    while (atual != NULL) {
        printf("Posicao %d: [%d] ", posicao, atual->valor);

        if (atual->anterior == NULL) {
            printf("(NULL");
        } else {
            printf("(%d", atual->anterior->valor);
        }
        
        printf(" -> %d -> ", atual->valor);

        if (atual->proximo == NULL) {
            printf("NULL)");
        } else {
            printf("%d)", atual->proximo->valor);
        }
        
        printf("\n");
        atual = atual->proximo;
        posicao++;
    }
    printf("==========================\n");
}

void destruirLista(Lista *lista) {
    No *atual = lista->inicio;
    No *proximo;
    
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
    printf("Lista destruída e memória liberada.\n");
}

void exibirMenu() {
    printf("\n=== LISTA DUPLAMENTE ENCADEADA ===\n");
    printf("1. Inserir no inicio\n");
    printf("2. Inserir em posicao especifica\n");
    printf("3. Inserir no final\n");
    printf("4. Remover de posicao especifica\n");
    printf("5. Buscar valor\n");
    printf("6. Listar elementos\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Lista lista;
    inicializarLista(&lista);
    
    int opcao, valor, posicao;
    
    do {
        exibirMenu();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido no inicio: ");
                scanf("%d", &valor);
                inserirInicio(&lista, valor);
                break;
                
            case 2:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                printf("Digite a posicao (1 a %d): ", lista.tamanho + 1);
                scanf("%d", &posicao);
                inserirPosicao(&lista, valor, posicao);
                break;
                
            case 3:
                printf("Digite o valor a ser inserido no final: ");
                scanf("%d", &valor);
                inserirFinal(&lista, valor);
                break;
                
            case 4:
                printf("Digite a posicao a ser removida (1 a %d): ", lista.tamanho);
                scanf("%d", &posicao);
                removerPosicao(&lista, posicao);
                break;
                
            case 5:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                buscarValor(&lista, valor);
                break;
                
            case 6:
                listarElementos(&lista);
                break;
                
            case 7:
                printf("Encerrando programa...\n");
                break;
                
            default:
                printf("Opcao inválida! Tente novamente.\n");
        }
        
    } while (opcao != 7);
    
    destruirLista(&lista);
    return 0;
}