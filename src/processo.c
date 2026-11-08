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
 * tempo_restante: tempo total de execução necessário.
 * proximo_io: dispositivo da próxima operação de E/S.
 * tempo_io: duração da próxima operação de E/S.
 *
 * Retorna 1 se o processo foi criado com sucesso.
 * Retorna 0 se algum parâmetro for inválido.
 */
int criar_processo(
    PCB *processo,
    int ppid,
    int tempo_restante,
    DispositivoIO proximo_io,
    int tempo_io
) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return 0;
    }

    // Valida o tempo necessário para execução do processo.
    if (tempo_restante <= 0) {
        printf("Erro: tempo restante deve ser maior que zero.\n");
        return 0;
    }

    // Valida a duração da operação de E/S.
    if (tempo_io < 0) {
        printf("Erro: tempo de I/O não pode ser negativo.\n");
        return 0;
    }

    // Atribui o PID atual e incrementa o contador.
    processo->pid = proximo_pid++;

    // Inicializa os atributos do PCB.
    processo->ppid = ppid;
    processo->estado = NOVO;
    processo->prioridade = ALTA;
    processo->tempo_restante = tempo_restante;
    processo->proximo_io = proximo_io;
    processo->tempo_io = tempo_io;

    // Exibe uma mensagem confirmando a criação do processo.
    printf("PID %d: Processo criado com sucesso!\n", processo->pid);

    // Retorna 1 indicando que o processo foi criado com sucesso.
    return 1;
}


/*
 * Exibe no terminal todos os atributos de um processo.
 *
 * processo: ponteiro para o PCB cujos dados serão apresentados.
 */
void mostrar_processo(PCB *processo) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }

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

/*
 * Atualiza o estado de um processo.
 *
 * processo: ponteiro para o PCB cujo estado será atualizado.
 * novo_estado: novo estado que será atribuído ao processo.
 *
 * Retorna sem atualizar o estado se o ponteiro do PCB for inválido.
 */
void atualizar_estado(PCB *processo, EstadoProcesso novo_estado) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }
    processo->estado = novo_estado;
    printf("PID %d: Estado atualizado com sucesso!\n", processo->pid);
}


/*
 * Atualiza a prioridade de um processo.
 *
 * processo: ponteiro para o PCB cuja prioridade será atualizada.
 * nova_prioridade: nova prioridade que será atribuída ao processo.
 *
 * Retorna sem atualizar a prioridade se o ponteiro do PCB for inválido.
 */
void atualizar_prioridade(PCB *processo, Prioridade nova_prioridade) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }
    processo->prioridade = nova_prioridade;
    printf("PID %d: Prioridade atualizada com sucesso!\n", processo->pid);
}


/*
 * Atualiza o tempo restante de execução de um processo.
 *
 * processo: ponteiro para o PCB cujo tempo restante será atualizado.
 * tempo_executado: quantidade de tempo de CPU já utilizada pelo processo.
 *
 * Subtrai o tempo executado do tempo restante quando o valor informado
 * é válido, ou seja, maior que zero e menor ou igual ao tempo restante.
 *
 * Retorna sem atualizar o tempo restante se o ponteiro do PCB for inválido
 * ou se o tempo executado for inválido.
 */
void atualizar_tempo_restante(PCB *processo, int tempo_executado) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }
    if (tempo_executado > 0 && tempo_executado <= processo->tempo_restante) {
        processo->tempo_restante = processo->tempo_restante - tempo_executado;
        printf("PID %d: Tempo restante atualizado com sucesso!\n", processo->pid);
    } else 
        printf("Tempo executado inválido.\n");
}
