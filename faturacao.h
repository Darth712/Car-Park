#ifndef FATURACAO_H
#define FATURACAO_H
#include "tempo.h"
#include "saida.h"
#include "parque.h"

float calcula_tarifario(int mins, parque *park);
void faturacao(char *input, struct saida* exit_head, parque *park_head, tempo *current);

#endif