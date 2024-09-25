#ifndef FREE_H
#define FREE_H
#include "parque.h"
#include "veiculo.h"
#include "entrada.h"
#include "saida.h"

parque* free_parques(parque* park_head);
veiculo* free_veiculos(veiculo* vehicle_head);
entrada* free_entradas(entrada* entrance_head);
saida* free_saidas(saida* exit_head);
void free_listas(parque *park_head, veiculo *vehicle_head,
                 entrada *entrance_head, saida *exit_head);
int ler_r_parque(char* input, char* park_name);
int erro_r_parque(char *park_name, parque *park_head);
parque* remove_parque_da_lista(parque* park_head, char* park_name);
saida* remove_parque_das_saidas(saida* exit_head, char* park_name);
entrada* remove_parque_das_entradas(entrada* entrance_head, char* park_name);
veiculo* remove_parque_dos_veiculos(veiculo* vehicle_head, 
                                     char* park_name);
#endif