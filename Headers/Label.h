#ifndef LABEL_H
#define LABEL_H

#include <stdio.h>
#include "Var.h"

nodeL* searchLabel(labelTable *t, char name[MAX_CHAR_LABEL]);
void updateLabel(labelTable *t, char name[MAX_CHAR_LABEL], int newAddress, enum directive e);
void addLabel(labelTable *t, char newName[MAX_CHAR_LABEL], int newAddress, enum directive e);
void addData(labelTable *t, char name[MAX_CHAR_LABEL], int x);
void addEX(externTable *t, char *name, int add);
void freeData(nodeD *t);
void freeLabel(labelTable *t);
void freeEX(externTable *t);
void writeEntryTable(labelTable *t, char *fileName);
void writeExternTable(externTable *t, char *fileName);

#endif

