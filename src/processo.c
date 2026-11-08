#include <stdio.h>
#include "processo.h"

// Contador estático utilizado para gerar PIDs sequenciais.
// Inicia em 1 e mantém seu valor entre chamadas da função.
static int proximo_pid = 1;

/*
 * Inicializa um PCB com os dados de um novo processo.
 *
 * processo: ponteiro para o PCB que será preenchido.
 * ppid: identificador do processo pai.
 * prioridade: prioridade inicial do processo.
 * tempo_restante: tempo total de execução necessário.
 * proximo_io: dispositivo da próxima operação de E/S.
 * tempo_io: duração da próxima operação de E/S.
 */
void criar_processo(
    PCB *processo,
    int ppid,
    Prioridade prioridade,
    int tempo_restante,
    DispositivoIO proximo_io,
    int tempo_io
) {
    // Atribui o PID atual ao processo e incrementa o contador
    // para garantir um identificador diferente na próxima criação.
    processo->pid = proximo_pid++;
    
    // Inicializa os demais atributos do PCB com os parâmetros recebidos.
    processo->ppid = ppid;
    processo->estado = NOVO; // Todo processo recém-criado começa no estado NOVO.
    processo->prioridade = prioridade;
    processo->tempo_restante = tempo_restante;
    processo->proximo_io = proximo_io;
    processo->tempo_io = tempo_io;
}

/*
 * Exibe no terminal todos os atributos de um processo.
 *
 * processo: ponteiro para o PCB cujos dados serão apresentados.
 */
void mostrar_processo(PCB *processo) {
    printf("\n=== PROCESSO ===\n");

    // Exibe os identificadores do processo e de seu processo pai.
    printf("PID: %d\n", processo->pid);
    printf("PPID: %d\n", processo->ppid);

    // Converte o estado armazenado no enum em uma representação textual.
    printf("Estado: ");
    switch (processo->estado) {
        case NOVO: printf("NOVO\n"); break;
        case PRONTO: printf("PRONTO\n"); break;
        case EXECUTANDO: printf("EXECUTANDO\n"); break;
        case BLOQUEADO: printf("BLOQUEADO\n"); break;
        case FINALIZADO: printf("FINALIZADO\n"); break;
    }

    // Converte a prioridade armazenada no enum em texto.
    printf("Prioridade: ");
    switch (processo->prioridade) {
        case ALTA: printf("ALTA\n"); break;
        case BAIXA: printf("BAIXA\n"); break;
    }

    // Exibe o tempo de CPU que ainda falta para o processo executar.
    printf("Tempo restante: %d\n", processo->tempo_restante);

    // Converte o dispositivo da próxima operação de E/S em texto.
    printf("Próximo IO: ");
    switch (processo->proximo_io) {
        case NENHUM: printf("NENHUM\n"); break;
        case DISCO: printf("DISCO\n"); break;
        case FITA: printf("FITA\n"); break;
        case IMPRESSORA: printf("IMPRESSORA\n"); break;
    }

    // Exibe a duração prevista da próxima operação de E/S.
    printf("Duração IO: %d\n", processo->tempo_io);

    // Delimita visualmente os dados do processo no terminal.
    printf("================\n");
}
