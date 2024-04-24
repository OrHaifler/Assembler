#include "ast.h"
#include "validate.h"
#include "parsing_utils.h"

#define DEFINE_PREFIX ".define"
#define DEFINE_PREFIX_LENGTH 7 
#define DATA_PREFIX ".data"
#define DATA_PREFIX_LENGTH 5
#define STRING_PREFIX ".string"
#define STRING_PREFIX_LENGTH 7 
#define EXTERN_PREFIX ".external"
#define EXTERN_PREFIX_LENGTH 7
#define ENTRY_PREFIX ".entry"
#define ENTRY_PREFIX_LENGTH 6 
#define TYPE_LEN 10
#define DEFINE_TYPE ".mdefine"
#define DATA_STORING_TYPE ".data"
#define EXTERN_TYPE ".extern"
#define ENTRY_TYPE ".entry"
#define CODE_TYPE ".code"
#define FIRST_OPERAND 1
#define SECOND_OPERAND 2
#define OP_CODE_LEN 16


typedef struct Data Data;

int handle_define(AST *line_ast,SymbolTable **symbol_table); 
int handle_data(AST *line_ast,SymbolTable **symbol_table,Data *data_image,int *IC,int *DC); 
int handle_extern(AST *line_ast, SymbolTable **symbol_table, int *IC, int *DC); 
int handle_entry(AST *line_ast, SymbolTable **symbol_table, int *IC, int *DC); 
void handle_instruction(AST* line_ast,SymbolTable **symbol_table, Data* code, int *IC); 
