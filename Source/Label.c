#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Label.h"
#include "Var.h"

/* File Subject: Management of label tables */

/* PART A: Functions for adding, searching and updating the tables */

/**The function searches for a label within the labels table.
 * @param t:  a pointer to the label table.
 * @param name: the name of the label to search for.
 * @return If the label is found - a pointer to it, and if the label is not found - NULL. */
nodeL* searchLabel(labelTable* t, char name[MAX_CHAR_LABEL])
{
	nodeL* p;
	p = t->head;
	while (p != NULL)
	{
		if (!strcmp(p->name, name))
			return p;
		p = p->next;
	}
	return NULL;
}

/** The function allows updating the "address", "e" fields of an existing label within the label table.
 * @param t: a pointer to the labels table.
 * @param name: the name of the label to be updated.
 * @param newAddress: an address to be update. If there is no need to update the address, you can return its existing address.
 * @param e: whether the label has been defined as entry or external. If the label has not been defined in this topic, return NOT_DIR.
 * If the label does not exist - nothing will happen. */
void updateLabel(labelTable* t, char name[MAX_CHAR_LABEL], int newAddress, enum directive e)
{
	nodeL* l = searchLabel(t, name);
	if (l != NULL)
	{
		l->address = newAddress;
		l->e = e;
	}
}

/** The function adds a label to the labels table.
 * @param t: a pointer to the labels table.
 * @param newName: the name of the label.
 * @param newAddress: the address of the label. If the address is not known at this point, write the flag: -1.
 * @param e: whether the label has been defined as entry or extern. If the label has not been defined in this topic, write NOT_DIR.
 * If the label already exists in the labels table - nothing will happen. */
void addLabel(labelTable* t, char newName[MAX_CHAR_LABEL], int newAddress, enum directive e) {
	nodeL* p;
	p = t->head;
	if (searchLabel(t, newName) == NULL) {
		nodeL* newNode = (nodeL*)malloc(sizeof(nodeL));
		strcpy(newNode->name, newName);
		newNode->address = newAddress;
		newNode->e = e;
		newNode->type = NULL;
		newNode->next = NULL;
		if (t->head == NULL)
			t->head = newNode;
		else
		{
			while (p->next != NULL)
				p = p->next;
			p->next = newNode;
		}
	}
}

/** The function adds data to the data table.
 * @param t: a pointer to the label table.
 * @param name: the name of the label to which the data belongs.
 * @param x: the data to be added (as an integer).
 * If the label is not found in the labels table - nothing will happen. */
void addData(labelTable* t, char name[MAX_CHAR_LABEL], int x)
{
	nodeL* n = searchLabel(t, name);
	if (n != NULL) {
		nodeD* p = n->type;
		nodeD* newNode = (nodeD*)malloc(sizeof(nodeD));
		newNode->x = x;
		newNode->next = NULL;
		if (n->type == NULL)
			n->type = newNode;
		else {
			while (p->next != NULL)
				p = p->next;
			p->next = newNode;
		}
	}
}

/** The function adds a label to the extern labels table.
 * @param t: a pointer to the extern labels table.
 * @param name: the name of the label.
 * @param add: the address where a call for the label was made.*/
void addEX(externTable* t, char* name, int add)
{
	nodeEX* p;
	nodeEX* newNode = (nodeEX*)malloc(sizeof(nodeEX));
	p = t->head;
	strcpy(newNode->name, name);
	newNode->address = add;
	newNode->next = NULL;
	if (t->head == NULL)
		t->head = newNode;
	else
	{
		while (p->next != NULL)
			p = p->next;
		p->next = newNode;
	}
}

/* PART B: FREE TABLE'S MEMORY
 * The nodes in the tables use dynamic allocation.
 * These are auxiliary functions, which will release the memory at the end of use.
 * Theis release functions will be called from within the function that prints the tables. */

 /* B.1: Free Data Tables
  * The function frees the data table.
  * Since each label has its own data table, this function is called inside the function that frees the memory of the general label table,
  * and if the label contains a data table, it will be freed using this function.*/
void freeData(nodeD* t)
{
	nodeD* p;
	while (t != NULL)
	{
		p = t;
		t = t->next;
		free(p);
	}
}

/* B.2: Free Labels Table
 * The function frees the labels table.
 * If the function meets a data label, it calls the "freeData" function to release it as well.
 * The function is called from within the function that prints the entry labels table. */
void freeLabel(labelTable* t)
{
	nodeL* p;
	while (t->head != NULL)
	{
		p = t->head;
		t->head = t->head->next;
		if (p->type != NULL) /* Is it a data label? */
			freeData(p->type);
		free(p);
	}
}

/* B.3: Free Extern Labels Table
 * The function frees the extern labels table.
 * The function is called from within the function that prints the external labels table.*/
void freeEX(externTable* t)
{
	nodeEX* p;
	while (t->head != NULL)
	{
		p = t->head;
		t->head = t->head->next;
		free(p);
	}
}


/* Part C: Creating the tables of the external and entry labels */

/** The function exports to a file the table of entry labels: the name of the label, and its address in memory.
 * @param t: a pointer to the labels table.
 * @param fileName: the name of the source file (to produce the table on its name).
 * To create the entry label table, the function loops through the entire labels table. We will use this to free the memory of the entire table.*/
void writeEntryTable(labelTable* t, char* fileName)
{
	nodeL* p;
	FILE* ent = NULL;
	char updateName[50];
	p = t->head;
	sprintf(updateName, "%s.ent", fileName);
	ent = fopen(updateName, "w");
	if (ent == NULL)
		printf("writeEntryTable FAIL");
	while (p != NULL)
	{
		if (p->e == ENTRY)
			fprintf(ent, "%s %d \n", p->name, p->address);
		p = p->next;
	}
	fclose(ent);
	freeLabel(t);
}

/** The function exports to a file the table of extern labels: The name of the label, and the address where it was called.
 * @param t pointer to the extern labels table,
 * @param fileName: the name of the source file (to produce the table on its name).
 * After each row in the table is printed, the function will free its memory.*/
void writeExternTable(externTable* t, char* fileName)
{
	nodeEX* p;
	FILE* ext = NULL;
	char updateName[50];
	p = t->head;
	sprintf(updateName, "%s.ext", fileName);
	ext = fopen(updateName, "w");
	if (ext == NULL)
		printf("writeExternTable FAIL");
	while (p != NULL)
	{
		fprintf(ext, "%s %d \n", p->name, p->address);
		p = p->next;
	}
	fclose(ext);
	freeEX(t);
}
