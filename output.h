#include "addressing.h"


void build_output(char* fileName,Data* data_image,SymbolTable* symbol_table,ASTLines* lines);
char* remove_file_type(char* fileName);
void build_obj_file(char* fileName,Data* data_image);
void build_externals(char* fileName,SymbolTable* symbol_table,ASTLines* lines);
void build_ent_file(char* fileName,SymbolTable* symbol_table);
char bin_to_encoded_base4(char a,char b);
void int_to_string(char* buff,int num,int size);
