#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "funcoes.h"
#include "funcoesSecundarias.h"
#include "utilities.h"

// Interpretar os comandos inseridos pelo usuário
void interpretarComando(char *comando)
{
    char word1[20], word2[20];
    char completeCommand[40];
    char tableName[50];

    // Formando um comando a partir de duas palavras
    sscanf(comando, "%s %s", word1, word2);
    snprintf(completeCommand, sizeof(completeCommand), "%s %s", word1, word2);

    // O nome da tabela é usado na maioria dos comandos, então tratamos ele no início
    // Caso o comando não necessite do nome da tabela, essa variável apenas não será usada
    sscanf(comando + strlen(completeCommand) + 1, "%s", tableName);

    // Identificando o que o usuário solicitou e chamando sua respectiva função

    if (strcmp(word1, "stop") == 0)
        exit(0);

    else if (strcmp(completeCommand, "create table") == 0)
    {
        // Verificando se já existe uma tabela com o nome que o usuário inseriu
        if (tabelaExistente(tableName, comando, completeCommand))
            createTable(tableName);

        else
            return;
    }

    else if (strcmp(completeCommand, "insert in") == 0)
    {
        if (tabelaExistente(tableName, comando, completeCommand) == 0)
            insertData(tableName);

        else
        {
            invalidCommand(comando, 6);
            return;
        }
    }

    else if (strcmp(completeCommand, "list tables") == 0)
        listTable("tableNames");

    else if (strcmp(completeCommand, "list table") == 0)
    {
        if (!tabelaExistente(tableName, comando, completeCommand))
            listTable(tableName);

        else
        {
            invalidCommand(comando, 6);
            return;
        }
    }

    else if (strcmp(completeCommand, "delete table") == 0)
    {
        if (!tabelaExistente(tableName, comando, completeCommand))
            deleteTable(tableName);

        else
        {
            invalidCommand(comando, 6);
            return;
        }
    }

    else if (strcmp(completeCommand, "delete key") == 0)
        deleteKey(comando, completeCommand);

    else if (strcmp(completeCommand, "search in") == 0)
    {
        if (!tabelaExistente(tableName, comando, completeCommand))
            searchIn(tableName);

        else
        {
            invalidCommand(comando, 6);
            return;
        }
    }

    else
    {
        invalidCommand(completeCommand, 1);
        return;
    }
}

// Abrir arquivos de forma mais rápida e fácil
FILE *abrirArquivo(char *nome, int readType)
{
    FILE *file;

    char nameFile[50];

    strcpy(nameFile, returnTxt(nome));

    switch (readType)
    {
    case 1:
        file = fopen(nameFile, "r");
        break;

    case 2:
        file = fopen(nameFile, "w");
        break;

    case 3:
        file = fopen(nameFile, "r+");
        break;

    case 4:
        file = fopen(nameFile, "a");
        break;

    default:
        break;
    }

    if (file == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo da tabela %s.\n", nome);
        return NULL;
    }

    else
        return file;
}

// Verifica se a tabela que o usuário quer criar já existe
int tabelaExistente(char *tableName, const char *comando, char completeCommand[])
{
    FILE *file = abrirArquivo("tableNames", 1);
    if (file == NULL)
        return 0;

    char linha[500];

    if (strcmp(comando + strlen(completeCommand) + strlen(tableName) + 1, "\n") == 0)
    {
        fgets(linha, sizeof(linha), file);
        if (strstr(linha, tableName) != NULL)
        {
            invalidCommand("void", 5);
            return 0;
        }

        else
            return 1;
    }

    else
    {
        invalidCommand("void", 3);
        return 0;
    }
}

// Substituindo uma ',' por um ' '
void str_replace(char *str)
{
    char old_char = ',', new_char = ' ';
    while (*str)
    {
        if (*str == old_char)
            *str = new_char;

        printf("%c", *str);
        str++;
    }
}

// Recebe <name> e retorna <name>.txt
char *returnTxt(char *tableName)
{
    char *tableNameTxt = (char *)malloc(strlen(tableName) + 5); // +5 para ".txt" e o caractere nulo '\0'

    if (tableNameTxt == NULL)
    {
        // Tratando erro de alocação de memória, se necessário
        perror("Erro na alocação de memória\n");
        exit(EXIT_FAILURE);
    }

    // Construindo a string
    sprintf(tableNameTxt, "%s.txt", tableName);

    // Retornando o ponteiro para a string alocada dinamicamente
    return tableNameTxt;
}

// Verifica se a string inserida corresponde a um tipo de dado
int isType(const char *str, const char *type)
{
    char *endptr;
    if (strcmp(type, "int") == 0)
    {
        // Verificando se a string é um número inteiro
        strtol(str, &endptr, 10);
        return (*endptr == '\0');
    }
    else if (strcmp(type, "char") == 0)
    {
        // Verificando se a string é um caractere
        return (strlen(str) == 1);
    }
    else if (strcmp(type, "float") == 0 || strcmp(type, "double") == 0)
    {
        // Verificando se a string é um número de ponto flutuante ou double
        strtod(str, &endptr);
        return (*endptr == '\0');
    }
    else if (strcmp(type, "string") == 0)
    {
        // Sempre retorna verdadeiro para o tipo "string"
        return 1;
    }
    else
    {
        // Tipo desconhecido
        return 0;
    }
}

// Verifica qual é o tipo de dado da coluna
int typeValidate(const char *tipo)
{
    if (!strcmp(tipo, "char"))
        return 2;

    else if (!strcmp(tipo, "int"))
        return 1;

    else if (!strcmp(tipo, "float"))
        return 1;

    else if (!strcmp(tipo, "double"))
        return 1;

    else if (!strcmp(tipo, "string"))
        return 2;

    else
        return 0;
}

// Todas as respostas para comandos inválidos
void invalidCommand(const char *comando, int opcao)
{
    switch (opcao)
    {
    case 1:
        printf("--> %s: comando inválido.\n", comando);
        break;

    case 2:
        printf("--> %s: tipo inválido.\n", comando);
        break;

    case 3:
        printf("--> O nome da coluna não deve conter espaços em branco\n");
        break;

    case 4:
        printf("--> Insira dados compatíveis com o tipo de dados da coluna\n");
        break;

    case 5:
        printf("--> Existe uma tabela com este nome\n");
        break;

    case 6:
        printf("--> Insira o nome de uma tabela existente\n");
        break;

    default:
        break;
    }
}

// Exibe uma linha de uma tabela a partir de um índice
void returnLine(char *tableName, int idxLine)
{
    FILE *file = abrirArquivo(tableName, 1);
    if (file == NULL)
        return;

    char linha[200];
    int count = 0;

    // Lê o arquivo linha por linha
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        if (count == idxLine)
        {
            str_replace(linha);
            fclose(file);
            return;
        }
        count++;
    }

    return;
}

void cleamMemory(char **ptr, int tamanho)
{
    for (int i = 0; i < tamanho; i++)
        free(ptr[i]);
    free(ptr);
}