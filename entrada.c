/**
 * @file entrada.c
 * @author ist110199
 * @brief Funções relacionadas com o registo das entradas
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parque.h"
#include "entrada.h"
#include "tempo.h"
#include "veiculo.h"
#include "saida.h"

/**
 * @brief Lê os dados de uma entrada a partir de uma string de entrada.
 * 
 * @param input A string de entrada
 * @param park_name O nome do parque
 * @param plate A matrícula do veículo
 * @param d A data da entrada
 * @param h A hora da entrada
 * @return int 1 se a leitura for bem sucedida, senão 0
 */
int ler_entrada
(char* input, char* park_name, char plate[PLATE], char d[DATA], char h[HORA]) {
    if(sscanf
    (input,"e \"%[^\"]\" %8s %10s %5s ",park_name, plate, d, h) == 4
    || sscanf
    (input,"e %s %8s %10s %5s ",park_name, plate, d, h) == 4) {
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica os erros possíveis na entrada de um veículo.
 * 
 * @param park_name O nome do parque
 * @param plate A matrícula do veículo
 * @param d A data da entrada
 * @param h A hora da entrada
 * @param current O tempo atual
 * @param vehicle_head A cabeça da lista de veículos
 * @param park_head A cabeça da lista de parques
 * @param park Um ponteiro para o parque onde o veículo entrará
 * @return int 1 se houver erro, senão 0
 */
int erro_entrada (char* park_name,char plate[PLATE], 
                  data d, hora h, tempo *current, 
                  veiculo *vehicle_head, parque *park_head, parque **park) {
    if (park_head == NULL) {
        printf("%s: no such parking.\n",park_name);
        return 1;
    }
    // Iterar cada parque para ver se o parque existe
    if (!encontra_parque_entrada(park_head,park_name,park)) {
        return 1;
    }
    if (!matricula_correta(plate)) {
        printf("%s: invalid licence plate.\n",plate);
        return 1;
    }
    veiculo* temp_vehicle = vehicle_head;
    while (temp_vehicle != NULL) {
        if (!strcmp(temp_vehicle->matricula,plate) && temp_vehicle->mov == IN) {
            printf("%s: invalid vehicle entry.\n",plate);
            return 1;
        }
        temp_vehicle = temp_vehicle->next;
    }
    if (data_invalida_geral(h, d, current)) {
        printf("invalid date.\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Cria uma nova entrada.
 * 
 * @param plate A matrícula do veículo
 * @param t O tempo da entrada
 * @param park O parque onde o veículo entrou
 * @return entrada* A nova entrada criada
 */
entrada* cria_entrada (char plate[PLATE], tempo t, parque* park) {
    entrada* new_entrance = malloc(sizeof(entrada));
    strcpy(new_entrance->matricula, plate);
    new_entrance->tempo = t;
    new_entrance->parque = park;
    new_entrance->next = NULL;
    new_entrance->saida = NULL;
    return new_entrance;
}

/**
 * @brief Adiciona uma entrada à lista de entradas.
 * 
 * @param head A cabeça da lista de entradas
 * @param novo A nova entrada a ser adicionada
 * @return entrada* A cabeça da lista de entradas atualizada
 */
entrada* append_entrada(entrada* head, entrada* novo) {
    if (head == NULL) {
        return novo; // If the list is empty, this entrada becomes the head
    } 
    else {
        entrada* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novo; 
        return head;
    }
}

/**
 * @brief Registra uma nova entrada a partir de uma string de entrada.
 * 
 * @param input A string de entrada
 * @param current O tempo atual
 * @param vehicle_head A cabeça da lista de veículos
 * @param park_head A cabeça da lista de parques
 * @param entrance_head A cabeça da lista de entradas
 * @return entrada* A cabeça da lista de entradas atualizada
 */
entrada* regista_entrada(char* input, tempo *current, veiculo *vehicle_head,
                         parque* park_head, entrada* entrance_head) {
    char* park_name = malloc(sizeof(char) * MAX);
    char plate[PLATE], d[DATA], h[HORA];
    if (!ler_entrada(input, park_name, plate, d, h)) {
        free(park_name);
        return entrance_head;
    }
    tempo t;
    t.data = read_data(d);
    t.hora = read_hora(h);
    parque *park = NULL;
    if (erro_entrada(park_name, plate, t.data, t.hora, 
                     current, vehicle_head, park_head, &park)) {
        free(park_name);
        return entrance_head;
    }
    entrada* new_entrance = cria_entrada(plate, t, park);
    entrance_head = append_entrada(entrance_head,new_entrance);
    park->livres--;
    *current = t;
    printf("%s %d\n",new_entrance->parque->nome,new_entrance->parque->livres);
    free(park_name);
    return entrance_head;
}

/**
 * @brief Verifica se houve novas entradas.
 * 
 * @param x O número de entradas anteriormente registradas
 * @param entrance_head A cabeça da lista de entradas
 * @return int 1 se houve novas entradas, senão 0
 */
int registar(int *x ,entrada* entrance_head) {
    int y = 0;
    while(entrance_head != NULL) {
        y++;
        entrance_head = entrance_head->next;
    }
    if(*x == y) {
        return 0;
    }
    return 1;
}