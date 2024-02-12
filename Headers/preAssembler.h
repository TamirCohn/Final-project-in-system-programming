#ifndef PREASSEMBLER_H
#define PREASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 81
#define MAX_FILE_LENGTH 1000

typedef struct {
    char name[MAX_LINE_LENGTH];
    char content[MAX_FILE_LENGTH];
} Macro;

typedef struct Node {
    Macro data;
    struct Node* next;
} Node;

void AddMacroToList(Node** head, Macro macro);
void RemoveMacrosList(Node* head);
void preAssembler(const char* input_file_name);

#endif

