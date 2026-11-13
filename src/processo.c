#include <stdio.h>
#include <stdlib.h>
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
 * Retorna:
 * 1 se o estado for válido.
 * 0 caso contrário.
 */
static int estado_valido(EstadoProcesso estado) {
    return estado >= NOVO && estado <= FINALIZADO;
}

/*
 * Retorna a representação textual do dispositivo de E/S do processo.
 *
 * processo: ponteiro para o processo a ser consultado.
 *
 * Retorna uma string correspondente ao dispositivo de E/S.
 */
const char *traduzir_io(PCB *processo) {
    // Verifica qual é o próximo dispositivo de E/S do processo.
    switch (processo->proximo_io) {
        case NENHUM:
            return "NENHUM";
        case DISCO:
            return "DISCO";
        case FITA:
            return "FITA";
        case IMPRESSORA:
            return "IMPRESSORA";
        default:
            // Retorna mensagem de erro para valores inválidos.
            return "[ERRO] Tipo de E/S não definido corretamente.";
    }
}

/*
 * Verifica se uma prioridade informada pertence ao enum Prioridade.
 *
 * prioridade: prioridade que será validada.
 *
 * Retorna:
 * 1 se a prioridade for válida.
 * 0 caso contrário.
 */
static int prioridade_valida(Prioridade prioridade) {
    return prioridade >= ALTA && prioridade <= BAIXA;
}

/*
 * Verifica se um dispositivo informado pertence ao enum DispositivoIO.
 *
 * dispositivo: dispositivo que será validado.
 *
 * Retorna:
 * 1 se o dispositivo for válido.
 * 0 caso contrário.
 */
static int dispositivo_valido(DispositivoIO dispositivo) {
    return dispositivo >= NENHUM && dispositivo <= IMPRESSORA;
}

/*
 * Cria um novo processo e inicializa seu PCB.
 *
 * ppid: identificador do processo pai.
 * tempo_restante: quantidade total de tempo de CPU necessária.
 * tempo_cpu_ate_io: tempo de CPU até a próxima solicitação de E/S.
 * proximo_io: dispositivo da próxima operação de E/S.
 * tempo_io: duração da operação de E/S.
 *
 * Retorna:
 * O ponteiro para o PCB criado.
 * NULL caso ocorra algum erro.
 */
PCB *criar_processo(
    int ppid,
    int tempo_restante,
    int tempo_cpu_ate_io,
    DispositivoIO proximo_io,
    int tempo_io
) {
    // Valida o PPID.
    if (ppid < 0) {
        printf("[ERRO] PPID inválido!\n");
        return NULL;
    }

    // Valida o tempo total de CPU.
    if (tempo_restante <= 0) {
        printf("[ERRO] Tempo restante de CPU inválido!\n");
        return NULL;
    }

    // Valida o tempo de CPU até a próxima operação de E/S.
    if (tempo_cpu_ate_io < 0 || tempo_cpu_ate_io > tempo_restante) {
        printf("[ERRO] Tempo de CPU até E/S inválido!\n");
        return NULL;
    }

    // Valida o dispositivo de E/S informado.
    if (!dispositivo_valido(proximo_io)) {
        printf("[ERRO] Dispositivo de E/S inválido!\n");
        return NULL;
    }

    // Processos sem E/S não devem possuir tempo de CPU até E/S.
    if (proximo_io == NENHUM && tempo_cpu_ate_io != 0) {
        printf("[ERRO] Processo sem E/S não pode possuir tempo de CPU até E/S!\n");
        return NULL;
    }

    // Processos com E/S devem solicitar a operação antes de finalizar.
    if (proximo_io != NENHUM && tempo_cpu_ate_io >= tempo_restante) {
        printf("[ERRO] Operação de E/S deve ocorrer antes da finalização!\n");
        return NULL;
    }

    // Valida a duração da operação de E/S.
    if (tempo_io < 0) {
        printf("[ERRO] Tempo de E/S inválido!\n");
        return NULL;
    }

    // Processos sem E/S não devem possuir duração de E/S.
    if (proximo_io == NENHUM && tempo_io != 0) {
        printf("[ERRO] Processo sem E/S não pode possuir tempo de E/S!\n");
        return NULL;
    }

    // Processos com E/S devem possuir duração positiva.
    if (proximo_io != NENHUM && tempo_io <= 0) {
        printf("[ERRO] Processo com E/S deve possuir tempo de E/S positivo!\n");
        return NULL;
    }

    // Evita ultrapassar o limite de identificadores inteiros.
    if (proximo_pid == INT_MAX) {
        printf("[ERRO] Limite de PIDs atingido!\n");
        return NULL;
    }

    // Aloca memória para o PCB do processo.
    PCB *processo = malloc(sizeof(PCB));

    // Verifica se a alocação de memória falhou.
    if (processo == NULL) {
        printf("[ERRO] Falha ao alocar memória para o processo!\n");
        return NULL;
    }

    // Inicializa os campos do PCB.
    processo->pid = proximo_pid++;
    processo->ppid = ppid;
    processo->estado = NOVO;
    processo->prioridade = ALTA;
    processo->tempo_restante = tempo_restante;
    processo->tempo_cpu_ate_io = tempo_cpu_ate_io;
    processo->proximo_io = proximo_io;
    processo->tempo_io = tempo_io;

    printf("[SUCESSO] Processo PID %d criado!\n", processo->pid);

    return processo;
}

/*
 * Exibe as informações de um processo.
 *
 * processo: ponteiro para o PCB do processo.
 *
 * Retorna:
 * 1 se as informações forem exibidas.
 * 0 caso o processo seja inválido.
 */
int mostrar_processo(PCB *processo) {
    // Verifica se o processo existe.
    if (processo == NULL) {
        printf("[ERRO] Processo inválido!\n");
        return 0;
    }

    // Valida o estado do processo.
    if (!estado_valido(processo->estado)) {
        printf("[ERRO] Estado do processo inválido!\n");
        return 0;
    }

    // Valida a prioridade do processo.
    if (!prioridade_valida(processo->prioridade)) {
        printf("[ERRO] Prioridade do processo inválida!\n");
        return 0;
    }

    // Valida o dispositivo de E/S do processo.
    if (!dispositivo_valido(processo->proximo_io)) {
        printf("[ERRO] Dispositivo de E/S inválido!\n");
        return 0;
    }

    // Exibe as informações do processo.
    printf("\n========== PROCESSO ==========\n");
    printf("PID: %d\n", processo->pid);
    printf("PPID: %d\n", processo->ppid);
    printf("Estado: %d\n", processo->estado);
    printf("Prioridade: %d\n", processo->prioridade);
    printf("Tempo restante de CPU: %d\n", processo->tempo_restante);
    printf("Tempo CPU até E/S: %d\n", processo->tempo_cpu_ate_io);
    printf("Próxima operação de E/S: %d\n", processo->proximo_io);
    printf("Tempo de E/S: %d\n", processo->tempo_io);
    printf("=============================\n");

    return 1;
}

/*
 * Atualiza o estado de um processo.
 *
 * processo: ponteiro para o PCB do processo.
 * novo_estado: estado que será atribuído ao processo.
 *
 * Retorna:
 * 1 se o estado for atualizado.
 * 0 caso ocorra algum erro ou transição inválida.
 */
int atualizar_estado(PCB *processo, EstadoProcesso novo_estado) {
    // Verifica se o processo existe.
    if (processo == NULL) {
        printf("[ERRO] Processo inválido!\n");
        return 0;
    }

    // Valida o estado atual.
    if (!estado_valido(processo->estado)) {
        printf("[ERRO] Estado atual do processo inválido!\n");
        return 0;
    }

    // Valida o novo estado.
    if (!estado_valido(novo_estado)) {
        printf("[ERRO] Novo estado inválido!\n");
        return 0;
    }

    // Impede alterações em processos finalizados.
    if (processo->estado == FINALIZADO) {
        printf("[ERRO] Não é possível alterar o estado de um processo finalizado!\n");
        return 0;
    }

    // Verifica se a transição de estado é permitida.
    switch (processo->estado) {
        case NOVO:
            if (novo_estado != PRONTO) {
                printf("[ERRO] Transição de NOVO para estado inválida!\n");
                return 0;
            }
            break;

        case PRONTO:
            if (novo_estado != EXECUTANDO) {
                printf("[ERRO] Transição de PRONTO para estado inválida!\n");
                return 0;
            }
            break;

        case EXECUTANDO:
            if (novo_estado != PRONTO &&
                novo_estado != BLOQUEADO &&
                novo_estado != FINALIZADO) {
                printf("[ERRO] Transição de EXECUTANDO para estado inválida!\n");
                return 0;
            }
            break;

        case BLOQUEADO:
            if (novo_estado != PRONTO) {
                printf("[ERRO] Transição de BLOQUEADO para estado inválida!\n");
                return 0;
            }
            break;

        case FINALIZADO:
            return 0;
    }

    // Atualiza o estado do processo.
    processo->estado = novo_estado;

    printf("[SUCESSO] PID %d: Estado atualizado!\n", processo->pid);

    return 1;
}

/*
 * Atualiza a prioridade de um processo.
 *
 * processo: ponteiro para o PCB do processo.
 * nova_prioridade: prioridade que será atribuída.
 *
 * Retorna:
 * 1 se a prioridade for atualizada.
 * 0 caso ocorra algum erro.
 */
int atualizar_prioridade(PCB *processo, Prioridade nova_prioridade) {
    // Verifica se o processo existe.
    if (processo == NULL) {
        printf("[ERRO] Processo inválido!\n");
        return 0;
    }

    // Valida a nova prioridade.
    if (!prioridade_valida(nova_prioridade)) {
        printf("[ERRO] Nova prioridade inválida!\n");
        return 0;
    }

    // Impede alterações em processos finalizados.
    if (processo->estado == FINALIZADO) {
        printf("[ERRO] Não é possível alterar a prioridade de um processo finalizado!\n");
        return 0;
    }

    // Atualiza a prioridade do processo.
    processo->prioridade = nova_prioridade;

    printf("[SUCESSO] PID %d: Prioridade atualizada!\n", processo->pid);

    return 1;
}

/*
 * Atualiza o tempo restante de execução de um processo.
 *
 * processo: ponteiro para o PCB do processo.
 * tempo_executado: quantidade de tempo de CPU já utilizada pelo processo.
 *
 * Retorna:
 * 1 se o tempo restante for atualizado.
 * 0 caso ocorra algum erro.
 */
int atualizar_tempo_restante(PCB *processo, int tempo_executado) {
    // Verifica se o processo existe.
    if (processo == NULL) {
        printf("[ERRO] Processo inválido!\n");
        return 0;
    }

    // Valida se o tempo executado é positivo.
    if (tempo_executado <= 0) {
        printf("[ERRO] Tempo executado deve ser positivo!\n");
        return 0;
    }

    // Impede descontar mais tempo do que o processo possui.
    if (tempo_executado > processo->tempo_restante) {
        printf("[ERRO] Tempo executado maior que o tempo restante!\n");
        return 0;
    }

    // Atualiza o tempo restante de CPU.
    processo->tempo_restante -= tempo_executado;

    printf("[SUCESSO] PID %d: Tempo restante atualizado para %d!\n", processo->pid, processo->tempo_restante);

    return 1;
}
