#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */

int main(int argc, char const *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr, "ERROR: Invalid arguments\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}