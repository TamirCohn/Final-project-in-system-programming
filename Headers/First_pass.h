#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include <stdio.h>
#include "Declarations.h"
void pass_line(char* line, int line_number);
void updateAddress(labelTable *t, int start);
void first_pass(FILE* f);

#endif

