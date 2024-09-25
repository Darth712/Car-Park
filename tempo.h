#ifndef TEMPO_H
#define TEMPO_H
#include "parque.h"
#define DATA 11
#define HORA 6
#define PLATE 9
struct entrada;
struct saida;
struct veiculo;


typedef struct {
    int dia;
    int mes;
    int ano;
} data;

static const int days_per_month []={31,28,31,30,31,30,31,31,30,31,30,31};

/*Definicao de um struct da hora*/

typedef struct {
    int hrs;
    int min;
} hora;

typedef struct tempo{
    data data;
    hora hora;
} tempo;

data read_data(char date[DATA]);
hora read_hora(char hour[HORA]);
int data_invalida_geral(hora h, data d, tempo *current);
int data_invalida_entrada(hora h, data d,
                          tempo *current, struct entrada *entrance_head);
int data_invalida_saida(hora h, data d, struct saida *exit_head, 
                        struct veiculo **vehicle_head, char plate[PLATE],
                        tempo *current);
int time_spent(tempo t1, tempo t2);
int compara_datas(tempo *current, data d);

#endif