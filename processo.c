#include "processo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int ler_arquivo(Processo *processos, int *total, const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return 0; // Retorna 0 em caso de erro
    }

    char linha[1024];
    *total = 0;
    fgets(linha, sizeof(linha), arquivo); // Ignora cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (*total >= MAX_PROCESSOS) {    // Verifica o limite
            printf("Aviso: Limite máximo de processos atingido.\n");
            break;
        }

        char *token = strtok(linha, ",");
        if (!token) continue;
        processos[*total].id = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(processos[*total].numero, token, MAX_STR - 1);
        processos[*total].numero[MAX_STR - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(processos[*total].data_ajuizamento, token, MAX_STR - 1);
        processos[*total].data_ajuizamento[MAX_STR - 1] = '\0';

        token = strtok(NULL, ",");
        if (token && token[0] == '{') {
            processos[*total].id_classe = atoi(token + 1); // Ignora '{'
        }

        token = strtok(NULL, ",");
        processos[*total].qtd_assuntos = 0;
        if (token) {
            char *ptr = strtok(token, "{}");
            while (ptr && processos[*total].qtd_assuntos < MAX_ASSUNTOS) {
                processos[*total].id_assunto[processos[*total].qtd_assuntos++] = atoi(ptr);
                ptr = strtok(NULL, "{}");
            }
        }

        token = strtok(NULL, ",");
        if (token) {
            processos[*total].ano_eleicao = atoi(token);
        }

        (*total)++;
    }

    fclose(arquivo);
    printf("Arquivo carregado com sucesso! Total de processos: %d\n", *total);
    return 1;
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
    printf("Processos ordenados por ID (crescente).\n");
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
    printf("Processos ordenados por data de ajuizamento (decrescente).\n");
}

int contar_por_classe(Processo processos[], int total, int id_classe) {
    int contador = 0;
    for (int i = 0; i < total; i++) {
        if (processos[i].id_classe == id_classe) {
            contador++;
        }
    }
    return contador;
}

int contar_assuntos_unicos(Processo processos[], int total) {
    int assuntos[MAX_ASSUNTOS * total];
    int qtd_assuntos = 0;

    for (int i = 0; i < total; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int assunto = processos[i].id_assunto[j];
            int encontrado = 0;
            for (int k = 0; k < qtd_assuntos; k++) {
                if (assuntos[k] == assunto) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                assuntos[qtd_assuntos++] = assunto;
            }
        }
    }
    return qtd_assuntos;
}

void listar_multiplos_assuntos(Processo processos[], int total) {
    printf("Processos com múltiplos assuntos:\n");
    for (int i = 0; i < total; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("ID: %d, Número: %s, Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].qtd_assuntos; j++) {
                printf("%d ", processos[i].id_assunto[j]);
            }
            printf("\n");
        }
    }
}

int dias_em_tramitacao(const char *data_ajuizamento) {
    struct tm data = {0};
    if (sscanf(data_ajuizamento, "%d-%d-%d %d:%d:%d",
               &data.tm_year, &data.tm_mon, &data.tm_mday,
               &data.tm_hour, &data.tm_min, &data.tm_sec) != 6) {
        printf("Erro ao converter data: %s\n", data_ajuizamento);
        return -1; // Retorna um valor indicando erro
    }

    // Ajusta os valores para o formato esperado por mktime
    data.tm_year -= 1900; // Ajusta o ano (baseado em 1900)
    data.tm_mon -= 1;     // Ajusta o mês (0-11)

    time_t inicio = mktime(&data); // Converte a data para timestamp
    if (inicio == -1) {
        printf("Erro ao calcular o timestamp para '%s'\n", data_ajuizamento);
        return -1;
    }

    time_t hoje = time(NULL); // Obtém o timestamp atual
    double segundos = difftime(hoje, inicio); // Calcula a diferença em segundos
    return (int)(segundos / (60 * 60 * 24)); // Converte para dias
}

void salvar_processos_csv(Processo processos[], int total, const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo para escrita: %s\n", nome_arquivo);
        return;
    }

    fprintf(arquivo, "ID,Número,Data de Ajuizamento,Classe,Assuntos,Ano Eleição\n");
    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%d,%s,%s,%d,\"{",
                processos[i].id,
                processos[i].numero,
                processos[i].data_ajuizamento,
                processos[i].id_classe);

        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            fprintf(arquivo, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].qtd_assuntos - 1) {
                fprintf(arquivo, ",");
            }
        }

        fprintf(arquivo, "}\",%d\n", processos[i].ano_eleicao);
    }

    fclose(arquivo);
    printf("Processos salvos com sucesso no arquivo: %s\n", nome_arquivo);
}