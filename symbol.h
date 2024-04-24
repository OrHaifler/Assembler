#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"

typedef struct SymbolTable {
	char* name;
	int value;
	char type[TYPE_LEN];
    int is_extern;
	struct SymbolTable *next;
} SymbolTable;

int is_extern(AST *line_ast);
enum symTypes{immidiate,variable,constIndex,reg,variableExtern,indexExtern,error=-10};
void insert_symbol(AST *line_ast,SymbolTable **symbol_table,int IC,int DC); 
SymbolTable* is_in_symtable(char* name,SymbolTable* symbol_table);
int get_sym_type(char* operand,SymbolTable* symbol_table);
int get_defined(char *symbol, SymbolTable *symbol_table);
void update_table(SymbolTable *symbol_table); 
void remove_symbol(SymbolTable **symbol_table);
int valid_symbol(AST *line_ast, SymbolTable *symbol_table, int IC); 
int valid_label(char *op);
int valid_int(char *str_rep);
void *printError(char errorType[MAX_ERROR_LEN]); 


#endif