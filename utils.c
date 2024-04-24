#include "utils.h"
#include "data.h"
#include "symbol.h"

void get_operands(AST *line_ast) {
    char *copy,*op;
    int i = 0;
    if(!strlen(line_ast->carried)) {
        line_ast->operand_count = 0;
        return;
    }

    if(!valid_commas(line_ast)) {
        line_ast->error = 1;
        printError(INVALID_OPERANDS_ERROR);
        return;
    }   

    copy = strdup(line_ast->carried);
    op = strtok(copy,",");
    line_ast->first_operand = (char *)calloc(strlen(op) * sizeof(char), 1);
    strcpy(line_ast->first_operand,op);
    if(strlen(op) == strlen(line_ast->carried)) {
        line_ast->operand_count = 1;
        free(copy);
        return;
    } 
    op = strtok(NULL,",");
    line_ast->second_operand = (char *)calloc(strlen(op) * sizeof(char), 1);
    line_ast->operand_count = 2;
    strcpy(line_ast->second_operand,op);
    if(strlen(line_ast->first_operand) + strlen(line_ast->second_operand) + 1 != strlen(line_ast->carried)) {
        line_ast->error = 1;
        printError(INVALID_OPERAND_NUMBER_ERROR);
        return;
    }

    for(i = 0;i < strlen(line_ast->first_operand);i++)
    {
        if(isspace(line_ast->first_operand[i]))
        {
            line_ast->first_operand[i] = 0;
        }
    }
    for( i = 0;i < strlen(line_ast->second_operand);i++)
    {
        if(isspace(line_ast->second_operand[i]))
        {
            line_ast->second_operand[i] = 0;
        }
    }
    while(!line_ast->first_operand)
    {
        line_ast->first_operand++;
    }
    while(!line_ast->second_operand)
    {
        line_ast->second_operand++;
    }
    
    free(copy);
}


int symTypeToAre(int symType)
{
	switch(symType)
	{
		case immidiate:
			return 0;
		case variable:
			return 1;
		case constIndex:
			return 2;
		case reg:
			return 0;
		case variableExtern:
			return 1;
		case indexExtern:
			return 1;
        case -variable:
            return 1;
        case -constIndex:
            return 2;
    }
	return -1;
}


void get_symbol(AST *line_ast) {
    sscanf(line_ast->carried, "%m[^=] = %ms",&line_ast->symbol,&line_ast->value);
}


void *printError(char errorType[MAX_ERROR_LEN]) {
	fprintf(stderr, "%s\n", errorType);	
	return NULL;
}

void print_symtable(SymbolTable *symbol_table) {
    while(symbol_table) {
        printf("Name: %s, Type: %s, Value: %d\n", symbol_table->name,symbol_table->type,symbol_table->value);
        symbol_table = symbol_table->next;
    }
}


void print_data(Data *data) {
    while(data) {
        printf("Pos: %d  Encoding: %s\n",data->pos, data->encoding);
        data = data->next;
    }
}


