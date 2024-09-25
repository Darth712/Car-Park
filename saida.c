/**
 * @file saida.c
 * @author ist110199
 * @brief Funções relacionadas com o registo das saidas
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parque.h"
#include "entrada.h"
#include "tempo.h"
#include "veiculo.h"
#include "saida.h"
#include "faturacao.h"

/**
 * @brief Lê os dados de uma saída a partir de uma string de entrada.
 * 
 * @param input A string de entrada
 * @param park_name O nome do parque
 * @param plate A placa do veículo
 * @param d A data da saída
 * @param h A hora da saída
 * @return int 1 se a leitura for bem-sucedida, senão 0
 */
int ler_saida (char* input, char* park_name, 
               char plate[PLATE], char d[DATA], char h[HORA]) {
    if(sscanf
    (input,"s \"%[^\"]\" %8s %10s %5s ",park_name, plate, d, h) == 4
    || sscanf
    (input,"s %s %8s %10s %5s ",park_name, plate, d, h) == 4) {
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se ocorre um erro no registro de saída de um veículo.
 * 
 * @param vehicle_head A cabeça da lista de veículos
 * @param plate A placa do veículo
 * @return int 1 se ocorrer um erro, senão 0
 */
int erro_veiculo (veiculo *vehicle_head,char plate[PLATE]) {
    int found = 0;
    if (vehicle_head == NULL) {
        return 1;
    }
    veiculo* temp_vehicle = vehicle_head;
    while (temp_vehicle != NULL) {
        if (!strcmp(temp_vehicle->matricula,plate) && temp_vehicle->mov == OUT) {
            return 1;
        }
        if (!strcmp(temp_vehicle->matricula,plate) && temp_vehicle->mov == IN) {
            found = 1;
        }
        temp_vehicle = temp_vehicle->next;
    }
    if (found == 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se ocorre um erro no registro de saída de um veículo.
 * 
 * @param park_name O nome do parque
 * @param plate A placa do veículo
 * @param d A data da saída
 * @param h A hora da saída
 * @param current O tempo atual
 * @param vehicle_head A cabeça da lista de veículos
 * @param park_head A cabeça da lista de parques
 * @param park O parque em que o veículo está estacionado
 * @return int 1 se ocorrer um erro, senão 0
 */
int erro_saida (char* park_name, char plate[PLATE], 
                data d, hora h, tempo *current, struct veiculo *vehicle_head, 
                parque *park_head, parque **park) {
    if (park_head == NULL) {
        printf("%s: no such parking.\n",park_name);
        return 1;
    }
    // Iterar cada parque para ver se o parque existe
    if (!encontra_parque_saida(park_head,park_name,park)) {
        return 1;
    }
    if (!matricula_correta(plate)) {
        printf("%s: invalid licence plate.\n",plate);
        return 1;
    }
    if (erro_veiculo(vehicle_head, plate)) {
        printf("%s: invalid vehicle exit.\n",plate);
        return 1;
    }
    if (data_invalida_geral(h, d, current)) {
        printf("invalid date.\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Cria um registro de saída de um veículo.
 * 
 * @param plate A placa do veículo
 * @param t O tempo de saída
 * @param park O parque em que o veículo estava estacionado
 * @param existing_vehicle O veículo que está saindo
 * @param entrance_head A cabeça da lista de entradas
 * @return saida* O novo registro de saída
 */
saida* cria_saida (char plate[PLATE], tempo t, parque *park, 
                   struct veiculo *existing_vehicle, entrada *entrance_head) {
    int mins = time_spent (existing_vehicle->tempo_entrada,
                           existing_vehicle->tempo_saida);
    float billing = calcula_tarifario(mins,park);
    entrada* temp_entrance = entrance_head; 
    entrada* last_entrance = NULL;
    saida *new_exit = malloc(sizeof(saida));
    strcpy(new_exit->matricula,plate);
    new_exit->valor_pago = billing;
    new_exit->tempo = t;
    new_exit->parque = park;
    new_exit->next = NULL;
    while (temp_entrance != NULL) {
        if (!strcmp(temp_entrance->matricula,plate)) {
            last_entrance = temp_entrance;
        }
        temp_entrance = temp_entrance->next;
    }
    // Relaciona a última entrada com a sua saída, em que a matrícula seja igual
    if (last_entrance != NULL) {
        new_exit->entrada = last_entrance;
        last_entrance->saida = new_exit;
    }
    return new_exit;
}

/**
 * @brief Adiciona um novo registro de saída à lista de saídas.
 * 
 * @param head A cabeça da lista de saídas
 * @param new O novo registro de saída
 * @return saida* A cabeça da lista de saídas atualizada
 */
saida* append_saida(saida* head, saida* new) {
    if (head == NULL) {
        return new; 
    } 
    else {
        saida* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new; 
        return head;
    }
}

/**
 * @brief Imprime os detalhes de um registro de saída.
 * 
 * @param new_exit O registro de saída
 * @param existing_vehicle O veículo que está saindo
 */
void print_saida(saida *new_exit, veiculo *existing_vehicle) {
    printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n", 
            new_exit->matricula,
            existing_vehicle->tempo_entrada.data.dia,
            existing_vehicle->tempo_entrada.data.mes,
            existing_vehicle->tempo_entrada.data.ano,
            existing_vehicle->tempo_entrada.hora.hrs,
            existing_vehicle->tempo_entrada.hora.min,
            existing_vehicle->tempo_saida.data.dia,
            existing_vehicle->tempo_saida.data.mes,
            existing_vehicle->tempo_saida.data.ano,
            existing_vehicle->tempo_saida.hora.hrs,
            existing_vehicle->tempo_saida.hora.min,
            new_exit->valor_pago);
}

/**
 * @brief Registra a saída de um veículo.
 * 
 * @param input A string de entrada
 * @param vehicle_head A cabeça da lista de veículos
 * @param current O tempo atual
 * @param park_head A cabeça da lista de parques
 * @param exit_head A cabeça da lista de saídas
 * @param entrance_head A cabeça da lista de entradas
 * @return saida* A cabeça da lista de saídas atualizada
 */
saida* regista_saida (char *input, struct veiculo *vehicle_head,
                      tempo *current,parque* park_head, 
                      saida *exit_head, entrada* entrance_head) {
    char *park_name = malloc(sizeof(char) * MAX);
    char plate[PLATE], d[DATA], h[HORA];
    if (!ler_saida(input,park_name,plate,d,h)) {
        free(park_name);
        return exit_head;
    }
    tempo t;
    t.data = read_data(d);
    t.hora = read_hora(h);
    parque* park = NULL;
    if (erro_saida(park_name, plate, t.data, t.hora, current,
                   vehicle_head, park_head, &park)) {
        free(park_name);
        return exit_head;
    }
    // Procura o veiculo existente para sair
    veiculo* existing_vehicle = find_veiculo(vehicle_head, plate);
    existing_vehicle->mov = OUT;
    existing_vehicle->tempo_saida = t;
    saida *new_exit = cria_saida(plate,t,park,existing_vehicle, 
                                 entrance_head);
    exit_head = append_saida(exit_head,new_exit);
    park->livres++;
    *current = t;
    print_saida(new_exit,existing_vehicle);
    free(park_name);
    return exit_head;
}