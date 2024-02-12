#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "preAssembler.h"
#include "Identify.h"
#define MAX_LINE_LENGTH 81
#define MAX_FILE_LENGTH 1000
extern int error;

/**
 * Adds a new macro to the end of a linked list of macros.
 *
 * @param head Pointer to the head of the linked list.
 * @param macro The macro to be added to the linked list.
 */
void AddMacroToList(Node** head, Macro macro) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->data = macro;
	new_node->next = NULL;

	if (*head == NULL) {
		*head = new_node;
	}
	else {
		Node* current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
}

/**
 * Removes all nodes from a linked list of macros and frees the memory.
 *
 * @param head Pointer to the head of the linked list.
 */
void RemoveMacrosList(Node* head) {
	while (head != NULL) {
		Node* temp = head;
		head = head->next;
		free(temp);
	}
}
/**
 * Preprocesses the input assembly file, handling macro expansion and deleting blank lines and comments.
 *
 * @param input_file_name The name of the input file to preprocess.
 */
void PreAssembler(const char* input_file_name) {
	int i;
	int macroRun = 0;
	char line[MAX_LINE_LENGTH];
	char nameWithSuffix[MAX_LINE_LENGTH];
	char output_file_name[MAX_LINE_LENGTH];
	int afterMacroWord;
	Node* macros_list = NULL;
	int found_macro;
	FILE* input_file;
	FILE* output_file;
	Macro new_macro;
	Node* current;
	/* Construct input and output file names */
	strcpy(nameWithSuffix, input_file_name);
	strcpy(output_file_name, input_file_name);
	strcat(nameWithSuffix, ".as");
	strcat(output_file_name, ".am");

	/* Open input and output files */
	input_file = fopen(nameWithSuffix, "r");
	output_file = fopen(output_file_name, "w");

	i = 0;
	if (input_file == NULL || output_file == NULL) {
		printf("The files cannot be opened\n");
		error = 1;
		return;
	}
	rewind(input_file);

	/* Process each line in the input file */
	while (fgets(line, sizeof(line), input_file)) {
		/* Check if the line contains a "mcro" macro definition */
		if (strstr(line, "mcro ") == line) {
			macroRun = 1;
			afterMacroWord = sscanf(line, "mcro %s", new_macro.name);
			/* Validate macro name */
			if (idOp_name(new_macro.name) != NOT_OPERATION) {
				printf("A macro cannot be called by the name of an action: %s\n", line);
				fclose(input_file);
				fclose(output_file);
				RemoveMacrosList(macros_list);
				remove(output_file_name);
				error = 1;
				return;
			}
			current = macros_list;
			/*Checking if the macro name already exists*/
			while (current != NULL) {
				if (strcmp(current->data.name, new_macro.name) == 0) {
					printf("Macro name '%s' already exists in the list.\n", new_macro.name);
					fclose(input_file);
					fclose(output_file);
					RemoveMacrosList(macros_list);
					remove(output_file_name);
					error = 1;
					return;
				}
				current = current->next;
			}
			/*Checking whether the macro was defined in a proper format*/
			if (afterMacroWord != 1) {
				printf("The macro is set in an incorrect format: %s\n", line);
				fclose(input_file);
				fclose(output_file);
				RemoveMacrosList(macros_list);
				remove(output_file_name);
				error = 1;
				return;
			}
			for (i = 6 + strlen(new_macro.name); i < strlen(line); i++) {
				if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') {
					printf("The macro is set in an incorrect format: %s\n", line);
					fclose(input_file);
					fclose(output_file);
					RemoveMacrosList(macros_list);
					remove(output_file_name);
					error = 1;
					return;
				}
			}
			/* Add the new macro to the macros list */
			AddMacroToList(&macros_list, new_macro);
			continue;
		}
		if (macroRun == 1) {
			/* Check if the line marks the end of a macro */
			if (strstr(line, "endmcro") == line) {
				/*Error if there are additional characters after the endmcro*/
				for (i = 8; i < strlen(line); i++) {
					if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') {
						printf("The macro is set in an incorrect format: %s\n", line);
						fclose(input_file);
						fclose(output_file);
						RemoveMacrosList(macros_list);
						remove(output_file_name);
						error = 1;
						return;
					}
				}
				macroRun = 0;
				continue;
			}

			current = macros_list;
			/*Back to the last macro in the list*/
			while (current->next != NULL) {
				current = current->next;
			}
			i = 0;
			while (line[i] == ' ' || line[i] == '\t') {
				i++;
			}
			/*Printing the contents of the macro in the appropriate place*/
			if (line[i] != ';' && line[i] != '\n' && line[i] != '\0')
				strcat(current->data.content, line + i);
		}
	}
	rewind(input_file);
	i = 0;
	while (fgets(line, sizeof(line), input_file)) {
		i = 0;
		while (line[i] == '\t' || line[i] == ' ') {
			i++;
		}
		/*Skip comments and empty lines*/
		if (line[i] == ';' || line[i] == '\n') {
			i = 0;
			continue;
		}
		current = macros_list;
		found_macro = 0;
		/*Checks whether the line matches a macro name from the list*/
		while (current != NULL) {
			if (strncmp(line, current->data.name, strlen(current->data.name)) == 0) {
				for (i = strlen(current->data.name); i < strlen(line); i++) {
					if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0') {
						printf("The macro is set in an incorrect format: %s\n", line);
						fclose(input_file);
						fclose(output_file);
						RemoveMacrosList(macros_list);
						remove(output_file_name);
						error = 1;
						return;
					}
				}
				found_macro = 1;
				break;
			}
			current = current->next;
		}
		/*Print the contents of the found macro to a file*/
		if (found_macro == 1) {
			fprintf(output_file, "%s", current->data.content);
		}
		else {
			/*Skip the definition lines*/
			if (strstr(line, "mcro ") == line) {
				macroRun = 1;
				continue;
			}
			if (strstr(line, "endmcro") == line) {
				macroRun = 0;
				continue;
			}
			if (macroRun == 1) {
				continue;
			}
			fprintf(output_file, "%s", line);
		}
	}
	fclose(input_file);
	fclose(output_file);
	RemoveMacrosList(macros_list);
}
