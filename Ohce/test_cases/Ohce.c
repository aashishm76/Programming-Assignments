// Aashish Madamanchi
// aa109958

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ohce.h"

double difficultyRating(void)
{
  double diffRate = 4;
}

double hoursSpent(void)
{
  double hours = 25;
}

int main(int argc, char **argv)
{
  /* All the variables */
  int counter, charIndex, charIndex2, echoCheck, i;
  int strIndex = 1; // Set the strIndex to 1 because we want to exclude the ./a.out comman line input
  int flag = 0;

  counter = argc; // counter = # of command line entries
  echoCheck = argc-1; // echoCheck = # of the command line entries - 1

  /* Reverse the strings */
  while (strIndex != counter) // While strIndex does not equal counter this loop will run
  {
    charIndex = strlen(argv[strIndex]); // charIndex = # of characters in each command line argument enetered
    charIndex2 = charIndex; // charIndex2 = # of characters in each command line argument enetered
    while (charIndex !=0) // while charIndex does not equal 0 run this loop
    {
      charIndex --; // Decrement the index of the array of characters so you skip the null terminator at the end of an array of characters
      printf("%c", argv[strIndex][charIndex]); // Print each character in reverse
    }

    /* Loop to search for the word echo */
    for (i=0; i < charIndex2-3; i++) // For i less than charIndex - 3 (this is because the word echo is technically index 3 in an array) run this loop
      if ((argv[strIndex][i] == 'e') || (argv[strIndex][i] == 'E'))
        if ((argv[strIndex][i+1] == 'c') || (argv[strIndex][i+1] == 'C'))
          if ((argv[strIndex][i+2] == 'h') || (argv[strIndex][i+2] == 'H'))
            if ((argv[strIndex][i+3] == 'o') || (argv[strIndex][i+3] == 'O'))
              flag = 1;

    strIndex++; // Go to the next string to reverse
    (strIndex == counter) ? printf("\n") : printf(" "); // Check if you have reached the last string. When strIndex = counter this will print out either a new line character if not itll print a space
  }

  /* Echo Check */
  if (flag == 1)
  {
    printf("Echo, echo, echo!\n");
  }

  return 0;
}
