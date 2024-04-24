#include "first_pass.h"


/* firstPass - traverse all strings in a given file, create a symbol table and data/code image */
SymbolTable *firstPass(char* fileName,ASTLines* lines,SymbolTable* symbol_table,Data* data_image,int *error, int* IC,int* DC) 
{
    int tmp;
    char currLine[MAX_LEN] = {0};

    FILE* inputFile = fopen(fileName,"r");

    AST* currAst;
    ASTNode* currNode;

    /* check whether the given file is valid */
    if(!inputFile) {
        printf("Error opening file! either file doesn't exist or is used by another program!\n");

        return NULL;
    }


    while(fgets(currLine,MAX_LEN,inputFile))
    {
        tmp = *IC;

        if(currLine[strlen(currLine) - 1] == '\n') currLine[strlen(currLine) - 1] = '\0'; /* delete the \n character at the end of the line */

        currAst = build_ast(currLine,&symbol_table,data_image,error,IC,DC); /* build an AST for the current line */

        if(currAst) {
            currNode = calloc(sizeof(ASTNode), 1);

            currAst->IC = tmp;

            currNode->line_ast = currAst;
            currNode->next = NULL;

            if(!lines->first) {
                lines->first = currNode;
                lines->last = currNode;
            } else {
                lines->last->next = currNode;
                lines->last = currNode;
            }
        }
    } 

    data_image = data_image->next;

    return symbol_table;
}