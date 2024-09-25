/**
 * @file parque.c
 * @author ist110199
 * @brief Funções relacionadas com o registo dos parques
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parque.h"
#include "entrada.h"
#include "veiculo.h"

/**
 * @brief Lista os parques.
 * 
 * @param head A cabeça da lista de parques
 */
void lista_parques(parque *head) {
    while(head != NULL) {
        printf("%s %d %d\n",head->nome, head->cap, head->livres);
        head = head->next;
    }
}

/**
 * @brief Ordena um vetor de strings em ordem alfabética.
 * 
 * @param str O vetor de strings
 * @param n O tamanho do vetor
 */
void sort_strings(char** str, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(str[j], str[j + 1]) > 0) {
                // Troca a str[j] e a str[j+1]
                char* temp = str[j];
                str[j] = str[j + 1];
                str[j + 1] = temp;
            }
        }
    }
}

/**
 * @brief Lista os parques ordenados por nome.
 * 
 * @param head A cabeça da lista de parques
 */
void lista_r_parques_sorted(parque *head) {
    // Conta o número de parques
    int count = 0;
    parque* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    char** park_names = malloc(count * sizeof(char*));
    // Copia os nomes dos parques para os vetores
    temp = head;
    for (int i = 0; i < count; i++) {
        park_names[i] = temp->nome;
        temp = temp->next;
    }
    sort_strings(park_names, count);

    for (int i = 0; i < count; i++) {
        printf("%s\n", park_names[i]);
    }
    free(park_names);
}

/**
 * @brief Encontra um parque na lista de parques para entrada.
 * 
 * @param park_head A cabeça da lista de parques
 * @param park_name O nome do parque a ser encontrado
 * @param park Um ponteiro para o parque encontrado
 * @return int 1 se o parque for encontrado, senão 0
 */
int encontra_parque_entrada(parque* park_head, char* park_name, parque **park) {
    parque* temp_park = park_head;
    int found = 0;
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome,park_name)) {
            found = 1;
            if (temp_park->livres == 0) {
                printf("%s: parking is full.\n",temp_park->nome);
                return 0;
            }
            // Parque onde o veiculo vai entrar
            *park = temp_park;
        }
        temp_park = temp_park->next;
    }
    if (found == 0) {
        printf("%s: no such parking.\n",park_name);
        return 0;
    }
    return 1;
}

/**
 * @brief Encontra um parque na lista de parques para saída.
 * 
 * @param park_head A cabeça da lista de parques
 * @param park_name O nome do parque a ser encontrado
 * @param park Um ponteiro para o parque encontrado
 * @return int 1 se o parque for encontrado, senão 0
 */
int encontra_parque_saida(parque* park_head, char* park_name, parque **park) {
        parque* temp_park = park_head;
    int found = 0;
    while (temp_park != NULL) {
        if (!strcmp(temp_park->nome,park_name)) {
            found = 1;
            // Parque onde o veiculo vai entrar
            *park = temp_park;
        }
        temp_park = temp_park->next;
    }
    if (found == 0) {
        printf("%s: no such parking.\n",park_name);
        return 0;
    }
    return 1;
}

/**
 * @brief Verifica se há erro no parque.
 * 
 * @param park_name O nome do parque
 * @param cap A capacidade do parque
 * @param val_15 O valor de 15 minutos
 * @param val_1h O valor de 1 hora
 * @param val_d O valor diário
 * @param head A cabeça da lista de parques
 * @return int 1 se houver erro, senão 0
 */
int erro_parque (char* park_name, int cap, float val_15,
                 float val_1h, float val_d,parque *head) {
    int n = 0;
    parque* temp = head;
    while (temp != NULL) {
        if (!strcmp(temp->nome,park_name)) {
            printf("%s: parking already exists.\n", park_name);
            return 1;
        }
        temp = temp->next;
        n += 1;
    }
    if (cap <= 0) {   
        printf("%d: invalid capacity.\n",cap);
        return 1;
        }
    if (!(val_d > val_1h && val_1h > val_15) 
    || val_d  <= 0 
    || val_15 <= 0 
    || val_1h <= 0) {  
        printf("invalid cost.\n");
        return 1;
        }
    if (n >= 20) {
            printf("too many parks.\n");
        return 1;
        }
    return 0;
}

/**
 * @brief Lê os dados de um parque a partir de uma string de entrada.
 * 
 * @param input A string de entrada
 * @param park_name O nome do parque a ser lido
 * @param cap A capacidade do parque a ser lida
 * @param val_15 O valor de 15 minutos a ser lido
 * @param val_1h O valor de 1 hora a ser lido
 * @param val_d O valor diário a ser lido
 * @return int 1 se a leitura for bem-sucedida, senão 0
 */
int ler_parque (char* input,char* park_name,int* cap,
                float* val_15,float* val_1h,float* val_d) {
    if(sscanf
    (input, "p \"%[^\"]\" %d %f %f %f", park_name, cap, val_15, val_1h, val_d)
     == 5 
    || sscanf
    (input, "p %s %d %f %f %f", park_name, cap, val_15, val_1h, val_d) == 5) {
        return 1; // Successfully parsed input
    } 
    return 0; // Parsing failed
}

/**
 * @brief Cria um novo parque.
 * 
 * @param park_name O nome do parque
 * @param cap A capacidade do parque
 * @param val_15 O valor de 15 minutos
 * @param val_1h O valor de 1 hora
 * @param val_d O valor diário
 * @return parque* O novo parque criado
 */
parque* create_parque (char* park_name, int cap, 
                       float val_15, float val_1h, float val_d) {
    parque* novo = (parque*) malloc(sizeof(parque));
    novo->nome = strdup(park_name);
    novo->cap = cap;
    novo->livres = cap;
    novo->val_15 = val_15;
    novo->val_1h = val_1h;
    novo->val_d = val_d;
    novo->next = NULL;
    return novo;
}

/**
 * @brief Adiciona um parque à lista de parques.
 * 
 * @param head A cabeça da lista de parques
 * @param novo O novo parque a ser adicionado
 * @return parque* A cabeça da lista de parques atualizada
 */
parque* append_parque(parque* head, parque* novo) {
    if (head == NULL) {
        return novo; // If the list is empty, this park becomes the head
    } 
    else {
        parque* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = novo; 
        return head;
    }
}

/**
 * @brief Registra um novo parque a partir de uma string de entrada.
 * 
 * @param input A string de entrada
 * @param head A cabeça da lista de parques
 * @return parque* A cabeça da lista de parques atualizada
 */
parque* regista_parque(char *input, parque *head) {
    char* park_name = malloc(sizeof(char) * MAX);
    int cap;
    float val_15, val_1h, val_d;
    if (!ler_parque(input, park_name, &cap, &val_15, &val_1h, &val_d)) {
        free(park_name);
        return head;
    }
    if (erro_parque(park_name, cap, val_15, val_1h, val_d, head)) {
        free(park_name);
        return head;
    }
    parque *novo = create_parque(park_name, cap, val_15, val_1h, val_d);
    head = append_parque(head, novo);
    free(park_name);
    return head;
}