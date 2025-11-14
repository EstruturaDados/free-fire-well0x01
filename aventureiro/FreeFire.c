#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

// ============================================
// STRUCTS
// ============================================

// Estrutura básica de item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// Nó da lista encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ============================================
// VARIÁVEIS GLOBAIS DE CONTAGEM DE COMPARAÇÕES
// ============================================
int comparacoesSequencialVetor = 0;
int comparacoesBinaria = 0;
int comparacoesSequencialLista = 0;


// ============================================
// FUNÇÕES DO VETOR
// ============================================

// Inserir item no vetor
void inserirItemVetor(Item vetor[], int* tamanho) {
    if (*tamanho >= MAX_ITENS) {
        printf("\nA mochila (vetor) está cheia!\n");
        return;
    }

    Item novo;

    printf("\nNome do item: ");
    scanf("%s", novo.nome);

    printf("Tipo: ");
    scanf("%s", novo.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    vetor[*tamanho] = novo;
    (*tamanho)++;

    printf("\nItem inserido no vetor!\n");
}

// Remover item do vetor
void removerItemVetor(Item vetor[], int* tamanho) {
    if (*tamanho == 0) {
        printf("\nO vetor está vazio.\n");
        return;
    }

    char nome[30];
    printf("\nNome do item para remover: ");
    scanf("%s", nome);

    for (int i = 0; i < *tamanho; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            for (int j = i; j < *tamanho - 1; j++) {
                vetor[j] = vetor[j + 1];
            }
            (*tamanho)--;
            printf("\nItem removido do vetor!\n");
            return;
        }
    }

    printf("\nItem não encontrado.\n");
}

// Listar itens do vetor
void listarVetor(Item vetor[], int tamanho) {
    printf("\n===== Itens no Vetor =====\n");

    if (tamanho == 0) {
        printf("Vetor vazio.\n");
        return;
    }

    for (int i = 0; i < tamanho; i++) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

// Busca sequencial no vetor
int buscarSequencialVetor(Item vetor[], int tamanho, char nome[]) {
    comparacoesSequencialVetor = 0;

    for (int i = 0; i < tamanho; i++) {
        comparacoesSequencialVetor++;

        if (strcmp(vetor[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Ordenar vetor (Bubble Sort)
void ordenarVetor(Item vetor[], int tamanho) {
    Item temp;

    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - 1 - i; j++) {
            if (strcmp(vetor[j].nome, vetor[j + 1].nome) > 0) {
                temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }

    printf("\nVetor ordenado por nome!\n");
}

// Busca binária no vetor ordenado
int buscarBinariaVetor(Item vetor[], int tamanho, char nome[]) {
    comparacoesBinaria = 0;

    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBinaria++;

        int cmp = strcmp(vetor[meio].nome, nome);

        if (cmp == 0) {
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1;
}


// ============================================
// FUNÇÕES DA LISTA ENCADEADA
// ============================================

// Inserir item na lista
void inserirItemLista(No** lista) {
    No* novo = (No*)malloc(sizeof(No));

    printf("\nNome do item: ");
    scanf("%s", novo->dados.nome);

    printf("Tipo: ");
    scanf("%s", novo->dados.tipo);

    printf("Quantidade: ");
    scanf("%d", &novo->dados.quantidade);

    novo->proximo = *lista;
    *lista = novo;

    printf("\nItem inserido na lista!\n");
}

// Remover item da lista
void removerItemLista(No** lista) {
    if (*lista == NULL) {
        printf("\nLista vazia.\n");
        return;
    }

    char nome[30];
    printf("\nNome do item para remover: ");
    scanf("%s", nome);

    No *atual = *lista, *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL)
                *lista = atual->proximo;
            else
                anterior->proximo = atual->proximo;

            free(atual);
            printf("\nItem removido da lista!\n");
            return;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    printf("\nItem não encontrado.\n");
}

// Listar itens da lista
void listarLista(No* lista) {
    printf("\n===== Itens na Lista =====\n");

    if (lista == NULL) {
        printf("Lista vazia.\n");
        return;
    }

    while (lista != NULL) {
        printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
               lista->dados.nome, lista->dados.tipo, lista->dados.quantidade);
        lista = lista->proximo;
    }
}

// Busca sequencial na lista
No* buscarSequencialLista(No* lista, char nome[]) {
    comparacoesSequencialLista = 0;

    while (lista != NULL) {
        comparacoesSequencialLista++;

        if (strcmp(lista->dados.nome, nome) == 0) {
            return lista;
        }

        lista = lista->proximo;
    }
    return NULL;
}


// ============================================
// MENUS
// ============================================

void menuVetor() {
    Item vetor[MAX_ITENS];
    int tamanho = 0;
    int op;
    char nomeBusca[30];

    do {
        printf("\n===== MENU VETOR =====\n");
        printf("1 - Inserir\n");
        printf("2 - Remover\n");
        printf("3 - Listar\n");
        printf("4 - Busca Sequencial\n");
        printf("5 - Ordenar\n");
        printf("6 - Busca Binária\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                inserirItemVetor(vetor, &tamanho);
                break;

            case 2:
                removerItemVetor(vetor, &tamanho);
                break;

            case 3:
                listarVetor(vetor, tamanho);
                break;

            case 4:
                printf("\nNome para buscar: ");
                scanf("%s", nomeBusca);
                int pos;
                pos = buscarSequencialVetor(vetor, tamanho, nomeBusca);
                if (pos >= 0)
                    printf("\nItem encontrado no índice %d\n", pos);
                else
                    printf("\nItem não encontrado.\n");
                printf("Comparações: %d\n", comparacoesSequencialVetor);
                break;

            case 5:
                ordenarVetor(vetor, tamanho);
                break;

            case 6:
                printf("\nNome para buscar: ");
                scanf("%s", nomeBusca);
                int p = buscarBinariaVetor(vetor, tamanho, nomeBusca);
                if (p >= 0)
                    printf("\nItem encontrado no índice %d\n", p);
                else
                    printf("\nItem não encontrado.\n");
                printf("Comparações: %d\n", comparacoesBinaria);
                break;
        }

    } while (op != 0);
}


void menuLista() {
    No* lista = NULL;
    int op;
    char nomeBusca[30];

    do {
        printf("\n===== MENU LISTA =====\n");
        printf("1 - Inserir\n");
        printf("2 - Remover\n");
        printf("3 - Listar\n");
        printf("4 - Busca Sequencial\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                inserirItemLista(&lista);
                break;

            case 2:
                removerItemLista(&lista);
                break;

            case 3:
                listarLista(lista);
                break;

            case 4:
                printf("\nNome para buscar: ");
                scanf("%s", nomeBusca);
                No* resultado = buscarSequencialLista(lista, nomeBusca);
                if (resultado)
                    printf("\nItem encontrado: %s\n", resultado->dados.nome);
                else
                    printf("\nItem não encontrado.\n");

                printf("Comparações: %d\n", comparacoesSequencialLista);
                break;
        }

    } while (op != 0);
}


// ============================================
// MAIN
// ============================================

int main() {
    int op;

    do {
        printf("\n===== SISTEMA DE MOCHILA =====\n");
        printf("1 - Usar Vetor\n");
        printf("2 - Usar Lista Encadeada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                menuVetor();
                break;

            case 2:
                menuLista();
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpção inválida!\n");
        }

    } while (op != 0);

    return 0;
}
