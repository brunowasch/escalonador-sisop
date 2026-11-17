#include <stdio.h>
#include <stdlib.h>

#include "processo.h"
#include "escalonador.h"

int main() {
    const int quantum = 3;
    PCB *processos[] = {
        criar_processo(0, 8, 3, DISCO, 2),
        criar_processo(0, 5, 0, NENHUM, 0),
        criar_processo(0, 10, 4, FITA, 3),
        criar_processo(0, 6, 2, IMPRESSORA, 2)
    };
    const int total_processos = (int)(sizeof(processos) / sizeof(processos[0]));
    Escalonador *escalonador = criar_escalonador(quantum);

    if (escalonador == NULL) {
        fprintf(stderr, "[ERRO] Falha ao criar escalonador.\n");
        for (int i = 0; i < total_processos; i++)
            free(processos[i]);
        return 1;
    }

    for (int i = 0; i < total_processos; i++) {
        if (processos[i] == NULL || !adicionar_processo(escalonador, processos[i])) {
            fprintf(stderr, "[ERRO] Falha ao criar ou adicionar processo.\n");
            liberar_escalonador(&escalonador);
            for (int j = 0; j < total_processos; j++)
                free(processos[j]);
            return 1;
        }
    }

    printf("[CONFIG] quantum=%d | processos=%d\n", quantum, total_processos);
    while (escalonador->fila_alta->inicio != NULL || escalonador->fila_baixa->inicio != NULL) {
        PCB *processo = selecionar_processo(escalonador);
        if (processo == NULL || !executar_processo(escalonador, processo)) {
            fprintf(stderr, "[ERRO] Falha durante a simulação.\n");
            liberar_escalonador(&escalonador);
            for (int i = 0; i < total_processos; i++)
                free(processos[i]);
            return 1;
        }
    }

    mostrar_historico(escalonador->historico);

    // Acumula valores por processo para calcular as métricas agregadas.
    int soma_cpu = 0;
    int soma_espera = 0;
    int soma_turnaround = 0;
    int total_io_disco = 0;
    int total_io_fita = 0;
    int total_io_impressora = 0;
    for (int i = 0; i < total_processos; i++) {
        PCB *p = processos[i];
        soma_cpu += p->tempo_cpu_total;
        total_io_disco += p->quantidade_io_disco;
        total_io_fita += p->quantidade_io_fita;
        total_io_impressora += p->quantidade_io_impressora;

        // Turnaround é chegada até finalização; espera exclui CPU e E/S do processo.
        if (p->estado == FINALIZADO) {
            int turnaround = p->tempo_finalizacao - p->tempo_chegada;
            soma_turnaround += turnaround;
            soma_espera += turnaround - p->tempo_cpu_total - p->tempo_io_total;
        }
    }

    // Neste modelo serial, o tempo total menos CPU corresponde ao tempo de E/S,
    // durante o qual nenhum processo executa na CPU.
    int cpu_ociosa = escalonador->tempo_simulacao - soma_cpu;
    double percentual_ocioso = escalonador->tempo_simulacao > 0
        ? 100.0 * cpu_ociosa / escalonador->tempo_simulacao
        : 0.0;

    printf("\n[FIM] Processos finalizados: %d/%d | Tempo total: %d\n",
           escalonador->processos_finalizados, total_processos, escalonador->tempo_simulacao);
    printf("\n========== RESULTADOS ==========\n");
    printf("Tempo total: %d unidades\n", escalonador->tempo_simulacao);
    printf("Processos finalizados: %d/%d\n", escalonador->processos_finalizados, total_processos);
    printf("Preempções: %d\n", escalonador->preempcoes);
    printf("E/S concluídas: disco=%d | fita=%d | impressora=%d\n", total_io_disco, total_io_fita, total_io_impressora);
    if (escalonador->processos_finalizados > 0) {
        printf("Tempo médio de espera: %.2f unidades\n", (double)soma_espera / escalonador->processos_finalizados);
        printf("Turnaround médio: %.2f unidades\n", (double)soma_turnaround / escalonador->processos_finalizados);
    }
    printf("Tempo de CPU ociosa: %d unidades (%.2f%%)\n", cpu_ociosa, percentual_ocioso);

    liberar_escalonador(&escalonador);
    for (int i = 0; i < total_processos; i++)
        free(processos[i]);

    return 0;
}
