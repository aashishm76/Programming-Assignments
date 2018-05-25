// Sean Szumlanski
// COP 3502, Spring 2018

// ==================
// Ohce: UnitTest07.c
// ==================
// This test case is a simple boundary check to make sure the difficultyRating()
// function is implemented correctly.
//
// For instructions on compiling your program with this source code, please see
// the assignment PDF.


#include <stdio.h>

double difficultyRating(void);

int unit_test(void)
{
	if (difficultyRating() < 1.0 || difficultyRating() > 5.0)
		printf("fail whale :(\n");
	else
		printf("Hooray!\n");

	return 0;
}
