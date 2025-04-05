#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

//função para ler o arquivo CSV e armazenar os dados em uma estrutura Processo
void ler_arquivo(Processo processos[], int *total) {
    FILE *arquivo = fopen("processo_043_202409032338.csv", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[200];
    *total = 0;

    //Ignorar o cabecalho do arquivo CSV
    fgets(linha, sizeof(linha), arquivo);

    //ler cada linha do arquivo e armazenar os dados na estrutura Processo
    while (fgets(linha, sizeof(linha), arquivo)) {
        sscanf(linha, "%d,%[^,],%[^,],%d,{%d},%d",
               &processos[*total].id,
               processos[*total].numero,
               processos[*total].data_ajuizamento,
               &processos[*total].id_classe,
               &processos[*total].id_assunto[0], //suporta apenas um id_assunto por enquanto
               &processos[*total].ano_eleicao);
        (*total)++;
    }

    fclose(arquivo);
    printf("Arquivo carregado com sucesso! Total de processos: %d\n", *total); //exibe o total de processos lidos
}

void ordenar_por_id(Processo processos[], int *total) {
    for (int i = 0; i < total - 1; i++) { 
        int minIndex = i;
        
        for (int j = i + 1; j < total; j++) { 
            if (processos[j].id < processos[minIndex].id) {
                minIndex = j;
            }
        }
        
        if (minIndex != i) {
            Processo temp = processos[i];
            processos[i] = processos[minIndex];
            processos[minIndex] = temp;
        }
        printf("Processos ordenados por id (crescente).\n");
    }
}

void data_juizamento(Processo processos[], int *total) {
        for (int i = 0; i < total - 1; i++) {
            int maxIndex = i;
    
            for (int j = i + 1; j < total; j++) {
                if (strcmp(processos[j].data_ajuizamento, processos[maxIndex].data_ajuizamento) > 0) {
                    maxIndex = j;
                }
            }
    
            if (maxIndex != i) {
                Processo temp = processos[i];
                processos[i] = processos[maxIndex];
                processos[maxIndex] = temp;
            }
        }
    
        printf("Processos ordenados por data de ajuizamento (decrescente).\n");
    }