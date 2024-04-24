#include "handle_parsing.h"
#include "addressing.h"
#include "ast.h"
#include "data.h"


/* handle_define - check if the line's instruction is .define, if it is, insert the symbol with it's value into the symbol table */
int handle_define(AST *line_ast,SymbolTable **symbol_table) {
	int valid_sym,valid_operand,define,i;
	char *symbol;

	define = is_define(line_ast); /* check whether line_ast->instruction is ".define" */
	if(!define) return 0; /* if the instruction is not .define, return 0 */

	get_symbol(line_ast); /* get the symbol, i.e., if the line is .define sz=5, it'll copy sz into line_ast->symbol and 5 into line_ast->value */
	valid_operand = valid_op(line_ast); /* check if the symbol's value is an integer */ 
	strcpy(line_ast->type,DEFINE_TYPE); /* mark the line's type as .mdefine */

	if(valid_operand) { /* if the operand is valid, insert it into the symbol table with the corresponding value */
		for(i = 0;i < strlen(line_ast->symbol);i++) {
			if(isspace(line_ast->symbol[i])) {
				line_ast->symbol[i] = 0;
			}
		}

		while(!*line_ast->symbol) {
			line_ast->symbol++;
		}

		insert_symbol(line_ast,symbol_table,0,0);
	} else { /* if the operand is not valid, print an error and return */
		line_ast->error = 1;
		fprintf(stderr, "ERROR in: %s", line_ast->carried);
		printError(INVALID_OPERANDS_ERROR);	
	}

	return 1;
}


/* handle_data - check if the line's instruction is .data or .string and handles them */ 
int handle_data(AST *line_ast,SymbolTable **symbol_table,Data *data_image,int *IC,int *DC) {
	int valid_operands,valid_sym;	

	if(is_data(line_ast)) { /* check whether the instruction is .data */
		valid_operands = valid_data(line_ast,*symbol_table); /* check if the operands are valid */

		if(valid_operands) {
			strcpy(line_ast->type,DATA_STORING_TYPE); /* mark the line's type as .data */

			if(line_ast->label_flag) { /* if there's a label and it's valid, insert it with the current data counter */
				valid_sym = valid_symbol(line_ast,*symbol_table, *IC);

				if(valid_sym) {
					insert_symbol(line_ast,symbol_table,*IC,*DC);
				} else { /* the symbol is invalid, print an error and return */ 
					line_ast->error = 1;
					printError(DUPLICATE_SYMBOL_ERROR);

					return 0;
				}
			}

			store_data(line_ast,*symbol_table,data_image,IC, DC); /* store the instruction's data in the data image */

			return 1;
		} else { /* the operands are invalid, print an error and return */
			line_ast->error = 1;
			printError(INVALID_OPERANDS_ERROR);

			return 0;
		}
	}


	if(is_string(line_ast)) { /* check whether the instruction is .string */
		strcpy(line_ast->type,DATA_STORING_TYPE); /* mark the line as .data */

		if(line_ast->label_flag) { /* if there's a label and it's valid, insert it with the current data counter */
			valid_sym = valid_symbol(line_ast,*symbol_table, *IC);

			if(valid_sym) {
				insert_symbol(line_ast,symbol_table,0,0);
				store_string(line_ast,data_image,IC, DC);

				return 1;
			} else {
					line_ast->error = 1;
					printError(DUPLICATE_SYMBOL_ERROR);

					return 0;
			}

			return 0;
		}
		store_string(line_ast,data_image,IC, DC); /* store the string in the data image */

		return 1;
	}	
	
	return 0;
}


/* handle_instruction - handle a code instruction(i.e., not a .data,.string,.extern or .entry instructions), encode the first word and calculate L */
void handle_instruction(AST* line_ast,SymbolTable **symbol_table, Data* code, int *IC) {
	if(line_ast->label_flag) { /* if there's a label and it's valid, insert it with the current instruction counter */
		strcpy(line_ast->type,CODE_TYPE);
		insert_symbol(line_ast,symbol_table,*IC,0);
	}

	insert_code(line_ast, *symbol_table, code, IC); /* insert the code into the Data structure */

	*IC = *IC + calculate_length(line_ast,*symbol_table) + 1; /* update IC by the line's L */
}


/* handle_extern - handle an .extern instruction */
int handle_extern(AST *line_ast, SymbolTable **symbol_table, int *IC, int *DC) {
	if(!is_extern(line_ast)) return 0; /* check if the instruction is .extern */

	line_ast->symbol = line_ast->carried; /* copy the external label into line_ast->symbol */
	strcpy(line_ast->type,EXTERN_TYPE); /* mark the line as an .external line */

	insert_symbol(line_ast,symbol_table,0,0); /* insert the symbol into the symbol table */

	return 1;
}


/* handle_extern - handle an .extern instruction */
int handle_entry(AST *line_ast, SymbolTable **symbol_table, int *IC, int *DC) {
	if(!is_entry(line_ast)) return 0; /* check if the instruction is .entry*/

	line_ast->symbol = line_ast->carried; /* copy the entry label into line_ast->symbol */
	strcpy(line_ast->type,ENTRY_TYPE); /* mark the line as an .entry line */

	insert_symbol(line_ast,symbol_table,*IC, *DC); /* insert the symbol into the symbol table */

	return 1;
}
