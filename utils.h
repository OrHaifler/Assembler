#ifndef UTILS_H
#define UTILS_H

#include "data.h"
#include "ast.h"
#include "symbol.h"

void get_operands(AST *line_ast); 
void get_symbol(AST *line_ast); 
void print_symtable(SymbolTable *symbol_table); 
void print_data(Data *data); 
int symTypeToAre(int symType);

#endif