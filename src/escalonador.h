
#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "fila.h"
#include "processo.h"

/*
 * Estrutura responsável por armazenar
 * as informações do escalonador.
 */
typedef struct {
    Fila *fila_alta;
    Fila *fila_baixa;
    Fila *fila_io;

    int quantum;
    int tempo_simulacao;
    int processos_finalizados;
    int preempcoes;
} Escalonador;

/*
 * Estrutura que representa uma operação de E/S
 * em andamento.
 */
typedef struct {
    DispositivoIO tipo;
    int tempo_restante;
    PCB *processo;
} Dispositivo;

/*
 * Verifica se uma fila existe.
 *
 * fila: ponteiro para a fila.
 *
 * Retorna:
 * 1 se a fila for válida.
 * 0 caso contrário.
 */
int isFilaValida(Fila *fila);

/*
 * Cria e inicializa um escalonador.
 *
 * quantum: intervalo máximo de execução.
 *
 * Retorna:
 * Ponteiro para o escalonador criado.
 * NULL caso ocorra algum erro.
 */
Escalonador *criar_escalonador(int quantum);

/*
 * Libera a memória do escalonador.
 *
 * escalonador: endereço do ponteiro do escalonador.
 *
 * Retorna:
 * 1 se for liberado.
 * 0 caso ocorra algum erro.
 */
int liberar_escalonador(Escalonador **escalonador);

/*
 * Adiciona um processo novo ao escalonador.
 *
 * escalonador: ponteiro para o escalonador.
 * processo: ponteiro para o processo.
 *
 * Retorna:
 * 1 se for adicionado.
 * 0 caso ocorra algum erro.
 */
int adicionar_processo(Escalonador *escalonador, PCB *processo);

/*
 * Seleciona o próximo processo para execução.
 *
 * escalonador: ponteiro para o escalonador.
 *
 * Retorna:
 * Ponteiro para o processo selecionado.
 * NULL caso não exista processo disponível.
 */
PCB *selecionar_processo(Escalonador *escalonador);

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
int executar_processo(Escalonador *escalonador, PCB *processo);

/*
 * Gerencia a operação de E/S de um processo bloqueado.
 *
 * escalonador: ponteiro para o escalonador.
 * processo: ponteiro para o processo que realizará a E/S.
 *
 * Retorna 1 em caso de sucesso ou 0 em caso de erro.
 */
int gerenciar_io(Escalonador *escalonador, PCB *processo);

#endif
