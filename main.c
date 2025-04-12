#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

#define NOME_ARQUIVO "processo_043_202409032338.csv"

int main() {
    Processo processos[MAX_PROCESSOS];  // Definindo o tamanho máximo de processos
    int total = 0;  
    int opcao;  

    // Carrega o arquivo e verifica se houve erro
    if (!ler_arquivo(processos, &total, NOME_ARQUIVO)) {    
        printf("Erro: Nenhum processo foi carregado. Verifique se o arquivo '%s' existe e tem permissões adequadas.\n", NOME_ARQUIVO);
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
            return 1;
        }

        // Executa a opção escolhida
        switch(opcao) {
            case 1:
                ordenar_por_id(processos, total);
                printf("Primeiros 5 processos após ordenação:\n");
                for (int i = 0; i < 5 && i < total; i++) {
                    printf("ID: %d | Número: %s\n", processos[i].id, processos[i].numero);
                }
                break;
                
            case 2:
                ordenar_por_data(processos, total);
                printf("Primeiros 5 processos após ordenação:\n");
                for (int i = 0; i < 5 && i < total; i++) {
                    printf("Data: %s | ID: %d\n", processos[i].data_ajuizamento, processos[i].id);
                }
                break;
                
            case 3: {
                int classe;
                printf("Digite o id_classe: ");
                if (scanf("%d", &classe) != 1) {
                    printf("Erro: Entrada inválida para id_classe.\n");
                    while (getchar() != '\n'); // Limpa o buffer
                    break;
                }
                int count = contar_por_classe(processos, total, classe);
                printf("Total de processos com id_classe %d: %d\n", classe, count);
                break;
            }
            
            case 4: {
                int totalAssuntos = contar_assuntos_unicos(processos, total);
                if (totalAssuntos >= 0) {
                    printf("Total de assuntos únicos: %d\n", totalAssuntos);
                }
                break;
            }
            
            case 5:
                listar_multiplos_assuntos(processos, total);
                break;
                
            case 6: {
                int index;
                int tentativas = 3;  // Tentativas para entrada inválida
                while (tentativas > 0) {
                    printf("Digite o índice do processo (entre 0 e %d): ", total - 1);
                    if (scanf("%d", &index) == 1) {
                        if (index >= 0 && index < total) {
                            int dias = dias_em_tramitacao(processos[index].data_ajuizamento);
                            if (dias >= 0) {
                                printf("O processo %s está em tramitação há %d dias.\n", 
                                       processos[index].numero, dias);
                            } else {
                                printf("Erro ao calcular os dias em tramitação para o processo de índice %d.\n", index);
                            }
                            break;
                        } else {
                            printf("Erro: Índice fora do intervalo válido (0 a %d).\n", total - 1);
                        }
                    } else {
                        printf("Erro: Entrada inválida! Certifique-se de digitar um número.\n");
                        while (getchar() != '\n'); // Limpa o buffer
                    }
                    printf("Você tem %d tentativa(s) restante(s).\n", --tentativas);
                }
                if (tentativas == 0) {  // Excede o número de tentativas
                    printf("Número máximo de tentativas excedido. Voltando ao menu principal.\n");
                }
                break;
            }
            
            case 0:
                // Encerra o programa
                printf("\n=============================\n");
                printf("Programa encerrado. Obrigado por utilizar o sistema!\n");
                printf("=============================\n\n");
                break;
                
            default:
                printf("Opção inválida! Escolha um número entre 0 e 6.\n"); // Opção não reconhecida
        }

    } while(opcao != 0);  // Continua até o usuário escolher sair

    return 0;
}