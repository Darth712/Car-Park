/* iaed24 - ist1110199 - project */
/**
 * @file free.c
 * @author ist110199
 * @brief Funções relacionadas com a libertação de memória
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parque.h"
#include "entrada.h"
#include "veiculo.h"
#include "saida.h"

/**
 * @brief Liberta a memória alocada para os parques.
 * 
 * @param park_head A cabeça da lista de parques
 * @return parque* Ponteiro para NULL
 */
parque* free_parques(parque* park_head) {
    while(park_head != NULL) {
        parque* next_parque = park_head->next;
        free(park_head->nome);
        free(park_head);
        park_head = next_parque;
    }
    return NULL;
}

/**
 * @brief Liberta a memória alocada para os veículos.
 * 
 * @param vehicle_head A cabeça da lista de veículos
 * @return veiculo* Ponteiro para NULL
 */
veiculo* free_veiculos(veiculo* vehicle_head) {
    while(vehicle_head != NULL) {
        veiculo* next_vehicle = vehicle_head->next;
        free(vehicle_head);
        vehicle_head = next_vehicle;
    }
    return NULL;
}

/**
 * @brief Liberta a memória alocada para as entradas.
 * 
 * @param entrance_head A cabeça da lista de entradas
 * @return entrada* Ponteiro para NULL
 */
entrada* free_entradas(entrada* entrance_head) {
    while(entrance_head != NULL) {
        entrada* next_entrance = entrance_head->next;
        free(entrance_head);
        entrance_head = next_entrance;
    }
    return NULL;
}

/**
 * @brief Liberta a memória alocada para as saídas.
 * 
 * @param exit_head A cabeça da lista de saídas
 * @return saida* Ponteiro para NULL
 */
saida* free_saidas(saida* exit_head) {
    while(exit_head != NULL) {
        saida* next_exit = exit_head->next;
        free(exit_head);
        exit_head = next_exit;
    }
    return NULL;
}

/**
 * @brief Liberta a memória alocada para todas as listas.
 * 
 * @param park_head A cabeça da lista de parques
 * @param vehicle_head A cabeça da lista de veículos
 * @param entrance_head A cabeça da lista de entradas
 * @param exit_head A cabeça da lista de saídas
 */
void free_listas(parque* park_head, veiculo* vehicle_head,
                 entrada* entrance_head, saida* exit_head) {
    free_parques(park_head);
    free_veiculos(vehicle_head);
    free_entradas(entrance_head);
    free_saidas(exit_head);
}

/**
 * @brief Lê o nome do parque de uma entrada.
 * 
 * @param input A entrada contendo o nome do parque
 * @param park_name O nome do parque a ser lido
 * @return int 1 se o nome do parque foi lido com sucesso, senão 0
 */
int ler_r_parque(char* input, char* park_name) {
    if(sscanf(input, "r \"%[^\"]\"",park_name) == 1 
    || sscanf(input, "r %s",park_name) == 1) {
        return 1;
    }
    return 0;
}

/**
 * @brief Remove a associação do parque aos veículos estacionados.
 * 
 * @param vehicle_head A cabeça da lista de veículos
 * @param park_name O nome do parque a ser removido dos veículos
 * @return veiculo* A cabeça da lista de veículos
 */
veiculo* remove_parque_dos_veiculos(veiculo* vehicle_head, 
                                    char* park_name) {
    veiculo* temp_vehicle = vehicle_head;
    while (temp_vehicle != NULL) {
        if (temp_vehicle->parque != NULL && !strcmp(temp_vehicle->parque->nome, 
                                                    park_name)) {
            temp_vehicle->parque = NULL;
            temp_vehicle->mov = OUT;
        }
        temp_vehicle = temp_vehicle->next;
    }
    return vehicle_head;
}

/**
 * @brief Remove as entradas associadas ao parque.
 * 
 * @param entrance_head A cabeça da lista de entradas
 * @param park_name O nome do parque a ser removido das entradas
 * @return entrada* A cabeça da lista de entradas
 */
entrada* remove_parque_das_entradas(entrada* entrance_head, char* park_name) {
    // Necessário criar um entrada prévia para ligar com o next do temporário
    entrada* prev_entrance = NULL;
    entrada* temp_entrance = entrance_head;
    while (temp_entrance != NULL) {
        if (!strcmp(temp_entrance->parque->nome, park_name)) {
            if (prev_entrance) {
                prev_entrance->next = temp_entrance->next;
            } 
            else {
                entrance_head = temp_entrance->next;
            }
            entrada* next = temp_entrance->next; // Store next node
            free(temp_entrance);
            temp_entrance = next; // Move to the next node
        } 
        else {
            prev_entrance = temp_entrance;
            temp_entrance = temp_entrance->next;
        }
    }
    return entrance_head;
}

/**
 * @brief Remove as saídas associadas ao parque.
 * 
 * @param exit_head A cabeça da lista de saídas
 * @param park_name O nome do parque a ser removido das saídas
 * @return saida* A cabeça da lista de saídas
 */
saida* remove_parque_das_saidas(saida* exit_head, char* park_name) {
    // Necessário criar um saída prévia para ligar com o next do temporário
    saida* prev_exit = NULL;
    saida* temp_exit = exit_head;
    while (temp_exit != NULL) {
        if (!strcmp(temp_exit->parque->nome, park_name)) {
            if (prev_exit) {
                prev_exit->next = temp_exit->next;
            } 
            else {
                exit_head = temp_exit->next;
            }
            saida* next = temp_exit->next; // Store next node
            free(temp_exit);
            temp_exit = next; // Move to the next node
        } 
        else {
            prev_exit = temp_exit;
            temp_exit = temp_exit->next;
        }
    }
    return exit_head;
}

/**
 * @brief Remove um parque da lista de parques.
 * 
 * @param park_head A cabeça da lista de parques
 * @param park_name O nome do parque a ser removido
 * @return parque* A cabeça da lista de parques
 */
parque* remove_parque_da_lista(parque* park_head, char* park_name) {
    parque* temp_park = park_head;
    // Necessário criar um parque prévio para ligar com o next do temporário
    parque* prev_parque = NULL; 
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome, park_name)) {
            if (prev_parque) {
                prev_parque->next = temp_park->next;
            } 
            else {
                park_head = temp_park->next;
            }
            free(temp_park->nome);
            free(temp_park);
            temp_park = NULL; // Atribuição correta
            break;
        } 
        else {
            prev_parque = temp_park;
            temp_park = temp_park->next;
        }
    }
    return park_head;
}

/**
 * @brief Verifica se ocorreu um erro ao remover um parque.
 * 
 * @param park_name O nome do parque
 * @param park_head A cabeça da lista de parques
 * @return int 1 senão houver erro, senão 0
 */
int erro_r_parque(char *park_name, parque *park_head) {
    // Procura pelo parque
    parque* temp_park = park_head;
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome, park_name)) {
            return 1;
        }
        temp_park = temp_park->next;
    }
    printf("%s: no such parking.\n",park_name);
    // Se o parque não for encontrado, imprime uma mensagem e retorna
    return 0;
}
