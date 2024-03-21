#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "funcoes.h"
#include "funcoesSecundarias.h"
#include "utilities.h"

// Definindo uma estrutura para armazenar o que o usuário inserir
typedef struct
{
    char tipo[20];
    char nome[50];
} Coluna;

// Criando uma tabela com um arquivo .txt
void createTable(char *tableName)
{
    Coluna *colunas = NULL;
    int numColunas;
    char entrada[100];
    int verification = 1;

    printf("\nDigite o tipo de cada coluna seguido do nome (Exemplo: 'int idade string nome float nota'):\n");
    fgets(entrada, sizeof(entrada), stdin);

    char *token = strtok(entrada, " ");
    for (numColunas = 0; token != NULL; numColunas++)
    {
        // Alocando memória para armazenar as informações da coluna
        colunas = realloc(colunas, (numColunas + 1) * sizeof(Coluna));

        // Dividindo o token em nome e tipo
        sscanf(token, "%s", colunas[numColunas].tipo);
        token = strtok(NULL, " ");
        sscanf(token, "%s", colunas[numColunas].nome);

        // Verificando se o tipo de dado fornecido está correto
        if (typeValidate(colunas[numColunas].tipo) == 0)
        {
            verification = 0;
            invalidCommand(colunas[numColunas].tipo, 2);
            break;
        }

        // Avançando para o próximo token
        token = strtok(NULL, " ");
    }

    if (verification)
    {
        FILE *arquivo = abrirArquivo(tableName, 4);
        FILE *tablesDB = abrirArquivo("tableNames", 4);

        if (arquivo == NULL || tablesDB == NULL)
            return;

        // Inserindo o nome da tabela
        fprintf(tablesDB, "%s,", tableName);
        fprintf(arquivo, "%s\n", tableName);

        // Inserindo a linha de tipos
        fprintf(arquivo, "int,");
        for (int i = 0; i < numColunas; i++)
            fprintf(arquivo, "%s,", colunas[i].tipo);
        fprintf(arquivo, "\n");

        // Inserido a linha com nome das células
        fprintf(arquivo, "primaryKey,");
        for (int i = 0; i < numColunas; i++)
            fprintf(arquivo, "%s,", colunas[i].nome);

        fprintf(arquivo, "\n");

        // Fechando os arquivos
        fclose(arquivo);
        fclose(tablesDB);
    }
    else
    {
        return;
    }

    free(colunas);
}

// Inserindo na tabela as tuplas informadas pelo usuário
void insertData(char *tableName)
{
    FILE *arquivo = abrirArquivo(tableName, 3);

    if (arquivo == NULL)
        return;

    char entrada[200];
    char **columnType = (char **)malloc(30 * sizeof(char *));
    int compatibilidade, lineCount = 0, columnCount = 0;

    printf("Digite 'stop' para encerrar a inserção de dados\n\n");

    printf("Insira os seguintes dados: ");
    returnLine(tableName, 2);
    printf("\n");

    // Obtendo a linha que descreve os tipos de cada coluna
    char typesLine[200];
    fscanf(arquivo, "%49[^\n]", typesLine);
    while (fgetc(arquivo) != '\n')
        ;
    fscanf(arquivo, "%199[^\n]", typesLine);

    if (fseek(arquivo, 0, SEEK_END) != 0)
    {
        perror("Erro ao posicionar o ponteiro no final do arquivo\n");
        fclose(arquivo);
        free(columnType);
        return;
    }

    columnType[0] = strtok(typesLine, ",");
    for (int i = 0; columnType[i] != NULL; i++, columnCount++)
        columnType[i + 1] = strtok(NULL, ",");

    while (1)
    {
        // Lendo a entrada do usuário
        fgets(entrada, sizeof(entrada), stdin);
        if (strcmp(entrada, "stop\n") == 0)
            break;

        // Removendo o caractere de nova linha, se estiver presente
        size_t length = strlen(entrada);
        if (length > 0 && entrada[length - 1] == '\n')
            entrada[length - 1] = '\0';

        char **linhas = (char **)malloc(strlen(entrada) * sizeof(char *));
        linhas[0] = strtok(entrada, " ");
        for (int i = 0; linhas[i] != NULL; i++, lineCount++)
            linhas[i + 1] = strtok(NULL, " ");

        // Verificando se o usuário inseriu a quantidade de dados necessária
        if (columnCount == lineCount)
        {
            for (int i = 0; i < lineCount; i++)
            {
                // Verificando se os dados inseridos condizem com os tipos de dados das colunas
                if (!isType(linhas[i], columnType[i]))
                {
                    compatibilidade = 0;
                    printf("%s\n", linhas[i]);
                }
            }
        }
        else
            compatibilidade = 0;

        // Caso os dados passem nos testes, eles serão registrados
        if (compatibilidade)
        {
            for (int i = 0; i < lineCount; i++)
                fprintf(arquivo, "%s,", linhas[i]);
            fprintf(arquivo, "\n");
        }
        else
        {
            invalidCommand(linhas[0], 4);
            compatibilidade = 1;
        }

        lineCount = 0;
    }

    printf("\nDados inseridos com sucesso na tabela %s.\n", tableName);

    // Libera a memória alocada para columnType
    free(columnType);
    fclose(arquivo);
    return;
}

// Listando todas as tabelas
void listTable(char *tableName)
{
    char linha[1000];
    FILE *arquivo = abrirArquivo(tableName, 1);

    if (arquivo == NULL)
        return;

    printf("\n");
    // Lendo e imprimindo cada linha do arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL)
        str_replace(linha);

    printf("\n");

    // Fechando o arquivo
    fclose(arquivo);
}

// Deletando o arquivo .txt, mas ainda falta excluir o nome da tabela tableNames.txt
void deleteTable(char *str)
{
    char nomeArquivo[50];

    // Copiando a string para nomeArquivo
    strcpy(nomeArquivo, returnTxt(str));

    // Tentando excluir o arquivo
    if (remove(nomeArquivo) == 0)
        printf("Arquivo \"%s\" excluído com sucesso.\n", str);

    else
        perror("Erro ao excluir o arquivo\n");

    removeTableName(str);
}

// "Deletando" uma linha em função da sua chave primária
void deleteKey(char *comando, char *completeCommand)
{
    // lendo o resto do comando
    char key[5];
    char tableName[50];
    sscanf(comando + strlen(completeCommand) + 1, "%s", key);
    sscanf(comando + strlen(completeCommand) + strlen(key) + 4, "%s", tableName);

    FILE *arquivoOriginal = abrirArquivo(tableName, 1);
    FILE *arquivoTemporario = abrirArquivo("temporario", 2);

    if (arquivoOriginal == NULL || arquivoTemporario == NULL)
        return;

    char allKeys[50];
    char linha[500];

    // Pegando somente a primeira string de cada linha, delimitado por uma ","
    while (fgets(linha, sizeof(linha), arquivoOriginal) != NULL)
    {
        sscanf(linha, "%[^,]", allKeys);

        // Quando a chave inserida pelo usuário for igual a chave que está sendo analisada,
        // a linha em questão não será reescrita
        if (strcmp(allKeys, key) != 0)
            fprintf(arquivoTemporario, "%s", linha);
    }

    printf("Tupla excluida com sucesso\n");

    // Fechando os arquivos
    fclose(arquivoOriginal);
    fclose(arquivoTemporario);

    // Removendo o arquivo original
    if (remove(returnTxt(tableName)) != 0)
    {
        perror("Erro ao remover o arquivo original\n");
        return;
    }

    // Renomeando o arquivo temporário para o nome original
    if (rename("temporario.txt", returnTxt(tableName)) != 0)
    {
        perror("Erro ao renomear o arquivo temporário\n");
        return;
    }

    return;
}

// Fazendo uma pesquisa em uma tabela
void searchIn(char *tableName)
{
    char *columnName = (char *)malloc(50 * sizeof(char));

    printf("\nDigite o nome da coluna a pesquisar:\n");

    returnLine(tableName, 2);
    fgets(columnName, 50, stdin);

    searchInColumn(tableName, columnName);

    free(columnName);
    return;
}

// Registra os dados da coluna que o usuário quer pesquisar, e como esses dados devem ser tratados
void searchInColumn(char *tableName, char *columnName)
{
    char linha[1000], linhaAux[1000];
    int resultado, idxColumns = 0;
    char **columns = (char **)malloc(sizeof(char *));
    char **linhaCerta = (char **)malloc(sizeof(char *));
    int *columnsInt = (int *)malloc(sizeof(int));
    char *token;
    FILE *file = abrirArquivo(tableName, 3);

    if (file == NULL)
        return;

    // Identificando a posição da coluna que o usuário inseriu
    int count = returnCount(file, columnName);

    // Obtendo as células da coluna que o usuário inseriu
    while (fgets(linha, sizeof(linha), file) != NULL)
    {
        strcpy(linhaAux, linha);
        token = strtok(linha, ",");
        for (int i = 0; i <= count; i++)
        {
            if (i == count)
            {
                resultado = searchOption(tableName, count);

                if (resultado == 1)
                {
                    columnsInt[idxColumns] = atoi(token);
                    idxColumns++;
                    columnsInt = realloc(columnsInt, (idxColumns + 1) * sizeof(int *));
                }
                else if (resultado == 2)
                {
                    columns[idxColumns] = (char *)malloc(50 * sizeof(char));
                    strcpy(columns[idxColumns], token);
                    idxColumns++;
                    columns = realloc(columns, (idxColumns + 1) * sizeof(char *));
                }

                linhaCerta[idxColumns - 1] = (char *)malloc(1000 * sizeof(char));
                strcpy(linhaCerta[idxColumns - 1], linhaAux);
                linhaCerta = realloc(linhaCerta, (idxColumns + 1) * sizeof(char *));

                break;
            }
            else
            {
                token = strtok(NULL, ",");
            }
        }
    }

    double valorFinal, valor;
    int opcao;
    printf("\n1 - Valores maior que o valor informado\n"
           "2 - Valores maior ou igual que o valor informado\n"
           "3 - Valores igual o valor informado\n"
           "4 - Valores menor que o valor informado\n"
           "5 - Valores menor ou igual que o valor informado\n\n"
           "Escolha uma opcao:\n");
    scanf("%d", &opcao);
    printf("Insira um valor de referência:\n");
    scanf("%lf", &valor);
    getchar();
    printf("\n");

    if (resultado != 1 && resultado != 2)
    {
        printf("Insira um valor válido\n");
        fclose(file);
        return;
    }

    // Comparando os dados da coluna selecionada com o critério imposto pelo usuário
    for (int i = 0; i < idxColumns; i++)
    {
        if (resultado == 1)
            valorFinal = columnsInt[i];

        else if (resultado == 2)
            valorFinal = (int)strlen(columns[i]);

        // comparacao() compara e exibi os valores válidos
        comparacao(opcao, valorFinal, valor, linhaCerta[i]);
    }

    if (resultado)
        free(columns);

    else if (resultado == 2)
        free(columnsInt);

    fclose(file);
    free(linhaCerta);
    return;
}