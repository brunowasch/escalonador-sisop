// Verifica se PROCESSO_H ainda não foi definido.
#ifndef PROCESSO_H
// Define PROCESSO_H para indicar que este header já foi incluído.
#define PROCESSO_H

/*
 * Enumeração que representa os possíveis estados
 * de um processo durante sua execução no simulador.
 */
typedef enum {
    NOVO,        // Processo recém-criado.
    PRONTO,      // Processo aguardando execução na fila.
    EXECUTANDO,  // Processo atualmente em execução.
    BLOQUEADO,   // Processo aguardando uma operação de E/S.
    FINALIZADO  // Processo que concluiu sua execução.
} EstadoProcesso;

/*
 * Enumeração que representa os níveis de prioridade
 * utilizados pelo escalonador.
 */
typedef enum {
    ALTA,  // Prioridade alta.
    BAIXA  // Prioridade baixa.
} Prioridade;

/*
 * Enumeração que identifica o dispositivo da próxima
 * operação de entrada e saída (E/S) do processo.
 */
typedef enum {
    NENHUM,     // Processo não possui próxima operação de E/S.
    DISCO,      // Operação de E/S em disco.
    FITA,       // Operação de E/S em fita.
    IMPRESSORA  // Operação de E/S na impressora.
} DispositivoIO;

/*
 * PCB (Process Control Block):
 * estrutura responsável por armazenar as informações
 * necessárias para o gerenciamento de um processo.
 */
typedef struct {
    int pid;                    // Identificador único do processo.
    int ppid;                   // Identificador do processo pai.
    EstadoProcesso estado;      // Estado atual do processo.
    Prioridade prioridade;      // Prioridade atual do processo.
    int tempo_restante;         // Tempo de CPU restante para execução.
    DispositivoIO proximo_io;   // Próximo dispositivo de E/S.
    int tempo_io;               // Duração da próxima operação de E/S.
} PCB;

/*
 * Inicializa um PCB com os dados de um novo processo.
 *
 * processo: endereço do PCB que será inicializado.
 * ppid: identificador do processo pai.
 * prioridade: prioridade inicial do processo.
 * tempo_restante: tempo total de execução necessário.
 * proximo_io: dispositivo da próxima operação de E/S.
 * tempo_io: duração da próxima operação de E/S.
 */
void criar_processo(
    PCB *processo,
    int ppid,
    Prioridade prioridade,
    int tempo_restante,
    DispositivoIO proximo_io,
    int tempo_io
);

/*
 * Exibe os atributos de um PCB no terminal.
 *
 * processo: endereço do PCB que será apresentado.
 */
void mostrar_processo(PCB *processo);

// Indica o fim do include guard iniciado pelo #ifndef PROCESSO_H.
#endif