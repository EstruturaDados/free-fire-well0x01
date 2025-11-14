/* torre_resgate.c
 *
 * Sistema de priorização e montagem de componentes da torre de fuga.
 * Implementa ordenações (Bubble, Insertion, Selection), mede comparações
 * e tempo de execução, e realiza busca binária por nome após ordenação por nome.
 *
 * Compile:
 *   gcc torre_resgate.c -o torre_resgate
 * Execute:
 *   ./torre_resgate
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define STRLEN 30
#define TYPELEN 20

// -----------------------------
// Estrutura de componente
// -----------------------------
typedef struct {
    char nome[STRLEN];
    char tipo[TYPELEN];
    int prioridade; // 1 .. 10
} Componente;

// -----------------------------
// Funções utilitárias
// -----------------------------

// Remove newline de fgets
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len-1] == '\n') s[len-1] = '\0';
}

// Mostra o vetor de componentes formatado
void mostrarComponentes(Componente arr[], int n) {
    printf("\n----- Componentes (total: %d) -----\n", n);
    if (n == 0) {
        printf("Nenhum componente cadastrado.\n");
        return;
    }
    printf("%-3s | %-28s | %-12s | %-9s\n", "Idx", "Nome", "Tipo", "Prioridade");
    printf("----+------------------------------+--------------+-----------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-28s | %-12s | %-9d\n",
               i, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("----------------------------------------\n");
}

// Le uma linha com fgets e remove newline; com prompt
void lerString(const char *prompt, char *out, int maxlen) {
    char buf[256];
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        out[0] = '\0';
        return;
    }
    trim_newline(buf);
    // garantir truncamento seguro
    strncpy(out, buf, maxlen-1);
    out[maxlen-1] = '\0';
}

// Lê inteiro com prompt usando fgets + sscanf (evita misturar scanf/fgets)
int lerInteiro(const char *prompt) {
    char buf[128];
    int v;
    while (1) {
        printf("%s", prompt);
        if (fgets(buf, sizeof(buf), stdin) == NULL) return 0;
        if (sscanf(buf, "%d", &v) == 1) return v;
        printf("Entrada inválida. Digite um número inteiro.\n");
    }
}

// -----------------------------
// Ordenações (cada função recebe arr, n, ponteiro para contador de comparações)
// -----------------------------

// Bubble sort por nome (string). Conta comparações de strcmp via *comparacoes.
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

// Insertion sort por tipo (string). Conta comparações de strcmp via *comparacoes.
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = arr[i];
        int j = i - 1;
        // comparar tipos; cada comparação com arr[j].tipo conta
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = chave;
    }
}

// Selection sort por prioridade (int). Conta comparações de inteiros via *comparacoes.
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Componente tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

// -----------------------------
// Medidor de tempo para algoritmos de ordenação
// SortFunc: ponteiro para função de ordenação com assinatura (Componente[], int, long*)
// Retorna tempo em segundos; comparacoes é preenchido pela função de ordenação.
// -----------------------------
typedef void (*SortFunc)(Componente[], int, long*);

double medirTempo(SortFunc alg, Componente arr[], int n, long *comparacoes) {
    clock_t inicio = clock();
    alg(arr, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / (double)CLOCKS_PER_SEC;
}

// -----------------------------
// Busca binária por nome (somente em vetor ordenado por nome).
// Conta comparações (cada strcmp é 1) em *comparacoes.
// Retorna índice ou -1.
// -----------------------------
int buscaBinariaPorNome(Componente arr[], int n, const char *nome, long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[meio].nome, nome);
        if (cmp == 0) return meio;
        else if (cmp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

// -----------------------------
// Função para copiar vetor (útil para testar múltiplos algoritmos com os mesmos dados)
// -----------------------------
void copiarVetor(Componente dest[], Componente src[], int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

// -----------------------------
// Menu e fluxo principal
// -----------------------------

int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0; // número atual de componentes

    printf("=== Módulo Final: Montagem da Torre de Resgate ===\n");

    // Cadastro inicial (opcional repetível via menu)
    while (1) {
        printf("\n(1) Cadastrar componentes  (2) Entrar no menu principal\n");
        int escolha = lerInteiro("Escolha: ");
        if (escolha == 1) {
            if (n >= MAX_COMPONENTES) {
                printf("Limite atingido (%d componentes). Não é possível cadastrar mais.\n", MAX_COMPONENTES);
                continue;
            }
            lerString("Nome do componente: ", componentes[n].nome, STRLEN);
            lerString("Tipo do componente: ", componentes[n].tipo, TYPELEN);
            int p;
            do {
                p = lerInteiro("Prioridade (1-10): ");
                if (p < 1 || p > 10) printf("Prioridade deve ser entre 1 e 10.\n");
            } while (p < 1 || p > 10);
            componentes[n].prioridade = p;
            n++;
            printf("Componente cadastrado. Total agora: %d\n", n);
        } else if (escolha == 2) {
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }

    // Menu principal
    int opc = -1;
    // variável auxiliar para verificar se o vetor está ordenado por nome
    int ordenadoPorNome = 0;

    while (opc != 0) {
        printf("\n===== MENU PRINCIPAL =====\n");
        printf("1 - Mostrar componentes\n");
        printf("2 - Cadastrar novo componente\n");
        printf("3 - Ordenar por Nome (Bubble Sort)\n");
        printf("4 - Ordenar por Tipo (Insertion Sort)\n");
        printf("5 - Ordenar por Prioridade (Selection Sort)\n");
        printf("6 - Busca binária por Nome (apenas se ordenado por nome)\n");
        printf("7 - Medir/Comparar todos os algoritmos (mesmos dados)\n");
        printf("0 - Sair\n");
        opc = lerInteiro("Escolha: ");

        if (opc == 1) {
            mostrarComponentes(componentes, n);
        } else if (opc == 2) {
            if (n >= MAX_COMPONENTES) {
                printf("Não é possível cadastrar mais (cheio).\n");
            } else {
                lerString("Nome do componente: ", componentes[n].nome, STRLEN);
                lerString("Tipo do componente: ", componentes[n].tipo, TYPELEN);
                int p;
                do {
                    p = lerInteiro("Prioridade (1-10): ");
                    if (p < 1 || p > 10) printf("Prioridade deve ser entre 1 e 10.\n");
                } while (p < 1 || p > 10);
                componentes[n].prioridade = p;
                n++;
                printf("Componente cadastrado. Total agora: %d\n", n);
                ordenadoPorNome = 0; // nova adição quebra ordenação por nome
            }
        } else if (opc == 3) {
            if (n == 0) { printf("Vetor vazio.\n"); continue; }
            long comps = 0;
            // ordenar in-place (modifica componentes)
            double t = medirTempo(bubbleSortNome, componentes, n, &comps);
            printf("Bubble Sort (por nome) finalizado.\n");
            printf("Comparações (strcmp): %ld\n", comps);
            printf("Tempo: %.6f s\n", t);
            ordenadoPorNome = 1;
            mostrarComponentes(componentes, n);
        } else if (opc == 4) {
            if (n == 0) { printf("Vetor vazio.\n"); continue; }
            long comps = 0;
            double t = medirTempo(insertionSortTipo, componentes, n, &comps);
            printf("Insertion Sort (por tipo) finalizado.\n");
            printf("Comparações (strcmp): %ld\n", comps);
            printf("Tempo: %.6f s\n", t);
            ordenadoPorNome = 0;
            mostrarComponentes(componentes, n);
        } else if (opc == 5) {
            if (n == 0) { printf("Vetor vazio.\n"); continue; }
            long comps = 0;
            double t = medirTempo(selectionSortPrioridade, componentes, n, &comps);
            printf("Selection Sort (por prioridade) finalizado.\n");
            printf("Comparações (int): %ld\n", comps);
            printf("Tempo: %.6f s\n", t);
            ordenadoPorNome = 0;
            mostrarComponentes(componentes, n);
        } else if (opc == 6) {
            if (!ordenadoPorNome) {
                printf("Atenção: a busca binária requer que o vetor esteja ordenado por NOME.\n");
                printf("Ordene por nome (opção 3) antes de usar a busca binária.\n");
                continue;
            }
            if (n == 0) { printf("Vetor vazio.\n"); continue; }
            char chave[STRLEN];
            lerString("Nome do componente (chave) para busca binária: ", chave, STRLEN);
            long comps = 0;
            int pos = buscaBinariaPorNome(componentes, n, chave, &comps);
            if (pos >= 0) {
                printf("Componente encontrado no índice %d.\n", pos);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[pos].nome, componentes[pos].tipo, componentes[pos].prioridade);
            } else {
                printf("Componente '%s' não encontrado.\n", chave);
            }
            printf("Comparações (strcmp) feitas na busca binária: %ld\n", comps);
        } else if (opc == 7) {
            if (n == 0) { printf("Vetor vazio.\n"); continue; }
            // Cópias dos dados para testar os 3 algoritmos sem interferência
            Componente copia1[MAX_COMPONENTES], copia2[MAX_COMPONENTES], copia3[MAX_COMPONENTES];
            copiarVetor(copia1, componentes, n);
            copiarVetor(copia2, componentes, n);
            copiarVetor(copia3, componentes, n);

            long c1=0, c2=0, c3=0;
            double t1 = medirTempo(bubbleSortNome, copia1, n, &c1);
            double t2 = medirTempo(insertionSortTipo, copia2, n, &c2);
            double t3 = medirTempo(selectionSortPrioridade, copia3, n, &c3);

            printf("\n--- Resultados de comparação (mesmos dados originais) ---\n");
            printf("Bubble Sort (nome): Comparações(strcmp)=%ld, Tempo=%.6f s\n", c1, t1);
            printf("Insertion Sort (tipo): Comparações(strcmp)=%ld, Tempo=%.6f s\n", c2, t2);
            printf("Selection Sort (prioridade): Comparações(int)=%ld, Tempo=%.6f s\n", c3, t3);

            printf("\nVetor ordenado por nome (exemplo - bubble):\n");
            mostrarComponentes(copia1, n);

            printf("\nVetor ordenado por tipo (exemplo - insertion):\n");
            mostrarComponentes(copia2, n);

            printf("\nVetor ordenado por prioridade (exemplo - selection):\n");
            mostrarComponentes(copia3, n);

            // Observação: este teste NÃO altera o vetor original 'componentes'
            printf("\n(Observação: os resultados acima são de cópias; o vetor original não foi modificado.)\n");
        } else if (opc == 0) {
            printf("Encerrando módulo. Boa sorte na fuga!\n");
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}
