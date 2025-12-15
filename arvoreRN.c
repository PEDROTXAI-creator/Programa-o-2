#include <stdio.h>
#include <stdlib.h>

// Cores para os nós da árvore
typedef enum { VERMELHO, NEGRO } Cor;

// Estrutura do nó da Árvore Rubro-Negra
typedef struct No {
    int valor;
    Cor cor;
    struct No *esquerda;
    struct No *direita;
    struct No *pai;
} No;

// Estrutura da Árvore Rubro-Negra
typedef struct {
    No *raiz;
    No *nulo;  // Nó nulo (folhas)
} ArvoreRN;

// Função para criar um nó nulo (todas as folhas são nulos)
No* criarNoNulo(No *pai) {
    No *no = (No*)malloc(sizeof(No));
    no->valor = 0;
    no->cor = NEGRO;
    no->esquerda = NULL;
    no->direita = NULL;
    no->pai = pai;
    return no;
}

// Função para inicializar a árvore
void inicializarArvore(ArvoreRN *arvore) {
    arvore->nulo = criarNoNulo(NULL);
    arvore->raiz = arvore->nulo;
}

// Função para criar um novo nó
No* criarNo(int valor) {
    No *novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    novoNo->valor = valor;
    novoNo->cor = VERMELHO;  // Novo nó é sempre vermelho
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->pai = NULL;
    return novoNo;
}

// Função para verificar se a árvore está vazia
int arvoreVazia(ArvoreRN *arvore) {
    return arvore->raiz == arvore->nulo;
}

// ROTAÇÕES

// Rotação à esquerda
void rotacaoEsquerda(ArvoreRN *arvore, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    
    if (y->esquerda != arvore->nulo) {
        y->esquerda->pai = x;
    }
    
    y->pai = x->pai;
    
    if (x->pai == arvore->nulo) {
        arvore->raiz = y;
    } else if (x == x->pai->esquerda) {
        x->pai->esquerda = y;
    } else {
        x->pai->direita = y;
    }
    
    y->esquerda = x;
    x->pai = y;
}

// Rotação à direita
void rotacaoDireita(ArvoreRN *arvore, No *y) {
    No *x = y->esquerda;
    y->esquerda = x->direita;
    
    if (x->direita != arvore->nulo) {
        x->direita->pai = y;
    }
    
    x->pai = y->pai;
    
    if (y->pai == arvore->nulo) {
        arvore->raiz = x;
    } else if (y == y->pai->esquerda) {
        y->pai->esquerda = x;
    } else {
        y->pai->direita = x;
    }
    
    x->direita = y;
    y->pai = x;
}

// FUNÇÕES AUXILIARES

// Encontrar o nó com menor valor
No* encontrarMinimo(ArvoreRN *arvore, No *no) {
    while (no->esquerda != arvore->nulo) {
        no = no->esquerda;
    }
    return no;
}

// Encontrar o nó com maior valor
No* encontrarMaximo(ArvoreRN *arvore, No *no) {
    while (no->direita != arvore->nulo) {
        no = no->direita;
    }
    return no;
}

// Buscar um nó na árvore
No* buscarNo(ArvoreRN *arvore, No *no, int valor) {
    if (no == arvore->nulo || valor == no->valor) {
        return no;
    }
    
    if (valor < no->valor) {
        return buscarNo(arvore, no->esquerda, valor);
    } else {
        return buscarNo(arvore, no->direita, valor);
    }
}

// CORREÇÃO DE INSERÇÃO

// Corrigir violações após inserção
void corrigirInsercao(ArvoreRN *arvore, No *k) {
    No *tio;
    
    while (k->pai->cor == VERMELHO) {
        // Caso: Pai é filho esquerdo do avô
        if (k->pai == k->pai->pai->esquerda) {
            tio = k->pai->pai->direita;
            
            // CASO 1: Tio é vermelho
            if (tio->cor == VERMELHO) {
                k->pai->cor = NEGRO;
                tio->cor = NEGRO;
                k->pai->pai->cor = VERMELHO;
                k = k->pai->pai;
            } else {
                // CASO 2: k é filho direito
                if (k == k->pai->direita) {
                    k = k->pai;
                    rotacaoEsquerda(arvore, k);
                }
                
                // CASO 3: k é filho esquerdo
                k->pai->cor = NEGRO;
                k->pai->pai->cor = VERMELHO;
                rotacaoDireita(arvore, k->pai->pai);
            }
        } 
        // Caso: Pai é filho direito do avô (simétrico)
        else {
            tio = k->pai->pai->esquerda;
            
            // CASO 1: Tio é vermelho
            if (tio->cor == VERMELHO) {
                k->pai->cor = NEGRO;
                tio->cor = NEGRO;
                k->pai->pai->cor = VERMELHO;
                k = k->pai->pai;
            } else {
                // CASO 2: k é filho esquerdo
                if (k == k->pai->esquerda) {
                    k = k->pai;
                    rotacaoDireita(arvore, k);
                }
                
                // CASO 3: k é filho direito
                k->pai->cor = NEGRO;
                k->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, k->pai->pai);
            }
        }
        
        if (k == arvore->raiz) {
            break;
        }
    }
    
    arvore->raiz->cor = NEGRO;  // Regra 2: raiz sempre negra
}

// INSERÇÃO

// Inserir um valor na árvore
void inserir(ArvoreRN *arvore, int valor) {
    No *novoNo = criarNo(valor);
    if (novoNo == NULL) return;
    
    // Configurar os ponteiros do novo nó
    novoNo->esquerda = arvore->nulo;
    novoNo->direita = arvore->nulo;
    novoNo->pai = arvore->nulo;
    
    // Inserção como em BST normal
    No *y = arvore->nulo;
    No *x = arvore->raiz;
    
    while (x != arvore->nulo) {
        y = x;
        if (novoNo->valor < x->valor) {
            x = x->esquerda;
        } else {
            x = x->direita;
        }
    }
    
    novoNo->pai = y;
    
    if (y == arvore->nulo) {
        arvore->raiz = novoNo;
    } else if (novoNo->valor < y->valor) {
        y->esquerda = novoNo;
    } else {
        y->direita = novoNo;
    }
    
    // Se novo nó é raiz, apenas colocar como negro e retornar
    if (novoNo->pai == arvore->nulo) {
        novoNo->cor = NEGRO;
        printf("Valor %d inserido (raiz).\n", valor);
        return;
    }
    
    // Se avô é nulo, não precisa corrigir
    if (novoNo->pai->pai == arvore->nulo) {
        printf("Valor %d inserido.\n", valor);
        return;
    }
    
    // Corrigir violações
    corrigirInsercao(arvore, novoNo);
    printf("Valor %d inserido e árvore balanceada.\n", valor);
}

// BUSCA

// Buscar um valor na árvore
int buscar(ArvoreRN *arvore, int valor) {
    No *resultado = buscarNo(arvore, arvore->raiz, valor);
    return resultado != arvore->nulo;
}

// CORREÇÃO DE REMOÇÃO

// Corrigir violações após remoção
void corrigirRemocao(ArvoreRN *arvore, No *x) {
    No *irmao;
    
    while (x != arvore->raiz && x->cor == NEGRO) {
        if (x == x->pai->esquerda) {
            irmao = x->pai->direita;
            
            // CASO 1: Irmão é vermelho
            if (irmao->cor == VERMELHO) {
                irmao->cor = NEGRO;
                x->pai->cor = VERMELHO;
                rotacaoEsquerda(arvore, x->pai);
                irmao = x->pai->direita;
            }
            
            // CASO 2: Ambos os filhos do irmão são negros
            if (irmao->esquerda->cor == NEGRO && irmao->direita->cor == NEGRO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                // CASO 3: Filho esquerdo do irmão é vermelho, direito é negro
                if (irmao->direita->cor == NEGRO) {
                    irmao->esquerda->cor = NEGRO;
                    irmao->cor = VERMELHO;
                    rotacaoDireita(arvore, irmao);
                    irmao = x->pai->direita;
                }
                
                // CASO 4: Filho direito do irmão é vermelho
                irmao->cor = x->pai->cor;
                x->pai->cor = NEGRO;
                irmao->direita->cor = NEGRO;
                rotacaoEsquerda(arvore, x->pai);
                x = arvore->raiz;
            }
        } else {
            // Caso simétrico
            irmao = x->pai->esquerda;
            
            // CASO 1: Irmão é vermelho
            if (irmao->cor == VERMELHO) {
                irmao->cor = NEGRO;
                x->pai->cor = VERMELHO;
                rotacaoDireita(arvore, x->pai);
                irmao = x->pai->esquerda;
            }
            
            // CASO 2: Ambos os filhos do irmão são negros
            if (irmao->direita->cor == NEGRO && irmao->esquerda->cor == NEGRO) {
                irmao->cor = VERMELHO;
                x = x->pai;
            } else {
                // CASO 3: Filho direito do irmão é vermelho, esquerdo é negro
                if (irmao->esquerda->cor == NEGRO) {
                    irmao->direita->cor = NEGRO;
                    irmao->cor = VERMELHO;
                    rotacaoEsquerda(arvore, irmao);
                    irmao = x->pai->esquerda;
                }
                
                // CASO 4: Filho esquerdo do irmão é vermelho
                irmao->cor = x->pai->cor;
                x->pai->cor = NEGRO;
                irmao->esquerda->cor = NEGRO;
                rotacaoDireita(arvore, x->pai);
                x = arvore->raiz;
            }
        }
    }
    
    x->cor = NEGRO;
}

// Substituir um nó por outro na árvore
void transplantar(ArvoreRN *arvore, No *u, No *v) {
    if (u->pai == arvore->nulo) {
        arvore->raiz = v;
    } else if (u == u->pai->esquerda) {
        u->pai->esquerda = v;
    } else {
        u->pai->direita = v;
    }
    v->pai = u->pai;
}

// REMOÇÃO

// Remover um valor da árvore
int remover(ArvoreRN *arvore, int valor) {
    No *z = buscarNo(arvore, arvore->raiz, valor);
    if (z == arvore->nulo) {
        return 0;  // Valor não encontrado
    }
    
    No *y = z;
    No *x;
    Cor corOriginalY = y->cor;
    
    if (z->esquerda == arvore->nulo) {
        x = z->direita;
        transplantar(arvore, z, z->direita);
    } else if (z->direita == arvore->nulo) {
        x = z->esquerda;
        transplantar(arvore, z, z->esquerda);
    } else {
        y = encontrarMinimo(arvore, z->direita);
        corOriginalY = y->cor;
        x = y->direita;
        
        if (y->pai == z) {
            x->pai = y;
        } else {
            transplantar(arvore, y, y->direita);
            y->direita = z->direita;
            y->direita->pai = y;
        }
        
        transplantar(arvore, z, y);
        y->esquerda = z->esquerda;
        y->esquerda->pai = y;
        y->cor = z->cor;
    }
    
    free(z);
    
    if (corOriginalY == NEGRO) {
        corrigirRemocao(arvore, x);
    }
    
    return 1;  // Remoção bem-sucedida
}

// PERCURSOS

// Pré-ordem: Raiz → Esquerda → Direita
void preOrdem(ArvoreRN *arvore, No *no) {
    if (no != arvore->nulo) {
        printf("%d(%s) ", no->valor, no->cor == VERMELHO ? "V" : "N");
        preOrdem(arvore, no->esquerda);
        preOrdem(arvore, no->direita);
    }
}

// Em ordem: Esquerda → Raiz → Direita
void emOrdem(ArvoreRN *arvore, No *no) {
    if (no != arvore->nulo) {
        emOrdem(arvore, no->esquerda);
        printf("%d(%s) ", no->valor, no->cor == VERMELHO ? "V" : "N");
        emOrdem(arvore, no->direita);
    }
}

// Pós-ordem: Esquerda → Direita → Raiz
void posOrdem(ArvoreRN *arvore, No *no) {
    if (no != arvore->nulo) {
        posOrdem(arvore, no->esquerda);
        posOrdem(arvore, no->direita);
        printf("%d(%s) ", no->valor, no->cor == VERMELHO ? "V" : "N");
    }
}

// FUNÇÕES AUXILIARES E MENU

// Função para liberar toda a memória da árvore
void liberarArvore(ArvoreRN *arvore, No *no) {
    if (no != arvore->nulo) {
        liberarArvore(arvore, no->esquerda);
        liberarArvore(arvore, no->direita);
        free(no);
    }
}

// Função para exibir o menu principal
void exibirMenuPrincipal() {
    printf("\n=== ÁRVORE RUBRO-NEGRA ===\n");
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
    ArvoreRN arvore;
    inicializarArvore(&arvore);
    
    int opcao, subOpcao, valor;
    
    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                inserir(&arvore, valor);
                break;
                
            case 2:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                if (buscar(&arvore, valor)) {
                    printf("Valor %d encontrado na árvore.\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;
                
            case 3:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                if (remover(&arvore, valor)) {
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
                            preOrdem(&arvore, arvore.raiz);
                            printf("\n");
                            break;
                        case 2:
                            emOrdem(&arvore, arvore.raiz);
                            printf("\n");
                            break;
                        case 3:
                            posOrdem(&arvore, arvore.raiz);
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
    liberarArvore(&arvore, arvore.raiz);
    free(arvore.nulo);
    printf("Memória liberada. Programa encerrado.\n");
    
    return 0;
}