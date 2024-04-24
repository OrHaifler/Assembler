#include "validate.h"
#include "ast.h"

int valid_data(AST *line_ast, SymbolTable *symbol_table) {
	if(!valid_commas(line_ast)) return 0;
	char *operand_copy,*operand,token;
	operand_copy = (char *)calloc(strlen(line_ast->carried) * sizeof(char), 1);
	strcpy(operand_copy,line_ast->carried);
	operand = strtok(operand_copy,",");
	while(operand != NULL) {
		if(!valid_int(operand) && !valid_defined(operand,symbol_table)) return 0;
		operand = strtok(NULL,",");
	}

	free(operand_copy);


	return 1;
}


int valid_op(AST *line_ast) {
	char *copy;
	copy = (char *)calloc(strlen(line_ast->value), 1);
	strcpy(copy,line_ast->value);
	if(strlen(copy) == 0) return 0;
	if(*copy == '-' || *copy == '+') copy++;
	while(*copy) {
		if(!isdigit(*copy)) return 0;
		copy++;
	}

	return 1;
}

int valid_int(char *str_rep) {
	char *copy;
	copy = (char *)calloc(strlen(str_rep), 1);
	strcpy(copy,str_rep);
	if(strlen(copy) == 0) return 0;
	if(*copy == '-' || *copy == '+') copy++;
	while(*copy) {
		if(!isdigit(*copy)) return 0;
		copy++;
	}

	return 1;
}


int valid_symbol(AST *line_ast, SymbolTable *symbol_table, int IC) {
	while(symbol_table != NULL) {
		if(!strcmp(line_ast->symbol,symbol_table->name)) {
			if(!strcmp(line_ast->type,ENTRY_TYPE) && strcmp(symbol_table->type,ENTRY_TYPE)) {
				strcpy(symbol_table->type,ENTRY_TYPE);
				
				return 2;
			}

			if(strcmp(line_ast->type,ENTRY_TYPE) && !strcmp(symbol_table->type,ENTRY_TYPE)) {
				symbol_table->value = IC;

				return 2;
			}

			return 0;
		}
		symbol_table = symbol_table->next;
	}	
	return 1;
}

int valid_defined(char *symbol, SymbolTable *symbol_table) {
	while(symbol_table) {
		if(!strcmp(symbol_table->name,symbol)) {
			if(!strcmp(symbol_table->type,DEFINE_TYPE)) {
				return 1;
			}

			return 0;
		}
		
		symbol_table = symbol_table->next;
	}

	return 0;
}

int valid_define(char *op) {
	if(is_reg(op)) return 0;

	while(op) {
		if(!islower(*op)) return 0;
		op = op + 1;
	}

	return 1;
}

int valid_label(char *op) {
	if(strlen(op) > 31) return 0;
	if(!isalpha(*op)) return 0;

	op = op + 1;

	while(strlen(op) > 0) {
		if(!isalnum(*op)) return 0;
		op = op + 1;
	}

	return 1;
}


int valid_commas(AST *line_ast) {
    int commas = 0, operands = 1, i = 0;
    char *copy;
    copy = strdup(line_ast->carried);

    if(copy[0] == ',') return 0;

    copy++;

    for(i = 0; i < strlen(copy); i++) {
        if(copy[i] == ',') {
            commas++;
            continue;
        }

        if(copy[i - 1] == ',') operands++;
    }


    if(commas >= operands) return 0;

    return 1; 
}


void valid_inst(AST* line_ast) {
	int i = 0;
    for(i = 0; i < 16; i++) {
        if(!strcmp(line_ast->instruction,instruction_table[i])) {
			line_ast->inst_idx = i;
			return;
		}
    }

	line_ast->inst_idx = -1;
}

int valid_reg(char *op) {
    int reg;
    reg = atoi(op); 
    if(reg >= 0 && reg <= 7) {
        return 1;
    }

    return 0;
}
