/* iaed24 - ist1110199 - project */
/**
 * @file tempo.c
 * @author ist1110199
 * @brief Funções associadas com o tempo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tempo.h"
#include "entrada.h"
#include "saida.h"
#include "parque.h"
#include "veiculo.h"

/**
 * @brief Verifica se uma data e hora são válidas de forma geral.
 * 
 * @param h A hora
 * @param d A data
 * @param current O tempo atual
 * @return int 1 se a data e hora são válidas, senão 0
 */
int data_invalida_geral(hora h, data d, tempo *current) {
    if (!(h.hrs >= 0 && h.hrs <= 23) || !(h.min >= 0 && h.min <= 59)) {
        return 1;
    }
    // Verifica se o mês está certo
    if (d.mes < 1 || d.mes > 12) {
        return 1;
    }
    // Verifica se os dias dos meses estão certos
    if (d.dia < 1 || d.dia > days_per_month[d.mes - 1]) {
        return 1;
    }
    tempo t;
    t.hora = h;
    t.data = d;
    if (time_spent(*current,t) < 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se uma data e hora de saída são válidas.
 * 
 * @param h A hora
 * @param d A data
 * @param exit_head A cabeça da lista de saídas
 * @param vehicle_head O endereço da cabeça da lista de veículos
 * @param plate A matrícula do veículo
 * @param current O tempo atual
 * @return int 1 se a data e hora de saída são válidas, senão 0
 */
int data_invalida_saida(hora h, data d, saida *exit_head, 
                        veiculo **vehicle_head, char plate[PLATE],
                        tempo *current) {
    tempo t2;
    t2.data = d;
    t2.hora = h;
    if (exit_head != NULL) {
        saida *temp = exit_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        tempo t1 = temp->tempo;
        if (time_spent(t1,t2) < 0 ) {
            return 1;
        }
    }
    if (data_invalida_geral(h,d, current)){
        return 1;
    }
    veiculo *exisiting_vehicle = find_veiculo(*vehicle_head, plate);
    if (time_spent(exisiting_vehicle->tempo_entrada,t2) < 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Verifica se uma data e hora de entrada são válidas.
 * 
 * @param h A hora
 * @param d A data
 * @param current O tempo atual
 * @param entrance_head A cabeça da lista de entradas
 * @return int 1 se a data e hora de entrada são válidas, senão 0
 */
int data_invalida_entrada(hora h, data d, 
                          tempo *current, entrada *entrance_head) {
    if (entrance_head != NULL) {
        entrada *temp = entrance_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        tempo t1 = temp->tempo;
        tempo t2;
        t2.data = d;
        t2.hora = h;
        if (time_spent(t1,t2) < 0 ) {
            return 1;
        }
    }
        // Verifica se as horas estão corretas
    if (data_invalida_geral(h,d, current))
        return 1;
    return 0;
}

/**
 * @brief Lê uma data de uma string.
 * 
 * @param date A string contendo a data
 * @return data A estrutura contendo a data lida
 */
data read_data(char date[DATA]) {
    data d;
    sscanf(date,"%2d-%2d-%4d ",&d.dia,&d.mes,&d.ano);
    return d;
}

/**
 * @brief Lê uma hora de uma string.
 * 
 * @param hour A string contendo a hora
 * @return hora A estrutura contendo a hora lida
 */
hora read_hora(char hour[HORA]) {
    hora h;
    sscanf(hour,"%2d:%2d ",&h.hrs,&h.min);
    return h;
}

/**
 * @brief Calcula o tempo decorrido entre duas instâncias de tempo.
 * 
 * @param t1 O primeiro tempo
 * @param t2 O segundo tempo
 * @return int O tempo decorrido em minutos
 */
int time_spent(tempo t1, tempo t2) {
    int total_minutes1 = 0, total_minutes2 = 0, years_diff = 0;
    if (t1.data.ano > t2.data.ano) {
        return -1;
    }
    else if (t2.data.ano > t1.data.ano) {
        years_diff = (t2.data.ano - t1.data.ano - 1) * 365 * 24 * 60;
    }
    for (int i = 0; i < t1.data.mes - 1; i++) {
        total_minutes1 += days_per_month[i] * 24 * 60;
    }
    for (int i = 0; i < t2.data.mes - 1; i++) {
        total_minutes2 += days_per_month[i] * 24 * 60;
    }
    if (t1.data.dia != 0) {
        total_minutes1 += 
        (t1.data.dia - 1) * 24 * 60 + t1.hora.hrs * 60 + t1.hora.min;
    }
    else if (t1.data.dia == 0) {
        total_minutes1 = 0;
    }
    total_minutes2 += 
    (t2.data.dia - 1) * 24 * 60 + t2.hora.hrs * 60 + t2.hora.min;
    return total_minutes2 - total_minutes1 + years_diff;
}

/**
 * @brief Compara duas datas.
 * 
 * @param current O tempo atual
 * @param d A data a ser comparada
 * @return int 1 se a data é anterior ao tempo atual, senão 0
 */
int compara_datas(tempo *current, data d) {
    if((d.ano > current->data.ano) 
    || (d.ano == current->data.ano && d.mes > current->data.mes) 
    || (d.ano == current->data.ano && d.mes == current->data.mes 
    && d.dia > current->data.dia) ) {
        return 0;
    }
    return 1;
}