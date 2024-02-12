#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "First_pass.h"
#include "Declarations.h"

/* File subject: First pass of the assembler.
 * The first pass has 2 goals:
 * First, check the validity of the code.
 * The second, build the symbol table, and the data table.
 * Note: the code can be understood only after knowing the files: Label.c, Identify.c, Cut&Paste.c */

 /**
  * The function manages the first pass.
  * It goes line by line in the file, performs checks on its validity, and builds the symbol table at the same time.
  * After finishing going through the file, if it was found to be valid, it goes through the symbol table and updates the addresses.
  * @param f A pointer to a file created by the pre assembler. */
void first_pass(FILE* f)
{
	char line[LINE_LENGTH];
	int line_count = 1;
	while (fgets(line, LINE_LENGTH, f) != NULL) /* Read lina after line until the end of the file.*/
	{
		pass_line(line, line_count);
		line_count++;
	}
	if (!error)
		updateAddress(&labels, START_POINT);
}

void pass_line(char* line, int line_number)
{
	char* name = NULL, * p, * end, * com, * op1 = NULL, * op2 = NULL;
	char newline[LINE_LENGTH];
	int ad;
	nodeL* l;
	enum directive d;

	/* There may be a label at the beginning of the line, which should be the target of an operation result or a directive that follows it.
	 * If it exists, we will save its name, and remove it from the line for the rest of the transition.
	 * Note: a label that is not legally written at the beginning of the line,
	 * will not be considered a label but as a word of instruction, and we will treat it accordingly. */
	if (isValidLabel(line))
	{
		strcpy(newline, line);
		name = strtok(newline, ":  \t");
		line = removeLabel(line);
	}

	/* Now we move to the body of the line, and identify the instruction word in it,
	 * which is actually the first word in the line (after the label, if there was one).*/
	com = firstWord(line);

	/* The structure of the test will be as follows:
	 * First, we will check whether it is a directive (beginning with a dot) and check its validity.
	 * If this is not a directive, we will check whether it is an operation, and we will check its validity.
	 * Otherwise, we will conclude that it is an unrecognized command. */

	 /* directive */
	if (com[0] == '.')
	{
		d = idDir(com);
		switch (d)
		{
		case EXTERN:
		case ENTRY:
			if (name != NULL) /*There was a label before the directive*/
				/* This is a warning which not an error */
				printf("line %d: An 'entry' or 'extern' directive cannot come after a label\n", line_number);
			else
			{
				name = getFirstOpr(line);/* After the directive, there should come the label to which the directive refers.*/
				if (name == NULL) {
					printf("line %d: entry directive has to label\n", line_number);
					error = 1;
					return;
				}
				if (valLab(name, line_number)) /* The label that appears after the directive is a valid label */
				{
					l = searchLabel(&labels, name);
					if (l != NULL) /* The label exist already in the label table */
					{
						if (l->address != 0 && d == EXTERN) /*If the label address is non-zero, it means that it is already set as a source label*/
						{
							printf("line %d: A source label cannot be set as external\n", line_number);
							error = 1;
							return;
						}
						if (l->e != NOT_DIR && l->e != d) {
							printf("line %d:The label has already been defined differently, the directive has been rejected\n", line_number);
							error = 1;
							return;
						}
						/*If we got this far, that means the line is valid*/
						if (d == EXTERN)
							ad = 0;
						else
							ad = l->address; /*If the directive is internal, we don't want to change the existing address of the label*/

						updateLabel(&labels, name, ad, d);
					}
					else /* The label doesn't exist in the label table */
					{
						if (d == EXTERN)
							ad = 0;
						else
							ad = -1; /* -1 is a flag for empty label */
						addLabel(&labels, name, ad, d);
					}
				}
			}
			break;
		case DATA:
			if (name == NULL) /*There was not a label before the directive*/
			{
				printf("line %d: A '.data' directive, cannot come without a label to be the destination for this data\n", line_number);
				error = 1;
				return;
			}
			if (valData(line)) /* The directive line and the data in it- are valid */
			{
				l = searchLabel(&labels, name);
				if (l != NULL) /* The label exist already in the label table */
				{
					if (l->e == EXTERN)
					{
						printf("line %d: Data cannot be set to an external label\n", line_number);
						error = 1;
						return;
					}
					if (l->address != -1)
					{
						/*If the label already exists, and it is not external, the only possibility we will insert data into it is:
						 * if it has been defined as entry, and has not yet been initialized (at the address of the flag-1 is found).
						 * If the label exists, and has a valid address, it means that it has already been defined differently, and the data cannot be entered into it.*/
						printf("line %d: The content of the label has already been defined elsewhere, the data directive has been rejected\n", line_number);
						error = 1;
						return;
					}
					updateLabel(&labels, name, dc, l->e);
				}
				else /* The label doesn't exist in the label table */
					addLabel(&labels, name, dc, NOT_DIR);

				/* If we have come this far, it means that the directive line and the data are there, they are valid.
				 * Now, we need to insert into the data table of the label, the data that appears.*/
				p = strtok(NULL, ", .data"); /* We skip the directive straight to the data */
				while (p != NULL) /* We will go through all the data until we are done */
				{
					addData(&labels, name, atoi(p));
					p = strtok(NULL, ", .data");
					dc++;
				}
			}
			else
			{
				printf("line %d: The data is invalid\n", line_number);
				error = 1;
			}
			break;
		case STRING:
			if (name == NULL)
			{
				printf("line %d: A '.string' directive cannot come without a label to be the destination for the data\n", line_number);
				error = 1;
				return;
			}
			if (valString(line))
			{
				l = searchLabel(&labels, name);
				if (l != NULL)
				{
					if (l->e == EXTERN)
					{
						printf("line %d: Data cannot be set to an external label\n", line_number);
						error = 1;
						return;
					}
					if (l->address != -1)
					{
						printf("line %d: The content of the label has already been defined elsewhere, the data directive has been rejected\n", line_number);
						error = 1;
						return;
					}
					updateLabel(&labels, name, dc, l->e);
				}
				else
					addLabel(&labels, name, dc, NOT_DIR);
				p = strchr(line, '\"'); /* find the open " */
				end = strchr(++p, '\"'); /* find the close " */
				while (p != end) /* pass over the string */
				{
					addData(&labels, name, (int)*p);
					p++;
					dc++;
				}
				dc++; /* for char end of string '\0' */
			}
			else
			{
				printf("line %d: The data is invalid\n", line_number);
				error = 1;
			}
			return;
		case NOT_DIR:
			printf("line %d: Invalid directive\n", line_number);
			error = 1;
			break;
		}
		return;
	}

	/* operation */
	if (idOp_name(com) != NOT_OPERATION)
	{
		op1 = getFirstOpr(line);
		op2 = getSecondOpr(line);
		if (matchOpParam(com, op1, op2, noMorOpr(line), line_number))
		{
			if (name != NULL)
			{
				l = searchLabel(&labels, name);
				if (l == NULL)
					addLabel(&labels, name, ic, NOT_DIR);
				else
				{
					if (l->e == EXTERN)
					{
						printf("line %d: An external label cannot be a source label\n", line_number);
						error = 1;
						return;
					}
					if (l->address != -1)
					{
						printf("line %d: The content of the label has already been defined elsewhere, the operation has been rejected\n", line_number);
						error = 1;
						return;
					}
					updateLabel(&labels, name, ic, NOT_DIR);
				}
			}
			ic++; /*instruction*/
			if (op1 != NULL)
			{
				if (op2 != NULL)
				{
					if (idParam(op1, line_number) == REGISTER && idParam(op2, line_number) == REGISTER)
					{
						ic++;
					}
					else
					{
						ic += 2;
					}
				}
				else
				{
					ic++;
				}
			}
		}
		else
		{
			return;
		}
	}

	/* unrecognized command */
	else
	{
		printf("line %d: The line contain an unrecognized command\n", line_number);
		error = 1;
		return;
	}
}

void updateAddress(labelTable* t, int start)
{
	nodeL* p = t->head;
	while (p != NULL)
	{
		if (p->e == EXTERN) {
			p = (p->next);
			continue;
		}
		if (p->address == -1)
			printf("WARNING: In The symbol table has an internal label that has not been initialized");
		if (p->type == NULL)
			p->address += start;
		else
			p->address += (ic + start);
		p = p->next;
	}
}





