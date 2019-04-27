#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */

/*
	as file parsed, put event "intervals" into array. Sort by arrival times (if equal, alphabetical order)
		each object in array has variables: arrival_time, p_mem, run_time, rem_run_time, process_id
		
		as they arrive, will put them in memory if arrival_time < current_time
		
		after every advancment of time, increment rem_run_time
			keep track of next process that will finish (pointer)
			
	If next process arrival time < next process finish time
		attempt to load in next process
		else if next process arrival time > next process finish time
			take out process from physical memory
		else if next process arrival time = next process finish time
			take out process
			attempt to load in next process
		
	"attempt to load in next process" fnction
		will take as input memory array, process being loaded in, and alg("best fit", "next fit", ect.)
		
		first_fit: loop through memory starting from i = 0
			when '.' is reached, record i. keep looping until != '.' is reached. 
			If (i - recorded_i) >= process.p_mem
			{
				for (int k = recordedi, k < process.p_mem; k++)
				{
					memory[k] = process.id
				}
				break;
			}
			else
			{
				keep looping through until end of memory is reached
				if end is reached -> defragment if process didnt have enough space but there is enough total memory
			}
			
		next_fit: keep index of last loaded message recorded
			can also loop back to beginning of memory
			
		best_fit: record all indexes in momory where process can fit, compare and choose
		
	defragment function:
	
	removal function:
		
*/
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
