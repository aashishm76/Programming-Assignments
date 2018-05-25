// Aashish Madamanchi
// aa109958

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ListyString.h"

// Create new nodes using this helper function
ListyNode *createNode(char letter)
{
	ListyNode *newNode = malloc(sizeof(ListyNode));
	newNode->data = letter;
	newNode->next = NULL;

	return newNode;
}

int processInputFile(char *filename)
{
	char listyEntry[1024];
	char buffer[1024];
	char bufferKEY[2];

	FILE *inputFile = NULL;
	inputFile = fopen(filename, "r");

	// Check if file passed in failed to open
	if (inputFile == NULL)
		return 1;

	// Scan in the first line which becomes our listy string
	fscanf(inputFile, "%s", listyEntry);

	// Create the listyString using the above value
	ListyString *listy = createListyString(listyEntry);

	while(fscanf(inputFile, "%s", buffer) != EOF)
	{
		// printf("START LOOP\n");

		switch (buffer[0])
		{
			case '@':
				// printf("ENTERED @\n");
				fscanf(inputFile, "%s %s", bufferKEY, buffer);
				replaceChar(listy, bufferKEY[0], buffer);
				break;

			case '+':
				// printf("ENTERED @\n");
				fscanf(inputFile, "%s", buffer);
				listyCat(listy, buffer);
				break;

			case '-':
				// printf("ENTERED -\n");
				fscanf(inputFile, "%s", bufferKEY);
				replaceChar(listy, bufferKEY[0], NULL);
				break;

			case '~':
				// printf("ENTERED ~\n");
				reverseListyString(listy);
				break;

			case '?':
				// printf("ENTERED ?\n");
				printf("%d\n", listyLength(listy));
				break;

			case '!':
				// printf("ENTERED !\n");
				printListyString(listy);
				break;
		}

	}

	// If all passes return 0
	return 0;
}

ListyString *createListyString(char *str)
{
	int i;
	ListyNode *temp;

	// Check if str passed is null or "" and if it is set length to 0 and head pointer in ListyString struct to NULL
	if (str == NULL || str[0] == '\0')
	{
		// Malloc space for listy string struct
		ListyString *listy = malloc(sizeof(ListyString));
		listy->head = NULL;
		listy->length = 0;
		return listy;
	}

	// Create the ListyString struct with a listy pointer
	ListyString *listy = malloc(sizeof(ListyString));

	// Set the ListyString length member to the length of the string passed in the parameter
	listy->length =  strlen(str);

	// Initialize the first node (aka the head node)
	listy->head = createNode(str[0]);

	// Set temp = head
	temp = listy->head;

	// Loop thru and create nodes as needed till you reach the NULL terminator for the string
	for(i=1; i < listy->length; i++)
	{
		temp->next = createNode(str[i]);
		temp = temp->next;
	}

	return listy;
}

ListyString *destroyListyString(ListyString *listy)
{
	if (listy == NULL || listy->head == NULL)
		return NULL;

	ListyNode *deleteTemp, *loopTemp;

	// Use a placeholder pointer to loop thru linked list
	deleteTemp = listy->head;
	loopTemp = listy->head;

	while (loopTemp != NULL)
	{
		deleteTemp = loopTemp->next;
		free(loopTemp);
		loopTemp = deleteTemp;
	}
	return NULL;
}

ListyString *cloneListyString(ListyString *listy)
{
	ListyNode *temp, *copyTemp;

	// Check if listy is NULL
	if (listy == NULL)
		return NULL;

	// Check if listy->head is null
	if (listy->head == NULL)
	{
		// Malloc out space for a new ListyString Struct and set head to NULL and length to 0
		ListyString *listyNew = malloc(sizeof(ListyString));
		listyNew->head = NULL;
		listyNew->length = 0;
		return listyNew;
	}

	// Otherwise malloc out space for a copied ListyString struct
	ListyString *listyCopy = malloc(sizeof(ListyString));

	// Set the length member of the copy ListyString struct equal to the value of length in original struct
	listyCopy->length = listy->length;

	temp = listy->head;
	listyCopy->head = createNode(temp->data);

	copyTemp = listyCopy->head;
	temp = temp->next;

	while(temp->next = NULL)
	{
		copyTemp->next = createNode(temp->data);
		copyTemp = copyTemp->next;
		temp = temp->next;
	}
}

void replaceChar(ListyString *listy, char key, char *str)
{
	ListyNode *current, *prev = NULL, *temp, *newReplaceNode;
	int i, replacelen;

	// Listy Checks
	if (listy == NULL || listy->head == NULL)
		return;

	if (str != NULL)
		replacelen = strlen(str);

	// Set current = listy->head
	current = listy->head;

	while (current != NULL)
	{
		if (current->data == key)
		{
			// printf("TRIGGERED WHEN CURRENT->DATA = KEY\n");
			if (str == NULL || str[0] == '\0')
			{
				// printf("PASSED IN EMPTY STRING\n");
				if (prev == NULL)
				{
					temp = current;
					current = current->next;
					listy->head = current;
					free(temp);
				}
				else if (current->next == NULL)
				{
					// printf("IF NEXT NODE DATA = KEY TRIGGER THIS LOOP\n");

					temp = current;
					prev->next = current->next;
					current = current->next;
					free(temp);
					listy->length--;
				}
				else
				{
					// printf("Do everything else\n");
					temp = current;
					prev->next = temp->next;
					current = prev->next;
					free(temp);
					listy->length--;
				}
			}
			else
			{
				// printf("TRIGGERED WHEN STR ISNT EMPTY OR NULL\n");
				current->data = str[0];
				for (i = 1; i < replacelen; i++)
				{
					newReplaceNode = createNode(str[i]);
					newReplaceNode->next = current->next;
					current->next = newReplaceNode;
					current = current->next;
					listy->length++;
				}

				prev = current;
				current = current->next;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void reverseListyString(ListyString *listy)
{
	ListyNode *current, *tempHold, *prev;

	if (listy == NULL || listy->head == NULL)
		return;

	current = listy->head;
	prev = NULL;

	while (current != NULL)
	{
		tempHold = current->next;
		current->next = prev;
		prev = current;
		current = tempHold;
	}
	listy->head = prev;
}

ListyString *listyCat(ListyString *listy, char *str)
{
	ListyNode *catTemp;
	int catStrLen, i;

	// Check if the str is NULL or empty and if so return an unchanged listy
	if (str == NULL || str[0] == '\0')
		return listy;

	catStrLen = strlen(str);
	// Check if listy is NULL
	if (listy == NULL)
	{
		// Is the string empty?
		if (str[0] == '\0')
		{
			listy->head = createNode(str[0]);
			catTemp = listy->head;
			for (i = 1; i < catStrLen; i++)
			{
				catTemp->next = createNode(str[i]);
				catTemp = catTemp->next;
			}
			return listy;
		}
		// Is the string NULL?
		else if (str == NULL)
			return NULL;
	}

	else if (listy->head == NULL)
	{
		listy->head = createNode(str[0]);
		catTemp = listy->head;
		for (i=1; i < catStrLen; i++)
		{
			catTemp->next = createNode(str[i]);
			catTemp = catTemp->next;
		}
		listy->length = catStrLen;
		return listy;
	}
	// Loop to end of the linked list and cat on the string
	catTemp = listy->head;
	while (catTemp->next != NULL)
	{
		catTemp = catTemp->next;
	}
	for (i=0; i < catStrLen; i++)
	{
		catTemp->next = createNode(str[i]);
		catTemp = catTemp->next;
	}
	listy->length = listy->length + catStrLen;
	return listy;
}

int listyCmp(ListyString *listy1, ListyString *listy2)
{
	ListyNode *tempListy1, *tempListy2;
	int len1, len2;

	//Check if listy1 or listy2 pointers are the same
	if (listy1 == listy2)
		return 0;

	if (listy1 == NULL || listy2 == NULL)
		return 1;

	if (listy1->head == listy2->head)
		return 0;

	len1 = listy1->length;
	len2 = listy2->length;

	// Checks if the strings being passed are the same length otherwise we know they arent gonna be the same
	if (len1 != len2)
		return 1;

	tempListy1 = listy1->head;
	tempListy2 = listy2->head;

	while (tempListy1->data == tempListy2->data)
	{
		if (tempListy1->next == NULL || tempListy2->next == NULL)
			return 0;
		tempListy1 = tempListy1->next;
		tempListy2 = tempListy2->next;
	}
	return 1;
}

int listyLength(ListyString *listy)
{
	if (listy == NULL)
		return -1;
	else if (listy->head == NULL)
		return 0;

	return listy->length;
}

void printListyString(ListyString *listy)
{
	if (listy == NULL || listy->length == 0 || listy->head == NULL)
	{
		printf("(empty string)\n");
		return;
	}

	ListyNode *temp = listy->head;

	while (temp != NULL)
	{
		printf("%c", temp->data);
		temp = temp->next;
	}
	printf("\n");
}

double difficultyRating(void)
{
	return 5;
}

double hoursSpent(void)
{
	return 45;
}

int main(int argc, char **argv)
{
	processInputFile(argv[1]);
	return 0;
}
