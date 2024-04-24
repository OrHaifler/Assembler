#include "ast.h"
#include "data.h"


/* instruction_table - stores all valid instruction, instruction_operands - stores each command's number of required operands
(e.g., for mov there have to be two operands, so the corresponding value to mov's opcode will be 2 */
char instruction_table[OP_CODE_LEN][4] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"};
int instruction_operands[OP_CODE_LEN] = {2,2,2,2,1,1,2,1,1,1,1,1,1,1,0,0};

/* build_ast - build an AST, symbol table and data/code image for a given line */ 
AST *build_ast(char *line,SymbolTable **symbol_table,Data *data_image,int *error, int *IC,int *DC) {
    int define,data,ext,ent; /* flags to keep track of the given line's type */ 

    AST *line_ast = (AST *)calloc(sizeof(AST), 1);  /* allocate memory for  the line's AST*/

    get_label(line_ast,line);/* check if there's a label, if there is, then line_ast->label_flag = 1 and the label will be copied into line_ast->symbol  */

    if(line_ast->label_flag) {
        if(!valid_label(line_ast->symbol)) {
            /* if the label's name isn't valid(i.e. there's a symbol with the same name, or the name's format is invalid) print an error and return */
            *error = 1;
            printError(INVALID_LABEL_ERROR);

            return NULL;
        }         
    }

    /* copy the line's instruction into line_ast->instruction */ 
    get_instruction(line_ast,line);

    define = handle_define(line_ast,symbol_table);/* check whether the instruction is .define, if it is, handle it */

    if(define) { /* if the line has both label and define instructions, remove the define's symbol, print an error and return */ 
        if(line_ast->label_flag) {
            remove_symbol(symbol_table);
            printError(LABEL_DEFINE_ERROR); 

            *error = 1;
        }

        return line_ast;
    } 

    if(line_ast->error) {
        *error = 1;

        return NULL; /* if there was an error, return NULL */
    }

    line_ast->op_code = get_op_code(line_ast); /* get the instruction's op code */

    /* handle .string,.data,.extern and .entry instructions */
    data = handle_data(line_ast,symbol_table,data_image,IC,DC);
    ext = handle_extern(line_ast,symbol_table,IC,DC);
    ent = handle_entry(line_ast,symbol_table,IC,DC);

    if(line_ast->error) {
        *error = 1;

        return NULL; /* if there was any error, return NULL */
    }

    if(data || ext || ent) return line_ast; /* if the current instruction was .string,.data,.extern or .entry, return the line's ast */
    

    get_operands(line_ast); /* copy the line's operands into line_ast->first_operands and line_ast->second_operand */

    if(line_ast->error) {
        *error = 1;

        return NULL;
    }

    valid_inst(line_ast); /* check if the line's instruction is valid */

    if(line_ast->inst_idx == -1) {
        *error = 1;
        fprintf(stderr,"ERROR IN: %s\n", line_ast->instruction);
        printError(INVALID_INSTRUCTION_ERROR);

        return NULL;
    }

    /* check that the number of operands is valid, e.g., for mov there have to be exactly two operands, which means line_ast->operand_type = 2 */
    if(instruction_operands[line_ast->inst_idx] != line_ast->operand_count) {  
        *error = 1;
        printError(INVALID_OPERAND_NUMBER_ERROR);

        return NULL;
    }

    /* get both operands addressing types, used for encoding the first word and calculation of L */
    line_ast->first_operand_type = get_sym_type(line_ast->first_operand,*symbol_table);
    line_ast->second_operand_type = get_sym_type(line_ast->second_operand,*symbol_table);

    /* handle the instruction, encode the first word and calculate L */
    handle_instruction(line_ast,symbol_table,data_image,IC);

    if(line_ast->error) {
        *error = 1;

        return NULL;
    }

    return line_ast;
}
