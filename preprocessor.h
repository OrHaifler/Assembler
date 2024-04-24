#ifndef __PREPROC__
#define __PREPROC__

#include "ast.h"
#include "macroTable.h"

macroTable* processFile(char* fileName);
char* checkMacroDecleration(char* line);
void replaceFile(char* fileName);

#endif