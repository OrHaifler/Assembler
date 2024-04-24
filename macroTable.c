#include "macroTable.h"



/*
Function for adding an entery into a macro table where the macro already has known code lines

Input: a macro table instance, name for the macro, and the code the macro has
Output: 1 - error, 0 - success
*/
int addToMacroTableWithCode(macroTable* table,char* name,char* code)
{
	/*edge case*/
	if(!table)
	{
		return ERROR;
	}

	/*create a new instance*/
	macroTableEntery* entery = (macroTableEntery*)calloc(sizeof(macroTableEntery), 1);
	entery->macroName = (char*)calloc(strlen(name), 1);
	strcpy(entery->macroName,name);
	entery->next = NULL;

	macroCodeLine* codeLine = calloc(sizeof(macroCodeLine), 1);
	codeLine->code = calloc(sizeof(code), 1);
	strcpy(codeLine->code,code);
	codeLine->next = NULL;
	entery->line = codeLine;


	/*add the instance*/
	table->len++;
	if(!table->head)
	{
		table->head = entery;
	}
	else
	{
		macroTableEntery* head = table->head;
		while(head->next)
		{
			head = head->next;
		}
		head->next = entery;
	}
	return SUCSESS;
}

/*
Function for adding an entery into a macro table with empty code

Input: a macro table instance, name for the macro
Output: 1 - error, 0 - success
*/
int addToMacroTable(macroTable* table,char* name)
{
	/*edge case*/
	if(!table)
	{
		return ERROR;
	}
	/*create a new instance*/
	macroTableEntery* entery = (macroTableEntery*)calloc(sizeof(macroTableEntery), 1);

	entery->macroName = (char*)calloc(strlen(name), 1);
	
	strcpy(entery->macroName,name);
	entery->line = NULL;
	entery->next = NULL;

	/*add the instance*/
	table->len++;
	if(!table->head)
	{
		table->head = entery;
	}
	else
	{
		macroTableEntery* head = table->head;
		while(head->next)
		{
			head = head->next;
		}
		head->next = entery;
	}
	return SUCSESS;
}

/*
Function getting an macroTableEntery instance from a macroTable instance based on the macro name

Input: a macro table instance, name for the macro
Output: pointer for the entery instance, null if it doesn't exist
*/
macroTableEntery* getEntery(macroTable* table,char* name)
{
	if(!table)
		return NULL;
	macroTableEntery* curr = table->head;
	while(curr)
	{
		if(!strcmp(curr->macroName,name))
		{
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

/*
Function for adding an code to a already existing macro file

Input: a macro table instance, name for the macro, the code to add to the macro
Output: 1 - error, 0 - success
*/
int addCodeLineToTable(macroTable* table,char* name,char* code)
{
	int oldLen = 0;
    if(!table)
		return ERROR;
	macroTableEntery* curr = table->head;
	macroCodeLine* currLine = NULL;
	while(curr)
	{
		if(!strcmp(curr->macroName,name))
		{
			if (!curr->line)
			{
				curr->line = calloc(sizeof(macroCodeLine), 1);
				curr->line->code = calloc(sizeof(code), 1);
				strcpy(curr->line->code, code);
				curr->line->next = NULL;
			}
			else
			{
				currLine = curr->line;
				while (currLine->next)
				{
					currLine = curr->line->next;
				}
				currLine->next = calloc(sizeof(macroCodeLine), 1);
				currLine->next->code = calloc(sizeof(code), 1);
				strcpy(currLine->next->code, code);
				currLine->next->next = NULL;
			}
			return SUCSESS;
		}
		curr = curr->next;
	}
	return ERROR;
}

/*
Function for freeing all allocated memory in a macro table

Input: a macro table instance
Output: none
*/
void deleteTable(macroTable* table)
{
    deleteEnteries(table->head);
    free(table);
}

/*
Function for freeing all allocated memory in a macroTableEntery linked list

Input: a macro table entery instance
Output: none
*/
void deleteEnteries(macroTableEntery* curr)
{
    if(curr)
    {
	    deleteEnteries(curr->next);
        free(curr->macroName);
		deleteLines(curr->line);
        free(curr);
    }
}

/*
Function for freeing all allocated memory in a macroTableEnteryCodeLine linked list
*/
void deleteLines(macroCodeLine* curr)
{
	if (curr)
	{
		deleteLines(curr->next);
		free(curr->code);
	}
}
