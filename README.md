# Trabalho Final de Sistemas Operacionais

Este projeto é um simulador de escalonamento de processos, implementando a estratégia **Round Robin com Feedback**, utilizando a linguagem de programação C.

## Grupo
- Aluno 1: Artur Emmert
- Aluno 2: Bruno Waschburger Silva
- Aluno 3: Douglas Soares Paz
- Aluno 4: Richard Carcuchinski Vargas
- Aluno 5: Vicente Pacheco Machado

## Linguagem utilizada
<img alt="C" title="C" width="40px" src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/c/c-original.svg"/>

## Premissas do escalonador
- Quantum:
- Número máximo de processos:
- Tempos de CPU:
- Tempos de I/O:
- Critério de geração dos processos:
- Semente aleatória, se aplicável:

## Como executar o projeto
```bash
# Execução em C:
gcc src/*.c -o escalonador
./escalonador

# Utilizando Docker:
docker build -t so-escalonador-grupo-3 .
docker run --rm so-escalonador-grupo-3
```

## O que aparece na saída
Explique brevemente os eventos impressos pelo simulador.

## Limitações conhecidas
Liste pontos que o grupo não conseguiu implementar ou simplificações realizadas.