#include "addressing.h"


/* calculate_length - calculates L, i.e., how many encoding lines will be added in the second pass */
int calculate_length(AST *line_ast, SymbolTable *symbol_table) {
    /* first_type and second_type are the two(or less) operand types of line_ast */
	int first_type, second_type, L;

    if(!line_ast) return 0;

    /* If there aren't any operands, no lines will be added in the second pass */
	if(line_ast->operand_count == 0) return 0;

	/* if there is a single operads, we'll need to check if it's a label addressing to determine how many lines will be added in the second pass */
	if(line_ast->operand_count == 1) { 
        /* get the first operand's addressing type */
		first_type = get_sym_type(line_ast->first_operand, symbol_table);
        /* if the operand isn't valid, then first_type == error, so we're printing an error and returning null */
		if(first_type == error) {
			line_ast->error = 1;
			printError(INVALID_OPERANDS_ERROR);

			return NULL;
		}

        /* if the operand's addressing type is a constant indexing type(i.e., label) L should be 2, other wise, L should be 1 */ 
		if(first_type == constIndex || first_type == -constIndex || first_type == indexExtern)
			return 2;
		else
			return 1;	
	} else { /* if there's two operands, we'll need to check each one's addressing type */
		first_type = get_sym_type(line_ast->first_operand, symbol_table); /* get the first operand's addressing type */ 
		second_type = get_sym_type(line_ast->second_operand, symbol_table); /* get the second operand's addressing type */
		

		if(first_type == error || second_type == error) { /* if at least one of the operands is invalid, prind an error and return NULL */
			line_ast->error = 1;
			printError(INVALID_OPERANDS_ERROR);

			return NULL;
		}

		if(first_type == reg && second_type == reg) return 1; /* if both addressing types are registers, L should be 1 */

        /* if both addressing types are constant indexing addressing then L should be 4, 
        else if one is index addressing and other direct the 3 other wise, L should be 2 */ 
		if((first_type == constIndex || first_type == -constIndex) && (second_type == constIndex || second_type == -constIndex)) return 4;

		if((first_type == constIndex || first_type == -constIndex) || (second_type == constIndex || second_type == -constIndex)) return 3;

		return 2;
	}
}


/* is_reg - check whether a string is a valid register name */
int is_reg(char* name)
{
    if(!name) return 0;

    return (!strcmp(name,"r0") || !strcmp(name,"r1") || !strcmp(name,"r2") || !strcmp(name,"r3") || !strcmp(name,"r4") || !strcmp(name,"r5") || !strcmp(name,"r6") || !strcmp(name,"r7"));
}


