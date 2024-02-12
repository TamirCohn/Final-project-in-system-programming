#ifndef IDENTIFY_H
#define IDENTIFY_H

#include <stdio.h>
#include "Declarations.h"

int idOpcode(char *name);
enum operation idOp_name(char *name);
enum parameter idParam(char *name, int line_number);
int valReg(char *name, int line_number);
int valNum(char *name, int line_number);
int valLab(char *name, int line_number);
int regNum(char *name);
int noMorOpr(const char *line);
int matchOpParam(char *firstWord, char *f, char *s, int end, int line_number);
enum directive idDir(char *name);
int valData(char *line);
int valString(const char *line);
int isValidLabel(char *line);

#endif

