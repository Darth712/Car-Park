#ifndef VEICULO_H
#define VEICULO_H
#include "parque.h"
#include "tempo.h"
#include <stdio.h>
#define MAX BUFSIZ
#define IN 1
#define OUT 0

struct entrada;
struct saida;

typedef struct veiculo {
    char matricula[PLATE];
    int mov;
    tempo tempo_entrada;
    tempo tempo_saida;
    parque *parque;
    struct veiculo *next;
} veiculo;

int matricula_correta(const char plate[PLATE]);
veiculo* find_veiculo(veiculo* vehicle_head, char plate[PLATE]);
veiculo* regista_veiculo (char* input, veiculo* vehicle_head, 
                          parque* park_head);
void v_veiculo(char *input, veiculo* vehicle_head, 
                    struct entrada* entrada_head, parque *park_head);
                    

#endif