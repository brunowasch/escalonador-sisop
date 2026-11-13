#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "historico.h"

/*
 * Cria e inicializa um histórico de eventos.
 *
 * Retorna:
 * Ponteiro para o histórico criado.
 * NULL caso ocorra algum erro.
 */
Historico *criar_historico(void) {
    // Aloca memória para o histórico.
    Historico *historico = malloc(sizeof(Historico));

    // Verifica se a alocação falhou.
    if (historico == NULL) {
        printf("[ERRO] Falha ao alocar histórico!\n");
        return NULL;
    }

    // Inicializa os atributos.
    historico->eventos = NULL;
    historico->quantidade = 0;
    historico->capacidade = 0;

    return historico;
}

/*
 * Registra um evento no histórico da simulação.
 *
 * historico: histórico que receberá o evento.
 * tempo: instante da simulação em que ocorreu.
 * pid: identificador do processo envolvido.
 * tipo: classificação do evento.
 * descricao: descrição textual do acontecimento.
 *
 * Retorna:
 * 1 se o evento for registrado.
 * 0 caso ocorra algum erro.
 */
int registrar_evento(
    Historico *historico,
    int tempo,
    int pid,
    TipoEvento tipo,
    const char *descricao
) {
    // Valida os parâmetros.
    if (historico == NULL || descricao == NULL || tempo < 0 || pid <= 0) {
        printf("[ERRO] Dados inválidos para registrar evento!\n");
        return 0;
    }

    // Verifica se o vetor precisa aumentar.
    if (historico->quantidade == historico->capacidade) {
        // Define a capacidade inicial ou dobra a capacidade atual.
        int nova_capacidade = historico->capacidade == 0
            ? 10
            : historico->capacidade * 2;

        // Aloca memória para os eventos.
        Evento *eventos = realloc(
            historico->eventos,
            nova_capacidade * sizeof(Evento)
        );

        // Verifica se a realocação falhou.
        if (eventos == NULL) {
            printf("[ERRO] Falha ao expandir histórico!\n");
            return 0;
        }

        // Atualiza o vetor e sua capacidade.
        historico->eventos = eventos;
        historico->capacidade = nova_capacidade;
    }

    // Obtém a posição do próximo evento.
    Evento *evento = &historico->eventos[historico->quantidade];

    // Inicializa os atributos do evento.
    evento->tempo = tempo;
    evento->pid = pid;
    evento->tipo = tipo;

    // Copia a descrição para dentro da estrutura.
    snprintf(evento->descricao, sizeof(evento->descricao), "%s", descricao);

    // Incrementa a quantidade de eventos registrados.
    historico->quantidade++;

    return 1;
}

/*
 * Traduz o tipo de evento para uma string.
 *
 * tipo: tipo de evento que será traduzido.
 *
 * Retorna:
 * Nome do evento ou "DESCONHECIDO" caso inválido.
 */
static const char *traduzir_evento(TipoEvento tipo) {
    switch (tipo) {
        case EVENTO_CRIACAO:
            return "CRIACAO";
        case EVENTO_EXECUCAO_CPU:
            return "CPU";
        case EVENTO_PREEMPCAO:
            return "PREEMPCAO";
        case EVENTO_BLOQUEIO_IO:
            return "BLOQUEIO_IO";
        case EVENTO_CONCLUSAO_IO:
            return "CONCLUSAO_IO";
        case EVENTO_FINALIZACAO:
            return "FINALIZACAO";
        default:
            return "DESCONHECIDO";
    }
}

/*
 * Exibe todos os eventos registrados no histórico.
 *
 * historico: histórico que será exibido.
 *
 * Retorna:
 * 1 se os eventos forem exibidos.
 * 0 caso o histórico seja inválido.
 */
int mostrar_historico(Historico *historico) {
    // Verifica se o histórico existe.
    if (historico == NULL) {
        printf("[ERRO] Histórico inválido!\n");
        return 0;
    }

    printf("\n========== HISTÓRICO ==========\n");

    // Percorre os eventos registrados.
    for (int i = 0; i < historico->quantidade; i++) {
        Evento *evento = &historico->eventos[i];

        printf(
            "[t=%d] [%s] PID %d: %s\n",
            evento->tempo,
            traduzir_evento(evento->tipo),
            evento->pid,
            evento->descricao
        );
    }

    printf("==============================\n");

    return 1;
}

/*
 * Libera a memória utilizada pelo histórico.
 *
 * historico: endereço do ponteiro do histórico.
 *
 * Retorna:
 * 1 se for liberado.
 * 0 caso ocorra algum erro.
 */
int liberar_historico(Historico **historico) {
    // Valida o ponteiro.
    if (historico == NULL || *historico == NULL)
        return 0;

    // Libera o vetor de eventos.
    free((*historico)->eventos);

    // Libera a estrutura do histórico.
    free(*historico);

    // Invalida o ponteiro original.
    *historico = NULL;

    return 1;
}