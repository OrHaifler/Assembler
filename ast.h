#ifndef AST_H 
#define AST_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TYPE_LEN 10
#define DEFINE_PREFIX ".define"
#define DEFINE_PREFIX_LENGTH 7
#define DATA_PREFIX ".data"
#define DATA_PREFIX_LENGTH 5
#define STRING_PREFIX ".string"
#define STRING_PREFIX_LENGTH 7 
#define EXTERN_PREFIX ".extern"
#define EXTERN_PREFIX_LENGTH 7
#define ENTRY_PREFIX ".entry"
#define ENTRY_PREFIX_LENGTH 6 
#define DEFINE_TYPE ".mdefine"
#define DATA_STORING_TYPE ".data"
#define EXTERN_TYPE ".external"
#define ENTRY_TYPE ".entry"
#define CODE_TYPE ".code"
#define ENCODING_LENGTH 14 
#define OP_CODE_LEN 16
#define MAX_ERROR_LEN 50 
#define INVALID_INSTRUCTION_ERROR "Invalid instruction"
#define INVALID_LABEL_ERROR "Invalid label"
#define LABEL_DEFINE_ERROR "Label occurs in a define instruction"
#define DUPLICATE_SYMBOL_ERROR "Symbol already exists"
#define INVALID_OPERANDS_ERROR "At least one of the operands is invalid"
#define INVALID_OPERAND_NUMBER_ERROR "There are more/less operands than there should be"
#define INVALID_COMMAS_ERROR "There are too many commas"

extern char instruction_table[OP_CODE_LEN][4]; 
extern int instruction_operands[OP_CODE_LEN];


typedef struct SymbolTable SymbolTable;
typedef struct ASTLines ASTLines;
typedef struct ASTNode ASTNode;

typedef struct Data{
	char encoding[ENCODING_LENGTH];
	struct Data *next;
    int data;
    int pos;
} Data;


typedef struct AST {
    char *instruction;
    char *first_operand;
    char *second_operand;
    char *carried;
    char *symbol;
    char *value;
	char type[TYPE_LEN];
    int operand_count;
    int inst_idx;
    int label_flag;
    int op_code;
    int first_operand_type;
    int second_operand_type;
    int error;
    int IC;
} AST;

typedef struct ASTNode{
    AST *line_ast;
	struct ASTNode *next;
} ASTNode;

typedef struct ASTLines{
    ASTNode *first;
    ASTNode *last;
} ASTLines;



void get_operands(AST *line_ast);
void get_word(AST *line_ast); 
void get_label(AST *line_ast,char *line);
void get_instruction(AST *line_ast,char *line);
void get_symbol(AST *line_ast);
void valid_inst(AST* line_ast); 
int valid_commas(AST *line_ast); 
int valid_reg(char *op); 
AST *build_ast(char *line,SymbolTable **symbol_table,Data *data_image,int *error, int *IC,int *DC);
int get_op_code(AST *line_ast);
SymbolTable* is_in_symtable(char* name,SymbolTable* symbol_table);
int is_reg(char* name);
int get_sym_type(char* operand,SymbolTable* symbol_table);
int get_operand_type(char* operand,SymbolTable* symbol_table);
int valid_var_name(char* name);
void print_symtable(SymbolTable *symbol_table);
void print_data(Data *data);
void get_operands(AST *line_ast); 
void get_symbol(AST *line_ast); 
void *printError(char errorType[MAX_ERROR_LEN]); 
void print_symtable(SymbolTable *symbol_table); 
void print_data(Data *data); 
int has_extern(SymbolTable* symbol_table);


#endif
