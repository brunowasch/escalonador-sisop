# Trabalho Final de Sistemas Operacionais

Simulador didático de escalonamento de processos em C, usando Round Robin com feedback entre filas de prioridade.

## Grupo

- Aluno 1: Artur Emmert
- Aluno 2: Bruno Waschburger Silva
- Aluno 3: Douglas Soares Paz
- Aluno 4: Richard Carcuchinski Vargas
- Aluno 5: Vicente Pacheco Machado

## Linguagem utilizada

C.

## Premissas do escalonador

- Quantum: 3 unidades de tempo (definido em `src/main.c`, podendo ser alterado).
- Número máximo de processos nesta execução: 4 (processos de cenário fixo definidos em `src/main.c`).
- Tempos de CPU: 8, 5, 10 e 6 unidades para P1 a P4, respectivamente.
- Tempos de CPU até o único evento de E/S: 3 para P1, 4 para P3 e 2 para P4. P2 não solicita E/S.
- Dispositivos e durações: disco por 2 unidades, fita por 3 unidades e impressora por 2 unidades.
- Critério de criação: os quatro processos de demonstração são criados no início da simulação, com PPID 0.
- Critério de finalização: a simulação termina quando todos os processos tiverem concluído seu tempo de CPU.
- Fila alta tem preferência. Processos novos entram nela; processos que esgotam o quantum sem finalizar ou solicitar E/S são rebaixados para a fila baixa.
- Após E/S, disco retorna à fila baixa; fita e impressora retornam à fila alta.

## Como executar o projeto

Com GCC instalado:

```bash
gcc src/*.c -o escalonador
./escalonador
```

Execute o binário gerado com `escalonador.exe`.

### Docker (bônus)

```bash
docker build -t so-escalonador-grupo-3 .
docker run --rm so-escalonador-grupo-3
```

O Dockerfile compila os arquivos C de `src/` com GCC e inicia o simulador ao executar o container.

## O que aparece na saída

O histórico apresenta eventos com o instante simulado e o PID: criação, unidades de CPU executadas, preempção, bloqueio e conclusão de E/S e finalização. Ao final, o resumo informa tempo total, processos finalizados, preempções, quantidade de E/S por dispositivo, tempo médio de espera, turnaround médio e tempo/percentual de CPU ociosa.

As médias consideram apenas processos finalizados. Turnaround é o tempo entre a entrada do processo no escalonador e sua finalização. O tempo de espera é calculado como turnaround menos CPU e E/S acumuladas. Como todos chegam no instante zero e a E/S é serial, essa conta corresponde ao tempo acumulado em filas de prontos neste modelo.

## Simplificações e limitações conhecidas

- A E/S é síncrona no simulador: quando um processo solicita uma operação, o relógio avança pela duração inteira dela antes de o escalonador selecionar outro processo. Assim, a fila de E/S representa o estado bloqueado, mas não há operações concorrentes, dispositivos independentes nem execução de outro processo durante a E/S.
- Cada processo da demonstração solicita no máximo uma operação de E/S. Ao retornar, seus campos de E/S são limpos e ele continua apenas com o tempo de CPU restante.
- A prioridade alta sempre tem preferência sobre a baixa; não há envelhecimento de processos nem mecanismo contra possível espera prolongada na fila baixa.
- O cenário é fixo no código: não há leitura de arquivo, entrada interativa ou geração aleatória de processos.
- A porcentagem de CPU ociosa usa o tempo total transcorrido menos o tempo total de CPU. Neste modelo serial, o tempo de E/S também deixa a CPU ociosa.

## Relatório

O relatório do grupo será incluído no repositório separadamente, explicando as escolhas do algoritmo, as premissas e as simplificações descritas aqui.
