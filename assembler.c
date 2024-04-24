#include "assembler.h"
#include "ast.h"


SymbolTable *firstPass(char* fileName,ASTLines* lines,SymbolTable* symbol_table,Data* data_image,int *error, int* IC,int* DC);
void freeAllLines(ASTLines *lines);
void freeAllASTNodes(ASTNode* node);
void freeAllSymbols(SymbolTable *symbol_table);
void freeAllData(Data *data_image);
void freeAst(AST* ast_line);
void freeMacroCodeLine(macroCodeLine* line);
void freeMacroEntery(macroTableEntery* entery);
void freeMacroTable(macroTable* macro_table);

/* main - combining all functions into a full parsing and encoding process */ 
int main(int argc,char* argv[]) {
    int IC = 0, DC = 0; /* initialize the instruction and data counters */
    int flag = 0;
    int error = 0;
    macroTable* m;

    ASTLines* lines = calloc(sizeof(ASTLines), 1); /* a linked list of all file lines, used in the second pass */
    SymbolTable* symbol_table = calloc(sizeof(SymbolTable), 1); /* a symbol table for keeping track of all parsed symbols and their values */ 

    Data* data_image = calloc(sizeof(Data), 1); /* the data image, used to store all data(arrays, string etc.) and code lines with their encodings */

    data_image->next = NULL;

    /* intialize a first symbol for easier addition of symbols */
    symbol_table->name = "symbol";
    symbol_table->next = NULL;

    /* check if the argument count is valid, if there are more/less arguments than 2, print an error */ 
    if(argc > 2)  {
        printf("Only one file\n");
    } else if(argc < 2) {
         printf("A file name is needed\n");
    } else {
        FILE* file = fopen(argv[1],"r");

        /* check whether the given file is valid*/ 
        if(!file) {
            printf("Error opening file! either file doesn't exist or is used by another program!\n");
        } else {
            fclose(file);

            m = processFile(argv[1]); /* preprocessing step - replaces all macro definition and occurrences and stores it in the same file */
            symbol_table = firstPass(argv[1],lines,symbol_table,data_image,&error,&IC,&DC); /* first pass - creates a symbol table and encodes the first word of each code line */

            /* if the first pass failed, we can't proceed to the second pass */ 
            if(error == 1 || !symbol_table) {
                free(lines);
                free(symbol_table);
                free(data_image);

                return 0; 
            }

            data_image = data_image->next;

            update_table(symbol_table);

            flag = second_pass(lines,symbol_table,data_image); /* second pass - encodes the remaining words in each code line and creates .asm,.ext and .ent files */ 

            /* if the second pass has finished without errors, update the data counters and build output files */
            if(!flag) { 
                update_data(data_image);
                build_output(argv[1],data_image,symbol_table,lines);
            }
        }
    }


    /* free all allocated structures*/
    /* freeAllLines(lines);
    freeAllSymbols(symbol_table);
    freeAllData(data_image);
    freeMacroTable(m);*/

    return 0;
}

void freeMacroCodeLine(macroCodeLine* line)
{
    if(line)
    {
        freeMacroCodeLine(line->next);
        free(line->code);
        free(line);
    }
}

void freeMacroEntery(macroTableEntery* entery)
{
    if(entery)
    {
        freeMacroEntery(entery->next);
        free(entery->macroName);
        if(entery->line) freeMacroCodeLine(entery->line);
        free(entery);
    }
}

void freeMacroTable(macroTable* macro_table)
{
    if(macro_table)
    {
        if(macro_table->head) freeMacroEntery(macro_table->head);
        free(macro_table);
    }
}

void freeAllLines(ASTLines *lines)
{
    if(lines)
    {
        if(lines->first) freeAllASTNodes(lines->first);
        free(lines);
    }
}

void freeAllASTNodes(ASTNode* node)
{
    if(node)
    {
        freeAllASTNodes(node->next);
        printf("Freeing: %s %s,%s\n",node->line_ast->instruction,node->line_ast->first_operand,node->line_ast->second_operand);
        if(node->line_ast) freeAst(node->line_ast);
        free(node);
    }
}

void freeAst(AST* ast_line)
{
    if(ast_line->instruction) free(ast_line->instruction);
    if(ast_line->first_operand) free(ast_line->first_operand);
    if(ast_line->second_operand) free(ast_line->second_operand);
    if(ast_line->carried != ast_line->symbol) 
    {
        if(ast_line->carried) free(ast_line->carried);
        if(ast_line->symbol) free(ast_line->symbol);
    }
    else if(ast_line->symbol) free(ast_line->symbol);
    if(ast_line->value) free(ast_line->value);
    if(ast_line) free(ast_line);
}

void freeAllSymbols(SymbolTable *symbol_table)
{
    if(symbol_table)
    {
        freeAllSymbols(symbol_table->next);
        if(symbol_table->name) free(symbol_table->name);
        free(symbol_table);
    }
}

void freeAllData(Data *data_image)
{
    if(data_image)
    {
        freeAllData(data_image->next);
        free(data_image);
    }
}
