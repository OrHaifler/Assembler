#include <stdio.h>
#include "output.h"
#include "symbol.h"

void build_output(char* fileName,Data* data_image,SymbolTable* symbol_table,ASTLines* lines)
{
    char* onlyName = remove_file_type(fileName);
    char* obj = calloc(strlen(fileName),sizeof(char));
    char* ext = calloc(strlen(fileName),sizeof(char));
    char* ent = calloc(strlen(fileName),sizeof(char));
    strcpy(obj,onlyName);
    strcpy(ext,onlyName);
    strcpy(ent,onlyName);
    strcpy(obj+strlen(onlyName),".obj");
    strcpy(ext+strlen(onlyName),".ext");
    strcpy(ent+strlen(onlyName),".ent");
    build_obj_file(obj,data_image);
    if(has_extern(symbol_table)) build_externals(ext,symbol_table,lines);
    if(has_entery(symbol_table)) build_ent_file(ent,symbol_table);
    free(obj);
    free(ext);
    free(ent);
}

char* remove_file_type(char* fileName)
{
    char* onlyName = calloc(strlen(fileName) + 4,sizeof(char));
    int i = 0;
    while(fileName[i] != '.')
    {
        onlyName[i] = fileName[i]; 
        i++;
    }
    return onlyName;
}
void build_obj_file(char* fileName,Data* data_image)
{
    FILE* file = fopen(fileName,"w");
    int i = 0,j = 0;
    char codeBuff[7] = {0};
    char lineBuff[4];
    if(!file)
    {
        printf("Error creating obj file!\n");
        return;
    }
    while(data_image)
    {
        for(i = 0,j=0;i < 7;i++,j+=2)
        {
            codeBuff[i] = bin_to_encoded_base4(data_image->encoding[j],data_image->encoding[j+1]);
        }
        fprintf(file,"%04d\t%s\n",data_image->pos,codeBuff);
        data_image = data_image->next;
    }
    fclose(file);
}

void int_to_string(char* buff,int num,int size)
{
    int i = 0;
    for(i = size-1;i >= 0;i--,num/=10)
    {
        buff[i] = num%10 + '0';
    }
}

void build_externals(char* fileName,SymbolTable* symbol_table,ASTLines* lines)
{
    FILE* file = fopen(fileName,"w");
    if(!file)
    {
        printf("Error creating ext file\n");
        return;
    }
    ASTNode* curr = lines->first;
    while(curr)
    {
        if(curr->line_ast)
        {
            SymbolTable* op1 = is_in_symtable(curr->line_ast->first_operand,symbol_table);
            SymbolTable* op2 = is_in_symtable(curr->line_ast->second_operand,symbol_table);
            if(op1 && op1->is_extern)
            {
                fprintf(file,"%s\t%04d\n",curr->line_ast->first_operand,curr->line_ast->IC + 100 + 1);
            }
            if(op2 && op2->is_extern)
            {
                if(curr->line_ast->first_operand_type == constIndex || curr->line_ast->first_operand_type == indexExtern) {
                    fprintf(file,"%s\t%04d\n",curr->line_ast->second_operand,curr->line_ast->IC + 100 + 3);
                } else {
                    fprintf(file,"%s\t%04d\n",curr->line_ast->second_operand,curr->line_ast->IC + 100 + 2);
                }
            }
        }
        curr = curr->next;
    }
    fclose(file);
}
void build_ent_file(char* fileName,SymbolTable* symbol_table)
{
    FILE* file = fopen(fileName,"w");
    if(!file)
    {
        printf("Error creating ent file\n");
        return;
    }
    while(symbol_table)
    {
        if(symbol_table && !strcmp(symbol_table->type,ENTRY_TYPE))
        {
            fprintf(file,"%s\t%04d\n",symbol_table->name,symbol_table->value);
        }
        symbol_table = symbol_table->next;
    }
    fclose(file);
}

char bin_to_encoded_base4(char a,char b)
{
    if(a == '0' && b == '0')
    {
        return '*';
    }
    if(a == '0' && b == '1')
    {
        return '#';
    }
    if(a == '1' && b == '0')
    {
        return '%';
    }
    if(a == '1' && b == '1')
    {
        return '!';
    }
    return 0;
}
