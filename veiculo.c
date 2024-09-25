/* iaed24 - ist11101 - project */
/**
 * @file veiculo.c
 * @author ist1101
 * @brief Funções relacionadas com o registo de veículos
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
 * @brief Verifica se a matrícula e correta
 * 
 * @param plate A matrícula a ser verificada
 * @return int 1 se a matrícula e correta, senão 0
 */
int matricula_correta(const char plate[PLATE]) {
    int letter_pairs = 0;
    int digit_pairs = 0;
    for (int i = 0; i < 7; i += 3) {
        if (isdigit(plate[i]) && isdigit(plate[i + 1])) {
            digit_pairs += 1;
        }
        if (isupper(plate[i]) && isupper (plate[i + 1])) {
            letter_pairs += 1;
        }
    }
    if((digit_pairs == 1 && letter_pairs == 2) 
    || (digit_pairs == 2 && letter_pairs == 1))
        return 1;
    return 0;
}

/**
 * @brief Cria um veículo para a lista de veículos
 * 
 * @param plate A matrícula do veículo
 * @param park O parque onde o veículo está estacionado
 * @return veiculo* O novo veículo criado
 */
veiculo* cria_veiculo(char plate[PLATE], parque* park) {
    veiculo* new_vehicle = (veiculo*)malloc(sizeof(veiculo));
    strcpy(new_vehicle->matricula, plate);
    new_vehicle->mov = 1;
    new_vehicle->parque = park;
    new_vehicle->next = NULL;
    return new_vehicle;
}

/**
 * @brief Encontra um veículo na lista de veículos pelo numero de matrícula
 * 
 * @param vehicle_head A cabeça da lista de veículos
 * @param plate A matrícula do veículo a ser encontrado
 * @return veículo* O veículo encontrado ou NULL senão encontrado
 */
veiculo* find_veiculo(veiculo* vehicle_head, char plate[PLATE]) {
    veiculo* temp_vehicle = vehicle_head;
    while (temp_vehicle != NULL) {
        if (!strcmp(temp_vehicle->matricula, plate)) {
            return temp_vehicle;
        }
        temp_vehicle = temp_vehicle->next;
    }
    return NULL; // Matrícula não encontrada
}

/**
 * @brief Atualiza os dados de um veículo existente
 * 
 * @param temp_vehicle O veículo a ser atualizado
 * @param t O tempo de entrada/saída a ser atualizado
 */
void update_veiculo(veiculo* temp_vehicle, tempo t) {
    if (time_spent (temp_vehicle->tempo_entrada, t) < 0) {
        printf("invalid data.\n");
    } 
    else {
        temp_vehicle->tempo_entrada = t; 
        temp_vehicle->mov = 1;
    }
}

/**
 * @brief Adiciona um novo veículo a lista de veículos
 * 
 * @param vehicle_head A cabeça da lista de veículos
 * @param new_vehicle O novo veículo a ser adicionado
 * @return veículo* A nova cabeça da lista de veículos
 */
veiculo* append_veiculo(veiculo* vehicle_head, veiculo* new_vehicle) {
    if (vehicle_head == NULL) {
        return new_vehicle;
    } 
    else {
        veiculo* temp_vehicle = vehicle_head;
        while (temp_vehicle->next != NULL) {
            temp_vehicle = temp_vehicle->next;
        }
        temp_vehicle->next = new_vehicle;
        return vehicle_head;
    }
}

/**
 * @brief Registra um novo veículo no sistema
 * 
 * @param input A entrada contendo informações do veículo
 * @param vehicle_head A cabeça da lista de veículos
 * @param park_head A cabeça da lista de parques
 * @return veículo* A nova cabeça da lista de veículos
 */
veiculo* regista_veiculo (char* input, veiculo* vehicle_head, 
                          parque* park_head) {
    char* park_name = malloc (sizeof(char) * MAX);
    char plate[PLATE],d[DATA],h[PLATE];
    parque *park = NULL;
    tempo t;
    ler_entrada(input,park_name,plate,d,h);
    t.data = read_data(d);
    t.hora = read_hora(h);
    parque* temp_park = park_head;
    // Encontra o parque onde o veículo entrou
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome,park_name)) {
            park = temp_park;
            break;
        }
        temp_park = temp_park->next;
    }
    // Caso o veículo já esteja na lista dos veículos
    veiculo* existing_veiculo = find_veiculo(vehicle_head, plate);
    if (existing_veiculo != NULL) {
        update_veiculo(existing_veiculo, t);
        free(park_name);
        return vehicle_head;
    }
    veiculo* new_vehicle = cria_veiculo(plate, park);
    new_vehicle->tempo_entrada = t;
    vehicle_head = append_veiculo(vehicle_head, new_vehicle);
    free(park_name);
    return vehicle_head;
}

/**
 * @brief Verifica se uma matrícula ja esta presente na lista de veículos
 * 
 * @param plate A matrícula a ser verificada
 * @param vehicle_head A cabeça da lista de veículos
 * @return int 1 se a matrícula estiver presente, senão 0
 */
int plate_presente(char plate[PLATE], veiculo *vehicle_head) {
    veiculo* temp = vehicle_head;
    if (temp == NULL) {
        return 0;
    }
    while (temp != NULL) {
        if (!strcmp(temp->matricula,plate)) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

/**
 * @brief Lista os parques ordenados para a função v
 * 
 * @param head 
 * @return char** Os nomes do parques ordenados
 */
char** lista_v_parques_sorted(parque *head) {
    // Conta o número de parques
    int count = 0;
    parque* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    char** park_names = malloc(count * sizeof(char*));
    // Copia os nomes dos parques para os vetores
    temp = head;
    for (int i = 0; i < count; i++) {
        park_names[i] = temp->nome;
        temp = temp->next;
    }
    sort_strings(park_names,count);
    return park_names;
}

void print_v(entrada *temp_entrance, char *park_name) {
    if (temp_entrance->saida == NULL) {
        printf("%s %02d-%02d-%04d %02d:%02d\n",
               park_name,
               temp_entrance->tempo.data.dia,
               temp_entrance->tempo.data.mes,
               temp_entrance->tempo.data.ano,
               temp_entrance->tempo.hora.hrs,
               temp_entrance->tempo.hora.min);
    } else {
        printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d\n",
               park_name,
               temp_entrance->tempo.data.dia,
               temp_entrance->tempo.data.mes,
               temp_entrance->tempo.data.ano,
               temp_entrance->tempo.hora.hrs,
               temp_entrance->tempo.hora.min,
               temp_entrance->saida->tempo.data.dia,
               temp_entrance->saida->tempo.data.mes,
               temp_entrance->saida->tempo.data.ano,
               temp_entrance->saida->tempo.hora.hrs,
               temp_entrance->saida->tempo.hora.min);
    }
}

void match_entrada_parque(entrada *entrance_head, 
                          char *plate, char **park_names, int park_count) {
    entrada *temp_entrance = entrance_head;
    for (int i = 0; i < park_count; i++) {
        temp_entrance = entrance_head;
        while (temp_entrance != NULL) {
            if (!strcmp(park_names[i], temp_entrance->parque->nome) &&
                !strcmp(temp_entrance->matricula, plate)) {
                print_v(temp_entrance, park_names[i]);
            }
            temp_entrance = temp_entrance->next;
        }
    }
}

/**
 * @brief Imprime as entradas de registo de um veículo
 * 
 * @param input A entrada contendo a matrícula do veículo
 * @param vehicle_head A cabeça da lista de veículos
 * @param entrance_head A cabeça da lista de entradas
 * @param park_head A cabeça da lista dos parques
 */
void v_veiculo(char *input, veiculo *vehicle_head, entrada *entrance_head,
               parque *park_head) {
    char plate[PLATE];
    entrada *temp_entrance = entrance_head;
    if (sscanf(input, "v %s", plate) != 1) return;
    if (!matricula_correta(plate)) {
        printf("%s: invalid licence plate.\n", plate);
        return;
    }
    if (!plate_presente(plate, vehicle_head) || temp_entrance == NULL) {
        printf("%s: no entries found in any parking.\n", plate);
        return;
    }
    int park_count = 0;
    parque *temp = park_head;
    while (temp != NULL) {
        park_count++;
        temp = temp->next;
    }
    char **park_names = lista_v_parques_sorted(park_head);
    match_entrada_parque(entrance_head, plate, park_names, park_count);
    free(park_names);
}