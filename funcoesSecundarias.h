#ifndef FUNCOESSECUNDARIAS_H
#define FUNCOESSECUNDARIAS_H

void comparacao(int opcao, double valorFinal, double valor, char *linhaCerta);
void removeTableName(char *tableNameToRemove);
int returnCount(FILE *file, char *columnName);
int searchOption(char *tableName, int count);
void removeSpaces(char *str);

#endif