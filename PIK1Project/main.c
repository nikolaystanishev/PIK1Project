#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <stdbool.h>

/*
 * Main file for the project
 */

enum MenuOptions
{
	ReadFromFileWriteToFile = 0,
	ReadFromFileWriteToSTDOUT = 1,
	ReadFromSTDINWriteToFile = 2,
	ReadFromSTDINWriteToSTDOUT = 3
};

int menuPicker();
void validateSelectedOperation(int * selectedOperation);
void printMenu(int selectedOption, char activeOperation);
void operateStreams(FILE * inStream, FILE * outStream);
int isLineEmpty(char * line);
int getNumberOfOperators(char * line);
void * numberToString(int number, char * string);
void readUserInput(bool fromInputFile, bool fromOutputFile, FILE ** inputFile, FILE ** outputFile);

int main()
{
	SetConsoleOutputCP(1251);

	FILE ** inputFile = NULL, **outputFile = NULL;

	// pick function for the selected operation
	switch (menuPicker())
	{
	case ReadFromFileWriteToFile: readUserInput(true, true, &inputFile, &outputFile); break;
	case ReadFromFileWriteToSTDOUT: readUserInput(true, false, &inputFile, &outputFile); break;
	case ReadFromSTDINWriteToFile: readUserInput(false, true, &inputFile, &outputFile); break;
	case ReadFromSTDINWriteToSTDOUT: readUserInput(false, false, &inputFile, &outputFile); break;
	default:
		break;
	}
	operateStreams(inputFile, outputFile);
	system("pause");
	return 0;
}

int menuPicker()
{
	int selectedOperation = 0;
	char activeOperation = '>', enterKey = 13, userClick, upwardKey = 80, downwordKey = 72;
	printMenu(selectedOperation, activeOperation);
	while (getch() != enterKey) {
		// as I want to use the arrow keys we need to take the second char too as it differentiates them
		userClick = getch();
		if (userClick == upwardKey) {
			selectedOperation++;
		}
		else if (userClick == downwordKey) {
			selectedOperation--;
		}
		validateSelectedOperation(&selectedOperation);
		printMenu(selectedOperation, activeOperation);
	}
	return selectedOperation;
}

void validateSelectedOperation(int * selectedOperation)
{
	if (*selectedOperation < 0) {
		*selectedOperation = 3;
	}
	if (*selectedOperation >= 4) {
		*selectedOperation = 0;
	}
}

void printMenu(int selectedOption, char activeOperation)
{
	system("cls");
	char * opertaions[] = { "������ �� ���� � ����� � ���� ����",
						    "������ �� ���� � ��������� �� stdout",
							"������ �� stdin � ��������� � ����",
							"������ �� stdin � �������� �� stdout" };

	for (int i = 0; i < 4; i++) {
		if (i == selectedOption) {
			printf("%c", activeOperation);
		}
		else {
			printf(" ");
		}

		printf("%s\n", opertaions[i]);
	}
}

void operateStreams(FILE * inStream, FILE * outStream)
{
	char lineBuffer[128];
	memset(lineBuffer, 0, 128 * (sizeof lineBuffer[0]));
	int numberOfEmptyLines = 0, numberOfOperators = 0;
	bool noError = true;
	while (fgets(lineBuffer, sizeof(lineBuffer), inStream)) {
		numberOfEmptyLines += isLineEmpty(lineBuffer);
		numberOfOperators += getNumberOfOperators(lineBuffer);

		memset(lineBuffer, 0, 128 * (sizeof lineBuffer[0]));
	}

	char numAsString[100] = {'0', '\0'};
	fputs("���� ������ ������: ", outStream);
	fputs(numberToString(numberOfEmptyLines, numAsString), outStream);
	fputs(" ���� ���������: ", outStream);
	fputs(numberToString(numberOfOperators, numAsString), outStream);
	fputc('\n', outStream);
}

int isLineEmpty(char *line) {
	if (strstr(line, "\n") != NULL) {
		return strlen(line) <= 1;
	}
	return strlen(line) <= 0;
}

int getNumberOfOperators(char * line) {
	int matches = 0;
	char *operators[] = { "(", "[", "->", ".", "!", "+", "++", "-", "--", "&", "&&", "|", "||", "*", ">>" }; // TODO add them all
	for (int i = 0; i < 15; i++) {
		int k = 0;
		while (strstr(line + k, operators[i]) != NULL) {
			k += strlen(operators[i]);
			matches++;
		}
	}
	return matches;
}

void readUserInput(bool fromInputFile, bool fromOutputFile, FILE ** inputFile, FILE ** outputFile)
{
	char inputFileName[40], outputFileName[40];
	if (fromInputFile) {
		printf("���� �������� ����� �� ����� �� ������: ");
		scanf("%s", inputFileName);
		*inputFile = fopen(inputFileName, "r+");
	}
	else {
		*inputFile = stdin;
	}

	if (fromOutputFile) {
		printf("���� �������� ����� �� ����� �� ������ �� ��������: ");
		scanf("%s", outputFileName);
		*outputFile = fopen(outputFileName, "w+");
	}
	else {
		*outputFile = stdout;
	}
}


char * numberToString(int number, char * string) {
	if (number == 0) {
		return string;
	}

	int i = 0;
	while (number > 0) {
		char digit = '0' + number % 10;
		number /= 10;
		string[i] = digit;
		i++;
	}
	
	string[i] = '\0';

	for (int k = 0; k < strlen(string) / 2; k++) {
		char swap = string[k];
		string[k] = string[strlen(string) - 1 - k];
		string[strlen(string) - 1 - k] = swap;
	}
	return string;
}