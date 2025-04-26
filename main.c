#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "processo.h"

#define NOME_ARQUIVO "processo_043_202409032338.csv"

void limpa_chaves(char *str) {
    int len = strlen(str);
    if (str[0] == '{') {
        memmove(str, str + 1, len - 1);
        str[len - 2] = '\0';  // remove a chave final
    }
}

int main() {
    Processo *processos = malloc(sizeof(Processo) * MAX_PROCESSOS);
    if (processos == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;  // Se não conseguir alocar, sai do programa
    }

    int total = 0;  
    int opcao;  

    // Carrega o arquivo e verifica se houve erro
    if (!ler_arquivo(processos, &total, NOME_ARQUIVO)) {    
        printf("Erro: Nenhum processo foi carregado. Verifique se o arquivo '%s' existe e tem permissões adequadas.\n", NOME_ARQUIVO);
        free(processos);  // Libera a memória antes de sair
        return 1;  
    } 

    do {
        // Exibe o menu de opções
        printf("\nMenu de Opções:\n");
        printf("1. Ordenar por ID (crescente)\n");
        printf("2. Ordenar por data de ajuizamento (decrescente)\n");
        printf("3. Contar processos por id_classe\n");
        printf("4. Contar assuntos únicos\n");
        printf("5. Listar processos com múltiplos assuntos\n");
        printf("6. Ver dias em tramitação de um processo\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        
        int tentativas = 3; // Número de tentativas para entrada inválida
        while (tentativas > 0) {
            if (scanf("%d", &opcao) == 1) {
                break; // Entrada válida, sai do loop
            }
            printf("Erro: Entrada inválida! Você tem %d tentativa(s) restante(s).\n", --tentativas);
            while (getchar() != '\n'); // Limpa o buffer
        }
        
        if (tentativas == 0) {  // Excede o número de tentativas
            printf("Erro: Número máximo de tentativas excedido. Encerrando o programa.\n");
            free(processos);  // Libera a memória antes de sair
            return 1;
        }

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, total);
                salvar_processos_csv(processos, total, "processos_ordenados.csv");
                printf("Primeiros 5 processos após ordenação por ID:\n");
                for (int i = 0; i < 5 && i < total; i++) {
                    printf("ID: %d | Número: %s\n", processos[i].id, processos[i].numero);
                }
                break;

            case 2:
                ordenar_por_data(processos, total);
                salvar_processos_csv(processos, total, "processos_ordenados_data.csv");
                printf("Primeiros 5 processos após ordenação por data:\n");
                for (int i = 0; i < 5 && i < total; i++) {
                    printf("Data: %s | ID: %d\n", processos[i].data_ajuizamento, processos[i].id);
                }
                break;

            case 3: {
                int id;
                printf("Digite o id_classe: ");
                if (scanf("%d", &id) == 1) {
                    int c = contar_por_classe(processos, total, id);
                    printf("Total de processos com id_classe %d: %d\n", id, c);
                } else {
                    printf("Entrada inválida.\n");
                }
                break;
            }

            case 4: {
                int unicos = contar_assuntos_unicos(processos, total);
                printf("Total de assuntos únicos: %d\n", unicos);
                break;
            }

            case 5:
                listar_multiplos_assuntos(processos, total);
                break;

            case 6: {
                int index;
                printf("Digite o índice do processo (0 a %d): ", total - 1);
                if (scanf("%d", &index) == 1 && index >= 0 && index < total) {
                    int dias = dias_em_tramitacao(processos[index].data_ajuizamento);
                    if (dias >= 0) {
                        printf("Dias em tramitação: %d\n", dias);
                    } else {
                        printf("Erro ao calcular dias.\n");
                    }
                } else {
                    printf("Índice inválido.\n");
                }
                break;
            }

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    free(processos);  // Libera a memória antes de sair
    return 0;
}
