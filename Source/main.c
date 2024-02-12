#include <stdio.h>
#include <string.h>
#include "preAssembler.h"
#include "Label.h"
#include "Identify.h"
#include "CutPaste.h"
#include "BinBase64.h"
#include "First_pass.h"
#include "Second_pass.h"
#include "Var.h"
#define MAX_FILE_LENGTH 1000
int error, ic, dc;
labelTable labels;
externTable et;

int main(int argc, const char* argv[])
{
	int i;
	char updateName[MAX_FILE_LENGTH];
	char fileName[MAX_FILE_LENGTH];
	FILE* amFile;

	if (argc <= 1)
	{
		printf("No File where typed.\n");
		return 0;
	}

	for (i = 1; i < argc; i++)
	{
		strcpy(fileName, argv[i]);
		printf("------File: %s-------\n", fileName);
		error = 0;
		ic = 0;
		dc = 0;
		labels.head = NULL;
		et.head = NULL;

		PreAssembler(fileName);
		if (error)
			continue;

		sprintf(updateName, "%s.am", fileName);
		amFile = fopen(updateName, "r");
		if (amFile == NULL) {
			printf("Failure to open the file created by the pre-assembler");
			continue;
		}

		first_pass(amFile);
		if (error) {
			freeLabel(&labels);
			continue;
		}
		fseek(amFile, 0, SEEK_SET); /*Back to the top of the file*/
		second_pass(amFile, fileName);

		writeEntryTable(&labels, fileName);
		writeExternTable(&et, fileName);

		fclose(amFile);
	}
	return 0;
}
