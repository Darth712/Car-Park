#ifndef PARQUE_H
#define PARQUE_H

typedef struct parque{
    char *nome;
    int cap;
    int livres;
    float val_15;
    float val_1h;
    float val_d;
    struct parque *next;
} parque;

void lista_r_parques_sorted(parque *park_head);
void lista_parques(parque *park_head);
int encontra_parque_entrada (parque* park_head,
                             char* park_name, parque **park);
int encontra_parque_saida (parque* park_head,
                           char* park_name, parque **park);
parque* regista_parque(char *input, parque *park_head);
void sort_strings(char** str, int n);
#endif
