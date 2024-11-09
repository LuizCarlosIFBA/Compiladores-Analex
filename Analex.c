#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Analex.h"

// Declaração de variáveis globais
TOKEN tk;
FILE *fd;
int contLinha = 1;

// Função auxiliar para verificar se o caractere é uma letra ou um underscore
int isLetterOrUnderscore(char c) {
    return isalpha(c) || c == '_';
}

// Função auxiliar para verificar se o caractere é um dígito
int isDigit(char c) {
    return isdigit(c);
}

// Função para ler próximo caractere do arquivo
int nextChar(FILE *fd) {
    return fgetc(fd);
}

// Função para reconhecer identificadores e palavras reservadas
void getIdentifierOrKeyword(FILE *fd, TOKEN *token) {
    char lexema[TAM_MAX_LEXEMA + 1];
    int i = 0;
    char c = fgetc(fd);

    while (isLetterOrUnderscore(c) || isDigit(c)) {
        if (i < TAM_MAX_LEXEMA) {
            lexema[i++] = c;
        }
        c = nextChar(fd);
    }
    lexema[i] = '\0';
    ungetc(c, fd);

    // Comparação com palavras reservadas
    if (strcmp(lexema, "CONST") == 0) token->cat = CONST;
    else if (strcmp(lexema, "INT") == 0) token->cat = INT;
    else if (strcmp(lexema, "REAL") == 0) token->cat = REAL;
    else {
        token->cat = ID;
        strncpy(token->lexema, lexema, TAM_MAX_LEXEMA);
    }
}

// Função para reconhecer números inteiros e reais
void getNumber(FILE *fd, TOKEN *token) {
    char lexema[TAM_MAX_LEXEMA + 1];
    int i = 0;
    int isReal = 0;
    char c = fgetc(fd);

    while (isDigit(c) || c == '.') {
        if (i < TAM_MAX_LEXEMA) {
            lexema[i++] = c;
            if (c == '.') isReal = 1;
        }
        c = nextChar(fd);
    }
    lexema[i] = '\0';
    ungetc(c, fd);

    if (isReal) {
        token->cat = CT_REAL;
        token->valfloat = atof(lexema);
    } else {
        token->cat = CT_INTEIRA;
        token->valInt = atoi(lexema);
    }
}

// Função para reconhecer um caractere entre aspas simples
void getChar(FILE *fd, TOKEN *token) {
    char c = nextChar(fd);  // Ler o caractere dentro das aspas simples
    token->cat = CT_CHAR;
    token->charLiteral = c;  // Armazena o valor char específico
    nextChar(fd);  // Ignorar o caractere de fechamento
}

// Função para reconhecer uma sequência de caracteres (strings)
void getCharacter(FILE *fd, TOKEN *token) {
    token->cat = CT_CHAR;
    int i = 0;
    char c;

    // Lê cada caractere até encontrar o delimitador final ou o limite do buffer
    while ((c = nextChar(fd)) != '\'' && i < TAM_MAX_LEXEMA - 1) {
        token->lexema[i++] = c;
    }
    token->lexema[i] = '\0'; // Termina a sequência de caracteres com '\0'
}

// Função principal do analisador léxico
TOKEN Analex(FILE *fd) {
    TOKEN token;
    char c;

    while ((c = nextChar(fd)) != EOF) {
        if (isspace(c)) {
            if (c == '\n') contLinha++;
            continue;
        } else if (isLetterOrUnderscore(c)) {
            ungetc(c, fd);
            getIdentifierOrKeyword(fd, &token);
            return token;
        } else if (isDigit(c)) {
            ungetc(c, fd);
            getNumber(fd, &token);
            return token;
        } else if (c == '\'') {
            getChar(fd, &token);
            return token;
        } else {
            // Outros tokens, como operadores e delimitadores
            token.cat = SN;
            token.codigo = c;  // Pode ser alterado para um código específico para cada símbolo
            return token;
        }
    }

    // Token indicando fim de arquivo
    token.cat = FIM_ARQ;
    return token;
}

// Função main para testar o analisador léxico
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
        return 1;
    }

    FILE *fd = fopen(argv[1], "r");
    if (fd == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    TOKEN token;
    printf("Tokens:\n");
    while (1) {
        token = Analex(fd);
        if (token.cat == FIM_ARQ) break; // Fim do arquivo

        // Exemplo de impressão do token; personalize conforme o que quer exibir
        switch (token.cat) {
            case CONST:
                printf("Token CONST\n");
                break;
            case INT:
                printf("Token INT\n");
                break;
            case REAL:
                printf("Token REAL\n");
                break;
            case ID:
                printf("Token ID: %s\n", token.lexema);
                break;
            case CT_INTEIRA:
                printf("Token Inteiro: %d\n", token.valInt);
                break;
            case CT_REAL:
                printf("Token Real: %f\n", token.valfloat);
                break;
            case CT_CHAR:
                printf("Token CHAR: '%c'\n", token.charLiteral);  // Imprime o valor do caractere
                break;
            case SN:
                printf("Token Símbolo: %c\n", token.codigo);
                break;
            default:
                printf("Token desconhecido\n");
                break;
        }
    }

    fclose(fd);
    return 0;
}

