// Aashish Madamanchi
// aa109958
// COP3502 - Dr. Szulmanski

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TriePrediction.h"

/* Helper Functions */

// Credit to Dr. S for this trie print helper function
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

TrieNode *createTrieNode(void)
{
	TrieNode *newTrie = calloc(1, sizeof(TrieNode));

	// Set the subtrie initially to NULL
	newTrie->subtrie = NULL;
	newTrie->count = 0;

	return newTrie;
}

void lowerHelper(char *str)
{
	int length, i;

	length = strlen(str);

	for (i = 0; i < length; i++)
	{
		str[i] = tolower(str[i]);
	}
}

int punctuationCheck(char *str)
{
	int length;
	int flag = 0;

	length = strlen(str);

	switch(str[length - 1])
	{
		case '.' : case '?' : case '!':

			flag = 0;

		default:

			flag = 1;
			break;
	}

	return flag;
}

void stripPunctuation(char *strInput)
{
	int i, j, length;
	char scrubbedInput[MAX_CHARACTERS_PER_WORD + 1];

	length = strlen(strInput);
	j = 0;

	for (i = 0; i < length; i++)
	{
		if(isalpha(strInput[i]))
		{
			scrubbedInput[j] = tolower(strInput[i]);
			j++;
		}
	}

	scrubbedInput[j] = '\0';
	strcpy(strInput, scrubbedInput);
}

void insertString(TrieNode *root, char *str)
{
	int i, index, length;
	TrieNode *wizard;

	length = strlen(str);

	// Wizard is the trieNode traversal pointer
	wizard = root;

	for(i = 0; i < length; i++)
	{
		index = str[i] - 'a';

		if(wizard->children[index] == NULL)
			wizard->children[index] = createTrieNode();

		wizard = wizard->children[index];
	}

	// Increment the count at the terminal node
	wizard->count++;
}

TrieNode *pathfinder(TrieNode *root, char *str)
{
	TrieNode *searcher = NULL;
	int i, length, index;

	// Check if str is empty or NULL
	if(str == NULL || str[0] == '\0')
		return NULL;

	// Traverse thru the trie
	searcher = root;
	length = strlen(str);

	for (i = 0; i < length; i++)
	{
		index = str[i] - 'a';

		// Check if the character exists
		if (searcher->children[index] == NULL)
		{
			// printf("LETTER NOT FOUND\n");
			return NULL;
		}
		else
		{
			// printf("LETTER FOUND...MOVING SEARCH PARTY FORWARD\n");
			searcher = searcher->children[index];
		}
	}

	// printf("Get node was called\n");

	return searcher;
}

void getFrequentWordHelper(TrieNode *root, char *tempBuffer, char *str, int a, int *countMax)
{
	// This function requires the input of the original str, a tempBuffer to hold any edits we make, and a pointer to the maxCount so pointer arithematic can be used to access the maxCount
	int i = 0;

	// Base case
	if (root == NULL)
		return;

	// // If the node we are currently on has a higher count that countMax, copy that string in
	// if (root->count > *countMax)
	// {
	// 	// Update the count to the word that has the higher count and then copy that from tempBuffer to str
	// 	*countMax = root->count;
	// 	strcpy(str, tempBuffer);
	// }

	// Terminate the string, otherwise this fails to work!
	tempBuffer[a + 1] = '\0';

	// This effectively searches the subtries as well
	for(i = 0; i < 26; i++)
	{
		// If the node we are currently on has a higher count that countMax, copy that string in
		if (root->count > *countMax)
		{
			// Update the count to the word that has the higher count and then copy that from tempBuffer to str
			*countMax = root->count;
			strcpy(str, tempBuffer);
		}

		tempBuffer[a] = i + 'a';
		getFrequentWordHelper(root->children[i], tempBuffer, str, a+1, countMax);
	}

	// Truncate the string that is the largest word with a NULL terminator
	tempBuffer[a] = '\0';
}

int countAllNodes(TrieNode *root)
{
	int i, count;
	count = 0;

	if (root == NULL)
		return 0;

	for(i = 0; i < 26; i++)
	{
		count += countAllNodes(root->children[i]);
	}

	return root->count + count;
}

/* Main Functions */

// Credit to Dr. S for this print Trie function
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

TrieNode *buildTrie(char *filename)
{
	int length, endSent = 0;
	TrieNode *root, *placeHolder = NULL;
	TrieNode *prev = NULL;
	char corpusInput[MAX_CHARACTERS_PER_WORD + 1];
	char scrubbedStr[MAX_CHARACTERS_PER_WORD + 1];

	FILE *inputFile;
	inputFile = fopen(filename, "r");

	// Create the root node
	root = createTrieNode();

	while(fscanf(inputFile, "%s", corpusInput) != EOF)
	{
	  // Scrub the strings
	  strcpy(scrubbedStr, corpusInput);

		length = strlen(corpusInput);
		if(corpusInput[length - 1] == '?' || corpusInput[length - 1] == '!' || corpusInput[length - 1] == '.')
		{
			stripPunctuation(scrubbedStr);

			// Insertion into main trie
			insertString(root, scrubbedStr);

			// Subtrie insertion using a placeholder pointer to keep track of previous word entered
			if(placeHolder == NULL)
			{
				placeHolder = pathfinder(root, scrubbedStr);
			}
			else
			{
				if(!endSent)
				{
					if(placeHolder->subtrie == NULL)
						placeHolder->subtrie = createTrieNode();

					insertString(placeHolder->subtrie, scrubbedStr);
				}

				placeHolder = pathfinder(root, scrubbedStr);
			}
			endSent = 1;
		}
		else
		{
			stripPunctuation(scrubbedStr);

			// Insertion into main trie
			insertString(root, scrubbedStr);


			// Subtrie insertion using a placeholder pointer to keep track of previous word entered
			if(placeHolder == NULL)
			{
				placeHolder = pathfinder(root, scrubbedStr);
			}
			else
			{
				if(!endSent)
				{
					if(placeHolder->subtrie == NULL)
						placeHolder->subtrie = createTrieNode();

					insertString(placeHolder->subtrie, scrubbedStr);
				}

				placeHolder = pathfinder(root, scrubbedStr);

			}
			endSent = 0;
		}
	}
	// Closing and clean up crew
	fclose(inputFile);

	return root;
}

int processInputFile(TrieNode *root, char *filename)
{
	int len, textPredictionNum, i;
	char inputArg[MAX_CHARACTERS_PER_WORD + 1];
	char tempHold[MAX_CHARACTERS_PER_WORD + 1];
	char finalWord[MAX_CHARACTERS_PER_WORD + 1];
	TrieNode *holdNode;

	// Load in file
	FILE *inputFile;
	inputFile = fopen(filename, "r");

	if(inputFile == NULL)
		return 1;

	// Start scanning in file contents
	while (fscanf(inputFile, "%s", inputArg) != EOF)
	{
		//If "!" simply print out the trie
		if (strcmp(inputArg, "!") == 0)
		{
			printTrie(root, 0);
		}
		else if (strcmp(inputArg, "@") == 0)
		{
			// Scan in the word we are going to use for text prediction
			fscanf(inputFile, "%s", inputArg);
			fscanf(inputFile, "%d", &textPredictionNum);

			printf("%s", inputArg);

			// Strip punctuation and tolower everything, then store into a temp array that we use to test
			strcpy(tempHold, inputArg);
			stripPunctuation(tempHold);

 			for (i=0; i < textPredictionNum; i++)
			{
				// Get the location of the final node
				holdNode = getNode(root, tempHold);

				// Check if the node is NULL
				if (holdNode == NULL)
					break;

				// Check if the subtrie is NULL
				if (holdNode->subtrie == NULL)
					break;

				// Now call the most frequent word function to get the most frequent word in the subtrie
				getMostFrequentWord(holdNode->subtrie, tempHold);
				printf(" %s", tempHold);
			}
			printf("\n");
		}
		else
		{
			// This is the str command

			printf("%s\n", inputArg);

			// Check if word exists at all in main trie
			if(getNode(root, inputArg) == NULL)
			{
				printf("(INVALID STRING)\n");
				continue;
			}

			// If the word does exist then check if the subtrie is empty
			holdNode = getNode(root, inputArg);

			if(holdNode->subtrie == NULL)
			{
				printf("(EMPTY)\n");
				continue;
			}

			// Go ahead and print the trie and it's subtrie contents if everything passes
			printTrie(holdNode->subtrie, 1);
		}
	}

	// Close the file and exit out freeing anything you created
}

TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	for(i = 0; i < 26; i++)
	{
		if(root->children[i] != NULL)
			destroyTrie(root->children[i]);
	}

	if(root->subtrie != NULL)
		destroyTrie(root->subtrie);

	free(root);
	return NULL;
}

TrieNode *getNode(TrieNode *root, char *str)
{
	TrieNode *searcher = NULL;
	int i, length, index;
	char scrubbedString[MAX_CHARACTERS_PER_WORD + 1];

	// Check if str is empty or NULL
	if(str == NULL || str[0] == '\0')
		return NULL;

	// If string isnt empty go ahead and scrub it just in case this function is called by itself
	strcpy(scrubbedString, str);
	stripPunctuation(scrubbedString);

	// Traverse thru the trie
	searcher = root;
	length = strlen(scrubbedString);

	for (i = 0; i < length; i++)
	{
		index = scrubbedString[i] - 'a';

		// Check if the character exists
		if (searcher->children[index] == NULL)
		{
			// printf("LETTER NOT FOUND\n");
			return NULL;
		}
		else
		{
			// printf("LETTER FOUND...MOVING SEARCH PARTY FORWARD\n");
			searcher = searcher->children[index];
		}
	}

	if(searcher->count == 0)
		return NULL;

	return searcher;
}

void getMostFrequentWord(TrieNode *root, char *str)
{
	char tempBuffer[MAX_CHARACTERS_PER_WORD + 1];
	int countMax = 0;

	// Check if root is NULL, if it is we return an empty string
	if (root == NULL)
	{
		strcpy(str, "");
		return;
	}

	// If the str passed in is NULL simply return from the function
	if (str == NULL)
		return;

	// If a built trie is passed into this function we need to remove the punctuation from the word
	stripPunctuation(str);

	// Helper function is called here to do the work
	// This works by passing in root, and both tempBuffer/str so they can be searched for in each trie and subtrie
	getFrequentWordHelper(root, tempBuffer, str, 0,  &countMax);

	//If the helper doesnt find any words then the count remains zero and the str would be empty
	if (countMax == 0)
		strcpy(str, "");

}

int containsWord(TrieNode *root, char *str)
{
	TrieNode *hunter = NULL;
	int i, length, index;

	// Traverse through the trie
	hunter = root;
	length = strlen(str);

	// Loop through and check the trie for the letter and verify if the count exists
	for (i = 0; i < length; i++)
	{
		index = str[i] - 'a';

		// Check if the first character exists, if it doesnt then the word isn't in the trie
		if (hunter->children[index] == NULL)
			return 0;
		else
		{
			// Loop through each character moving the hunter forward
			hunter = hunter->children[index];
		}
	}

	// If the count at that node is zero that means this word doesn't exist
	if (hunter->count == 0)
		return 0;

	return 1;
}

int prefixCount(TrieNode *root, char *str)
{
	int i, count, length, index;
	TrieNode *temp;

	// NULL checks
	if (root == NULL)
		return 0;

	if (str == NULL)
		return 0;

	length = strlen(str);
	temp = root;

	for(i = 0; i < length; i++)
	{
		index = str[i] - 'a';

		temp = temp->children[index];

		if (temp == NULL)
			return 0;
	}

	 return countAllNodes(temp);
}

double difficultyRating(void)
{
	return 5;
}

double hoursSpent(void)
{
	return 100;
}

int main(int argc, char **argv)
{
	TrieNode *root;
	char *corpusFile = argv[1];
	char *inputFile = argv[2];

	// Build the trie first
	root = buildTrie(corpusFile);

	// Process the input file
	processInputFile(root, inputFile);

	// Now destory the trie
	root = destroyTrie(root);

	// Clean up crew
	free(root);

	return 0;
}
