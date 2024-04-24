#include "encoding.h"

char* encodeFirstWord(AST* line_ast,SymbolTable *symbol_table)
{
	int op1type = line_ast->first_operand_type,op2type = line_ast->second_operand_type;
	SymbolTable *op1 = NULL, *op2 = NULL;
	char *encoded = (char *)calloc(ENCODING_LENGTH * sizeof(char), 1);
	memset(encoded,'0',ENCODING_LENGTH);

	/*encode like there is no error*/
	if(op1type==error) op1type = 0;
	if(op2type==error) op2type = 0;
	if(op1type< 0) op1type = line_ast->first_operand_type * (-1);
	if(op2type < 0) op2type = line_ast->second_operand_type * (-1);


	/*incase there is extern symbol then use the default sym type*/
	if(op1type == variableExtern || op1type == indexExtern) op1type -= 3;
	if(op2type == variableExtern || op2type == indexExtern) op2type -= 3;

	if(line_ast->operand_count == 2)
	{
		/*change the first operator type area manualy*/
		char* encodedType1 = encode(op1type);
		memcpy(encoded+8,encodedType1+12,2);
		free(encodedType1);

		/*change the second operator type area manualy*/
		char* encodedType2 = encode(op2type);
		memcpy(encoded+10,encodedType2+12,2);
		free(encodedType2);
	}
	else if(line_ast->operand_count == 1)
	{
		/*change the first operator type area manualy*/
		char* encodedType1 = encode(op1type);
		memcpy(encoded+10,encodedType1+12,2);
		free(encodedType1);
	}
	/*change the opcode area manualy*/
	char* opcode = encode(line_ast->op_code);
	encoded[7] = opcode[13];
	encoded[6] = opcode[12];
	encoded[5] = opcode[11];
	encoded[4] = opcode[10];

	return encoded;
}



char *encode(int n) { 
    char *binaryNum = (char *)calloc(ENCODING_LENGTH * sizeof(char), 1);
	int negative = 0;
	memset(binaryNum,'0',ENCODING_LENGTH);

	if(n < 0) {
		n *= -1;
		negative = 1;
	}

    int i = 0;
    while (n > 0) {
        binaryNum[13 - i] = n % 2 + '0';
        n = n / 2;
        i++;

		if(i > 13) return NULL;
    }

	if(negative)
	{
		findOnesComplement(binaryNum);
		addOne(binaryNum);
	}
	return binaryNum;
}



/* ---------- encoding utility functions ----------  */

/* reverse - reverse a given string */
void reverse(char *str) {
    int length = strlen(str), i = 0;

    for (i = 0; i < length / 2; i++) {
        char temp = str[i];

        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}


/* findOnesComplement - find the one's complement of a given binary string */
void findOnesComplement(char *binary) {
    int length = strlen(binary), i = 0;

    for (i = 0; i < length; i++) {
        if (binary[i] == '0')
            binary[i] = '1';
        else if (binary[i] == '1')
            binary[i] = '0';
    }
}


/* addOne - add one to a binary string */
void addOne(char *binary) {
    int length = strlen(binary), i = 0;
    int carry = 1;
    for (i = length - 1; i >= 0; i--) {
        if (binary[i] == '0' && carry == 1) {
            binary[i] = '1';
            carry = 0;
            break;
        } else if (binary[i] == '1' && carry == 1) {
            binary[i] = '0';
        }
    }
}
