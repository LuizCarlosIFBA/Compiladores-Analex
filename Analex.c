#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "Analex.h"

#define TAM_LEXEMA 50
#define TAM_NUM 20

void error(char msg[])
{
    printf("%s\n", msg);
    exit(1);
}

TOKEN AnaLex(FILE *fd)
{
    int estado;
    char lexema[TAM_LEXEMA] = "";
    int tamL = 0;
    char digitos[TAM_NUM] = "";
    int tamD = 0;

    TOKEN t;
    estado = 0;
    while (true)
    {
        char c = fgetc(fd);
        switch (estado)
        {
        case 0:
            if (c == ' ' || c == '\t')
                estado = 0;
            else if (c >= 'a' && c <= 'z')
            { // inicio de identificador - inicializa lexema
                estado = 8;
                lexema[tamL] = c;
                lexema[++tamL] = '\0';
            }
            else if (c == '"')
            { // Início de uma string
                estado = 8;
                tamL = 0; // Reset para acumular string
            }
            else if (c >= '0' && c <= '9')
            { // inicio de constante inteira - inicializa digitos
                estado = 1;
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else if (c == '+')
            { // sinal de adicao - monta e devolve token
                estado = 28;
                t.cat = SN;
                t.codigo = ADICAO;
                return t;
            }
            else if (c == '-')
            { // sinal de subtracao - monta e devolve token
                estado = 27;
                t.cat = SN;
                t.codigo = SUBTRACAO;
                return t;
            }
            else if (c == '*')
            { // sinal de multiplicacao - monta e devolve token
                estado = 26;
                t.cat = SN;
                t.codigo = MULTIPLIC;
                return t;
            }
            else if (c == '/')
            { // sinal de divisao - monta e devolve token
                estado = 29;
                t.cat = SN;
                t.codigo = DIVISAO;
                return t;
            }
            else if (c == '=')
            { // sinal de atribuicao - monta e devolve token
                estado = 40;
                t.cat = SN;
                t.codigo = ATRIB;
                return t;
            }
            else if (c == '(')
            { // sinal de abre parenteses - monta e devolve token
                estado = 20;
                t.cat = SN;
                t.codigo = ABRE_PAR;
                return t;
            }
            else if (c == ')')
            { // sinal de fecha parenteses - monta e devolve token
                estado = 21;
                t.cat = SN;
                t.codigo = FECHA_PAR;
                return t;
            }
            else if (c == '\n')
            {
                estado = 33;
                t.cat = FIM_EXPR; // fim de linha (ou expressao) encontrado
                contLinha++;
                return t;
            }
            else if (c == EOF)
            { // fim do arquivo fonte encontrado
                t.cat = FIM_ARQ;
                return t;
            }
            else
                error("Caracter invalido na expressao!"); // sem transicao valida no AFD
            break;

        case 111:
            if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == '_'))
            {
                estado = 1;
                lexema[tamL] = c; // acumula caracteres lidos em lexema
                lexema[++tamL] = '\0';
            }
            else
            {               // transicao OUTRO* do estado 1 do AFD
                estado = 2; // monta token identificador e retorna
                ungetc(c, fd);
                t.cat = ID;
                strcpy(t.lexema, lexema);
                return t;
            }
            break;

        case 1:
            if (c >= '0' && c <= '9')
            {
                estado = 2;
                digitos[tamD] = c; // acumula digitos lidos na variavel digitos
                digitos[++tamD] = '\0';
            }
            else if (c == '.')
            {               // ponto decimal encontrado
                estado = 3; // vai para estado de número real
                digitos[tamD] = c;
                digitos[++tamD] = '\0';
            }
            else
            {               // transicao OUTRO* do estado 10 do AFD
                estado = 1; // monta token constante inteira e retorna
                ungetc(c, fd);
                t.cat = CT_I;
                t.valInt = atoi(digitos);
                return t;
            }
            break;

        case 3: // estado de número real
            if (c >= '0' && c <= '9')
            {
                digitos[tamD] = c; // acumula dígitos da parte fracionária
                digitos[++tamD] = '\0';
            }
            else
            {
                estado = 6; // monta token constante real e retorna
                ungetc(c, fd);
                t.cat = CT_R;
                t.valReal = atof(digitos); // converte string para float
                return t;
            }
            break;

        case 8: // Estado para capturar conteúdo de uma string
            if (c >= 32 && c <= 126 && c != '"')
            { // ASCII imprimível e diferente de aspas
                if (tamL < TAM_MAX_LEXEMA - 1)
                { // Evita overflow
                    lexema[tamL++] = c;
                    lexema[tamL] = '\0';
                }
            }
            else if (tamL>1)
            { // Fim da string
                t.cat = CT_S;
                strcpy(t.valString, lexema);
                return t;
            }else { // Caracter inválido em string
                t.cat = CT_C;
                strcpy(t.valChar, lexema);
                return t;
            }
            break;
        case 10:
            if (c >= 32 && c <= 126 && c != '"')
            { // ASCII imprimível e diferente de aspas
                if (tamL == 1)
                {                        // Evita overflow
                    lexema[tamL++] = c;  // Adiciona o char ao lexema
                    lexema[tamL] = '\0'; // Finaliza o lexema com '\0'
                }
            }
            else if (c == '"')
            {                                // Fim da string (aspas encontradas)
                t.cat = CT_C;                // Define a categoria como STRINGCON
                strcpy(t.valChar, lexema); // Copia o conteúdo da string reconhecida
                return t;                    // Retorna o token
            }
            else
            {                                          // Caracter inválido em string
                error("Caracter inválido no char!"); // Exibe mensagem de erro
            }
        }
    }
}

int main()
{
    FILE *fd;
    TOKEN tk;

    if ((fd = fopen("expressao.dat", "r")) == NULL)
        error("Arquivo de entrada da expressao nao encontrado!");

    printf("LINHA %d: ", contLinha);

    while (1)
    { // laço infinito para processar a expressão até o fim de arquivo
        tk = AnaLex(fd);
        switch (tk.cat)
        {
        case ID:
            printf("<ID, %s> ", tk.lexema);
            break;
        case SN:
            switch (tk.codigo)
            {
            case ADICAO:
                printf("<SN, ADICAO> ");
                break;
            case SUBTRACAO:
                printf("<SN, SUBTRACAO> ");
                break;
            case MULTIPLIC:
                printf("<SN, MULTIPLICACAO> ");
                break;
            case DIVISAO:
                printf("<SN, DIVISAO> ");
                break;
            case ATRIB:
                printf("<SN, ATRIBUICAO> ");
                break;
            case ABRE_PAR:
                printf("<SN, ABRE_PARENTESES> ");
                break;
            case FECHA_PAR:
                printf("<SN, FECHA_PARENTESES> ");
                break;
            }
            break;
        case CT_I:
            printf("<INTCON, %d> ", tk.valInt);
            break;
        case CT_R:
            printf("<REALCON, %.2f> ", tk.valReal);
            break;
        case CT_S:
            printf("<STRINGCON, \"%s\"> ", tk.valString);
            break;
        case CT_C:
            printf("<CHARCON, \"%s\"> ", tk.valChar);
            break;
        case FIM_EXPR:
            printf("<FIM_EXPR, %d>\n", 0);
            printf("LINHA %d: ", contLinha);
            break;
        case FIM_ARQ:
            printf(" <Fim do arquivo encontrado>\n");
            break;
        }
        if (tk.cat == FIM_ARQ)
            break;
    }

    fclose(fd);
    return 0;
}
