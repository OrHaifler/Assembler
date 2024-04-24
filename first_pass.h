#include "ast.h"
#include "data.h"
#include "second_pass.h"

#define MAX_LEN 50


SymbolTable *firstPass(char* fileName,ASTLines* lines,SymbolTable* symbol_table,Data* data_image,int *error, int* IC,int* DC);
