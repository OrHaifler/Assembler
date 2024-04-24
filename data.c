#include "data.h"
#include "ast.h"

/* insert_code - insert a new code line's encoding to the Data structure, used in the first pass*/ 
void insert_code(AST* line_ast, SymbolTable *symbol_table, Data* code, int *IC) {
	/* allocate memory for the new line's encoding */
	Data *new_code = (Data *)calloc(sizeof(Data), 1);

	/* mark the encoding type(i.e., mark that it isn't a data encoding) */
	new_code->data = 0;

	/* copy the line's encoding into the Data encoding field */
	char* firstWord = encodeFirstWord(line_ast,symbol_table);
	strcpy(new_code->encoding, firstWord);

	/* traverse the linked list until reaching the end of it, then add the new encoding */
	while(code->next) code = code->next;

	code->next = new_code;
	if(firstWord) free(firstWord);
}

/* insert_op - insert second pass's words encodings to the Data structure */
Data *insert_op(AST *line_ast, Data *code, char **encoding, int size) {
	/* store the next Data line */
	Data *last = code->next; 

	int i = 0;

	/* insert each new encoding to the Data structuce, between code and code->next*/
	for(i = 0; i < size && encoding; i++) {
		Data *next = (Data *)calloc(sizeof(Data), 1);
		strcpy(next->encoding,encoding[i]);
		code->next = next;
		code = code->next;
	} 

	code->next = last;

	return last;
}

/* store_data - stores a .data line's values in the Data structure */ 
void store_data(AST *line_ast,SymbolTable *symbol_table,Data *code,int *IC, int *DC) {
	char *val;

	/* get the first value */
	val = strtok(line_ast->carried,",");

	/* traverse until reaching the end */
	while(code->next) code = code->next;
	
	while(val) {
		Data *next_code = (Data *)calloc(sizeof(Data), 1);

		/* mark the encoding as a data encoding */
		next_code->data = 1;

		/* check if the current value is a symbol with a .mdefine type */
		if(valid_defined(val,symbol_table)) {
			/* encode the symbol's value and copy it into next_code->encoding */
			strcpy(next_code->encoding,encode(get_defined(val,symbol_table)));
		} else {
			/* encode the value and copy it into next_code->encoding */
			strcpy(next_code->encoding,encode(atoi(val)));
		}

		/* get the next values, append the current encoding and update IC and DC */
		val = strtok(NULL,",");
		code->next = next_code;
		code = next_code;

		*DC = *DC + 1;
		*IC = *IC + 1;
	}	
}

/* store_string - stores a .string line's values in the Data structure */ 
void store_string(AST *line_ast,Data *code,int *IC, int *DC) {

	/* traverse until reaching the end */
	while(code->next) code = code->next;

	/* allocate memory for the new encoding */
	Data *next_code = (Data *)calloc(sizeof(Data), 1);

	/* mark the encoding as a data encoding */
	next_code->data = 1;

	if(line_ast->carried[0] != '"') {
		line_ast->error = 1;
		printError(INVALID_OPERANDS_ERROR);

		return;
	} 

	line_ast->carried++;

	while(strlen(line_ast->carried) > 1) {
		Data *next_code = (Data *)calloc(sizeof(Data), 1);

		/* copy the current character's encoding into next_code->encoding */
		strcpy(next_code->encoding, encode(*line_ast->carried));

		/* move to the next character, append the encoding to the Data structure and update DC */
		line_ast->carried++;
		code->next = next_code;
		code = next_code;

		*DC = *DC + 1;
		*IC = *IC + 1;
	}

	/* append a null terminator */
	code->next = get_nullterm();

	*IC = *IC + 1;
}

/* update_data - update the Data structure after the passes */
void update_data(Data *data) {
	int i = 100;

	while(data) {
		data->pos = i++; 
		data = data->next;
	}
}

/* data_label - check whether op is a defined label by traversing the symbol table */
int data_label(char *op, SymbolTable *symbol_table) {
	while(symbol_table) {
		if(!strcmp(op, symbol_table->name)) {
			if(!strcmp(symbol_table->type,DATA_STORING_TYPE)) {
				return 1;	
			}

			return 0;
		}
	}

	return 0;
}

/* get_nullterm - allocate memory for a null terminator, encode a null terminator and return it */
Data *get_nullterm(void) {
    Data *nullterm = (Data *)calloc(sizeof(Data), 1);
    strcpy(nullterm->encoding,encode(0));
    
    return nullterm;
}



