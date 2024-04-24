#include "ast.h"
#include "encoding.h"
#include "handle_parsing.h"
#include "symbol.h"

void insert_code(AST* line_ast, SymbolTable *symbol_table, Data* code, int *IC); 
Data *insert_op(AST *line_ast, Data *code, char **encoding, int size); 
void store_data(AST *line_ast,SymbolTable *symbol_table,Data *code,int *IC, int *DC); 
void store_string(AST *line_ast,Data *code,int *IC, int *DC); 
int data_label(char *op, SymbolTable *symbol_table); 
Data *get_nullterm(void); 
void update_data(Data *data);