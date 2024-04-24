#include "ast.h"
#include "validate.h"
#include "data.h"


int calculate_length(AST *line_ast, SymbolTable *symbol_table); 
int immediate_addressing(char *op, SymbolTable *symbol_table); 
int arr_addressing(char *op, SymbolTable *symbol_table); 
int reg_addressing(char *op); 
int is_reg(char* name);
int bracket_addressing(char *op, SymbolTable *symbol_table); 