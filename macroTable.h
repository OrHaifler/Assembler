#include "ast.h"

#ifndef __MACROT__
#define __MACROT__

#define ERROR -1
#define SUCSESS 0

typedef struct macroCodeLine
{
    char* code;
    struct macroCodeLine* next;
}macroCodeLine;

typedef struct macroTableEntery
{
    char* macroName;
    macroCodeLine* line;
    struct macroTableEntery* next;
}macroTableEntery;

typedef struct macroTable
{
    macroTableEntery* head;
    int len;
}macroTable;

int addToMacroTableWithCode(macroTable* table,char* name,char* code);
int addToMacroTable(macroTable* table,char* name);
macroTableEntery* getEntery(macroTable* table,char* name);
int addCodeLineToTable(macroTable* table,char* name,char* code);
void deleteTable(macroTable* table);
void deleteEnteries(macroTableEntery* curr);
void deleteLines(macroCodeLine* curr);
#endif