#ifndef ANAALEX_H
#define ANAALEX_H

#define TAM_MAX_LEXEMA 100

// Estrutura do token focada apenas em números inteiros e reais
typedef struct {
    int cat; // Categoria do token (CT_INTEIRA ou CT_REAL)
    union {
        int valInt;
        float valfloat;
    };
} TOKEN;

// Definição de constantes para categorias de tokens
#define CT_INTEIRA 5
#define CT_REAL 6
#define FIM_ARQ 10  // Indica fim de arquivo

// Declaração das funções
TOKEN Analex(FILE *fd);
void getNumber(FILE *fd, TOKEN *token);

#endif

