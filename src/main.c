#include <stdio.h>
#include <stdlib.h>

#include "processo.h"
#include "escalonador.h"

int main() {

    // Cria o escalonador com quantum de 3 unidades de tempo.
    Escalonador *escalonador = criar_escalonador(3);

    // Verifica se o escalonador foi criado corretamente.
    if (escalonador == NULL) {
        printf("[ERRO] Falha ao criar escalonador.\n");
        return 1;
    }

    int total_processos = 4;
    // Cria os processos de teste.
    PCB *p1 = criar_processo(0, 8, 3, DISCO, 2);
    PCB *p2 = criar_processo(0, 5, 0, NENHUM, 0);
    PCB *p3 = criar_processo(0, 10, 4, FITA, 3);
    PCB *p4 = criar_processo(0, 6, 2, IMPRESSORA, 2);

    // Verifica se os processos foram criados corretamente.
    if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL) {
        printf("[ERRO] Falha ao criar processos.\n");
        liberar_escalonador(&escalonador);
        return 1;
    }

    // Adiciona os processos ao escalonador.
    adicionar_processo(escalonador, p1);
    adicionar_processo(escalonador, p2);
    adicionar_processo(escalonador, p3);
    adicionar_processo(escalonador, p4);

    // Executa o escalonador enquanto houver processos prontos.
    while (escalonador->fila_alta->inicio != NULL || escalonador->fila_baixa->inicio != NULL) {

        // Seleciona o próximo processo.
        PCB *processo = selecionar_processo(escalonador);

        // Verifica se existe processo selecionado.
        if (processo == NULL) {
            printf("[ERRO] Nenhum processo selecionado.\n");
            break;
        }

        // Executa o processo selecionado.
        if (!executar_processo(escalonador, processo)) {
            printf("[ERRO] Falha ao executar processo.\n");
            break;
        }
    }
    printf("\n[FIM] Processos Finalizados: %d/%d | Tempo total: %d\n", escalonador->processos_finalizados, total_processos, escalonador->tempo_simulacao);
    // Exibe os resultados da simulação.
    printf("\n========== RESULTADOS ==========\n");
    printf("Tempo total: %d\n", escalonador->tempo_simulacao);
    printf("Processos finalizados: %d\n", escalonador->processos_finalizados);
    printf("Preempções: %d\n", escalonador->preempcoes);

    // Libera os recursos do escalonador.
    liberar_escalonador(&escalonador);

    // Libera os PCBs criados.
    free(p1);
    free(p2);
    free(p3);
    free(p4);

    return 0;
}
