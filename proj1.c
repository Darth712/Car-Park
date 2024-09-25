/* iaed24 - ist1110199 - project */
/**
 * @file proj1.c
 * @author ist110199
 * @brief Ficheiro principal
 * Para melhor compreensão das structs e das variáveis, todos os structs são
 * construídos em português (incluindo os seus dados), 
 * enquanto que as variáveis são em ingles.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parque.h"
#include "entrada.h"
#include "veiculo.h"
#include "saida.h"
#include "free.h"
#include "faturacao.h"

/**
 * @brief Inicializa a variável do tempo current.
 * É sempre atualizado quando se adiciona uma entrada/saída.
 * 
 * @param current Ponteiro para a variável do tempo atual
 */
void inicializar_tempo(tempo *current) {
    current->data.dia = 1;
    current->data.mes = 1;
    current->data.ano = 1;
    current->hora.hrs = 0;
    current->hora.min = 0;
}

/**
 * @brief Liberta a memória de todas as listas quando sai do programa.
 * 
 * @param park_head Ponteiro para a cabeça da lista de parques
 * @param vehicle_head Ponteiro para a cabeça da lista de veículos
 * @param entrance_head Ponteiro para a cabeça da lista de entradas
 * @param exit_head Ponteiro para a cabeça da lista de saídas
 */
void processa_q(parque **park_head, veiculo **vehicle_head, 
                  entrada **entrance_head, saida **exit_head) {
    free_listas(*park_head, *vehicle_head, *entrance_head, *exit_head);
    exit(0);
}

/**
 * @brief Processa o comando 'p'.
 * 
 * @param input O input do comando 'p'
 * @param park_head Ponteiro para a cabeça da lista de parques
 */
void processa_p(char *input, parque **park_head) {
    // Caso o input seja não tenha argumentos
    if ((strlen(input) - 1) == 1) {
        lista_parques(*park_head);
    } 
    else {
        *park_head = regista_parque(input, *park_head);
    }
}

/**
 * @brief Processa o comando 'e'.
 * 
 * @param input O input do comando 'e'
 * @param current Ponteiro para a variável do tempo atual
 * @param vehicle_head Ponteiro para a cabeça da lista de veículos
 * @param park_head Ponteiro para a cabeça da lista de parques
 * @param entrance_head Ponteiro para a cabeça da lista de entradas
 * @param entradas Ponteiro para o número de entradas
 */
void processa_e(char *input, tempo *current, 
                       veiculo **vehicle_head, parque **park_head, 
                       entrada **entrance_head, int *entradas) {
    *entrance_head = regista_entrada(input, current, *vehicle_head, 
                                    *park_head, *entrance_head);
    // Se registar uma entrada, regista um novo veículo
    if (registar(entradas, *entrance_head)) {
        *vehicle_head = regista_veiculo(input, *vehicle_head, *park_head);
        (*entradas)++;
    }
}

/**
 * @brief Processa o comando 's'.
 * 
 * @param input O input do comando 's'
 * @param vehicle_head Ponteiro para a cabeça da lista de veículos
 * @param current Ponteiro para a variável do tempo atual
 * @param park_head Ponteiro para a cabeça da lista de parques
 * @param exit_head Ponteiro para a cabeça da lista de saídas
 * @param entrance_head Ponteiro para a cabeça da lista de entradas
 */
void processa_s(char *input, veiculo **vehicle_head, 
                       tempo *current, parque **park_head, 
                       saida **exit_head, entrada **entrance_head) {
    *exit_head = regista_saida(input, *vehicle_head, current, 
                                *park_head, *exit_head, *entrance_head);
}

/**
 * @brief Processa o comando 'v'.
 * 
 * @param input O input do comando 'v'
 * @param vehicle_head Ponteiro para a cabeça da lista de veículos
 * @param entrance_head Ponteiro para a cabeça da lista de entradas
 */
void processa_v(char *input, veiculo **vehicle_head, 
                       entrada **entrance_head, parque **park_head) {
    v_veiculo(input, *vehicle_head, *entrance_head, *park_head);
}

/**
 * @brief Processa o comando 'f'.
 * 
 * @param input O input do comando 'f'
 * @param exit_head Ponteiro para a cabeça da lista de saídas
 * @param park_head Ponteiro para a cabeça da lista de parques
 * @param current Ponteiro para a variável do tempo atual
 */
void processa_f(char *input, saida **exit_head, 
                       parque **park_head, tempo *current) {
    faturacao(input, *exit_head, *park_head, current);
}

/**
 * @brief Processa o comando 'r'.
 * 
 * @param input O input do comando 'r'
 * @param park_head Ponteiro para a cabeça da lista de parques
 * @param vehicle_head Ponteiro para a cabeça da lista de veículos
 * @param entrance_head Ponteiro para a cabeça da lista de entradas
 * @param exit_head Ponteiro para a cabeça da lista de saídas
 */
void processa_r(char *input,
                       parque **park_head, veiculo **vehicle_head,
                       entrada **entrance_head, saida **exit_head) {
    // Caso não leia corretamente o input recebido, retorna
    char *nome_parque = malloc(sizeof(char) * MAX);
    if (!ler_r_parque(input, nome_parque)) {
        free(nome_parque);
        return;
    }
    /* Caso o nome do parque não estiver na lista de parques, ou seja, 
       caso ainda não tenha sido criado
    */
    if (!erro_r_parque(nome_parque, *park_head)) {
        free(nome_parque);
        return;
    }
    /* Remove os veículos, saidas, entradas associadas ao parque, e 
       remove o parque por fim
    */
    *vehicle_head = remove_parque_dos_veiculos(*vehicle_head, nome_parque);
    *entrance_head = remove_parque_das_entradas(*entrance_head, nome_parque);
    *exit_head = remove_parque_das_saidas(*exit_head, nome_parque);
    *park_head = remove_parque_da_lista(*park_head, nome_parque);
    free(nome_parque);
    lista_r_parques_sorted(*park_head);
}

/**
 * @brief Processa todos os comandos, dependendo do input obtido.
 * 
 * @param com O comando a ser processado
 * @param input O input do comando
 * @param park_head Ponteiro para a cabeça da lista de parques
 * @param entrance_head Ponteiro para a cabeça da lista de entradas
 * @param vehicle_head Ponteiro para a cabeça da lista de veículos
 * @param exit_head Ponteiro para a cabeça da lista de saídas
 * @param entradas Ponteiro para o número de entradas
 * @param current Ponteiro para a variável do tempo atual
 */
void processa_comando(char com, char *input, parque **park_head, 
                     entrada **entrance_head, veiculo **vehicle_head, 
                     saida **exit_head, int *entradas, tempo *current) {
    switch (com) {
        case ('q'): 
            processa_q(park_head, vehicle_head, entrance_head, exit_head);
            break;
        case ('p'):
            processa_p(input, park_head);
            break;
        case ('e'):
            processa_e(input, current, vehicle_head, 
                              park_head, entrance_head, entradas);
            break;
        case ('s'): 
            processa_s(input, vehicle_head, current, 
                              park_head, exit_head, entrance_head);
            break;
        case ('v'):
            processa_v(input, vehicle_head, entrance_head, park_head);
            break;
        case ('f'):
            processa_f(input, exit_head, park_head, current);
            break;
        case ('r'): 
            processa_r(input, park_head, vehicle_head, 
                              entrance_head, exit_head);
            break;
    }
}

/**
 * @brief Função principal do projeto
 */
int main () {
    // Inicialização das variáveis
    parque *park_head = NULL;
    veiculo *vehicle_head = NULL;
    entrada *entrance_head = NULL;
    saida *exit_head = NULL;
    int entradas = 0;
    char com, input[MAX];
    tempo current;
    inicializar_tempo(&current);
    // Loop onde irá ser corrido o projeto
    while (1) {
        fgets(input, MAX,stdin);
        sscanf(input, "%c ", &com);
        processa_comando(com, input, &park_head, &entrance_head, 
                        &vehicle_head, &exit_head, &entradas, &current);
    }
    return 0;
}