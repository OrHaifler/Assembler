#include "ast.h"
#include "data.h"
#include "symbol.h"

int second_pass(ASTLines *lines,SymbolTable *symbol_table,Data *data); 
char** encodeSymbols(AST* line_ast,SymbolTable *symbol_table,int* size);
char* symToEncoded(char* name,SymbolTable* symbol_table,int type,int number);