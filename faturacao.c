/* iaed24 - ist1110199 - project */
/**
 * @file faturacao.c
 * @author ist1110199
 * @brief Funções associadas com a faturação
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tempo.h"
#include "saida.h"
#include "parque.h"
#include "veiculo.h"

/**
 * @brief Calcula a faturação com base no tempo de estacionamento 
 *        e no tarifário do parque.
 * 
 * @param mins O tempo de estacionamento em minutos
 * @param park O parque onde o veículo estacionou
 * @return float O valor faturado
 */
float calcula_tarifario(int mins, parque *park) {
    float billing = 0;
    int total_1_day = mins / (24 * 60);
    int rest = mins % (24 * 60);
    // Faturação dos dias passados
    float daily_billing = total_1_day * park->val_d;
    // Faturação do resto que restou
    if (rest > 0) {
        float price = park->val_15;
        int after_1_hour = 0;
        for (int i = 0; i < rest; i += 15) {
            if (i >= 60 && after_1_hour == 0) {
                price = park->val_1h;
                after_1_hour = 1;
            }
            billing += price;
            if (billing >= park->val_d) {
                billing = park->val_d;
                break;
            }
        }
    }

    return billing + daily_billing;
}

/**
 * @brief Verifica se ocorre um erro na faturação com data especificada.
 * 
 * @param park_name O nome do parque
 * @param park_head A cabeça da lista de parques
 * @param d A string contendo a data
 * @param current O tempo atual
 * @return int 1 se ocorrer um erro, senão 0
 */
int erro_faturacao_com_data(char *park_name, parque* park_head, char d[DATA], 
                            tempo *current) {
    parque* temp_park = park_head;
    int found = 0;
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome, park_name)){
            found = 1;
            break;
        }
        temp_park = temp_park->next;
    }
    if (found == 0) {
        printf("%s: no such parking.\n",park_name);
        return 1;
    }
    
    data data = read_data(d);
    if (!compara_datas(current,data) && strcmp(d," ")) {
        printf("invalid date.\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se ocorre um erro na faturação sem data especificada.
 * 
 * @param park_name O nome do parque
 * @param park_head A cabeça da lista de parques
 * @return int 1 se ocorrer um erro, senão 0
 */
int erro_faturacao_sem_data(char *park_name, parque* park_head) {
    parque* temp_park = park_head;
    int found = 0;
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome, park_name)){
            found = 1;
            break;
        }
        temp_park = temp_park->next;
    }
    if (found == 0) {
        printf("%s: no such parking.\n",park_name);
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se é um novo dia.
 * 
 * @param temp_exit A saída a ser verificada
 * @param last_exit A última saída
 * @return int 1 se for um novo dia, senão 0
 */
int novo_dia(saida *temp_exit, saida *last_exit) {
    return last_exit != NULL &&
           (temp_exit->tempo.data.ano != last_exit->tempo.data.ano ||
            temp_exit->tempo.data.mes != last_exit->tempo.data.mes ||
            temp_exit->tempo.data.dia != last_exit->tempo.data.dia);
}

/**
 * @brief Verifica se o input contém uma data.
 * 
 * @param input A string contendo o input
 * @param park_name O nome do parque
 * @param d A string para armazenar a data, se existir
 * @return int 1 se contém uma data, senão 0
 */
int input_com_data(char *input, char *park_name, char d[]) {
    return (sscanf(input, "f \"%[^\"]\" %s", park_name, d) == 2 ||
            sscanf(input, "f %s %s", park_name, d) == 2) &&
           strlen(d) == 10;
}

/**
 * @brief Verifica se o input não contém uma data.
 * 
 * @param input A string contendo o input
 * @param park_name O nome do parque
 * @return int 1 se não contém uma data, senão 0
 */
int input_sem_data(char *input, char *park_name) {
    return (sscanf(input, "f \"%[^\"]\"", park_name) == 1 ||
            sscanf(input, "f %s", park_name) == 1);
}

/**
 * @brief Imprime o total faturado por um parque em um determinado dia.
 * 
 * @param last_exit A última saída do dia
 * @param total_park O total faturado pelo parque no dia
 */
void print_total_park(saida *last_exit, float total_park) {
    printf("%02d-%02d-%04d %.2f\n", 
           last_exit->tempo.data.dia,
           last_exit->tempo.data.mes, 
           last_exit->tempo.data.ano, 
           total_park);
}

/**
 * @brief Verifica se uma saída corresponde à data e ao parque especificados.
 * 
 * @param temp_exit A saída a ser verificada
 * @param park_name O nome do parque
 * @param t O tempo de referência
 * @return int 1 se corresponder, senão 0
 */
int is_match_for_date(saida *temp_exit, char *park_name, tempo t) {
    return !strcmp(temp_exit->parque->nome, park_name) &&
           temp_exit->tempo.data.dia == t.data.dia &&
           temp_exit->tempo.data.mes == t.data.mes &&
           temp_exit->tempo.data.ano == t.data.ano;
}

/**
 * @brief Imprime as vendas de um parque para uma data específica.
 * 
 * @param temp_exit A lista de saídas
 * @param park_name O nome do parque
 * @param t A data de referência
 */
void print_sales_for_date(saida *temp_exit, char *park_name, tempo t) {
    while (temp_exit != NULL) {
        if (is_match_for_date(temp_exit, park_name, t)) {
            printf("%s %02d:%02d %.2f\n", temp_exit->matricula,
                   temp_exit->tempo.hora.hrs, temp_exit->tempo.hora.min,
                   temp_exit->valor_pago);
        }
        temp_exit = temp_exit->next;
    }
}

/**
 * @brief Atualiza o total faturado pelo parque.
 * 
 * @param temp_exit A saída a ser considerada
 * @param last_exit A última saída
 * @param total_park O total faturado pelo parque
 * @param count O contador de dias
 */
void update_total_park(saida *temp_exit, saida **last_exit, 
                         float *total_park, int *count) {
    if (novo_dia(temp_exit, *last_exit)) {
        print_total_park(*last_exit, *total_park);
        *total_park = 0;
        (*count)++;
    }
    *total_park += temp_exit->valor_pago;
    *last_exit = temp_exit;
}

/**
 * @brief Imprime as vendas de um parque sem data específica.
 * 
 * @param temp_exit A lista de saídas
 * @param park_name O nome do parque
 * @param last_exit A última saída
 * @param total_park O total faturado pelo parque
 * @param count O contador de dias
 */
void print_sales_without_date(saida *temp_exit, 
                              char *park_name, saida **last_exit,
                              float *total_park, int *count) {
    while (temp_exit != NULL) {
        if (!strcmp(temp_exit->parque->nome, park_name)) {
            update_total_park(temp_exit, last_exit, total_park, count);
        }
        temp_exit = temp_exit->next;
    }
    if (*last_exit != NULL && *count > 0) {
        printf("%02d-%02d-%04d %.2f\n", (*last_exit)->tempo.data.dia,
               (*last_exit)->tempo.data.mes, 
               (*last_exit)->tempo.data.ano, 
               *total_park);
    }
}

/**
 * @brief Função principal para calcular a faturação.
 * 
 * @param input O input fornecido pelo utilizador
 * @param exit_head A cabeça da lista de saídas
 * @param park_head A cabeça da lista de parques
 * @param current O tempo atual
 */ 
void faturacao(char *input, saida* exit_head, parque* park_head, 
               tempo *current) {
    char *park_name = malloc(sizeof(char) * MAX);
    char d[DATA];
    tempo t = exit_head->tempo;
    saida* temp_exit = exit_head;
    saida* last_exit = NULL;
    float total_park = 0;
    int count = 1;
    if (input_com_data(input, park_name, d)) {
        if (erro_faturacao_com_data(park_name, park_head, d, current)) {
            free(park_name);
            return;
        }
        t.data = read_data(d);
        print_sales_for_date(temp_exit, park_name, t);
    } 
    else if (input_sem_data(input, park_name)) {
        if (erro_faturacao_sem_data(park_name, park_head)) {
            free(park_name);
            return;
        }
        print_sales_without_date(temp_exit, park_name, &last_exit, 
                                &total_park, &count);
    }
    free(park_name);
}