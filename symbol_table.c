#include "ast.h"
#include "symbol.h"


void insert_symbol(AST *line_ast,SymbolTable **symbol_table,int IC,int DC) {
	if(!valid_symbol(line_ast, *symbol_table, IC)) {
		line_ast->error = 1;
		printError(DUPLICATE_SYMBOL_ERROR);
		return;
	}


	if(valid_symbol(line_ast, *symbol_table, IC) == 1) {
        SymbolTable *head = (SymbolTable *)calloc(sizeof(SymbolTable), 1);
        if(is_extern(line_ast))
            head->is_extern = 1;
        else
            head->is_extern = 0; 
        head->name = (char *)calloc(strlen(line_ast->symbol) * sizeof(char), 1);
        strcpy(head->name,line_ast->symbol);
        if (!strcmp(line_ast->type, CODE_TYPE)) {
            head->value = IC;
        } else if (!strcmp(line_ast->type, DATA_STORING_TYPE)) {
            head->value = DC;
        } else if (!strcmp(line_ast->type, DEFINE_TYPE)) {
            head->value = atoi(line_ast->value);
        } else if(!strcmp(line_ast->type, ENTRY_TYPE)) {
            head->value = IC;
        }
        head->next = *symbol_table;
        strcpy(head->type,line_ast->type);
        (*symbol_table) = head;
    }

    
}

SymbolTable* is_in_symtable(char* name,SymbolTable* symbol_table)
{
    if(!name) return NULL;
    while(symbol_table)
    {
        if(!strncmp(symbol_table->name,name,strlen(name)))
        {
            return symbol_table;
        }
        symbol_table = symbol_table->next;
    }
    return NULL;
}


int get_sym_type(char* operand,SymbolTable* symbol_table)
{
    char* openBrackIndex, *closeBrackIndex, *index, *sym;
    SymbolTable* symbol = NULL;
    if(!operand) return error;
    if(is_reg(operand))
    {
        return reg;
    }
    if(valid_label(operand))
    {
        symbol = is_in_symtable(operand,symbol_table);
        if(symbol)
        {
            if(symbol->is_extern)
            return variableExtern;
        } 
        return -variable;
    }
    if(strchr(operand,'#'))
    {
        SymbolTable* sym = is_in_symtable(operand+1,symbol_table);
        if(valid_int(operand+1) || (sym && !strcmp(sym->type,DEFINE_TYPE)))
        {
            return immidiate;
        }
        return -immidiate;
    }

    if((openBrackIndex=strchr(operand,'[')) && (closeBrackIndex=strchr(operand,']')) && openBrackIndex < closeBrackIndex)
    {
        index = (char*)calloc(closeBrackIndex-openBrackIndex,1);
        memcpy(index,openBrackIndex+1,closeBrackIndex-openBrackIndex-1);
        sym = (char*)calloc(openBrackIndex-operand,1);
        memcpy(sym,operand,openBrackIndex-operand);
        symbol = is_in_symtable(sym,symbol_table);
        SymbolTable* indexSym = is_in_symtable(index,symbol_table);
        if((!indexSym && !valid_int(index)) || (indexSym && strcmp(indexSym->type,DEFINE_TYPE)))
        {
            free(index);
            free(sym);
            return -constIndex;
        }
        if(symbol)
        {
            free(index);
            free(sym);
            if(symbol->is_extern) return indexExtern;
            return constIndex;
        }
        free(index);
        free(sym); 
        return -constIndex;
    }
    return error;
}


int get_defined(char *symbol, SymbolTable *symbol_table) {
	while(symbol_table) {
		if(!strcmp(symbol_table->name,symbol)) {
			return symbol_table->value;
		}

		symbol_table = symbol_table->next;
	}
	return 0;
}


void update_table(SymbolTable *symbol_table) {
	while(symbol_table) {
		if(!strcmp(symbol_table->type,CODE_TYPE) || !strcmp(symbol_table->type, ENTRY_TYPE) || !strcmp(symbol_table->type, DATA_STORING_TYPE)) {
			symbol_table->value = symbol_table->value + 100;
		}
		
		symbol_table = symbol_table->next;
	}
}


void remove_symbol(SymbolTable **symbol_table) {
	*symbol_table = (*symbol_table)->next; 
}

int has_extern(SymbolTable* symbol_table)
{
    while(symbol_table)
    {
        if(symbol_table->is_extern) return 1;
        symbol_table = symbol_table->next;
    }
    return 0;
}

int has_entery(SymbolTable* symbol_table)
{
    while(symbol_table)
    {
        if(!strcmp(symbol_table->type,ENTRY_TYPE)) return 1;
        symbol_table = symbol_table->next;
    }
    return 0;
}
