#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */

void print_memory(char* Physical_Memory, int frames_per_line, int total_frames)
{
	
	for (int i = 0; i < frames_per_line; i++)
	{
		printf("-");
	}
	for (int i = 0; i < total_frames; i++)
	{
		if ((i%frames_per_line) == 0)
		{
			printf("\n%c", Physical_Memory[i]);
		}
		else
		{
			printf("%c", Physical_Memory[i]);
		}
	} 
	printf("\n");
	for (int i = 0; i < frames_per_line; i++)
	{
		printf("-");
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr, "ERROR: Incorrect number of arguments recieved\n");
		return EXIT_FAILURE;
	}
	if (atoi(argv[1]) == 0)
	{
		fprintf(stderr, "ERROR: %s should be the number of frames per line\n", argv[1]);
		return EXIT_FAILURE;
	}
	if (atoi(argv[2]) == 0)
	{
		fprintf(stderr, "ERROR: %s should be the number of frames in the memory\n", argv[2]);
		return EXIT_FAILURE;
	}
	if (atoi(argv[4]) == 0)
	{
		fprintf(stderr, "ERROR: %s should be the time to move a frame\n", argv[4]);
		return EXIT_FAILURE;
	}

	/* The first command-line argument specifies the number of frames to show on a line */
	int frames_per_line = atoi(argv[1]);
	/* The second command-line argument specifies the size of the memory */
	int total_frames = atoi(argv[2]);
	/* The third argument specifies the name of the input file to read infor your simulation. */
	const char* input_file = argv[3];
	/* The fourth argument is the time, in milliseconds, that it takes to move one frame of memory during defragmentation.*/
	int t_memmove = atoi(argv[4]);
	
	char* Physical_Memory = calloc(total_frames, sizeof(char*));
	for (int i = 0; i < total_frames; i++)
	{
		Physical_Memory[i] = '.';
	}
	#if DEBUG_MODE
	print_memory(Physical_Memory, frames_per_line, total_frames);
	#endif
	
	return EXIT_SUCCESS;
}
