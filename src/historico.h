#ifndef HISTORICO_H
#define HISTORICO_H

/*
 * Tipos de eventos registrados durante a simulação.
 */
typedef enum {
    EVENTO_CRIACAO,
    EVENTO_EXECUCAO_CPU,
    EVENTO_PREEMPCAO,
    EVENTO_BLOQUEIO_IO,
    EVENTO_CONCLUSAO_IO,
    EVENTO_FINALIZACAO
} TipoEvento;

/*
 * Representa um evento ocorrido na simulação.
 */
typedef struct {
    int tempo;
    int pid;
    TipoEvento tipo;
    char descricao[128];
} Evento;

/*
 * Histórico de eventos da simulação.
 */
typedef struct {
    Evento *eventos;
    int quantidade;
    int capacidade;
} Historico;

Historico *criar_historico(void);

int registrar_evento(
    Historico *historico,
    int tempo,
    int pid,
    TipoEvento tipo,
    const char *descricao
);

int mostrar_historico(Historico *historico);

int liberar_historico(Historico **historico);

#endif