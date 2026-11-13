
#include <stdio.h>
#include <stdlib.h>

#include "escalonador.h"

/*
 * Verifica se uma fila existe.
 *
 * fila: ponteiro para a fila que será validada.
 *
 * Retorna:
 * 1 se a fila for válida.
 * 0 caso contrário.
 */
int isFilaValida(Fila *fila) {
    if (fila == NULL)
        return 0;

    return 1;
}

/*
 * Cria e inicializa um escalonador.
 *
 * quantum: intervalo máximo de execução de um processo.
 *
 * Retorna:
 * Ponteiro para o escalonador criado.
 * NULL caso ocorra algum erro.
 */
Escalonador *criar_escalonador(int quantum) {
    // Valida o quantum.
    if (quantum <= 0) {
        printf("[ERRO] Quantum não pode ser menor ou igual a 0.\n");
        return NULL;
    }

    // Aloca memória para o escalonador.
    Escalonador *escalonador = malloc(sizeof(Escalonador));

    // Verifica se a alocação falhou.
    if (escalonador == NULL) {
        printf("[ERRO] Falha ao alocar escalonador.\n");
        return NULL;
    }

    // Inicializa as filas do escalonador.
    escalonador->fila_alta = criar_fila();
    escalonador->fila_baixa = criar_fila();
    escalonador->fila_io = criar_fila();

    // Verifica se todas as filas foram criadas corretamente.
    if (!isFilaValida(escalonador->fila_alta) ||
        !isFilaValida(escalonador->fila_baixa) ||
        !isFilaValida(escalonador->fila_io)) {

        printf("[ERRO] Falha ao criar uma ou mais filas.\n");

        liberar_escalonador(&escalonador);
        return NULL;
    }

    // Inicializa os atributos do escalonador.
    escalonador->quantum = quantum;
    escalonador->tempo_simulacao = 0;
    escalonador->processos_finalizados = 0;
    escalonador->preempcoes = 0;

    return escalonador;
}

/*
 * Libera a memória do escalonador.
 *
 * escalonador: endereço do ponteiro do escalonador.
 *
 * Retorna:
 * 1 se for liberado.
 * 0 caso ocorra algum erro.
 */
int liberar_escalonador(Escalonador **escalonador) {
    // Valida o ponteiro do escalonador.
    if (escalonador == NULL || *escalonador == NULL)
        return 0;

    // Libera as filas existentes.
    if ((*escalonador)->fila_alta != NULL)
        liberar_fila((*escalonador)->fila_alta);

    if ((*escalonador)->fila_baixa != NULL)
        liberar_fila((*escalonador)->fila_baixa);

    if ((*escalonador)->fila_io != NULL)
        liberar_fila((*escalonador)->fila_io);

    // Libera a memória do escalonador.
    free(*escalonador);

    // Invalida o ponteiro original.
    *escalonador = NULL;

    return 1;
}

/*
 * Adiciona um processo novo ao escalonador.
 *
 * escalonador: ponteiro para o escalonador.
 * processo: ponteiro para o processo.
 *
 * Retorna:
 * 1 se o processo for adicionado.
 * 0 caso ocorra algum erro.
 */
int adicionar_processo(Escalonador *escalonador, PCB *processo) {
    // Valida o escalonador.
    if (escalonador == NULL) {
        printf("[ERRO] Escalonador não existe.\n");
        return 0;
    }

    // Valida o processo.
    if (processo == NULL) {
        printf("[ERRO] Processo não existe.\n");
        return 0;
    }

    // Verifica se o processo está no estado NOVO.
    if (processo->estado != NOVO) {
        printf("[ERRO] Processo não está no estado NOVO.\n");
        return 0;
    }

    // Define a prioridade inicial como ALTA.
    if (!atualizar_prioridade(processo, ALTA))
        return 0;

    // Atualiza o estado para PRONTO.
    if (!atualizar_estado(processo, PRONTO))
        return 0;

    // Insere o processo na fila de alta prioridade.
    if (!enfileirar(escalonador->fila_alta, processo)) {
        printf("[ERRO] Falha ao inserir processo na fila.\n");
        return 0;
    }

    printf("[SUCESSO] Processo %d adicionado ao escalonador.\n",
           processo->pid);

    return 1;
}

/*
 * Seleciona o próximo processo para execução.
 *
 * escalonador: ponteiro para o escalonador.
 *
 * Retorna:
 * Ponteiro para o processo selecionado.
 * NULL caso não exista processo disponível.
 */
PCB *selecionar_processo(Escalonador *escalonador) {
    // Valida o escalonador.
    if (escalonador == NULL) {
        printf("[ERRO] Escalonador não existe.\n");
        return NULL;
    }

    PCB *processo = NULL;

    // Verifica primeiro a fila de alta prioridade.
    if (!fila_vazia(escalonador->fila_alta))
        processo = desenfileirar(escalonador->fila_alta);

    // Caso contrário, verifica a fila de baixa prioridade.
    else if (!fila_vazia(escalonador->fila_baixa))
        processo = desenfileirar(escalonador->fila_baixa);

    // Verifica se algum processo foi selecionado.
    if (processo == NULL)
        return NULL;

    // Atualiza o estado do processo.
    if (!atualizar_estado(processo, EXECUTANDO)) {
        printf("[ERRO] Falha ao atualizar estado do processo.\n");
        return NULL;
    }

    return processo;
}


/*
 * Executa um processo durante seu quantum.
 *
 * escalonador: ponteiro para o escalonador.
 * processo: ponteiro para o processo.
 *
 * Retorna:
 * 1 se a execução ocorrer.
 * 0 caso ocorra algum erro.
 */
int executar_processo(Escalonador *escalonador, PCB *processo) {
    // Valida os parâmetros.
    if (escalonador == NULL || processo == NULL) {
        printf("[ERRO] Escalonador ou processo inválido.\n");
        return 0;
    }

    // Verifica se o processo está executando.
    if (processo->estado != EXECUTANDO) {
        printf("[ERRO] Processo não está em execução.\n");
        return 0;
    }

    // Indica se o processo solicitou uma operação de E/S.
    int io_solicitada = 0;

    // Verifica se o processo deve solicitar E/S imediatamente.
    if (processo->proximo_io != NENHUM && processo->tempo_cpu_ate_io == 0)
        io_solicitada = 1;

    // Executa o processo durante o quantum,
    // enquanto não finalizar e não solicitar E/S.
    for (int i = 0; i < escalonador->quantum && processo->tempo_restante > 0 && !io_solicitada; i++) {
        // Executa uma unidade de CPU.
        if (!atualizar_tempo_restante(processo, 1))
            return 0;

        // Incrementa o tempo da simulação.
        escalonador->tempo_simulacao++;

        // Verifica se o processo possui uma operação de E/S.
        if (processo->proximo_io != NENHUM) {
            // Desconta uma unidade de CPU até a E/S.
            processo->tempo_cpu_ate_io--;
            // Verifica se chegou o momento da E/S.
            if (processo->tempo_cpu_ate_io == 0)
                io_solicitada = 1;
        }
    }

    // Verifica se o processo solicitou uma operação de E/S.
    if (io_solicitada) {
        // Atualiza o estado para BLOQUEADO.
        if (!atualizar_estado(processo, BLOQUEADO)) {
            printf("[ERRO] Falha ao bloquear processo.\n");
            return 0;
        }

        // Insere o processo na fila de E/S.
        if (!enfileirar(escalonador->fila_io, processo)) {
            printf("[ERRO] Falha ao inserir processo na fila de E/S.\n");
            return 0;
        }

        printf("[INFO] PID %d: Processo bloqueado para E/S -> fila E/S.\n", processo->pid);
        printf("[INFO] Sendo solicitado: %s\n", traduzir_io(processo));
        
        if (!gerenciar_io(escalonador, processo)) {
            printf("[ERRO] Falha ao gerenciar E/S.\n");
            return 0;
        }

    } else if (processo->tempo_restante == 0) {
        // Atualiza o estado para FINALIZADO.
        if (!atualizar_estado(processo, FINALIZADO)) {
            printf("[ERRO] Falha ao finalizar processo.\n");
            return 0;
        }
        // Incrementa o contador de processos finalizados.
        escalonador->processos_finalizados++;
        printf("[INFO] PID %d: Processo finalizado.\n", processo->pid);

    } else {
        // Atualiza o estado para PRONTO.
        if (!atualizar_estado(processo, PRONTO))
            return 0;

        // Rebaixa a prioridade do processo.
        if (!atualizar_prioridade(processo, BAIXA))
            return 0;

        // Insere o processo na fila de baixa prioridade.
        if (!enfileirar(escalonador->fila_baixa, processo)) {
            printf("[ERRO] Falha ao reinserir processo na fila.\n");
            return 0;
        }

        // Incrementa o contador de preempções.
        escalonador->preempcoes++;
        printf("[INFO] PID %d: Processo sofreu preempção!\n", processo->pid);
    }
    return 1;
}

/*
 * Gerencia a execução de uma operação de E/S de um processo bloqueado.
 *
 * escalonador: ponteiro para o escalonador responsável pela simulação.
 * processo: ponteiro para o processo que realizará a operação de E/S.
 *
 * A função remove o processo da fila de E/S, simula o tempo necessário
 * para concluir a operação e encaminha o PCB para a fila de pronto
 * correspondente ao dispositivo utilizado.
 *
 * Regras de retorno:
 * - Disco: fila de baixa prioridade.
 * - Fita: fila de alta prioridade.
 * - Impressora: fila de alta prioridade.
 *
 * Após a conclusão da operação:
 * - O estado do processo é atualizado para PRONTO.
 * - Os campos relacionados à operação de E/S são reinicializados.
 * - O tempo global da simulação é incrementado durante a E/S.
 *
 * Retorno:
 * 1: operação de E/S concluída com sucesso.
 * 0: erro na validação ou no gerenciamento da operação.
 */
int gerenciar_io(Escalonador *escalonador, PCB *processo) {

    // Verifica se os ponteiros recebidos são válidos.
    if (escalonador == NULL || processo == NULL) {
        printf("[ERRO] Escalonador ou processo inválido.\n");
        return 0;
    }

    // Verifica se o processo está bloqueado e possui uma operação
    // de E/S válida, com tempo de execução positivo.
    if (processo->estado != BLOQUEADO || processo->tempo_io <= 0 || (processo->proximo_io != DISCO && processo->proximo_io != FITA && processo->proximo_io != IMPRESSORA)) {
        printf("[ERRO] Processo não possui E/S válida.\n");
        return 0;
    }

    // Verifica se a fila de E/S existe.
    if (!isFilaValida(escalonador->fila_io)) {
        printf("[ERRO] Fila de E/S inválida.\n");
        return 0;
    }

    // Remove o primeiro processo da fila de E/S.
    PCB *removido = desenfileirar(escalonador->fila_io);

    // Verifica se a remoção foi realizada corretamente e se o PCB removido corresponde ao processo esperado.
    if (removido == NULL || removido != processo) {
        printf("[ERRO] Processo removido da fila de E/S incorreto.\n");
        return 0;
    }

    // Armazena o tempo total da operação de E/S.
    int j = processo->tempo_io;

    // Simula a execução da operação de E/S. Cada iteração representa uma unidade de tempo.
    for (int i = 0; i < j; i++) {
        processo->tempo_io -= 1;
        escalonador->tempo_simulacao++;
    }

    // Após a conclusão da E/S em disco, o processo retorna à fila de baixa prioridade.
    if (processo->proximo_io == DISCO && processo->estado == BLOQUEADO) {
        // Atualiza o estado do processo para PRONTO.
        if (!atualizar_estado(processo, PRONTO))
            return 0;

        // Define a prioridade baixa para o processo.
        if (!atualizar_prioridade(processo, BAIXA))
            return 0;

        // Insere o processo na fila de baixa prioridade.
        if (!enfileirar(escalonador->fila_baixa, processo))
            return 0;

        // Reinicializa os campos relacionados à operação de E/S.
        processo->proximo_io = NENHUM;
        processo->tempo_cpu_ate_io = 0;
        processo->tempo_io = 0;
    }

    // Após a conclusão da E/S em fita ou impressora, o processo retorna à fila de alta prioridade.
    else if ((processo->proximo_io == FITA || processo->proximo_io == IMPRESSORA) && processo->estado == BLOQUEADO) {
        // Atualiza o estado do processo para PRONTO.
        if (!atualizar_estado(processo, PRONTO))
            return 0;

        // Define a prioridade alta para o processo.
        if (!atualizar_prioridade(processo, ALTA))
            return 0;

        // Insere o processo na fila de alta prioridade.
        if (!enfileirar(escalonador->fila_alta, processo))
            return 0;

        // Reinicializa os campos relacionados à operação de E/S.
        processo->proximo_io = NENHUM;
        processo->tempo_cpu_ate_io = 0;
        processo->tempo_io = 0;
    }

    // Indica que a operação de E/S foi concluída com sucesso.
    return 1;
}


