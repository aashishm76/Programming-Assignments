// Sean Szumlanski
// COP 3502, Spring 2018

// ==================
// Ohce: UnitTest10.c
// ==================
// A simple test case in which we call your main function (which will have been
// renamed by Ohce.h if you set everything up correctly according to the
// instructions in the assignment PDF).


#include <stdio.h>
#include <stdlib.h>

int __hide_ohce_main__(int argc, char **argv);

int unit_test(void)
{
	int retval;
	char **array;
	array = malloc(sizeof(char *) * 2);
	array[0] = "./a.out";
	array[1] = "Echo";

	retval = __hide_ohce_main__(2, array);

	free(array);
	return retval;
}
