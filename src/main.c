#include <stdio.h>
#include <stdlib.h>
#include "processo.h"
#include "fila.h"

int main(void) {
    // Criando os processos
    PCB *processo1, *processo2;

    if (!criar_processo(&processo1, 0, 10, DISCO, 3)) {
        printf("[FALHA] Não foi possível criar o processo 1.\n");
        return 1;
    }

    if (!criar_processo(&processo2, 0, 3, FITA, 4)) {
        printf("[FALHA] Não foi possível criar o processo 2.\n");
        free(processo1);
        return 1;
    }

    // Criando a fila
    Fila *fila = criar_fila();

    if (fila == NULL) {
        printf("[FALHA] Não foi possível criar a fila.\n");
        free(processo1);
        free(processo2);
        return 1;
    }

    if (fila_vazia(fila)) {
        printf("[SUCESSO] Fila criada e vazia.\n");
    } else {
        printf("[FALHA] A fila deveria estar vazia.\n");
        liberar_fila(fila);
        free(processo1);
        free(processo2);
        return 1;
    }

    // Enfileirando os processos
    enfileirar(fila, processo1);
    enfileirar(fila, processo2);

    if (!fila_vazia(fila)) {
        printf("[SUCESSO] Processos enfileirados.\n");
    } else {
        printf("[FALHA] Os processos não foram enfileirados.\n");
        liberar_fila(fila);
        free(processo1);
        free(processo2);
        return 1;
    }

    // Testando FIFO
    PCB *removido1 = desenfileirar(fila);
    PCB *removido2 = desenfileirar(fila);

    if (removido1 == processo1 && removido2 == processo2) {
        printf("[SUCESSO] Ordem FIFO validada.\n");
    } else {
        printf("[FALHA] A ordem FIFO não foi respeitada.\n");
        liberar_fila(fila);
        free(processo1);
        free(processo2);
        return 1;
    }

    if (fila_vazia(fila)) {
        printf("[SUCESSO] Fila vazia após remoções.\n");
    } else {
        printf("[FALHA] A fila deveria estar vazia após remoções.\n");
        liberar_fila(fila);
        free(processo1);
        free(processo2);
        return 1;
    }

    // Liberando memória
    liberar_fila(fila);
    free(processo1);
    free(processo2);

    return 0;
}
