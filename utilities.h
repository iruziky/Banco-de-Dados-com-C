#ifndef UTILITIES_H
#define UTILITIES_H

char *returnTxt(char *tableName);
int typeValidate(const char *tipo);
int isType(const char *str, const char *type);
int tabelaExistente(char *tableName, const char *comando, char completeCommand[]);
void interpretarComando(char *comando);
void invalidCommand(const char *comando, int opcao);
void returnLine(char *tableName, int idxLine);
void str_replace(char *str);
FILE *abrirArquivo(char *nome, int readType);

#endif