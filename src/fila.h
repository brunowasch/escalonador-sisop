#ifndef FILA_H
#define FILA_H

#include "processo.h"

/*
 * Struct para representar um nó da fila de processos em uma lista encadeada.
 *
 * processo: ponteiro para o PCB do processo armazenado no nó.
 * proximo: ponteiro para o próximo nó na fila.
 * 
 */
typedef struct No {
    PCB *processo;
    struct No *proximo;
} No;

/**
 * Struct para representar a fila de processos.
 * inicio: ponteiro para o primeiro nó da fila.
 * fim: ponteiro para o último nó da fila.
 */
typedef struct {
    No *inicio;
    No *fim;
} Fila;

/*
 * Funções para manipulação da fila de processos.
 */
Fila *criar_fila();

/*
* Verifica se a fila está vazia.
*
* fila: ponteiro para a fila a ser verificada.
*
* Retorna 1 se a fila estiver vazia, caso contrário retorna 0.
*/
int fila_vazia(Fila *fila);

/*
* Adiciona um processo à fila.
*
* fila: ponteiro para a fila onde o processo será adicionado.
* processo: ponteiro para o PCB do processo a ser adicionado.
*/
int enfileirar(Fila *fila, PCB *processo);

/*
* Remove um processo da fila.
*
* fila: ponteiro para a fila onde o processo será removido.
*
* Retorna um ponteiro para o PCB do processo removido, ou NULL se a fila estiver vazia.
*/
PCB *desenfileirar(Fila *fila);

/*
* Exibe os processos na fila.
*
* fila: ponteiro para a fila a ser exibida.
*/
void mostrar_fila(Fila *fila);

/*
* Libera a memória ocupada pela fila.
*
* fila: ponteiro para a fila a ser liberada.
*/
void liberar_fila(Fila *fila);

#endif