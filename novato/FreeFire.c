#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// ---------------------------------------------------------
// STRUCT ITEM: representa cada objeto da mochila do jogador
// ---------------------------------------------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;


// ---------------------------------------------------------
// Função: listarItens
// Exibe todos os itens cadastrados na mochila
// ---------------------------------------------------------
void listarItens(Item mochila[], int contador) {
    printf("\n===== ITENS NA MOCHILA =====\n");

    if (contador == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < contador; i++) {
        printf("Item %d:\n", i + 1);
        printf("Nome: %s\n", mochila[i].nome);
        printf("Tipo: %s\n", mochila[i].tipo);
        printf("Quantidade: %d\n\n", mochila[i].quantidade);
    }
}


// ---------------------------------------------------------
// Função: buscarItem
// Procedimento de busca sequencial pelo nome
// Retorna o índice encontrado ou -1 se não encontrar
// ---------------------------------------------------------
int buscarItem(Item mochila[], int contador, char nome[]) {
    for (int i = 0; i < contador; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1; // não encontrou
}


// ---------------------------------------------------------
// Função: inserirItem
// Cadastra novo item na mochila
// ---------------------------------------------------------
void inserirItem(Item mochila[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("\nA mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    Item novo;

    printf("\nDigite o nome do item: ");
    scanf("%s", novo.nome);

    printf("Digite o tipo do item (arma, municao, cura...): ");
    scanf("%s", novo.tipo);

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    mochila[*contador] = novo;
    (*contador)++;

    printf("\nItem adicionado com sucesso!\n");
}


// ---------------------------------------------------------
// Função: removerItem
// Remove item pelo nome e ajusta o vetor
// ---------------------------------------------------------
void removerItem(Item mochila[], int *contador) {
    if (*contador == 0) {
        printf("\nA mochila já está vazia.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    scanf("%s", nomeRemover);

    int pos = buscarItem(mochila, *contador, nomeRemover);

    if (pos == -1) {
        printf("\nItem não encontrado.\n");
        return;
    }

    // desloca itens para a esquerda
    for (int i = pos; i < *contador - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    (*contador)--;

    printf("\nItem removido com sucesso!\n");
}


// ---------------------------------------------------------
// PROGRAMA PRINCIPAL
// Menu com opções de gerenciamento da mochila
// ---------------------------------------------------------
int main() {

    Item mochila[MAX_ITENS];
    int contador = 0;
    int opcao;

    do {
        printf("\n========== MENU DO INVENTARIO ==========\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item\n");
        printf("3 - Buscar item\n");
        printf("4 - Listar itens\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                inserirItem(mochila, &contador);
                listarItens(mochila, contador);
                break;

            case 2:
                removerItem(mochila, &contador);
                listarItens(mochila, contador);
                break;

            case 3: {
                char nomeBusca[30];
                printf("\nDigite o nome do item para busca: ");
                scanf("%s", nomeBusca);

                int pos = buscarItem(mochila, contador, nomeBusca);

                if (pos == -1) {
                    printf("\nItem nao encontrado.\n");
                } else {
                    printf("\nItem encontrado:\n");
                    printf("Nome: %s\n", mochila[pos].nome);
                    printf("Tipo: %s\n", mochila[pos].tipo);
                    printf("Quantidade: %d\n", mochila[pos].quantidade);
                }
                break;
            }

            case 4:
                listarItens(mochila, contador);
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

