#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Identify.h"
#include "Declarations.h"


/* File Subject: Identification and validity
 * The file deals with the identification and analysis of specific parts of the lines of code.
 * The identification also includes the validity of those parts.*/

 /* Part A: Operations and Operands */

 /* A.1: Operations */
 /** The function receives an operation name, and returns the opcode.
  * @param name: The operation name as a string
  * @return The opcode of the operation. If the operation is not defined, the value -1 will be returned.
  */
int idOpcode(char* name)
{
	/* The operation names are indexed by their opcode. That is, the index in the array is the opcode of the operation.
	 * The function go through the array and perform a comparison between each cell and the name received as a parameter.
	 * If a match is found - we will return the index, which is actually the operation's opcode.
	 */
	char* commands[16] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc",
						 "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop" };
	int i;
	for (i = 0; i < 16; i++) {
		if (!strcmp(commands[i], name))
			return i;
	}
	return -1;
}

/**
 *  The function receives a string, and if it is an action, the function will return its name.
 * @param name the string to check
 * @return the operation name (varies from 'enum operation' type).
 * @return If the string is not an operation name - the value 'NOT_OPERATION'.
 * @return If the string is NULL- the value 'NOT_EXIST'.
 */
enum operation idOp_name(char* name) {
	/* We use the idOpcode function to do the opposite process: send it the name of the operation, and it will return his opcode.
	 * According to the opcode, we will identify the name of the operation and classify it according to the values of the var 'enum operation'.*/
	switch (idOpcode(name))
	{
	case 0:
		return MOV;
		break;
	case 1:
		return CMP;
		break;
	case 2:
		return ADD;
		break;
	case 3:
		return SUB;
		break;
	case 4:
		return NOT;
		break;
	case 5:
		return CLR;
		break;
	case 6:
		return LEA;
		break;
	case 7:
		return INC;
		break;
	case 8:
		return DEC;
		break;
	case 9:
		return JMP;
		break;
	case 10:
		return BNE;
		break;
	case 11:
		return RED;
		break;
	case 12:
		return PRN;
		break;
	case 13:
		return JSR;
		break;
	case 14:
		return RTS;
		break;
	case 15:
		return STOP;
		break;
	case -1:
	default:
		return NOT_OPERATION;
	}
}

/* A.2: Operands */

/** The function receives a string, and checks whether it is a valid operand.
 * @param name the string to check.
 * @param line_number The line number in the source file (after the pre-assembler step), where the parameter 'name' appears.
 * @return If the string is a valid parameter- its name will be returned as one of the values of the "enum parameter" variable: number, label, or register.
 * @return If the string is not a valid parameter- will be returned "NOT_PARAM", and the error type will be printed.
 * @return If the string is not exist- will be returned "NOT_EXIST".
 */
enum parameter idParam(char* name, int line_number)
{
	/* A register will be defined as a name that begins with the character ׳@׳.
	 * A label will be defined when the first character is a letter.
	 * A number will be set when the first character is a number, or one of the characters: (+) , (-) and a number after it.*/
	if (name == NULL) /* The operand not exist */
		return NOT_EXIST;
	if (!strcmp(name, "undentify operand"))
	{
		printf("line %d: unidentified operand\n", line_number);
		error = 1;
		return NOT_PARAM;
	}
	if (name[0] == '@') /* Register */
	{
		if (valReg(name, line_number))
			return REGISTER;
		else
		{
			return NOT_PARAM; /* The error message is printed from the function that checks the specific valid. */
		}
	}
	else if (isalpha(name[0])) /* Label */
	{
		if (valLab(name, line_number))
			return LABEL;
		else
		{
			return NOT_PARAM; /* The error message is printed from the function that checks the specific valid. */
		}
	}
	else if (isdigit(name[0]) || ((name[0] == '+' || name[0] == '-') && isdigit(name[1]))) /*Number*/
	{
		if (valNum(name, line_number))
			return NUMBER;
		else
		{
			return NOT_PARAM; /* The error message is printed from the function that checks the specific valid. */
		}
	}
	else /* The string cannot be classified at all according to the rules we established above. */
	{
		printf("line %d: Invalid operand\n", line_number);
		error = 1;
		return NOT_PARAM;
	}
}

/**The function checks the validity of a register
 * @param name The name of the register to check
 * @param line_number The number of the code line where the register appeared.
 * @return If the register is valid: 1, otherwise: 0.
 * Note: the function assumes that it is a register, meaning that the first character is @, and it does not check this.*/
int valReg(char* name, int line_number)
{
	if (isalpha(name[1]) && name[1] == 'r' && name[2] <= '7' && name[2] >= '0' && name[3] == '\0')
		return 1;
	else
	{
		printf("line %d: Invalid register\n", line_number);
		error = 1;
		return 0;
	}
}

/**The function checks the validity of a register
 * @param name The name of the number to check
 * @param line_number The number of the code line where the number appeared.
 * @return If the number is valid: 1, otherwise: 0.
 * Note: the function assumes that it is a number, meaning that the first character is a number, or one of the characters: (+) , (-) and a number after it.*/
int valNum(char* name, int line_number) {
	int i = 1;
	while (name[i] != '\0') {
		if (isdigit(name[i]))
			i++;
		else {
			printf("line %d: Invalid number\n", line_number);
			error = 1;
			return 0;
		}
	}
	return 1;
}

/**The function checks the validity of a label (as operand)
 * @param name The name of the label to check
 * @param line_number The number of the code line where the label appeared.
 * @return If the label is valid: 1, otherwise: 0.
 * Note: the function assumes that it is a label, meaning that the first character is a letter.*/
int valLab(char* name, int line_number)
{
	int i = 1;
	if (idOp_name(strtok(name, ":")) != NOT_OPERATION)
	{
		printf("line %d: A label name cannot be an operation name\n", line_number);
		error = 1;
		return 0;
	}
	while (name[i] != '\0')
	{
		if (isalpha((name[i])) || isdigit(name[i]))
			i++;
		else
		{
			printf("line %d: Invalid label\n", line_number);
			error = 1;
			return 0;
		}
	}
	return 1;
}

/**The function receives a name of a register that it assumes to be correct, and returns the number of the register.
 * @param name the register name.
 * @return the number of the register. If for some reason the register number detection fails, -1 will be returned (not supposed to happen).
 */
int regNum(char* name)
{
	int i;
	for (i = 0; i <= 7; i++)
		if ((name)[2] - '0' == i)
			return i;
	return -1;
}
/**
 * Checks if there are no more operands in a line of code.
 *
 * @param line A pointer to the input line of code.
 * @return 1 if no more operands are found, 0 otherwise.
 */
int noMorOpr(const char* line) {
	while (*line) {
		if (*line != ',') {
			line++;
			continue;
		}
		break;
	}
	line++;
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	while (*line != ' ' && *line != '\t' && *line != '\n' && *line != '\0') {
		line++;
	}
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	if (*line == '\n' || *line == '\0')
		return 1;
	return 0;
}


/* A.3: Integration between the action and the parameters */

/**
 * The function receives 3 strings:
 * the first - the operation, the second - the first operand, the third - the second operand.
 * The function is a complex function and combines several functions from Part A of this file.
 * First, it detects the operation and operands and checks their validity, with the help of the other functions in this part.
 * After checking the validity of each parameter on its own, it checks the compatibility between the operation and the operands,
 * in two ways: the amount of operands, and the type of operands.
 * It also checks (using a helper function) that there are no operands or extra characters.
 * @param firstWord: The string that should be the operation
 * @param f: The first operand, if not exist- NULL;
 * @param s: The second operand, if not exist- NULL;
 * @param end: Flag variable, reserves whether there are extra characters at the end of the string.
 * @param line_number: The number of the line of code that the function analyze.
 * @return If the line of code is valid- The number 1.
 * @return If an error was found in the line of code- The number 0, An appropriate error message will also be printed.
 * If the error was due to a point invalidity of the operand or the operation - the error messages are printed from the auxiliary functions.
 * If the error was due to a mismatch between the operation and the operands- the error message are printed from this function.
 */
int matchOpParam(char* firstWord, char* f, char* s, int end, int line_number)
{
	/* Checking validity and matching in the function, works in the form of "barriers":
	 * If the line passed one test, it goes to the next test, and so on.
	 * If it passed all the tests successfully - the value 1 will be returned to indicate its validity.
	 * If an error was discovered in one of the tests -
	 * the test process will announce that it failed, and an error message will appear on this line.
	 */
	enum parameter first = idParam(f, line_number);
	enum parameter second = idParam(s, line_number);
	if (first == NOT_PARAM || second == NOT_PARAM)
		return 0;
	switch (idOp_name(firstWord)) {
		/* Many operations share common checks, so they are ordered one after the other as follows:
		 * each top operation from its friend includes all of its friend's checks, plus its own checks, until the instruction "return 1".
		 * Every time the instruction "return 1" appears, it means that the test process has ended on a group of functions that have common tests,
		 * and now a new test on a group of other functions begins.
		 * Note: the meaning of each test is clarified by the error message written below it, in the 'printf' function. */

	case LEA:
		if (first == NUMBER || first == REGISTER) {
			printf("line %d: Operand number 1 can't be a number or register, only a label\n", line_number);
			error = 1;
			return 0;
		}
	case MOV:
	case ADD:
	case SUB:
		if (second == NUMBER) {
			printf("line %d: Operand number 2 can't be a number\n", line_number);
			error = 1;
			return 0;
		}
	case CMP:
		if (first == NOT_EXIST) {
			printf("line %d: Operand number 1 is missing\n", line_number);
			error = 1;
			return 0;
		}
		if (second == NOT_EXIST) {
			printf("line %d: Operand number 2 is missing\n", line_number);
			error = 1;
			return 0;
		}
		if (!end) {
			printf("line %d: Too many operands or characters for the operation, only tow operands is required\n", line_number);
			error = 1;
			return 0;
		}
		return 1;
	case NOT:
	case CLR:
	case INC:
	case DEC:
	case JMP:
	case BNE:
	case RED:
	case JSR:
		if (first == NUMBER) {
			printf("line %d: Operand number 1 can't be a number\n", line_number);
			error = 1;
			return 0;
		}
	case PRN:
		if (first == NOT_EXIST) {
			printf("line %d: Operand number 1 is missing\n", line_number);
			error = 1;
			return 0;
		}
		if (second != NOT_EXIST) {
			printf("line %d: Too many parameters for the operation, only one is required\n", line_number);
			error = 1;
			return 0;
		}
		return 1;
	case RTS:
	case STOP:
		if (first != NOT_EXIST) {
			printf("line %d: No operands are required for operation\n", line_number);
			error = 1;
			return 0;
		}
		if (second != NOT_EXIST) {
			printf("line %d: No operands are required for operation\n", line_number);
			error = 1;
			return 0;
		}
		return 1;
	case NOT_OPERATION:
		return 0;
	}
	return 0;
}

/* Part B: Directives and Data */
/** The function receives a string and identifies whether it is the name of a directive, and what it is.
 * @param name The string to check if it is a directive.
 * @return If the string is a valid directive - the name of the directive will be returned, as one of the values of the variable "enum directive".
 * @return If the string is not a valid directive - the value 'NOT_DIR' will be returned.
 */
enum directive idDir(char* name) {
	if (!strcmp(name, ".data"))
		return DATA;
	if (!strcmp(name, ".string"))
		return STRING;
	if (!strcmp(name, ".entry"))
		return ENTRY;
	if (!strcmp(name, ".extern"))
		return EXTERN;
	return NOT_DIR;
}
/**
 * Checks if a line contains valid data declaration syntax.
 *
 * @param line A pointer to the input line of code.
 * @return 1 if the line is a valid data declaration, 0 otherwise.
 */
int valData(char* line) {
	int commaCount;
	int firstDigit;
	int notEmpty;
	int spaceBetweenDigits;
	int zeroFlag;
	int countSp;
	countSp = 0;
	zeroFlag = 0;
	spaceBetweenDigits = 0;
	notEmpty = 0;
	firstDigit = 1;
	commaCount = 0;

	while (*line == ' ' || *line == '\t') {
		countSp++;
		line++;
	}
	if (countSp == 0)
		return 0;
	if (strncmp(line, ".data", 5) == 0) {
		line += 5;
		if (*line == ' ' || *line == '\t')
			while (*line == ' ' || *line == ',' || *line == '+' || *line == '-' || isdigit(*line) || *line == '\t') {
				if (*line == '+' || *line == '-') {
					if (commaCount == 0 || !isdigit(*(line + 1)))
						return 0;
				}
				if (*line == ',' && commaCount)/*If there are two commas return 0*/
					return 0;
				if (commaCount == 0 && (*line == ' ' || *line == '\t') && notEmpty)
					spaceBetweenDigits = 1;
				if (isdigit(*line)) {/**If there is a digit then reset the comma counter, turn on the There is a digit flag,
								   check if there is no space between digits without a comma*/
					notEmpty = 1;
					commaCount = 0;
					if (spaceBetweenDigits)
						return 0;
					if (zeroFlag)
						return 0;

				}
				if (firstDigit && *line == '0') {/*If the first digit in a number is 0*/
					zeroFlag = 1;
					firstDigit = 0;
				}
				if (firstDigit && isdigit(*line))/*If there is another digit after the first digit, then turn off the flag of the first digit*/
					firstDigit = 0;
				if (*line == ',') { /*If there is a comma then activate the first digit flag*/
					zeroFlag = 0;
					spaceBetweenDigits = 0;
					firstDigit = 1;
					commaCount = 1;
					if (notEmpty == 0)
						return 0;
				}

				line++;
			}
	}
	if (!commaCount && notEmpty)
		return 1; /** The line is not valid */
	return 0;
}
/**
 * Checks if a line contains valid string declaration syntax.
 *
 * @param line A pointer to the input line of code.
 * @return 1 if the line is a valid string declaration, 0 otherwise.
 */
int valString(const char* line) {
	/*If there is no space after the label return 0*/
	if (*line != ' ' && *line != '\t')
		return 0;
	line++;
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	/** Check if the line contains the ".string" keyword after the label*/
	if (strncmp(line, ".string", 7) == 0) {
		line += 7; /** Move the index to skip over the ".string" keyword*/
		/*If no space after .string return 0*/
		if (*line != ' ' && *line != '\t')
			return 0;
		while (*line == ' ' || *line == '\t') {
			line++;
		}
		if (*line == '\"') {
			line++;
			/** Check if there are characters between the double quotes */
			while (*line >= 0 && *line <= 127) {
				if (*line == '\"')
					break;
				line++;
			}

			/** Check if there is a closing double quote and no additional characters after it*/
			if (*line == '\"') {
				line++;
				while (*line != '\0' && *line != '\n') {
					if (*line == ' ' || *line == '\t') {
						line++;
						continue;
					}
					else
						return 0;
				}
				return 1; /** The line is valid */
			}
		}
	}
	return 0; /** The line is not valid */
}
/**
 * Check if a given string is a valid label.
 *
 * @param line A pointer to the input string.
 * @return 1 if the input string is a valid label, 0 otherwise.
 */
int isValidLabel(char* line) {
	while (*line == ' ' || *line == '\t')
		line++;
	if (!isalpha(*line)) {
		return 0;
	}
	line++;
	while (*line != ':' && *line != '\0') {
		if (!isalnum(*line)) {
			return 0;
		}
		line++;
	}

	if (*line == ':')
		return 1;
	return 0;
}

