
#include <stdio.h>
#include <limits.h>
#include "processo.h"

// Contador estático utilizado para gerar PIDs sequenciais.
// Inicia em 1 e mantém seu valor entre chamadas da função.
static int proximo_pid = 1;

/*
 * Verifica se um estado informado pertence ao enum EstadoProcesso.
 *
 * estado: estado que será validado.
 *
 * Retorna 1 se o estado for válido.
 * Retorna 0 se o estado for inválido.
 */
static int estado_valido(EstadoProcesso estado) {
    return estado >= NOVO && estado <= FINALIZADO;
}

/*
 * Verifica se uma prioridade informada pertence ao enum Prioridade.
 *
 * prioridade: prioridade que será validada.
 *
 * Retorna 1 se a prioridade for válida.
 * Retorna 0 se a prioridade for inválida.
 */
static int prioridade_valida(Prioridade prioridade) {
    return prioridade == ALTA || prioridade == BAIXA;
}

/*
 * Verifica se um dispositivo informado pertence ao enum DispositivoIO.
 *
 * dispositivo: dispositivo que será validado.
 *
 * Retorna 1 se o dispositivo for válido.
 * Retorna 0 se o dispositivo for inválido.
 */
static int dispositivo_valido(DispositivoIO dispositivo) {
    return dispositivo >= NENHUM && dispositivo <= IMPRESSORA;
}

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

    // Valida o identificador do processo pai.
    if (ppid < 0) {
        printf("Erro: PPID não pode ser negativo.\n");
        return 0;
    }

    // Valida o tempo necessário para execução do processo.
    if (tempo_restante <= 0) {
        printf("Erro: tempo restante deve ser maior que zero.\n");
        return 0;
    }

    // Valida se o dispositivo de E/S informado existe.
    if (!dispositivo_valido(proximo_io)) {
        printf("Erro: dispositivo de E/S inválido.\n");
        return 0;
    }

    // Valida a duração da operação de E/S.
    if (tempo_io < 0) {
        printf("Erro: tempo de I/O não pode ser negativo.\n");
        return 0;
    }

    // Verifica se a ausência de E/S corresponde a uma duração zero.
    if (proximo_io == NENHUM && tempo_io != 0) {
        printf("Erro: processo sem E/S deve possuir tempo de I/O igual a zero.\n");
        return 0;
    }

    // Verifica se uma operação de E/S possui duração positiva.
    if (proximo_io != NENHUM && tempo_io == 0) {
        printf("Erro: operação de E/S deve possuir duração maior que zero.\n");
        return 0;
    }

    // Impede o overflow do contador de PID.
    if (proximo_pid == INT_MAX) {
        printf("Erro: limite de PIDs atingido.\n");
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
 *
 * Retorna sem exibir os dados se o ponteiro do PCB for inválido
 * ou se algum atributo enumérico possuir valor inválido.
 */
void mostrar_processo(PCB *processo) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }

    // Valida os atributos enuméricos antes de exibi-los.
    if (!estado_valido(processo->estado) ||
        !prioridade_valida(processo->prioridade) ||
        !dispositivo_valido(processo->proximo_io)) {
        printf("Erro: PCB possui atributos inválidos.\n");
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
 * Valida o estado informado e a transição entre estados.
 * Retorna sem atualizar o estado se algum parâmetro for inválido
 * ou se a transição não for permitida.
 */
void atualizar_estado(PCB *processo, EstadoProcesso novo_estado) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }

    // Verifica se o estado atual do processo é válido.
    if (!estado_valido(processo->estado)) {
        printf("Erro: estado atual do processo inválido.\n");
        return;
    }

    // Verifica se o novo estado pertence ao enum.
    if (!estado_valido(novo_estado)) {
        printf("Erro: novo estado inválido.\n");
        return;
    }

    // Impede alterações em processos finalizados.
    if (processo->estado == FINALIZADO) {
        printf("Erro: processo finalizado não pode ser alterado.\n");
        return;
    }

    // Verifica se a transição de estado é permitida.
    switch (processo->estado) {
        case NOVO:
            if (novo_estado != PRONTO) {
                printf("Erro: processo NOVO só pode passar para PRONTO.\n");
                return;
            }
            break;

        case PRONTO:
            if (novo_estado != EXECUTANDO) {
                printf("Erro: processo PRONTO só pode passar para EXECUTANDO.\n");
                return;
            }
            break;

        case EXECUTANDO:
            if (novo_estado != PRONTO && novo_estado != BLOQUEADO) {
                printf("Erro: transição de estado inválida.\n");
                return;
            }
            break;

        case BLOQUEADO:
            if (novo_estado != PRONTO) {
                printf("Erro: processo BLOQUEADO só pode passar para PRONTO.\n");
                return;
            }
            break;

        case FINALIZADO:
            return;
    }

    // Atualiza o estado após validar a transição.
    processo->estado = novo_estado;

    // Exibe uma mensagem confirmando a atualização.
    printf("PID %d: Estado atualizado com sucesso!\n", processo->pid);
}

/*
 * Atualiza a prioridade de um processo.
 *
 * processo: ponteiro para o PCB cuja prioridade será atualizada.
 * nova_prioridade: nova prioridade que será atribuída ao processo.
 *
 * Retorna sem atualizar a prioridade se o ponteiro do PCB,
 * a prioridade informada ou o estado do processo forem inválidos.
 */
void atualizar_prioridade(PCB *processo, Prioridade nova_prioridade) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }

    // Verifica se a prioridade informada é válida.
    if (!prioridade_valida(nova_prioridade)) {
        printf("Erro: prioridade inválida.\n");
        return;
    }

    // Impede alterações em processos finalizados.
    if (processo->estado == FINALIZADO) {
        printf("Erro: processo finalizado não pode ter prioridade alterada.\n");
        return;
    }

    // Atualiza a prioridade após validar os parâmetros.
    processo->prioridade = nova_prioridade;

    // Exibe uma mensagem confirmando a atualização.
    printf("PID %d: Prioridade atualizada com sucesso!\n", processo->pid);
}

/*
 * Atualiza o tempo restante de execução de um processo.
 *
 * processo: ponteiro para o PCB cujo tempo restante será atualizado.
 * tempo_executado: quantidade de tempo de CPU já utilizada pelo processo.
 *
 * Subtrai o tempo executado do tempo restante quando o valor informado
 * é válido e o processo está no estado EXECUTANDO.
 *
 * Finaliza automaticamente o processo quando seu tempo restante
 * chega a zero.
 *
 * Retorna sem atualizar o tempo restante se o ponteiro do PCB,
 * o estado do processo ou o tempo executado forem inválidos.
 */
void atualizar_tempo_restante(PCB *processo, int tempo_executado) {
    // Verifica se o ponteiro do PCB é válido.
    if (processo == NULL) {
        printf("Erro: PCB inválido.\n");
        return;
    }

    // Verifica se o processo está em execução.
    if (processo->estado != EXECUTANDO) {
        printf("Erro: somente processos EXECUTANDO podem consumir tempo de CPU.\n");
        return;
    }

    // Verifica se o tempo executado é positivo.
    if (tempo_executado <= 0) {
        printf("Erro: tempo executado deve ser maior que zero.\n");
        return;
    }

    // Verifica se o tempo executado não ultrapassa o tempo restante.
    if (tempo_executado > processo->tempo_restante) {
        printf("Erro: tempo executado excede o tempo restante.\n");
        return;
    }

    // Subtrai o tempo executado do tempo restante.
    processo->tempo_restante -= tempo_executado;

    // Finaliza o processo quando não há mais tempo de CPU.
    if (processo->tempo_restante == 0) {
        processo->estado = FINALIZADO;
        printf("PID %d: Processo finalizado!\n", processo->pid);
        return;
    }

    // Exibe uma mensagem confirmando a atualização.
    printf("PID %d: Tempo restante atualizado com sucesso!\n", processo->pid);
}
