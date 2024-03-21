#ifndef FUNCOES_H
#define FUNCOES_H

void listTable(char *tableName);
void deleteKey(char *comando, char *completeCommand);
void deleteTable(char *str);
void deleteLineFromFile(const char *str);
void searchIn(char *tableName);
void createTable(char *tableName);
void insertData(char *tableName);
void searchInColumn(char *tableName, char *columnName);

#endif