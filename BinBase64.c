#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinBase64.h"
/**
 * Converts an integer to its binary string representation with a specified number of bits.
 *
 * @param number The integer to convert to binary.
 * @param binaryString The string to store the binary representation.
 * @param bits The number of bits for the binary representation.
 */
void intToBinaryString(int number, char* binaryString, int bits) {
	int i;
	int bit;
	for (i = bits - 1; i >= 0; i--) {
		bit = (number >> i) & 1;
		binaryString[bits - 1 - i] = '0' + bit;
	}
	binaryString[bits] = '\0';
}
/**
 * Converts the action, source operand, and target operand to their combined binary representation.
 *
 * @param action The action index.
 * @param sourceOpr The source operand value.
 * @param targetOpr The target operand value.
 * @return The combined binary representation as a dynamically allocated string.
 */
char* transAction(int action, int sourceOpr, int targetOpr) {
	char binaryParam1[4];
	char binaryParam2[5];
	char binaryParam3[4];
	char* encodedResult;
	if (targetOpr == 0) {
		targetOpr = sourceOpr;
		sourceOpr = 0;
	}
	intToBinaryString(sourceOpr, binaryParam1, 3);
	intToBinaryString(action, binaryParam2, 4);
	intToBinaryString(targetOpr, binaryParam3, 3);
	encodedResult = (char*)malloc(13 * sizeof(char));
	sprintf(encodedResult, "%s%s%s%s", binaryParam1, binaryParam2, binaryParam3, "00\0");
	return encodedResult;
}
/**
 * Converts two register numbers to their combined binary representation.
 *
 * @param reg1Num The value of the first register.
 * @param reg2Num The value of the second register.
 * @return The combined binary representation as a dynamically allocated string.
 */
char* transTwoReg(int reg1Num, int reg2Num) {
	char binaryReg1[6];
	char binaryReg2[6];
	char* encodedResult;
	intToBinaryString(reg1Num, binaryReg1, 5);
	intToBinaryString(reg2Num, binaryReg2, 5);
	encodedResult = (char*)malloc(13 * sizeof(char));
	sprintf(encodedResult, "%s%s%s", binaryReg1, binaryReg2, "00\0");
	return encodedResult;
}
/**
 * Converts a register number to its binary representation.
 *
 * @param regNum The value of the register.
 * @return The binary representation as a dynamically allocated string.
 */
char* transOneReg(int regNum)
{
	char binaryReg[6];
	char* encodedResult;
	intToBinaryString(regNum, binaryReg, 5);
	encodedResult = (char*)malloc(13 * sizeof(char));
	sprintf(encodedResult, "%s%s", binaryReg, "0000000\0");
	return encodedResult;
}
/**
 * Converts a representative number to a binary string with an appropriate A_E_R
 *
 * @param number The number to convert.
 * @param a_r_e the representative number of A_R_E
 * @return the binary representation as a dynamically allocated string.
 */
char* transOpr(int number, int a_r_e)
{
	char binaryNumber[11];
	char* encodedResult;
	if (a_r_e == 'r')
		return transOneReg(number);
	intToBinaryString(number, binaryNumber, 10);
	encodedResult = (char*)malloc(13 * sizeof(char));
	if (a_r_e == 0)
		sprintf(encodedResult, "%s%s", binaryNumber, "00\0");
	if (a_r_e == 1)
		sprintf(encodedResult, "%s", "000000000001\0");
	if (a_r_e == 2)
		sprintf(encodedResult, "%s%s", binaryNumber, "10\0");
	return encodedResult;
}
/**
 * Converts data to its binary representation.
 *
 * @param data The data value to convert.
 * @return The binary representation as a dynamically allocated string.
 */
char* transData(int data)
{
	char binaryNumber[13];
	char* encodedResult = (char*)malloc(13 * sizeof(char));
	if (encodedResult == NULL)
		return NULL;
	intToBinaryString(data, binaryNumber, 12);
	binaryNumber[12] = '\0';
	strcpy(encodedResult, binaryNumber);
	return encodedResult;
}
/**
 * Converts a binary string to its decimal representation.
 *
 * @param binary The binary string to convert.
 * @return The decimal representation of the binary string.
 */
int binaryToDecimal(const char* binary)
{
	int decimal = 0;
	int i;

	if (strlen(binary) != 6)
		return -1;

	for (i = 0; i < 6; i++) {
		if (binary[i] == '1') {
			decimal += 1 << (5 - i);
		}
	}

	return decimal;
}
/**
 * Converts binary to base-64 and writes the result to a file.
 *
 * @param binary The binary string to convert.
 * @param f The file pointer to write the result.
 */
void base_64(char* binary, FILE* f)
{
	int i;
	char bin1[7];
	char bin2[7];
	int decimal1;
	int decimal2;
	char ch;
	char result[3];
	char* base64Array = malloc(64 * sizeof(char));
	ch = 'A';
	for (i = 0; i < 26; i++)
		base64Array[i] = ch++;
	ch = 'a';
	for (; i < 52; i++)
		base64Array[i] = ch++;
	ch = '0';
	for (; i < 62; i++)
		base64Array[i] = ch++;
	base64Array[62] = '+';
	base64Array[63] = '/';
	strncpy(bin1, binary, 6);
	bin1[6] = '\0';
	strncpy(bin2, binary + 6, 6);
	bin2[6] = '\0';
	decimal1 = binaryToDecimal(bin1);
	decimal2 = binaryToDecimal(bin2);
	result[0] = base64Array[decimal1];
	result[1] = base64Array[decimal2];
	result[2] = '\0';
	fprintf(f, "%s\n", result);
	free(binary);
	free(base64Array);
}
