#ifndef SAIDA_H
#define SAIDA_H
#include "tempo.h"
#include "parque.h"

struct entrada;
struct veiculo;

typedef struct saida {
    char matricula[PLATE];
    float valor_pago;
    struct tempo tempo;
    struct parque *parque;
    struct veiculo *veiculo;
    struct saida *next;
    struct entrada *entrada;
} saida;

saida* regista_saida (char *input, struct veiculo *vehicle_head,
                      tempo *current,parque* park_head, 
                      saida *exit, struct entrada* entrance_head);



#endif