#include "second_pass.h"
#include "ast.h"
#include "utils.h"

int second_pass(ASTLines *lines,SymbolTable *symbol_table,Data *data) {
	int flag = 0; 
    int IC = 0;
    int size;
    char **curr;
    ASTNode *first = lines->first;
	while(data && data->data == 1) {
		data = data->next;
	}
    while(first) {
        if(first->line_ast && strncmp(first->line_ast->type,DATA_PREFIX,9) && strcmp(first->line_ast->type,DEFINE_TYPE) && strcmp(first->line_ast->type,EXTERN_TYPE) && strcmp(first->line_ast->type,ENTRY_TYPE) && !first->line_ast->error)
        {
            curr = encodeSymbols(first->line_ast,symbol_table,&size);
			if(!curr) flag = 1;
            data = insert_op(first->line_ast,data,curr,size);
        }
        
        first = first->next;
    }

	return flag;
}


char** encodeSymbols(AST* line_ast,SymbolTable *symbol_table,int* size)
{
	int are = 0;
	*size = 0;
	char** ret = calloc(4,sizeof(char*));
	SymbolTable* sym = NULL;
	if(line_ast->first_operand && line_ast->first_operand_type < 0)
	{
		switch (line_ast->first_operand_type)
		{
		case -immidiate:
			printf("Invalid immidiate type \"%s\"!\n",line_ast->first_operand);
			return NULL;
			break;
		case error:
			printf("Unknown Symbol \"%s\"\n",line_ast->first_operand);
			return NULL;
			break;
		}

		if(line_ast->first_operand_type == -variable)
		{
			if(sym = is_in_symtable(line_ast->first_operand,symbol_table))
			{
				if(sym->is_extern) line_ast->first_operand_type = variable;
				else line_ast->first_operand_type = variableExtern;
			}
			else
			{ 
				printf("The variable \"%s\" doesn't exist!\n",line_ast->first_operand);
				return NULL;
			}
		}
	}
	if(line_ast->second_operand && line_ast->second_operand_type < 0)
	{
		switch (line_ast->second_operand_type)
		{
		case -immidiate:
			printf("Invalid immidiate type \"%s\"!\n",line_ast->second_operand);
			return NULL;
			break;
		case error:
			printf("Unknown Symbol \"%s\"\n",line_ast->second_operand);
			return NULL;
			break;
		}
		if(line_ast->second_operand_type == -variable)
		{
			if(sym = is_in_symtable(line_ast->second_operand,symbol_table))
			{
				if(sym->is_extern) line_ast->second_operand_type = variable;
				else line_ast->second_operand_type = variableExtern;
			}
			else 
			{
				printf("The variable \"%s\" doesn't exist!\n",line_ast->second_operand);
				return NULL;
			}
		}
	}

	if(line_ast->first_operand_type == reg && line_ast->second_operand_type == reg)
	{
		char *encoded = (char *)calloc(ENCODING_LENGTH * sizeof(char), 1);
		memset(encoded,'0',ENCODING_LENGTH);
		int firstReg = line_ast->first_operand[1]-'0', secondReg = line_ast->second_operand[1]-'0';
		char* encodedFirstReg = encode(firstReg), *encodedSecondReg = encode(secondReg);

		encoded[11] = encodedSecondReg[13];
		encoded[10] = encodedSecondReg[12];
		encoded[9] = encodedSecondReg[11];

		encoded[8] = encodedFirstReg[13];
		encoded[7] = encodedFirstReg[12];
		encoded[6] = encodedFirstReg[11];
		
		free(encodedFirstReg);
		free(encodedSecondReg);
		ret[*size] = encoded;
		*size = 1;
		return ret;
	}
	if(line_ast->first_operand)
	{
		if(line_ast->first_operand_type == constIndex || line_ast->first_operand_type == indexExtern || line_ast->first_operand_type == -constIndex || line_ast->first_operand_type == -indexExtern)
		{
			char* openBrackIndex,*closeBrackIndex,*index,*sym;
			if((openBrackIndex=strchr(line_ast->first_operand,'[')) && (closeBrackIndex=strchr(line_ast->first_operand,']')) && openBrackIndex < closeBrackIndex)
			{
				index = (char*)calloc(closeBrackIndex-openBrackIndex,1);
				memcpy(index,openBrackIndex+1,closeBrackIndex-openBrackIndex-1);
        		sym = (char*)calloc(openBrackIndex-line_ast->first_operand,1);
        		memcpy(sym,line_ast->first_operand,openBrackIndex-line_ast->first_operand);

        		SymbolTable* symbol = is_in_symtable(sym,symbol_table);
        		SymbolTable* indexSym = is_in_symtable(index,symbol_table);

				if(!symbol)
				{
					printf("Unknown array \"%s\"!\n",sym);
					return NULL;
				}
				else if((!indexSym && !valid_int(index)) || (indexSym && strcmp(indexSym->type,DEFINE_TYPE)))
				{
					printf("Unkown immidiate \"%s\"!\n",index);
					return NULL;
				}
				else
				{
					char* encoded = symToEncoded(symbol->name,symbol_table,line_ast->first_operand_type,0);
					if(!encoded) return NULL;
					ret[*size] = encoded;
					(*size)++;

					encoded = symToEncoded(index,symbol_table,immidiate,0);
					if(!encoded) return NULL;
					ret[*size] = encoded;
					(*size)++;
				}
			}
		}
		else
		{
			char* encoded = symToEncoded(line_ast->first_operand,symbol_table,line_ast->first_operand_type,FIRST_OPERAND);
			if(!encoded) return NULL;
			ret[*size] = encoded;
			(*size)++;
		} 
	}
	if(line_ast->second_operand)
	{
		if(line_ast->second_operand_type == constIndex || line_ast->second_operand_type == indexExtern || line_ast->second_operand_type == -constIndex || line_ast->second_operand_type == -indexExtern)
		{

			char* openBrackIndex,*closeBrackIndex,*index,*sym;
			if((openBrackIndex=strchr(line_ast->second_operand,'[')) && (closeBrackIndex=strchr(line_ast->second_operand,']')) && openBrackIndex < closeBrackIndex)
			{
				index = (char*)calloc(closeBrackIndex-openBrackIndex,1);
				memcpy(index,openBrackIndex+1,closeBrackIndex-openBrackIndex-1);

        		sym = (char*)calloc(openBrackIndex-line_ast->second_operand,1);
        		memcpy(sym,line_ast->second_operand,openBrackIndex-line_ast->second_operand);

        		SymbolTable* symbol = is_in_symtable(sym,symbol_table);
        		SymbolTable* indexSym = is_in_symtable(index,symbol_table);

				if(!symbol)
				{
					printf("Unknown array \"%s\"!\n",sym);
					return NULL;
				}
				else if((!indexSym && !valid_int(index)) || (indexSym && strcmp(indexSym->type,DEFINE_TYPE)))
				{
					printf("Unkown immidiate \"%s\"!\n",index);
					return NULL;
				}
				else
				{
					char* encoded = symToEncoded(symbol->name,symbol_table,line_ast->second_operand_type,0);
					if(!encoded) return NULL;
					ret[*size] = encoded;
					(*size)++;

					encoded = symToEncoded(index,symbol_table,immidiate,0);
					if(!encoded) return NULL;
					ret[*size] = encoded;
					(*size)++;
				}
			}
		}
		else
		{
			char* encoded = symToEncoded(line_ast->second_operand,symbol_table,line_ast->second_operand_type,SECOND_OPERAND);
			if(!encoded) return NULL;
			ret[*size] = encoded;
			(*size)++;
		} 
	}
	return ret;
}



char* symToEncoded(char* name,SymbolTable* symbol_table,int type,int number)
{
	if(strlen(name) > 1 && *name == '#') name += 1;
	
	int are = symTypeToAre(type);
	char *encoded = (char *)calloc(ENCODING_LENGTH * sizeof(char), 1);
	memset(encoded,'0',ENCODING_LENGTH);
	if(are > 0) {
		encoded[13] += are%2;
		encoded[12] += are/2;
	}
	SymbolTable* sym;
	sym = is_in_symtable(name,symbol_table);

	if(is_reg(name))
	{
		char* encodedReg = encode(name[1]-'0');
		if(number == SECOND_OPERAND)
		{
			encoded[11] = encodedReg[13];
			encoded[10] = encodedReg[12];
			encoded[9] = encodedReg[11];
		}
		else if(number == FIRST_OPERAND)
		{
			encoded[8] = encodedReg[13];
			encoded[7] = encodedReg[12];
			encoded[6] = encodedReg[11];
		}
		else return NULL;
	}
	else if(valid_int(name))
	{
		char* encodedValue = encode(atoi(name));
		memcpy(encoded,encodedValue+2,12);
	}
	else if(!sym)
	{
		free(encoded);
		return NULL;
	}
	else
	{
		char* encodedValue = encode(sym->value);
		memcpy(encoded,encodedValue+2,12);
	}
	return encoded;
}
