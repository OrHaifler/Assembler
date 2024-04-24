#include "ast.h"
#include <ctype.h>
#include <string.h>


/* get_op_code - check whether line_ast's instruction is valid, if it is, return the corresponding instruction's opcode */
int get_op_code(AST *line_ast) {
    int i = 0;
    for(i = 0;i < OP_CODE_LEN;i++) {
        /* compare the ast's instruction with each valid instruction */
        if(!strncmp(line_ast->instruction,instruction_table[i],OP_CODE_LEN)) {
            return i;
        }
    }

    /* if the ast's instruction isn't valid, return -1 */
    return -1;
}


/* get_instruction - get a line's instruction name, the instruction name is the second word is there's no label(i.e., line_ast->label_flag == 0), if there's a label flag it's the third word */ 
void get_instruction(AST *line_ast,char *line) {
    char *copy, *instruction;

    /* get a copy of the line, because strtok() changes the line by adding null terminators between tokens */
    copy = strdup(line);

    /* get the first word */
    instruction = strtok(copy, " ");

    /* if there's a label, get the second word */
    if (line_ast->label_flag) {
        instruction = strtok(NULL, " ");
    }

    /* allocate memory for the line's instruction and carried strings */
    line_ast->instruction = (char *)calloc((strlen(instruction) + 1) * sizeof(char), 1);
    line_ast->carried = (char *)calloc(strlen(line), 1);

    /* copy the line's instruction into line_ast->instruction */
    strcpy(line_ast->instruction,instruction);

    /* copy the rest of the line into line_ast->carried, used in both passes for the operands */
    if(line_ast->label_flag) {
        strcpy(line_ast->carried,line + strlen(line_ast->symbol) + strlen(line_ast->instruction) + 3);
    } else {
        strcpy(line_ast->carried,line + strlen(line_ast->instruction) + 1);
    }

    free(copy);

    return;
}

/* get_label - check if the line contains a label, if it does, copy it into line_ast->label */
void get_label(AST *line_ast,char *line) {
    int i = 0;

    while(i < strlen(line) && !isspace(line[i])) i++; /* find the first space occurence */
    
    if(line[i - 1] == ':') { /* check whether it's a label */
        line_ast->label_flag = 1;

        /* allocate memory for the line's label, and copy it into the symbol field of the line's ast */
        line_ast->symbol = (char *)calloc(i * sizeof(char), 1);

        strncpy(line_ast->symbol,line,i - 1);
    }
}

