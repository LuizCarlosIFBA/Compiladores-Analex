#ifndef ANALEX
#define ANALEX

#define TAM_MAX_LEXEMA 31

typedef enum{   
  FIM_ARQ, FIM_EXPR, STRINGCON, CHARCON, SN, CT_I, CT_R, ID
}TIPO_TOKEN;

typedef struct
{
    TIPO_TOKEN cat;
    union
    {
        char valStr[TAM_MAX_LEXEMA];
        char valChar;
        int valInt;
        float valReal;
    };
} TOKEN;
#endif

/* Contador de linhas do código fonte */
int contLinha = 1;

