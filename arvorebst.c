#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da Árvore Binária de Busca
typedef struct No {
    int valor;
    struct No *esquerda;
    struct No *direita;
} No;

// Estrutura da Árvore
typedef struct {
    No *raiz;
} Arvore;

// Função para criar um novo nó
No* criarNo(int valor) {
    No *novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    novoNo->valor = valor;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

// Função para inicializar a árvore
void inicializarArvore(Arvore *arvore) {
    arvore->raiz = NULL;
}

// Função para verificar se a árvore está vazia
int arvoreVazia(Arvore *arvore) {
    return arvore->raiz == NULL;
}

// 1. FUNÇÃO DE INSERÇÃO
No* inserir(No *raiz, int valor) {
    // Caso base: árvore vazia ou chegou na posição de inserção
    if (raiz == NULL) {
        return criarNo(valor);
    }
    
    // Inserir na subárvore esquerda se valor for menor
    if (valor < raiz->valor) {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    }
    // Inserir na subárvore direita se valor for maior
    // VALORES REPETIDOS SÃO IGNORADOS (não inseridos)
    else if (valor > raiz->valor) {
        raiz->direita = inserir(raiz->direita, valor);
    }
    // Se valor for igual (repetido), não faz nada
    
    return raiz;
}

// 2. FUNÇÃO DE BUSCA
No* buscar(No *raiz, int valor) {
    // Caso base: árvore vazia ou valor encontrado
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }
    
    // Buscar na subárvore esquerda se valor for menor
    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    }
    // Buscar na subárvore direita se valor for maior
    else {
        return buscar(raiz->direita, valor);
    }
}

// Função auxiliar para encontrar o menor valor (sucessor in-order)
No* encontrarMenor(No *raiz) {
    No *atual = raiz;
    // Percorre sempre para a esquerda até encontrar o menor
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// 3. FUNÇÃO DE REMOÇÃO
No* remover(No *raiz, int valor) {
    // Caso base: árvore vazia
    if (raiz == NULL) {
        return raiz;
    }
    
    // Encontrar o nó a ser removido
    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    }
    else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    }
    else {
        // Nó encontrado! Agora tratar os 3 casos de remoção:
        
        // CASO 1: Nó folha ou com apenas um filho
        if (raiz->esquerda == NULL) {
            No *temp = raiz->direita;
            free(raiz);
            return temp;
        }
        else if (raiz->direita == NULL) {
            No *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        
        // CASO 2: Nó com dois filhos
        // Encontrar o sucessor in-order (menor valor da subárvore direita)
        No *temp = encontrarMenor(raiz->direita);
        
        // Copiar o valor do sucessor para este nó
        raiz->valor = temp->valor;
        
        // Remover o sucessor in-order
        raiz->direita = remover(raiz->direita, temp->valor);
    }
    return raiz;
}

// 4. FUNÇÕES DE PERCURSO

// Pré-ordem: Raiz -> Esquerda -> Direita
void preOrdem(No *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor);
        preOrdem(raiz->esquerda);
        preOrdem(raiz->direita);
    }
}

// Em ordem: Esquerda -> Raiz -> Direita
void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d ", raiz->valor);
        emOrdem(raiz->direita);
    }
}

// Pós-ordem: Esquerda -> Direita -> Raiz
void posOrdem(No *raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esquerda);
        posOrdem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

// Função para liberar toda a memória da árvore
void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Função para exibir o menu principal
void exibirMenuPrincipal() {
    printf("\n=== ÁRVORE BINÁRIA DE BUSCA ===\n");
    printf("1 - Inserir valor\n");
    printf("2 - Buscar valor\n");
    printf("3 - Remover valor\n");
    printf("4 - Percorrer árvore\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

// Função para exibir o submenu de percursos
void exibirSubmenuPercursos() {
    printf("\n--- TIPOS DE PERCURSO ---\n");
    printf("1 - Pré-ordem\n");
    printf("2 - Em ordem\n");
    printf("3 - Pós-ordem\n");
    printf("Escolha o tipo de percurso: ");
}

// Função principal
int main() {
    Arvore arvore;
    inicializarArvore(&arvore);
    
    int opcao, subOpcao, valor;
    No *resultadoBusca;
    
    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                arvore.raiz = inserir(arvore.raiz, valor);
                printf("Valor %d inserido na árvore.\n", valor);
                break;
                
            case 2:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                resultadoBusca = buscar(arvore.raiz, valor);
                if (resultadoBusca != NULL) {
                    printf("Valor %d encontrado na árvore.\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;
                
            case 3:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                resultadoBusca = buscar(arvore.raiz, valor);
                if (resultadoBusca != NULL) {
                    arvore.raiz = remover(arvore.raiz, valor);
                    printf("Valor %d removido da árvore.\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;
                
            case 4:
                if (arvoreVazia(&arvore)) {
                    printf("Árvore vazia!\n");
                } else {
                    exibirSubmenuPercursos();
                    scanf("%d", &subOpcao);
                    
                    printf("Percorrendo árvore: ");
                    switch (subOpcao) {
                        case 1:
                            preOrdem(arvore.raiz);
                            printf("\n");
                            break;
                        case 2:
                            emOrdem(arvore.raiz);
                            printf("\n");
                            break;
                        case 3:
                            posOrdem(arvore.raiz);
                            printf("\n");
                            break;
                        default:
                            printf("Opção inválida!\n");
                    }
                }
                break;
                
            case 0:
                printf("Encerrando programa...\n");
                break;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    // Liberar toda a memória alocada
    liberarArvore(arvore.raiz);
    printf("Memória liberada. Programa encerrado.\n");
    
    return 0;
}