#ifndef PROCESSO_H  //impede que o arquivo seja incluído mais de uma vez
#define PROCESSO_H

//struct para armazenar os dados do csv
typedef struct {
    int id;
    char numero[30];
    char data_ajuizamento[15];
    int id_classe;
    int id_assunto[10]; // Suporte para múltiplos id_assunto
    int ano_eleicao;
} Processo;

// Declaração da função de leitura do arquivo
void ler_arquivo(Processo processos[], int *total);
void ordenar_por_id(Processo *processos, int num_processos);
void ordenar_por_data(Processo *processos, int num_processos);
int contar_por_classe(Processo *processos, int num_processos, int id_classe);
int contar_assuntos_unicos(Processo *processos, int num_processos);
void listar_multiplos_assuntos(Processo *processos, int num_processos);
int calcular_dias_em_tramitacao(const char *data_ajuizamento);
void salvar_csv(const char *nome_arquivo, Processo *processos, int num_processos);

#endif 
// Fim do arquivo processo.h