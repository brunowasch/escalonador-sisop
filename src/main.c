#include "processo.h"

int main() {
    // Criando os processos
    PCB processo1, processo2;
    criar_processo(&processo1, 0, ALTA, 10, DISCO, 3); 
    mostrar_processo(&processo1);
    criar_processo(&processo2, 0, BAIXA, 3, FITA, 4);
    mostrar_processo(&processo2);
}