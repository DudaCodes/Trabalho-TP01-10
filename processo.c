<<<<<<< HEAD
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "processo.h"

// Função para comparação de processos por ID (para qsort)
int comparar_por_id(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return p1->id - p2->id;
}

// Função para comparação de processos por data (para qsort)
int comparar_por_data(const void *a, const void *b) {
    Processo *p1 = (Processo *)a;
    Processo *p2 = (Processo *)b;
    return -strcmp(p1->data_ajuizamento, p2->data_ajuizamento);  // Negativo para ordem decrescente
}

//função para ler o arquivo CSV e carregar os dados dos processos
int ler_arquivo(Processo processos[], int *total, const char* nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo '%s': %s\n", nome_arquivo, strerror(errno));
        return 0;  // Retorna 0 para indicar falha
    }

    char linha[512];
    *total = 0;
    fgets(linha, sizeof(linha), arquivo); // Ignora cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (*total >= MAX_PROCESSOS) {    //verifica se atingiu o limite máximo de processos
            printf("Aviso: Limite máximo de processos (%d) atingido.\n", MAX_PROCESSOS);
            break;
        }

        // Variáveis para armazenar os dados temporariamente
        char id_str[10], numero[50], data[20], id_classe_str[10], assuntos_str[200], ano_str[10];
        int lidos = 0;
        
        //tenta ler os dados no primeiro formato
        lidos = sscanf(linha, "%[^,],\"%[^\"]\",%[^,],{%[^}]},{%[^}]%*c,%[^\n]", 
                       id_str, numero, data, id_classe_str, assuntos_str, ano_str);

        //se o primeiro formato falhar, tenta ler no segundo
        if (lidos != 6) {
            lidos = sscanf(linha, "%[^,],\"%[^\"]\",%[^,],{%[^}]},\"{%[^}]}\"%*c,%[^\n]", 
                           id_str, numero, data, id_classe_str, assuntos_str, ano_str);
        }

        if (lidos == 6) {
            processos[*total].id = atoi(id_str);
            strcpy(processos[*total].numero, numero);
            strcpy(processos[*total].data_ajuizamento, data);
            processos[*total].id_classe = atoi(id_classe_str);

            processos[*total].qtd_assuntos = 0;
            char *ptr = strtok(assuntos_str, ",");
            while (ptr && processos[*total].qtd_assuntos < MAX_ASSUNTOS) {
                processos[*total].id_assunto[processos[*total].qtd_assuntos++] = atoi(ptr);
                ptr = strtok(NULL, ",");
            }

            processos[*total].ano_eleicao = atoi(ano_str);
            (*total)++;
        } else {
            printf("Erro ao processar linha %d: %s\n", *total + 1, linha);
        }
    }

    fclose(arquivo);
    printf("Arquivo carregado com sucesso! Total de processos: %d\n", *total);
    return (*total > 0);  // Retorna 1 se carregou pelo menos um processo, 0 caso contrário
}

//função para ordenar os processos por ID em ordem crescente
void ordenar_por_id(Processo processos[], int total) {
    qsort(processos, total, sizeof(Processo), comparar_por_id);
    printf("Ordenado por ID (crescente).\n");
}

//função para ordenar os processos por data de ajuizamento em ordem decrescente
void ordenar_por_data(Processo processos[], int total) {
    qsort(processos, total, sizeof(Processo), comparar_por_data);
    printf("Ordenado por data de ajuizamento (decrescente).\n");
}

//função para contar o número de processos de uma classe específica
int contar_por_classe(Processo processos[], int total, int id_classe) {
    int cont = 0;
    for (int i = 0; i < total; i++) {
        if (processos[i].id_classe == id_classe)
            cont++;
    }
    return cont;
}

//função para contar o número de assuntos únicos
int contar_assuntos_unicos(Processo processos[], int total) {
    // Determina o maior ID de assunto para dimensionar o array
    int max_id_assunto = 0;
    for (int i = 0; i < total; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            if (processos[i].id_assunto[j] > max_id_assunto) {
                max_id_assunto = processos[i].id_assunto[j];
            }
        }
    }
    
    // Cria um array com tamanho adequado
    int *contador = calloc(max_id_assunto + 1, sizeof(int));
    if (!contador) {
        printf("Erro: Falha na alocação de memória para contagem de assuntos.\n");
        return -1;
    }
    
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
    
    free(contador);
    return total_unicos;
}

//função para listar os processos que possuem mais de um assunto
void listar_multiplos_assuntos(Processo processos[], int total) {
    printf("Processos com mais de um assunto:\n");
    int encontrados = 0;
    
    for (int i = 0; i < total; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("ID: %d | Numero: %s | Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].qtd_assuntos; j++) {
                printf("%d", processos[i].id_assunto[j]);
                if (j < processos[i].qtd_assuntos - 1) {
                    printf(", ");
                }
            }
            printf("\n");
            encontrados++;
        }
    }
    
    if (encontrados == 0) {
        printf("Nenhum processo com múltiplos assuntos encontrado.\n");
    } else {
        printf("Total de processos com múltiplos assuntos: %d\n", encontrados);
    }
}

// Função para calcular os dias em tramitação de um processo
int dias_em_tramitacao(const char *data_ajuizamento) {
    struct tm data = {0};
    
    // Usa sscanf para extrair os valores de ano, mês, dia, hora, minuto e segundo
    int campos_lidos = sscanf(data_ajuizamento, "%d-%d-%d %d:%d:%d",
               &data.tm_year, &data.tm_mon, &data.tm_mday,
               &data.tm_hour, &data.tm_min, &data.tm_sec);
    
    if (campos_lidos != 6) {
        printf("Erro ao converter data '%s'. Formato esperado: AAAA-MM-DD HH:MM:SS\n", data_ajuizamento);
        return -1; // Retorna um valor indicando erro
    }

    // Validação dos campos da data
    if (data.tm_year < 1900 || data.tm_mon < 1 || data.tm_mon > 12 || 
        data.tm_mday < 1 || data.tm_mday > 31) {
        printf("Erro: Data inválida '%s'\n", data_ajuizamento);
        return -1;
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
=======
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
>>>>>>> 661dd644065de6eaa7e85db02c6a460d5fbe7aba
