#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcoes.h"
#include "funcoesSecundarias.h"
#include "utilities.h"

int main()
{
    // Recebendo os comandos do usuário
    char comandos[100];

    printf("\nComandos disponíveis: \nlist tables\ncreate table <nome_da_tabela>\ninsert in <nome_da_tabela>\nlist table <nome_da_tabela>\ndelete table <nome_da_tabela>\nsearch in <nome_da_tabela>\ndelete key <primaryKey> in <nome_da_tabela>\n");

    while (1)
    {
        printf("\nDigite um comando:\n");
        printf("--> ");

        fgets(comandos, sizeof(comandos), stdin);
        interpretarComando(comandos);
    }

    return 0;
}