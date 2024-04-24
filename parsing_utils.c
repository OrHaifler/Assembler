#include "parsing_utils.h"

int is_define(AST *line_ast) {
	if(strcmp(line_ast->instruction,DEFINE_PREFIX)) return 0;

	return 1;			
}

int is_data(AST *line_ast) {
	if(strcmp(line_ast->instruction,DATA_PREFIX)) return 0;

	return 1;			
}
int is_string(AST *line_ast) {
	if(strcmp(line_ast->instruction,STRING_PREFIX)) return 0;

	return 1;			
}

int is_extern(AST *line_ast) {
	if(strcmp(line_ast->instruction,EXTERN_PREFIX)) return 0;

	return 1;			
}

int is_entry(AST *line_ast) {
	if(strcmp(line_ast->instruction,ENTRY_PREFIX)) return 0;

	return 1;			
}
