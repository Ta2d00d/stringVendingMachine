/*
Author: Benjamin Ellsworth
 */

/*
To run program after compiling use ./a.out input.txt
input.txt contains list of words seperated by spaces and ending with a -
Follow instructions displayed when program runs to pick strings from a vending machine
Letters(A-Z, a-z) are not case sensitive
Strings are picked using the coordinates displayed beneath them
Multiple strings can be picked from one page by seperating selections with a space(ex. A1 A2 B3)
Navigate pages using < and >
Use P or W and select punctuation to display selected strings as a sentence
W will also write sentence to a file of your choice
If punctuation is not selected or invalid punctuation is entered a period will be used
Program will continue to run until X is entered
Sentence will reset each time P or W are entered
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sQue.h" //header file contains que and related functions

#define clearLines 99
#define numRows 6
#define numCols 3
#define wordLen 12
#define inputLen 2

FILE* importFile(char *argv);
int setPages(FILE* input);
void fillArr(char strs[][numRows][numCols][wordLen], int numPages, FILE* input);
void printMachine(char strs[][numRows][numCols][wordLen], int numPages, int currentPage);
void clearScreen();
void getUserInput(char userInput[inputLen]);
void processSelection(char userInput[inputLen], char strs[][numRows][numCols][wordLen], int numPages, int *page);
void vend(char userInput[inputLen], char strs[][numRows][numCols][wordLen], int currentPage);
void previousPage(int numPages, int *page);
void nextPage(int numPages, int *page);
void genRandom(char userInput[inputLen], int numPages, int *page);
void printSen();
void out();

int main(int argc, char *argv[])
{
	int numPages;
	int currentPage = 0;
	char userInput[inputLen];
	FILE* inputFile;

	if(argc != 2)
	{
		printf("File required\nUsage: ./a.out input.txt\n");
		exit(1);
	}

	//open file
	inputFile = importFile(argv[1]);

	//count words in file and set number of pages
	numPages = setPages(inputFile);
	fclose(inputFile);
	inputFile = importFile(argv[1]);

	//fill 4d array
	char strs[numPages][numRows][numCols][wordLen]; //initialize 4d array [page][row][col][string]
	fillArr(strs, numPages, inputFile);

	//close file
	fclose(inputFile);

	//process input and repeat until user exits
	while(userInput[0] != 'X')
	{
		//refresh display
		if(userInput[0] != 'P' && userInput[0] != 'W')
			printMachine(strs, numPages, currentPage);
		//get next input
		getUserInput(userInput);
		//process input //pass currentPage by refrence for use in page movement functions
		processSelection(userInput, strs, numPages, &currentPage);
	}

	return 0;
}

FILE* importFile(char *argv)
{
	FILE* input;
	input = fopen(argv, "r");
	if(input == NULL)
	{
		printf("Unable to open input file");
		exit(1);
	}

	return input;
}

int setPages(FILE* input)
{
	//counts words in input file and returns number of pages to be created
	int pageNum = 0;
	int count = 0;
	char word[12];

	fscanf(input, "%s", word);
	while(word[0] != '-')
	{
		count++;
		fscanf(input, "%s", word);
	}
	pageNum = count / (numRows * numCols);
	if(count % (numRows * numCols) != 0)
		pageNum++;
	return pageNum;
}

void fillArr(char strs[][numRows][numCols][wordLen], int numPages, FILE* input)
{
	//loops for filling array
	int page, row, col, str, len;
	char word[wordLen];

	for(page = 0; page < numPages; page++)
	{

		for(row = 0; row < numRows; row++)
		{

			for(col = 0; col < numCols; col++)
			{
				fscanf(input, "%s", word);
				len = strlen(word);
				word[len] = '\0';

				for(str = 0; str < len; str++)
				{
					if(word[str] != '-')
					{
						strs[page][row][col][str] = word[str];
					}
					else
					{
						strs[page][row][col][str] = ' ';
					}
				}
				strs[page][row][col][len] = '\0';
			}

		}

	}
	return;
}

void printMachine(char strs[][numRows][numCols][wordLen], int numPages, int currentPage)
{
	//calls clearScreen() and prints display
	int row, col, str, len;

	clearScreen();
	printf("-------------------------------------------------\n");
	printf("|                                               |\n");
	printf("|                String-O-Matic                 |\n");
	printf("|                     v 1.0                     |\n");
	printf("|  ------------------------------------------   |\n");
	printf("|  |                 page %2d                |   |\n", currentPage + 1);
	printf("|  |                                        |   |\n");

	for(row = 0; row < numRows; row++)
	{
		printf("|  |");
		for(col = 0; col < numCols; col++)
		{
			printf("%12s ", strs[currentPage][row][col]);
		}
		printf(" |   |\n");
		printf("|  |");

		for(col = 0; col < numCols; col++)
		{
			printf("%11c%c", row + 'A', col + '1');
		}
		printf("    |   |\n");
		printf("|  |                                        |   |\n");

	}
	printf("|  ------------------------------------------   |\n");
	printf("|                                               |\n");
	printf("|      ---------------------------------        |\n");
	printf("|      |                               |        |\n");
	if(inLine() != NULL)
	{
		printf("|      | %12s                  |        |\n", endLine());
	}
	else
	{
		printf("|      |                               |        |\n");
	}
	printf("|      ---------------------------------        |\n");
	printf("|                                               |\n");
	printf("-------------------------------------------------\n");
	printf("\nSelect a string (ex. A2) or R for random\n");
	printf("< or > to change page\n");
	printf("P to print sentence or W to write to file\n");
	printf("X to exit\n");

	return;
}

void clearScreen()
{
	//prints number of defined new lines to create illusion of clear screen
	int i;
	for(i = 0; i < clearLines; i++)
	{
		printf("\n");
	}

	return;
}

void getUserInput(char userInput[inputLen])
{
	//stores user input
	fgets(userInput, inputLen + 2, stdin);
	userInput[inputLen] = '\0';

	return;
}

void processSelection(char userInput[inputLen], char strs[][numRows][numCols][wordLen], int numPages, int *page)
{
	//change any lower case chars to upper case
	if(userInput[0] >= 'a' && userInput[0] <= 'z')
		userInput[0] = userInput[0] - 32;

	//switch for user input
	switch(userInput[0])
	{
		case '<':
			previousPage(numPages, page);
			break;

		case '>':
			nextPage(numPages, page);
			break;

		case 'R':
			genRandom(userInput, numPages, page);
			processSelection(userInput, strs, numPages, page);
			break;

		case 'P':
			if(inLine() != NULL)
				printSen();
			break;

		case 'W':
			out();
			break;

		case 'X':
			exit(1);

		default:
			if(userInput[0] - 'A' <= numRows && userInput[1] - '0' <= numCols)
			{
				//push string to que
				vend(userInput, strs, *page);
			}
			else
			{
				printf("Invalid selection, please try again\n");
				getUserInput(userInput);
				processSelection(userInput, strs, numPages, page);
				return;
			}
	}
	return;
}

void vend(char userInput[inputLen], char strs[][numRows][numCols][wordLen], int currentPage)
{
	//change chars to ints for array navigation
	int letter = userInput[0] - 'A';
	int num = userInput[1] - '1';
	//push string to que
	enqueue(strs[currentPage][letter][num]);
	return;
}

void previousPage(int numPages, int *page)
{
	//change current page to previous page by refrence
	if(*page != 0)
		--*page;
	return;
}

void nextPage(int numPages, int *page)
{
	//change current page to next page by refrence
	if(*page != numPages - 1)
		++*page;
	return;
}

void genRandom(char userInput[inputLen], int numPages, int *page)
{
	//generates random page and selection
	srand(time(NULL));
	*page = rand() % numPages;
	userInput[0] = (rand() % numRows) + 'A';
	userInput[1] = (rand() % numCols) + '1';
}

void printSen()
{
	int i;
	char first[wordLen];
	char punc;

	printf("Choose punctuation(.!?): ");
	scanf("%c", &punc);
	getchar(); //eats junk
	if(punc != '.' && punc != '!' && punc !='?')
		punc = '.';

	//make a copy of first word in que
	for(i = 0; i < wordLen; i++)
	{
		first[i] = inLine()[i];
	}
	first[0] = first[0] - 32; //capitalize first letter in sentence
	printf("%s", first); //print first word
	dequeue(); // remove first word from que

	while(inLine() != NULL)
	{
		printf(" %s", dequeue()); // print the rest of the words in que
	}
	printf("%c\n", punc); // print punctuation
	return;
}

void out()
{
	//same as printSen with file output added
	FILE *outputFile;
	char outName[21];
	int len;
	int i;
	char first[wordLen];
	char punc;

	printf("Choose punctuation(.!?): ");
	scanf("%c", &punc);
	getchar(); //eats junk
	if(punc != '.' && punc != '!' && punc !='?')
		punc = '.';

	printf("Enter a file to write to:");
	fgets(outName, 20 , stdin);
	len = strlen(outName) - 1;
	outName[len] = '\0';

	outputFile = fopen(outName, "w");

	//make a copy of the first word in que
	for(i = 0; i < wordLen; i++)
	{
		first[i] = inLine()[i];
	}
	first[0] = first[0] - 32; //capitalize first letter in sentence
	printf("%s", first); //print first word
	fprintf(outputFile, "%s", first);
	dequeue(); // remove first word from que

	while(inLine() != NULL)
	{
		fprintf(outputFile, " %s", inLine()); // output the rest of the words in que
		printf(" %s", dequeue()); // print the rest of the words in que
	}
	fprintf(outputFile, "%c\n", punc); // print punctuation
	printf("%c\n", punc); // print punctuation

	fclose(outputFile);
	return;
}
