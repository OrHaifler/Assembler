#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef struct SymbolTable {
	char* name;
	int value;
	char type[TYPE_LEN];
    int is_extern;
	struct SymbolTable *next;
} SymbolTable;

enum symTypes{immidiate,variable,constIndex,reg,variableExtern,indexExtern,error=-10};
void insert_symbol(AST *line_ast,SymbolTable **symbol_table,int IC,int DC); 
SymbolTable* is_in_symtable(char* name,SymbolTable* symbol_table);
int get_sym_type(char* operand,SymbolTable* symbol_table);
int get_defined(char *symbol, SymbolTable *symbol_table);
void update_table(SymbolTable *symbol_table); 
void remove_symbol(SymbolTable **symbol_table);

#endif