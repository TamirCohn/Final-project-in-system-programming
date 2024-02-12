#include <string.h>
#include <ctype.h>
#include "CutPaste.h"
#define MAX_LINE_LENGTH 81
/**
 * Removes the label from a line.
 *
 * @param line A pointer to the input line.
 * @return A pointer to the line after the label, or the original line if no label is found.
 */
char* removeLabel(char* line) {
	char* colon;
	colon = strstr(line, ":");
	if (colon != NULL) {
		while (*(colon + 1) == ' ' && *(colon + 1) == '\t') {
			colon++;
		}
		return colon + 1;
	}
	return line;
}
/**
 * Extracts the first word from a line.
 *
 * @param line A pointer to the input line.
 * @return A pointer to the extracted first word.
 */
char* firstWord(const char* line) {
	int i;
	static char firstW[MAX_LINE_LENGTH];
	for (i = 0; i < MAX_LINE_LENGTH; i++)
		firstW[i] = '\0';
	i = 0;
	while (*line) {
		if (*line == ' ' || *line == '\t') {
			line++;
			continue;
		}
		break;
	}
	while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') {
		firstW[i++] = *line;
		line++;
	}
	firstW[i] = '\0';

	return firstW;
}
/**
 * Extracts the first operand from a line of code.
 *
 * @param line A pointer to the input line of code.
 * @return A pointer to the extracted first operand, or NULL if no operand is found.
 */
char* getFirstOpr(const char* line) {
	static char firstOpr[MAX_LINE_LENGTH];
	int i, countTabs;
	for (i = 0; i < MAX_LINE_LENGTH; i++)
		firstOpr[i] = '\0';
	countTabs = 0;
	i = 0;
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	while (*line != ' ' && *line != '\t' && *line != '\n' && *line) {
		line++;
	}
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	while (*line != ' ' && *line != ',' && *line != '\n' && *line != '\t' && *line != '\0') {
		firstOpr[i++] = *line;
		countTabs++;
		line++;
	}
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	if (*line != ',' && *line != '\n' && *line != '\0')
		return "undentify operand";
	if (countTabs == 0)
		return NULL;
	return firstOpr;
}
/**
 * Extracts the second operand from a line of code.
 *
 * @param line A pointer to the input line of code.
 * @return A pointer to the extracted second operand, or NULL if no operand is found.
 */
char* getSecondOpr(const char* line) {
	int i, countTabs;
	static char secondOpr[MAX_LINE_LENGTH];
	for (i = 0; i < MAX_LINE_LENGTH; i++)
		secondOpr[i] = '\0';
	countTabs = 0;
	i = 0;
	while (*line) {
		if (*line != ',') {
			line++;
			continue;
		}
		line++;
		break;
	}
	while (*line) {
		if (*line == ' ' || *line == '\t') {
			line++;
			continue;
		}
		break;
	}

	while (*line != '\0' && *line != ' ' && *line != '\n' && *line != '\t') {
		secondOpr[i++] = *line;
		line++;
		countTabs++;
	}
	while (*line == ' ' || *line == '\t') {
		line++;
	}
	if (countTabs == 0)
		return NULL;
	if (*line != '\n' && *line != '\0')
		return "undentify operand";
	return secondOpr;
}

