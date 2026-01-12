#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ===================== ESTRUTURAS DE DADOS =====================

// Estrutura de um nó da Árvore 2-3
typedef struct No23 {
    int chaves[2];          // Pode ter 1 ou 2 chaves
    int numChaves;          // Quantidade atual de chaves (1 ou 2)
    struct No23 *filhos[3]; // Pode ter 2 ou 3 filhos
    struct No23 *pai;       // Ponteiro para o pai
    bool ehFolha;           // Indica se é folha
} No23;

// Estrutura da Árvore 2-3
typedef struct {
    No23 *raiz;
} Arvore23;

// ===================== FUNÇÕES AUXILIARES =====================

// Criar um novo nó
No23* criarNo(int chave1, int chave2, bool ehFolha, No23 *pai) {
    No23 *novoNo = (No23*)malloc(sizeof(No23));
    if (novoNo == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return NULL;
    }
    
    novoNo->chaves[0] = chave1;
    novoNo->chaves[1] = chave2;
    novoNo->numChaves = (chave2 == -1) ? 1 : 2;
    novoNo->ehFolha = ehFolha;
    novoNo->pai = pai;
    
    // Inicializar todos os filhos como NULL
    for (int i = 0; i < 3; i++) {
        novoNo->filhos[i] = NULL;
    }
    
    return novoNo;
}

// Inicializar árvore
void inicializarArvore(Arvore23 *arvore) {
    arvore->raiz = NULL;
}

// Verificar se árvore está vazia
int arvoreVazia(Arvore23 *arvore) {
    return arvore->raiz == NULL;
}

// Encontrar o filho apropriado para uma chave
int encontrarFilho(No23 *no, int chave) {
    if (no->numChaves == 1) {
        if (chave < no->chaves[0]) return 0;
        else return 1;
    } else { // numChaves == 2
        if (chave < no->chaves[0]) return 0;
        else if (chave < no->chaves[1]) return 1;
        else return 2;
    }
}

// Ordenar chaves em um nó (mantém sempre ordenado)
void ordenarChaves(No23 *no) {
    if (no->numChaves == 2 && no->chaves[0] > no->chaves[1]) {
        // Troca as chaves
        int temp = no->chaves[0];
        no->chaves[0] = no->chaves[1];
        no->chaves[1] = temp;
    }
}

// ===================== FUNÇÃO DE BUSCA =====================

// Buscar uma chave na árvore
No23* buscar(No23 *raiz, int chave, int *posicao) {
    if (raiz == NULL) return NULL;
    
    // Verificar se a chave está neste nó
    for (int i = 0; i < raiz->numChaves; i++) {
        if (chave == raiz->chaves[i]) {
            *posicao = i;
            return raiz;
        }
    }
    
    // Se é folha e não encontrou, retorna NULL
    if (raiz->ehFolha) {
        return NULL;
    }
    
    // Encontrar qual filho seguir
    int filho = encontrarFilho(raiz, chave);
    return buscar(raiz->filhos[filho], chave, posicao);
}

// ===================== FUNÇÕES DE INSERÇÃO =====================

// Inserir chave em nó folha (ainda não cheio)
void inserirEmFolha(No23 *folha, int chave) {
    // Adiciona a nova chave
    folha->chaves[folha->numChaves] = chave;
    folha->numChaves++;
    
    // Ordena as chaves
    ordenarChaves(folha);
}

// Dividir um nó (split)
No23* dividirNo(No23 *no, int *chavePromovida) {
    // A chave do meio será promovida
    *chavePromovida = no->chaves[1];
    
    // Criar novo nó direito com a chave maior
    No23 *novoNo = criarNo(no->chaves[2], -1, no->ehFolha, no->pai);
    
    // Atualizar o nó original para ter apenas a chave menor
    no->chaves[1] = -1;
    no->numChaves = 1;
    
    // Se não for folha, redistribuir os filhos
    if (!no->ehFolha) {
        novoNo->filhos[0] = no->filhos[2];
        novoNo->filhos[1] = no->filhos[3];
        no->filhos[2] = NULL;
        no->filhos[3] = NULL;
        
        // Atualizar pais dos filhos movidos
        if (novoNo->filhos[0] != NULL) novoNo->filhos[0]->pai = novoNo;
        if (novoNo->filhos[1] != NULL) novoNo->filhos[1]->pai = novoNo;
    }
    
    return novoNo;
}

// Inserir chave em nó interno
void inserirEmPai(No23 *pai, int chavePromovida, No23 *novoNo) {
    if (pai == NULL) {
        // Criar nova raiz
        No23 *novaRaiz = criarNo(chavePromovida, -1, false, NULL);
        novaRaiz->filhos[0] = novoNo->pai; // nó original
        novaRaiz->filhos[1] = novoNo;
        
        // Atualizar pais
        novoNo->pai->pai = novaRaiz;
        novoNo->pai = novaRaiz;
        
        // Atualizar raiz da árvore
        novoNo->pai->pai = novaRaiz; // nó original
        return;
    }
    
    // Inserir chave promovida no pai
    if (pai->numChaves == 1) {
        // Pai tem espaço
        pai->chaves[1] = chavePromovida;
        pai->numChaves = 2;
        ordenarChaves(pai);
        
        // Inserir novo filho na posição correta
        if (novoNo->chaves[0] > pai->chaves[0]) {
            pai->filhos[2] = novoNo;
        } else {
            pai->filhos[2] = pai->filhos[1];
            pai->filhos[1] = novoNo;
        }
    } else {
        // Pai também está cheio, precisa dividir
        // Temporariamente adiciona a terceira chave
        pai->chaves[2] = chavePromovida;
        pai->numChaves = 3;
        
        // Ordenar as 3 chaves
        if (pai->chaves[0] > pai->chaves[1]) {
            int temp = pai->chaves[0];
            pai->chaves[0] = pai->chaves[1];
            pai->chaves[1] = temp;
        }
        if (pai->chaves[1] > pai->chaves[2]) {
            int temp = pai->chaves[1];
            pai->chaves[1] = pai->chaves[2];
            pai->chaves[2] = temp;
        }
        if (pai->chaves[0] > pai->chaves[1]) {
            int temp = pai->chaves[0];
            pai->chaves[0] = pai->chaves[1];
            pai->chaves[1] = temp;
        }
        
        // Dividir o pai
        int novaChavePromovida;
        No23 *novoPai = dividirNo(pai, &novaChavePromovida);
        
        // Configurar filhos do novo pai
        if (chavePromovida < novaChavePromovida) {
            novoPai->filhos[0] = pai->filhos[2];
            novoPai->filhos[1] = pai->filhos[3];
        } else {
            novoPai->filhos[0] = pai->filhos[3];
            novoPai->filhos[1] = novoNo;
        }
        
        // Atualizar pais
        if (novoPai->filhos[0] != NULL) novoPai->filhos[0]->pai = novoPai;
        if (novoPai->filhos[1] != NULL) novoPai->filhos[1]->pai = novoPai;
        
        // Chamar recursivamente para o avô
        inserirEmPai(pai->pai, novaChavePromovida, novoPai);
    }
}

// Função principal de inserção
No23* inserir(No23 *raiz, int chave) {
    // Caso especial: árvore vazia
    if (raiz == NULL) {
        return criarNo(chave, -1, true, NULL);
    }
    
    // Encontrar a folha correta para inserção
    No23 *atual = raiz;
    while (!atual->ehFolha) {
        int filho = encontrarFilho(atual, chave);
        atual = atual->filhos[filho];
    }
    
    // Verificar se a chave já existe
    for (int i = 0; i < atual->numChaves; i++) {
        if (atual->chaves[i] == chave) {
            printf("Chave %d já existe na árvore.\n", chave);
            return raiz;
        }
    }
    
    // Inserir na folha
    if (atual->numChaves == 1) {
        // Folha tem espaço
        inserirEmFolha(atual, chave);
        return raiz;
    } else {
        // Folha está cheia (tem 2 chaves)
        // Adicionar temporariamente a terceira chave
        atual->chaves[2] = chave;
        atual->numChaves = 3;
        
        // Ordenar as 3 chaves
        if (atual->chaves[0] > atual->chaves[1]) {
            int temp = atual->chaves[0];
            atual->chaves[0] = atual->chaves[1];
            atual->chaves[1] = temp;
        }
        if (atual->chaves[1] > atual->chaves[2]) {
            int temp = atual->chaves[1];
            atual->chaves[1] = atual->chaves[2];
            atual->chaves[2] = temp;
        }
        if (atual->chaves[0] > atual->chaves[1]) {
            int temp = atual->chaves[0];
            atual->chaves[0] = atual->chaves[1];
            atual->chaves[1] = temp;
        }
        
        // Dividir a folha
        int chavePromovida;
        No23 *novaFolha = dividirNo(atual, &chavePromovida);
        
        // Inserir no pai
        inserirEmPai(atual->pai, chavePromovida, novaFolha);
        
        // Retornar a raiz (pode ter mudado)
        while (raiz->pai != NULL) {
            raiz = raiz->pai;
        }
        return raiz;
    }
}

// ===================== FUNÇÕES DE REMOÇÃO =====================

// Encontrar predecessor (maior chave na subárvore esquerda)
int encontrarPredecessor(No23 *no) {
    while (!no->ehFolha) {
        no = no->filhos[no->numChaves]; // Último filho
    }
    return no->chaves[no->numChaves - 1];
}

// Encontrar sucessor (menor chave na subárvore direita)
int encontrarSucessor(No23 *no) {
    while (!no->ehFolha) {
        no = no->filhos[0]; // Primeiro filho
    }
    return no->chaves[0];
}

// Redistribuir chaves com irmão à esquerda
void redistribuirEsquerda(No23 *no, No23 *irmao, No23 *pai, int indiceNo) {
    // Mover chave do pai para o nó
    no->chaves[1] = no->chaves[0];
    no->chaves[0] = pai->chaves[indiceNo - 1];
    no->numChaves = 2;
    
    // Mover chave do irmão para o pai
    pai->chaves[indiceNo - 1] = irmao->chaves[irmao->numChaves - 1];
    irmao->numChaves--;
    
    // Se não for folha, mover filho também
    if (!no->ehFolha) {
        no->filhos[2] = no->filhos[1];
        no->filhos[1] = no->filhos[0];
        no->filhos[0] = irmao->filhos[irmao->numChaves + 1];
        if (no->filhos[0] != NULL) no->filhos[0]->pai = no;
    }
}

// Redistribuir chaves com irmão à direita
void redistribuirDireita(No23 *no, No23 *irmao, No23 *pai, int indiceNo) {
    // Mover chave do pai para o nó
    no->chaves[1] = pai->chaves[indiceNo];
    no->numChaves = 2;
    
    // Mover chave do irmão para o pai
    pai->chaves[indiceNo] = irmao->chaves[0];
    
    // Rearranjar chaves do irmão
    irmao->chaves[0] = irmao->chaves[1];
    irmao->numChaves--;
    
    // Se não for folha, mover filho também
    if (!no->ehFolha) {
        no->filhos[2] = irmao->filhos[0];
        if (no->filhos[2] != NULL) no->filhos[2]->pai = no;
        
        // Deslocar filhos do irmão
        irmao->filhos[0] = irmao->filhos[1];
        irmao->filhos[1] = irmao->filhos[2];
        irmao->filhos[2] = NULL;
    }
}

// Fundir nó com irmão à esquerda
void fundirEsquerda(No23 *no, No23 *irmao, No23 *pai, int indiceNo) {
    // Mover chave do pai para o irmão
    irmao->chaves[1] = pai->chaves[indiceNo - 1];
    irmao->numChaves = 2;
    
    // Mover chave do nó para o irmão
    irmao->chaves[2] = no->chaves[0];
    
    // Ordenar chaves no irmão
    if (irmao->chaves[1] > irmao->chaves[2]) {
        int temp = irmao->chaves[1];
        irmao->chaves[1] = irmao->chaves[2];
        irmao->chaves[2] = temp;
    }
    
    // Remover chave do pai
    for (int i = indiceNo - 1; i < pai->numChaves - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
        pai->filhos[i + 1] = pai->filhos[i + 2];
    }
    pai->numChaves--;
    
    // Se não for folha, mover filhos
    if (!no->ehFolha) {
        irmao->filhos[2] = no->filhos[0];
        irmao->filhos[3] = no->filhos[1];
        if (irmao->filhos[2] != NULL) irmao->filhos[2]->pai = irmao;
        if (irmao->filhos[3] != NULL) irmao->filhos[3]->pai = irmao;
    }
    
    // Liberar memória do nó
    free(no);
}

// Ajustar árvore após remoção
void ajustarAposRemocao(No23 *no) {
    if (no == NULL || no->numChaves >= 1) return;
    
    // Se é a raiz e está vazia
    if (no->pai == NULL) {
        if (no->numChaves == 0) {
            if (no->filhos[0] != NULL) {
                no->filhos[0]->pai = NULL;
                // A raiz atual vira o filho
                No23 *temp = no;
                no = no->filhos[0];
                free(temp);
            }
        }
        return;
    }
    
    // Encontrar índice deste nó no pai
    No23 *pai = no->pai;
    int indiceNo = 0;
    while (pai->filhos[indiceNo] != no) indiceNo++;
    
    // Tentar redistribuição com irmão esquerdo
    if (indiceNo > 0 && pai->filhos[indiceNo - 1]->numChaves > 1) {
        redistribuirEsquerda(no, pai->filhos[indiceNo - 1], pai, indiceNo);
        return;
    }
    
    // Tentar redistribuição com irmão direito
    if (indiceNo < pai->numChaves && pai->filhos[indiceNo + 1]->numChaves > 1) {
        redistribuirDireita(no, pai->filhos[indiceNo + 1], pai, indiceNo);
        return;
    }
    
    // Se não pode redistribuir, fundir
    if (indiceNo > 0) {
        // Fundir com irmão esquerdo
        fundirEsquerda(no, pai->filhos[indiceNo - 1], pai, indiceNo);
    } else {
        // Fundir com irmão direito
        fundirEsquerda(pai->filhos[indiceNo + 1], no, pai, indiceNo + 1);
    }
    
    // Ajustar recursivamente o pai
    ajustarAposRemocao(pai);
}

// Função principal de remoção
No23* removerChave(No23 *raiz, int chave) {
    if (raiz == NULL) {
        printf("Árvore vazia!\n");
        return NULL;
    }
    
    // Buscar a chave
    int posicao;
    No23 *no = buscar(raiz, chave, &posicao);
    
    if (no == NULL) {
        printf("Chave %d não encontrada.\n", chave);
        return raiz;
    }
    
    // CASO 1: Nó é folha
    if (no->ehFolha) {
        // Remover a chave
        for (int i = posicao; i < no->numChaves - 1; i++) {
            no->chaves[i] = no->chaves[i + 1];
        }
        no->numChaves--;
        
        // Ajustar se necessário
        if (no->numChaves < 1) {
            ajustarAposRemocao(no);
        }
    } 
    // CASO 2: Nó interno
    else {
        // Encontrar predecessor ou sucessor
        int chaveSubstituta;
        if (no->filhos[posicao]->numChaves >= 1) {
            // Usar predecessor
            chaveSubstituta = encontrarPredecessor(no->filhos[posicao]);
        } else {
            // Usar sucessor
            chaveSubstituta = encontrarSucessor(no->filhos[posicao + 1]);
        }
        
        // Substituir chave
        no->chaves[posicao] = chaveSubstituta;
        
        // Remover a chave substituta recursivamente
        if (no->filhos[posicao]->numChaves >= 1) {
            raiz = removerChave(raiz, chaveSubstituta);
        } else {
            raiz = removerChave(raiz, chaveSubstituta);
        }
    }
    
    // Encontrar nova raiz se necessário
    while (raiz->pai != NULL) {
        raiz = raiz->pai;
    }
    
    return raiz;
}

// ===================== FUNÇÕES DE EXIBIÇÃO =====================

// Percorrer em ordem (crescente)
void emOrdem(No23 *no) {
    if (no == NULL) return;
    
    if (no->ehFolha) {
        if (no->numChaves == 1) {
            printf("%d ", no->chaves[0]);
        } else {
            printf("%d %d ", no->chaves[0], no->chaves[1]);
        }
    } else {
        if (no->numChaves == 1) {
            emOrdem(no->filhos[0]);
            printf("%d ", no->chaves[0]);
            emOrdem(no->filhos[1]);
        } else {
            emOrdem(no->filhos[0]);
            printf("%d ", no->chaves[0]);
            emOrdem(no->filhos[1]);
            printf("%d ", no->chaves[1]);
            emOrdem(no->filhos[2]);
        }
    }
}

// Encontrar altura da árvore
int altura(No23 *no) {
    if (no == NULL) return 0;
    if (no->ehFolha) return 1;
    return 1 + altura(no->filhos[0]);
}

// Imprimir por nível (BFS)
void imprimirPorNivel(No23 *raiz) {
    if (raiz == NULL) return;
    
    int h = altura(raiz);
    
    // Array para simular fila
    No23 **fila = (No23**)malloc(sizeof(No23*) * 1000);
    int frente = 0, tras = 0;
    
    fila[tras++] = raiz;
    
    while (frente < tras) {
        int tamanhoNivel = tras - frente;
        
        for (int i = 0; i < tamanhoNivel; i++) {
            No23 *atual = fila[frente++];
            
            // Imprimir nó
            printf("[");
            for (int j = 0; j < atual->numChaves; j++) {
                printf("%d", atual->chaves[j]);
                if (j < atual->numChaves - 1) printf(", ");
            }
            printf("] ");
            
            // Adicionar filhos à fila
            if (!atual->ehFolha) {
                for (int j = 0; j <= atual->numChaves; j++) {
                    if (atual->filhos[j] != NULL) {
                        fila[tras++] = atual->filhos[j];
                    }
                }
            }
        }
        printf("\n");
    }
    
    free(fila);
}

// ===================== FUNÇÕES DE MENU E MAIN =====================

// Liberar memória da árvore
void liberarArvore(No23 *no) {
    if (no == NULL) return;
    
    if (!no->ehFolha) {
        for (int i = 0; i <= no->numChaves; i++) {
            liberarArvore(no->filhos[i]);
        }
    }
    
    free(no);
}

// Exibir menu principal
void exibirMenuPrincipal() {
    printf("\n=== ÁRVORE 2-3 ===\n");
    printf("1 - Inserir valor\n");
    printf("2 - Buscar valor\n");
    printf("3 - Remover valor\n");
    printf("4 - Percorrer árvore\n");
    printf("0 - Sair\n");
    printf("Escolha uma opção: ");
}

// Exibir submenu de percursos
void exibirSubmenuPercursos() {
    printf("\n--- TIPOS DE PERCURSO ---\n");
    printf("1 - Em ordem (crescente)\n");
    printf("2 - Por nível (estrutura)\n");
    printf("Escolha o tipo de percurso: ");
}

// Função principal
int main() {
    Arvore23 arvore;
    inicializarArvore(&arvore);
    
    int opcao, subOpcao, valor;
    
    do {
        exibirMenuPrincipal();
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                arvore.raiz = inserir(arvore.raiz, valor);
                printf("Valor %d inserido.\n", valor);
                break;
                
            case 2:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                int posicao;
                if (buscar(arvore.raiz, valor, &posicao) != NULL) {
                    printf("Valor %d encontrado na árvore.\n", valor);
                } else {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                }
                break;
                
            case 3:
                printf("Digite o valor a ser removido: ");
                scanf("%d", &valor);
                arvore.raiz = removerChave(arvore.raiz, valor);
                printf("Valor %d removido.\n", valor);
                break;
                
            case 4:
                if (arvoreVazia(&arvore)) {
                    printf("Árvore vazia!\n");
                } else {
                    exibirSubmenuPercursos();
                    scanf("%d", &subOpcao);
                    
                    switch (subOpcao) {
                        case 1:
                            printf("Percorrendo em ordem: ");
                            emOrdem(arvore.raiz);
                            printf("\n");
                            break;
                        case 2:
                            printf("Percorrendo por nível:\n");
                            imprimirPorNivel(arvore.raiz);
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
    
    // Liberar memória
    liberarArvore(arvore.raiz);
    printf("Memória liberada. Programa encerrado.\n");
    
    return 0;
}