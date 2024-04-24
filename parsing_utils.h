#include "ast.h"
#include "symbol.h"


int is_define(AST *line_ast); 
int is_data(AST *line_ast); 
int is_string(AST *line_ast); 
int is_extern(AST *line_ast); 
int is_entry(AST *line_ast); 