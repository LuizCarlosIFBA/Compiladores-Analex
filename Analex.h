#ifndef ANALEX
#define ANALEX

#define TAM_MAX_LEXEMA 31

// Cálculos aritméticos
enum TOKEN_CAT { ID = 1, SN, CT_I, CT_R, CT_S, CT_C, FIM_EXPR, FIM_ARQ };

/* Onde: ID: Identificador, SN: Sinal; CT_I: Constante numérica inteira, CT_R: Constante numérica real */
enum SINAIS { ATRIB = 1, ADICAO, SUBTRACAO, MULTIPLIC, DIVISAO, ABRE_PAR, FECHA_PAR, ABRE_COLCHETE, FECHA_COLCHETE,UNDERLINE,PONTO_VIRGULA}; // Sinais válidos da linguagem

typedef struct {
    enum TOKEN_CAT cat; // deve receber uma das constantes de enum TOKEN_CAT
    union { // parte variável do registro
        int codigo; // para tokens das categorias SN
        char valString[TAM_MAX_LEXEMA]; // valor de um caractere STRINGCON
        char valChar[TAM_MAX_LEXEMA]; // valor de um caractere sCHARCON
        char lexema[TAM_MAX_LEXEMA]; // cadeia de caracteres que corresponde ao nome do token da cat. ID
        int valInt; // valor da constante inteira em tokens da cat. CT_I
        float valReal; // valor da constante real em tokens da cat. CT_R
    };
} TOKEN; // Tipo TOKEN

#endif

/* Contador de linhas do código fonte */
int contLinha = 1;

