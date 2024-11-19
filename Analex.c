#include <stdio.h>
#include <stdlib.h>
#include "Analex.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h> // Para o uso de true e false

#define TAM_MAX_LEXEMA 31
#define TAM_NUM 20



TOKEN AnaLex(FILE *fd);

int main()
{
    FILE *fd = fopen("expressao.dat", "r");
    if (fd == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    TOKEN t;
    while ((t = AnaLex(fd)).cat != FIM_ARQ)
    {
        switch (t.cat)
        {
        case STRINGCON:
            printf("<STRINGCON, \"%s\">\n", t.valStr);
            break;
        case CHARCON:
            printf("<CHARCON, \"%c\">\n", t.valChar);
            break;
        case SN:
            printf("<SN, %c>\n", t.valChar);
            break;
        case CT_I:
            printf("<INTCON, %d>\n", t.valInt);
            break;
        case CT_R:
            printf("<REALCON, %.2f>\n", t.valReal);
            break;
        case FIM_EXPR:
            printf("<FIM_EXPR, 0>\n");
            break;
        case ID:
            printf("<ID, \"%s\">\n", t.valStr);
            break;
        default:
            break;
        }
    }
    fclose(fd);
    return 0;
}

TOKEN AnaLex(FILE *fd)
{
    int estado = 0;
    char lexema[TAM_MAX_LEXEMA] = "";
    int tamL = 0;
    char digitos[TAM_NUM] = "";
    int tamD = 0;

    TOKEN t;
    estado = 0;

    while (true)
    {
        char c = fgetc(fd);
        if (c == EOF)
        {
            t.cat = FIM_ARQ;
            return t;
        }

        c = tolower(c); // Converte o caractere para minúsculo, se necessário

        switch (estado)
        {
        case 0: // Estado inicial
            if (c == '"')
            { // Início de string
                estado = 9;
            }
            else if (c == '\'')
            { // Início de caractere
                estado = 10;
            }
            else if (isdigit(c))
            { // Início de número (inteiro ou real)
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
                estado = 1; // Estado de número
            }
            else if (c == '.')
            { // Ponto, possivelmente para número real
                estado = 16;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else if (isspace(c))
            { // Ignorar espaços em branco
                continue;
            }
            else if (isalpha(c))
            { // Verifica se é uma letra (possivelmente um identificador ou string)
                lexema[tamL++] = c;
                estado = -1; // Estado de string ou identificador
            }
            else
            { // Símbolos e operadores
                t.cat = SN;
                t.valChar = c;
                return t;
            }
            break;

        case 9: // Lê string (delimitada por aspas)
            if (c == '"')
            { // Fim da string
                t.cat = STRINGCON;
                lexema[tamL] = '\0';
                strcpy(t.valStr, lexema);
                return t;
            }
            else
            {
                lexema[tamL++] = c;
            }
            break;

        case 10: // Lê caractere (delimitado por aspas simples)
            if (c == '\'')
            { // Fim do caractere
                t.cat = CHARCON;
                t.valChar = lexema[0];
                return t;
            }
            else
            {
                lexema[tamL++] = c;
            }
            break;

        case 1: // Lê número inteiro ou real
            if (isdigit(c))
            { // Continua lendo números inteiros
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else if (c == '.')
            { // Número real
                estado = 3;
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else
            { // Fim do número inteiro
                ungetc(c, fd);
                t.cat = CT_I;
                t.valInt = atoi(digitos);
                return t;
            }
            break;

        case 3: // Lê número real
            if (isdigit(c))
            { // Continua lendo número real
                digitos[tamD++] = c;
                digitos[tamD] = '\0';
            }
            else
            { // Fim do número real
                ungetc(c, fd);
                t.cat = CT_R;
                t.valReal = atof(digitos);
                return t;
            }
            break;

        case -1: // Estado de string ou identificador
            if (isalnum(c) || c == '_')
            { // Parte do identificador (letras, números ou underline)
                lexema[tamL++] = c;
            }
            else
            { // Fim do identificador
                ungetc(c, fd);
                lexema[tamL] = '\0';
                t.cat = (tamL == 1 && !isdigit(lexema[0])) ? CHARCON : STRINGCON;
                if (tamL == 1)
                {
                    t.valChar = lexema[0];
                }
                else
                {
                    strcpy(t.valStr, lexema);
                }
                return t;
            }
            break;

        default:
            break;
        }
    }
}
