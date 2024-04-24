#include "preprocessor.h"
#include <ctype.h>
#define MAX_LEN 256

/*
Function for creating a macroTable based on an input file and replaceing all macro calls in that file

Input: a file name
Output: a macro table containing all macros in the file and their code
*/

macroTable* processFile(char* fileName)
{
	int lineLen = 0,newLen = 0,mcrFlag = 0,idx = 0;
	char* currMacro = NULL;
	char currLine[MAX_LEN] = {0};
	char *toPrint;
	char** macroList;
	FILE* inputFile = fopen(fileName,"r");
	FILE* outputFile = fopen("temp.temp","w");
	macroTable* table = calloc(sizeof(macroTable), 1);
	macroTableEntery* currEntery;
	table->head = NULL;
	table->len = 0;
	/*add a test to check the correct file type*/
	if(inputFile == NULL || outputFile == NULL)
	{
		printf("Failed to open file!");
		return NULL;
	}
	while (fgets(currLine,MAX_LEN,inputFile)) 
	{
		if(!strcmp("endmcr\n",currLine))
		{
			mcrFlag = 0;
		}
		else if(mcrFlag)
		{
			addCodeLineToTable(table,currMacro,currLine);
		}
		else if(currMacro = checkMacroDecleration(currLine))
		{
			addToMacroTable(table,currMacro);
			mcrFlag = 1;
		}
		else if(currEntery = getEntery(table,currLine))
		{
			
			macroCodeLine* curr = currEntery->line;
			while (curr)
			{
				fprintf(outputFile,curr->code);
				curr = curr->next;
			}
		}

		else if(strlen(currLine) > 0 && currLine[0] != ';' && currLine[0] != '\n')
        {
			idx = 0 ;

			while(isspace(currLine[idx]) && idx < MAX_LEN) idx++;
			
            fprintf(outputFile,currLine + idx);

        }
    }
	fclose(inputFile);
	fclose(outputFile);
	replaceFile(fileName);
	return NULL;
}

/*
Function for checking if a line has a macro decloration in it

Input: the code line
Output: the macro name, null - if the line doesn't have a macro decloration
*/
char* checkMacroDecleration(char* line)
{
	char* mcrName = NULL;
	char decloratoin[] = {0,0,0,0,0};
	if(strlen(line) > strlen("mcr "))
	{
		strncat(decloratoin,line,strlen("mcr "));
		if(!strcmp(decloratoin,"mcr "))
		{
			mcrName = calloc(sizeof(char),strlen(line) - strlen("mcr "));
			strncat(mcrName,line+strlen("mcr "),strlen(line)-strlen("mcr "));
		}
	}
	return mcrName;
}

/*
Function for replacing a file with the file temp.temp

input: the file name
output: none
*/
void replaceFile(char* fileName)
{
	char* command = calloc(1, strlen("mv temp.temp ") + strlen(fileName) + 1);
	strcat(command, "mv temp.temp ");
	strncat(command, fileName, strlen(fileName));
	system(command);
	free(command);
}
				