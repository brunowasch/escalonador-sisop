#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

/*
 * Cria uma nova fila de processos.
 *
 * Retorna um ponteiro para a fila criada, ou NULL se a alocação falhar.
 */
Fila *criar_fila() {
    // Aloca memória para a fila
    Fila *fila = malloc(sizeof(Fila)); 

    // Se a alocação falhar, retorna NULL
    if (fila == NULL)
        return NULL;

    // Inicializa os ponteiros de início e fim da fila como NULL
    fila->inicio = NULL;
    fila->fim = NULL;

    // Retorna o ponteiro para a fila criada
    return fila;
}

/*
 * Verifica se a fila está vazia.
 *
 * fila: ponteiro para a fila a ser verificada.
 *
 * Retorna 1 se a fila estiver vazia, caso contrário retorna 0.
 */
int fila_vazia(Fila *fila) {
    return fila->inicio == NULL && fila->fim == NULL;
}

/*
 * Adiciona um processo à fila.
 *
 * fila: ponteiro para a fila onde o processo será adicionado.
 * processo: ponteiro para o PCB do processo a ser adicionado.
 */
void enfileirar(Fila *fila, PCB *processo) {
    // Aloca memória para um novo nó da fila
    No *no = malloc(sizeof(No));

    // Verifica se a alocação falhou
    if (no == NULL) 
        return;

    // Inicializa o nó com o processo e define o próximo como NULL
    no->processo = processo;
    no->proximo = NULL;
    
    // Se a fila estiver vazia, o novo nó será o início e o fim da fila
    if (fila_vazia(fila)) {
        fila->inicio = no;
        fila->fim = no;
    }
    // Caso contrário, adiciona o nó ao final da fila e atualiza o ponteiro do fim 
    else {
        fila->fim->proximo = no;
        fila->fim = no;
    }
}

/*
 * Remove um processo da fila.
 *
 * fila: ponteiro para a fila onde o processo será removido.
 *
 * Retorna um ponteiro para o PCB do processo removido, ou NULL se a fila estiver vazia.
 */
PCB *desenfileirar(Fila *fila) {
    // Verifica se a fila está vazia
    if (fila_vazia(fila))
        return NULL;

    // Armazena o nó do início da fila e o processo associado a ele
    No *noAux = fila->inicio;
    // Atualiza o início da fila para o próximo nó
    PCB *processo = noAux->processo;
    fila->inicio = noAux->proximo;
    // Libera a memória do nó removido
    free(noAux);
    // Se a fila ficar vazia após a remoção, atualiza o ponteiro do fim para NULL
    if (fila->inicio == NULL) 
        fila->fim = NULL;
    // Retorna o ponteiro para o PCB do processo removido
    return processo;
}

/*
 * Mostra os processos da fila.
 *
 * fila: ponteiro para a fila a ser mostrada.
 */
void mostrar_fila(Fila *fila) {
    // Inicializa um ponteiro auxiliar para percorrer a fila
    No *noAux = fila->inicio;
    // Verifica se a fila está vazia e exibe uma mensagem apropriada
    if (noAux == NULL)  {
        printf("A fila de processos está vazia!\n");
        return;
    }
    // Percorre a fila e exibe os processos até o final
    while (noAux != NULL) {
        mostrar_processo(noAux->processo);
        noAux = noAux->proximo;
    }
}

/*
* Libera a memória ocupada pela fila.
*
* fila: ponteiro para a fila a ser liberada.
*/
void liberar_fila(Fila *fila) {
    // Verifica se o ponteiro da fila é válido
    if (fila == NULL)
        return;
    // Inicializa um ponteiro auxiliar para percorrer a fila e liberar cada nó
    No *noAux = fila->inicio;

    // Percorre a fila e libera cada nó até o final
    while(noAux != NULL) {
        No *proximo = noAux->proximo;
        free(noAux);
        noAux = proximo;
    }
    // Libera a memória da estrutura da fila
    free(fila);
}