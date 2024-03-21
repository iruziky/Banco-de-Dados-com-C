#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "funcoes.h"
#include "funcoesSecundarias.h"
#include "utilities.h"

// Identifica o tipo de dado de uma coluna em uma tabela
int searchOption(char *tableName, int count)
{
    FILE *file = abrirArquivo(tableName, 1);
    if (file == NULL)
        return 0;

    char *token;
    char linha[200];
    int resultado;

    fgets(linha, sizeof(linha), file);
    fgets(linha, sizeof(linha), file);

    token = strtok(linha, ",");
    for (int i = 0; i <= count; i++)
    {
        if (i == count)
        {
            resultado = typeValidate(token);
            return resultado;
        }
        else
        {
            token = strtok(NULL, ",");
        }
    }

    return 0;
}

// Removendo de tableNames.txt o nome da tabela a ser excluída
void removeTableName(char *tableNameToRemove)
{
    FILE *file = abrirArquivo("tableNames", 1);
    if (file == NULL)
        return;

    strcat(tableNameToRemove, ",");

    // Lendo o conteúdo do arquivo
    char *content = NULL;
    size_t size = 0;
    ssize_t bytesRead;
    while ((bytesRead = getline(&content, &size, file)) != -1)
    {
        // Procuraando a tabela a ser removida
        char *found = strstr(content, tableNameToRemove);
        if (found != NULL)
        {
            // Removendo a tabela
            memmove(found, found + strlen(tableNameToRemove), strlen(found + strlen(tableNameToRemove)) + 1);
        }
    }

    // Fechando o arquivo de leitura
    fclose(file);

    // Abrindo o arquivo para escrita (sobrescrevendo)
    file = abrirArquivo("tableNames", 2);

    // Escrevendo o conteúdo modificado de volta no arquivo
    fputs(content, file);
    fclose(file);
    free(content);
}

// Remove espaços em branco
void removeSpaces(char *str)
{
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++)
    {
        if (!isspace((unsigned char)str[i]))
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// Retorna o número da coluna que o usuário digitou
int returnCount(FILE *file, char *columnName)
{
    char linha[1000];
    char *token1 = (char *)malloc(50 * sizeof(char));
    int count = 0, verificador = 0;

    // Removendo espaços em branco da coluna inserida pelo usuário
    removeSpaces(columnName);

    // Identificando a coluna que o usuário inseriu
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        // Remove o caractere de nova linha, se presente
        linha[strcspn(linha, "\n")] = '\0';
        token1 = strtok(linha, ",");

        while (token1 != NULL)
        {
            // Removendo espaços em branco do token atual
            removeSpaces(token1);

            if (strcmp(token1, columnName) == 0)
            {
                verificador = 1;
                break;
            }
            token1 = strtok(NULL, ",");
            count++;
        }

        if (verificador)
            break;

        count = 0;
    }

    if (verificador)
        return count;

    else
        return -1;
}

// Compara se o valor da célula respeita o critério do usuário, caso sim, exibe esse valor
void comparacao(int opcao, double valorFinal, double valor, char *linhaCerta)
{
    switch (opcao)
    {

    case 1:
    {
        if (valorFinal > valor)
            str_replace(linhaCerta);
        return;
    }

    case 2:
    {
        if (valorFinal >= valor)
            str_replace(linhaCerta);
        return;
    }

    case 3:
    {
        if (valorFinal == valor)
            str_replace(linhaCerta);
        return;
    }

    case 4:
    {
        if (valorFinal < valor)
            str_replace(linhaCerta);
        return;
    }

    case 5:
    {
        if (valorFinal <= valor)
            str_replace(linhaCerta);
        return;
    }

    default:
        return;
    }
}