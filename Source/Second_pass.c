#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Second_pass.h"
#include "Declarations.h"

void second_pass(FILE* f, char* fileName)
{
	char line[LINE_LENGTH];
	FILE* ob = NULL;
	char updateName[50];
	sprintf(updateName, "%s.ob", fileName);
	ob = fopen(updateName, "w");
	fprintf(ob, "%d %d\n", ic, dc);
	if (ob != NULL)
	{
		while (fgets(line, LINE_LENGTH, f) != NULL) /* Read lina after line until the end of the end of file.*/
			print_instruction(line, ob);
		fseek(f, 0, SEEK_SET);
		while (fgets(line, LINE_LENGTH, f) != NULL) /* Read lina after line until the end of the end of file.*/
			print_data(line, ob);
		fclose(ob);
	}
	else
		printf("Second pass failed creat the file");
}

void print_instruction(char* line, FILE* f)
{
	static int count = START_POINT;
	char* name;
	char* p1, * p2;
	int opcode, n1, n2, content1, content2, ARE1, ARE2; /* n Addressing method*/
	enum parameter type1, type2;
	line = removeLabel(line);
	name = firstWord(line);
	if (idDir(name) != NOT_DIR)
		return;
	opcode = idOpcode(name);
	p1 = getFirstOpr(line);
	p2 = getSecondOpr(line);
	type1 = idParam(p1, 0);
	type2 = idParam(p2, 0);
	count++;
	switch (type1)
	{
	case NUMBER:
		n1 = 1;
		content1 = atoi(p1);
		ARE1 = 0;
		count++;
		break;
	case LABEL:
		n1 = 3;
		content1 = searchLabel(&labels, p1)->address;
		ARE1 = content1 == 0 ? 1 : 2;
		count++;
		if (searchLabel(&labels, p1)->e == EXTERN)
			addEX(&et, p1, count);
		break;
	case REGISTER: /* we wait to check the another operand to know how to update the count line*/
		n1 = 5;
		content1 = regNum(p1);
		ARE1 = 'r';
		break;
	case NOT_EXIST:
	case NOT_PARAM:
		n1 = 0;
		break;
	}
	switch (type2)
	{
	case NUMBER:
		n2 = 1;
		content2 = atoi(p2);
		ARE2 = 0;
		count++;
		break;
	case LABEL:
		n2 = 3;
		content2 = searchLabel(&labels, p2)->address;
		ARE2 = content2 == 0 ? 1 : 2;
		count++;
		if (searchLabel(&labels, p2)->e == EXTERN)
			addEX(&et, p2, count);
		break;
	case REGISTER:
		n2 = 5;
		content2 = regNum(p2);
		ARE2 = 'r';
		if (type1 == REGISTER)
			count++;
		else
			count += 2;
		break;
	case NOT_EXIST:
	case NOT_PARAM:
		n2 = 0;
		break;
	}

	base_64(transAction(opcode, n1, n2), f);

	if (type1 == REGISTER && type2 == REGISTER)
		base_64(transTwoReg(content1, content2), f);
	else
	{
		if (type1 != NOT_EXIST)
			base_64(transOpr(content1, ARE1), f);
		if (type2 != NOT_EXIST)
			base_64(transOpr(content2, ARE2), f);
	}
}

void print_data(char* line, FILE* f)
{
	nodeD* p;
	enum directive d;
	char* name;
	char newline[LINE_LENGTH];
	if (isValidLabel(line))
	{
		strcpy(newline, line);
		name = strtok(newline, ":  \t");
		line = removeLabel(line);
		d = idDir(firstWord(line));
		if (d != DATA && d != STRING)
			return;
		p = searchLabel(&labels, name)->type;
		while (p != NULL)
		{
			base_64(transData(p->x), f);
			p = p->next;
		}
		if (d == STRING)
			base_64(transData(0), f);
	}
}

