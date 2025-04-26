#ifndef PROCESSO_H 
#define PROCESSO_H

#define MAX_PROCESSOS 200000
#define MAX_STR 50
#define MAX_ASSUNTOS 10

// Estrutura para armazenar os dados do processo
typedef struct {
    int id;
    char numero[MAX_STR];
    char data_ajuizamento[MAX_STR];
    int id_classe;
    int id_assunto[MAX_ASSUNTOS];
    int qtd_assuntos;
    int ano_eleicao;
} Processo;

// Funções para manipulação de processos
int ler_arquivo(Processo *processos, int *total, const char* nome_arquivo);
void ordenar_por_id(Processo processos[], int total);
void ordenar_por_data(Processo processos[], int total);
int contar_por_classe(Processo processos[], int total, int id_classe);
int contar_assuntos_unicos(Processo processos[], int total);
void listar_multiplos_assuntos(Processo processos[], int total);
int dias_em_tramitacao(const char *data_ajuizamento);
void salvar_processos_csv(Processo processos[], int total, const char* nome_arquivo);


#endif
