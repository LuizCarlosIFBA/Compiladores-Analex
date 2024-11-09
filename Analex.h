#ifndef ANAALEX_H
#define ANAALEX_H

#define TAM_MAX_LEXEMA 100

typedef struct {
    int cat; // Categoria do token (ex: ID, INT, REAL, etc.)
    char lexema[TAM_MAX_LEXEMA]; // Armazena o lexema completo para identificadores e sequências
    union {
        int valInt;
        float valfloat;
        char charLiteral;  // Armazena um caractere único
    };
    int codigo; // Campo para armazenar códigos de símbolos (caso de SN)
} TOKEN;

// Defina suas funções e constantes
TOKEN Analex(FILE *fd);
void getIdentifierOrKeyword(FILE *fd, TOKEN *token);
void getNumber(FILE *fd, TOKEN *token);
void getCharacter(FILE *fd, TOKEN *token); // Para sequência de caracteres
void getChar(FILE *fd, TOKEN *token);      // Para um único caractere

// Definição de constantes para categorias de tokens
#define CONST 1
#define INT 2
#define REAL 3
#define ID 4
#define CT_INTEIRA 5
#define CT_REAL 6
#define CT_CHAR 7       // Um único caractere, armazenado em `charLiteral`
#define CT_CARACTERE 8  // Uma sequência de caracteres, armazenada em `lexema`
#define SN 9            // Símbolos de operadores
#define FIM_ARQ 10      // Indica fim de arquivo

#endif

