#include "ast.h"
#include "symbol.h"
#include <string.h>
#include <ctype.h>


int valid_data(AST *line_ast, SymbolTable *symbol_table); 
int valid_op(AST *line_ast);
int valid_int(char *str_rep);
int valid_symbol(AST *line_ast, SymbolTable *symbol_table, int IC); 
int valid_defined(char *symbol, SymbolTable *symbol_table); 
int valid_define(char *op); 
int valid_label(char *op);
int valid_commas(AST *line_ast); 
void valid_inst(AST* line_ast); 
int valid_reg(char *op);