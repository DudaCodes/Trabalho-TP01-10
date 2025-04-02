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
