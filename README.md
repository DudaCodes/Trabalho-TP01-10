# Trabalho de Programação - TP01

Este projeto foi desenvolvido como parte do Trabalho Prático 01 da disciplina de Estrutura de Dados, com o objetivo de aplicar conceitos de estruturas de dados e manipulação de arquivos em C.

## 📁 Estrutura do Projeto

- `processo.c`: Arquivo com a implementação das funções de ordenação dos processos.
- `processo.h`: Cabeçalho com definições e protótipos das funções.
- `main.c`: Arquivo principal que chama as funções de leitura, ordenação e impressão dos processos.
- `processo_043_202409032338.csv`: Arquivo de entrada com os dados dos processos.


## ⚙️ Funcionalidades

- Leitura de processos a partir de um arquivo CSV.
- Ordenação dos processos:
  - Por ID (em ordem crescente)
  - Por data de criação (em ordem crescente)

## 🚀 Como compilar e executar

No terminal, execute os comandos:

```bash
gcc -o programa main.c processo.c
./programa
