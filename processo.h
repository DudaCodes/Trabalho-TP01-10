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

#endif 
// Fim do arquivo processo.h