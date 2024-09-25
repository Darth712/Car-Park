#ifndef ENTRADA_H
#define ENTRADA_H
#include "tempo.h"
#include "parque.h"
#define PLATE 9

struct saida;
struct veiculo;

typedef struct entrada {
    char matricula[PLATE];
    struct tempo tempo;
    struct parque *parque;
    struct veiculo *veiculo;
    struct saida *saida;
    struct entrada *next;
} entrada;

int ler_entrada
(char* input, char* park_name, char plate[PLATE], char d[DATA], char h[HORA]);
entrada *regista_entrada(char *input, tempo *current, 
                         struct veiculo *veiculo_head,
                         parque *parque_head, entrada *entrada_head);
int registar(int *entradas ,entrada *entrada_head);
#endif

