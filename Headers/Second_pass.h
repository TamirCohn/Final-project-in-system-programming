#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include <string.h>
#include "Declarations.h"

void print_instruction(char* line, FILE* f);
void print_data(char* line, FILE* f);
void second_pass(FILE* f, char* fileName);

#endif

