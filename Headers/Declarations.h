#ifndef Declarations

#define Declarations

#include <stdio.h>
#include "Var.h"

extern int error, ic, dc;
extern labelTable labels;
extern externTable et;

/* ----------- identify ----------- */
int idOpcode(char* name);
enum operation idOp_name(char* name);
enum parameter idParam(char* name, int line_number);
int regNum(char* name);
int valReg(char* name, int line_number);
int valNum(char* name, int line_number);
int valLab(char* name, int line_number);
int noMorOpr(const char* line);
int matchOpParam(char* firstWord, char* f, char* s, int end, int line_number);
enum directive idDir(char* name);
int valData(char* line);
int valString(const char* line);
int isValidLabel(char* line);

/* ----------- Cut&Paste ----------- */
char* removeLabel(char* line);
char* firstWord(const char* line);
char* getFirstOpr(const char* line);
char* getSecondOpr(const char* line);

/* ----------- Label ----------- */
nodeL* searchLabel(labelTable* t, char name[MAX_CHAR_LABEL]);
void updateLabel(labelTable* t, char name[MAX_CHAR_LABEL], int newAddress, enum directive e);
void addLabel(labelTable* t, char newName[MAX_CHAR_LABEL], int newAddress, enum directive e);
void addData(labelTable* t, char name[MAX_CHAR_LABEL], int x);
void addEX(externTable* t, char* name, int add);
void writeEntryTable(labelTable* t, char* fileName);
void writeExternTable(externTable* t, char* fileName);

/* ----------- Bin&Base64 ----------- */
char* transAction(int action, int sourceOpr, int targetOpr);
char* transTwoReg(int reg1Num, int reg2Num);
char* transOpr(int number, int a_r_e);
char* transData(int data);
void base_64(char* binary, FILE* f);

/* ----------- Pre Assembler ----------- */
void PreAssembler(const char* input_file_name);

/* ----------- First pass ----------- */
void pass_line(char* line, int line_number);
void updateAddress(labelTable* t, int start);
void first_pass(FILE* f);

/* ----------- Second pass ----------- */
void print_instruction(char* line, FILE* f);
void print_data(char* line, FILE* f);
void second_pass(FILE* f, char* fileName);

#endif
