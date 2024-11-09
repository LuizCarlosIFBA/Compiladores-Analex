#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Analex.h"

// Declaração de variáveis globais
TOKEN tk;
FILE *fd;
int contLinha = 1;

// Função auxiliar para verificar se o caractere é um dígito
int isDigit(char c) {
    return isdigit(c);
}

// Função para ler próximo caractere do arquivo
int nextChar(FILE *fd) {
    return fgetc(fd);
}

// Função para reconhecer números inteiros e reais
void getNumber(FILE *fd, TOKEN *token) {
    char lexema[TAM_MAX_LEXEMA + 1];
    int i = 0;
    int isReal = 0;
    char c = fgetc(fd);

    // Lê dígitos e o ponto decimal para reconhecer números reais
    while (isDigit(c) || c == '.') {
        if (i < TAM_MAX_LEXEMA) {
            lexema[i++] = c;
            if (c == '.') isReal = 1; // Verifica se é um número real
        }
        c = nextChar(fd);
    }
    lexema[i] = '\0';
    ungetc(c, fd);

    // Define o tipo do token como inteiro ou real com base no conteúdo
    if (isReal) {
        token->cat = CT_REAL;
        token->valfloat = atof(lexema);
    } else {
        token->cat = CT_INTEIRA;
        token->valInt = atoi(lexema);
    }
}

// Função principal do analisador léxico
TOKEN Analex(FILE *fd) {
    TOKEN token;
    char c;

    // Loop para leitura do arquivo
    while ((c = nextChar(fd)) != EOF) {
        if (isspace(c)) {
            if (c == '\n') contLinha++;
            continue;
        } else if (isDigit(c)) {
            ungetc(c, fd);
            getNumber(fd, &token);
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
        if (token.cat == CT_INTEIRA) {
            printf("Token Intcon: %d\n", token.valInt);
        } else if (token.cat == CT_REAL) {
            printf("Token Realcon: %f\n", token.valfloat);
        }
    }

    fclose(fd);
    return 0;
}

