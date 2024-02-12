#ifndef BINBASE64_H
#define BINBASE64_H

void intToBinaryString(int number, char* binaryString, int bits);
char* transAction(int action, int sourceOpr, int targetOpr);
char* transTwoReg(int reg1Num, int reg2Num);
char* transOneReg(int regNum);
char* transOpr(int number, int a_r_e);
char* transData(int data);
int binaryToDecimal(const char *binary);
void base_64(char* binary, FILE* f);

#endif 

