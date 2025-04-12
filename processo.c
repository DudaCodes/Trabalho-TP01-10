#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

void ler_arquivo(Processo processos[], int *total) {
    FILE *arquivo = fopen("processo_043_202409032338.csv", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[512];
    *total = 0;
    fgets(linha, sizeof(linha), arquivo); // Ignora cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (*total >= MAX_PROCESSOS) {    //verifica o limite
            printf("Aviso: Limite máximo de processos atingido.\n");
            break;
        }
        //remove o caractere de nova linha
        char *token = strtok(linha, ",");
        processos[*total].id = atoi(token);

        token = strtok(NULL, ",");
        strcpy(processos[*total].numero, token);

        token = strtok(NULL, ",");
        strcpy(processos[*total].data_ajuizamento, token);

        token = strtok(NULL, ",");
        processos[*total].id_classe = atoi(token + 1); // pula '{'

        token = strtok(NULL, ",");
        processos[*total].qtd_assuntos = 0;
        char *ptr = strtok(token, "{}");
        while (ptr && processos[*total].qtd_assuntos < MAX_ASSUNTOS) {
            processos[*total].id_assunto[processos[*total].qtd_assuntos++] = atoi(ptr);
            ptr = strtok(NULL, "{}");
        }

        token = strtok(NULL, ",");
        processos[*total].ano_eleicao = atoi(token);

        (*total)++;
    }

    fclose(arquivo);
    printf("Arquivo carregado com sucesso! Total de processos: %d\n", *total);
}

void ordenar_por_id(Processo processos[], int total) {
    for (int i = 0; i < total - 1; i++) {
        int min = i;
        for (int j = i + 1; j < total; j++) {
            if (processos[j].id < processos[min].id) {
                min = j;
            }
        }
        if (min != i) {
            Processo temp = processos[i];
            processos[i] = processos[min];
            processos[min] = temp;
        }
    }
    printf("Ordenado por ID (crescente).\n");
}

void ordenar_por_data(Processo processos[], int total) {
    for (int i = 0; i < total - 1; i++) {
        int max = i;
        for (int j = i + 1; j < total; j++) {
            if (strcmp(processos[j].data_ajuizamento, processos[max].data_ajuizamento) > 0) {
                max = j;
            }
        }
        if (max != i) {
            Processo temp = processos[i];
            processos[i] = processos[max];
            processos[max] = temp;
        }
    }
    printf("Ordenado por data de ajuizamento (decrescente).\n");
}

int contar_por_classe(Processo processos[], int total, int id_classe) {
    int cont = 0;
    for (int i = 0; i < total; i++) {
        if (processos[i].id_classe == id_classe)
            cont++;
    }
    return cont;
}

int contar_assuntos_unicos(Processo processos[], int total) {
    int contador[20000] = {0};
    int total_unicos = 0;
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int assunto = processos[i].id_assunto[j];
            if (!contador[assunto]) {
                contador[assunto] = 1;
                total_unicos++;
            }
        }
    }
    return total_unicos;
}

void listar_multiplos_assuntos(Processo processos[], int total) {
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < total; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("ID: %d | Numero: %s | Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].qtd_assuntos; j++) {
                printf("%d ", processos[i].id_assunto[j]);
            }
            printf("\n");
        }
    }
}

int dias_em_tramitacao(const char *data_ajuizamento) {
    struct tm data = {0};
    if (strptime(data_ajuizamento, "%Y-%m-%d %H:%M:%S", &data) ==NULL) {
        printf("Erro ao converter data.\n");
        return -1; //retorna um valor indicando erro
    }
    time_t inicio = mktime(&data);
    time_t hoje = time(NULL);
    double segundos = difftime(hoje, inicio);
    return (int)(segundos / (60 * 60 * 24));
}
