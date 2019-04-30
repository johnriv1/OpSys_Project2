#include <stdio.h>  /* printf */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */
#include <string.h>

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

typedef struct
{
	char process_id;
	int p_mem;
	int arrival_time;
	/*arrival_time_orig just so that we can reset the arrival time value when running code multiple times for different placement algorithms*/
	int arrival_time_orig; 
	int run_time;
	int rem_run_time;
	/*will denote the position of first 'letter' of process in physical memory*/
	int mem_index;
}  Process;

int compare_process(const void *p1, const void *p2)
{
	const Process *elem1 = p1;    
   const Process *elem2 = p2;
   
   if (elem1->arrival_time < elem2->arrival_time)
      return -1;
   else if (elem1->arrival_time > elem2->arrival_time)
      return 1;
   else
   {
   	if (elem1->process_id < elem2->process_id)
   	{
   		return -1;
   	}
   	else
   	{
   		return 0;
   	}
   }
}

/* must keep track for next fit */
int first_pos_after_last_process = 0;

/*sort array of pointers to struct by their order in memory*/
int sort_mem(const void *p1, const void *p2)
{
	const Process *elem1 = *(Process **)p1;
	const Process *elem2 = *(Process **)p2;
	
	if (elem1->mem_index < elem2->mem_index)
		return -1;
	else if (elem1->mem_index > elem2->mem_index)
		return 1;
	return 0;
}

/*sort array of pointers to struct by their arrival time/order they were placed in memory*/
/*
int sort_mem_arrival_time(const void *p1, const void *p2)
{
	const Process *elem1 = *(Process **)p1;
	const Process *elem2 = *(Process **)p2;
	
	if (elem1->arrival_time < elem2->arrival_time)
		return -1;
	else if (elem1->arrival_time > elem2->arrival_time)
		return 1;
	else
	{
		if (elem1->process_id < elem2->process_id)
  			return -1;
  		else
  			return 1;
	}
}
*/

void print_memory(char* Physical_Memory, int frames_per_line, int total_frames)
{
	
	for (int i = 0; i < frames_per_line; i++)
	{
		printf("=");
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
		printf("=");
	}
	printf("\n");
}

void print_processes(Process* all_processes,int all_processes_size)
{
	for (int i = 0; i < all_processes_size; i++)
	{
		printf("< pid %c, p_mem %d; arrival_time %d, run_time %d, rem_run_time %d >\n", 
			all_processes[i].process_id, all_processes[i].p_mem, all_processes[i].arrival_time, 
			all_processes[i].run_time, all_processes[i].rem_run_time);
	}
}

void defragmentation(char** Physical_Memory, Process*** Mem_Processes, int num_processes_in_memory, int* time, int t_memmove, Process** all_processes, int all_processes_size, int next_arrival_index)
{
	/*sort processes in memory such that we may copy them back in correct order*/
	qsort((*Mem_Processes), num_processes_in_memory, sizeof(Process *), sort_mem);
	int pmem_index = 0;
	int num_frames_moved = 0;
	char* frame_ids_moved_array=calloc(strlen(*Physical_Memory), sizeof(char));
	int frame_ids_moved_array_size = 0;
	for (int i = 0; i < num_processes_in_memory; i++)
	{
		for (int k = 0; k < (*Mem_Processes)[i]->p_mem; k++)
		{
			(*Physical_Memory)[pmem_index] = (*Mem_Processes)[i]->process_id;
			if (k == 0)
			{
				if (pmem_index != (*Mem_Processes)[i]->mem_index)
				{
					/*have to update global variable first_pos_after_last_process*/
					if (((*Mem_Processes)[i]->mem_index + (*Mem_Processes)[i]->p_mem) == first_pos_after_last_process)
					{
						first_pos_after_last_process = first_pos_after_last_process - ((*Mem_Processes)[i]->mem_index - pmem_index);
					}
					
					num_frames_moved += (*Mem_Processes)[i]->p_mem;
					(*Mem_Processes)[i]->mem_index = pmem_index;
					frame_ids_moved_array[frame_ids_moved_array_size] = (*Mem_Processes)[i]->process_id;
					frame_ids_moved_array_size++;
				}
			}
			pmem_index++;
		}
	}
	for (; pmem_index < strlen(*Physical_Memory); pmem_index++)
	{
		(*Physical_Memory)[pmem_index] = '.';
	}
	/*increment time, and arrival times for all upcoming processes*/
	int defrag_time_increment = num_frames_moved*t_memmove;
	(*time) += defrag_time_increment;
	//for (int i = next_arrival_index; i < all_processes_size; i++)
	for (int i = 0; i < all_processes_size; i++)
	{
		#ifdef DEBUG_MODE
		printf("+++Process %c arrival time was %d\n", (*all_processes)[i].process_id, (*all_processes)[i].arrival_time);
		#endif
		(*all_processes)[i].arrival_time += defrag_time_increment;
		#ifdef DEBUG_MODE
		printf("+++Process %c arrival time is now %d\n", (*all_processes)[i].process_id, (*all_processes)[i].arrival_time);
		#endif
	}
	printf("time %dms: Defragmentation complete (moved %d frames:", *time, num_frames_moved);
	//printf("frame_ids_moved_array_size IS %d\n", frame_ids_moved_array_size);
	for (int i = 0; i < frame_ids_moved_array_size; i++)
	{
		if (i == frame_ids_moved_array_size - 1)
		{
			printf(" %c", frame_ids_moved_array[i]);
		}
		else
		{
			printf(" %c,", frame_ids_moved_array[i]);
		}
	}
	printf(")\n");
	free(frame_ids_moved_array);
}

/* TURN ALL OF THE APPROPRIATE LETTERS BACK INTO '.' 
   AND REMOVE PROCESS FROM Mem_Processes
   AND DECREMENT num_processes_in_memory */
void remove_process(char** Physical_Memory, Process*** Mem_Processes, int* num_processes_in_memory, int* total_free_memory, Process** process_being_removed)
{
	for (int i = 0; i < strlen(*Physical_Memory); i++)
	{
		if ((*Physical_Memory)[i] == (*process_being_removed)->process_id)
		{
			(*Physical_Memory)[i] = '.';
		}
	}
	for (int i = 0; i < (*num_processes_in_memory); i++)
	{
		if (((*Mem_Processes)[i])->process_id == (*process_being_removed)->process_id)
		{
			for (int k = i; k < (*num_processes_in_memory - 1); k++)
			{
				#ifdef DEBUG_MODE
				printf(">>*Mem_Process[%d] was %c\n", k, (*Mem_Processes)[k]->process_id);
				#endif
				(*Mem_Processes)[k] = (*Mem_Processes)[k+1];
				#ifdef DEBUG_MODE
				printf(">>*Mem_Process[%d] is now %c\n", k, (*Mem_Processes)[k]->process_id);
				#endif
			}
			(*num_processes_in_memory)--;
			(*Mem_Processes) = realloc((*Mem_Processes), (*num_processes_in_memory)*sizeof(Process *));
			break;
		}
	}
	(*total_free_memory) += (*process_being_removed)->p_mem;
	(*process_being_removed) = NULL;
}

int put_process_into_memory(char** Physical_Memory, Process*** Mem_Processes, 
	int* num_processes_in_memory, char* alg, int* total_free_memory, Process* process_loading_in)
{
	if ((*total_free_memory) < process_loading_in->p_mem)
	{
		return 0;
	}
	else
	{
		int enough_room = 0;
		if (strcmp(alg, "First-Fit") == 0)
		{
			int count = 0;
			int first = 0;
			for (int i = 0; i < strlen(*Physical_Memory); i++)
			{
				if ((*Physical_Memory)[i] == '.')
				{
					if (count == 0)
					{
						first = i;
					}
					count++;
					//printf("Count is now %d\n", count);
				}
				else if ((*Physical_Memory)[i] != '.')
				{
					count = 0;
					//printf("Reset count\n");
				}
				if (count ==  process_loading_in->p_mem)
				{
					enough_room = 1;
					break;
				}
			}
			if (enough_room == 1)
			{
				process_loading_in->mem_index = first;
				for (int i = 0; i < process_loading_in->p_mem; i++)
				{
					(*Physical_Memory)[first + i] = process_loading_in->process_id;
				}
				(*total_free_memory) -= process_loading_in->p_mem;
				/* add process to Mem_Processes */
				(*num_processes_in_memory)++;
				if ((*Mem_Processes) == NULL)
				{
					(*Mem_Processes) = calloc(*num_processes_in_memory, sizeof(int*));
				}
				else
				{
					(*Mem_Processes) = realloc((*Mem_Processes), (*num_processes_in_memory)*sizeof(Process *));
				}
				(*Mem_Processes)[(*num_processes_in_memory)-1] = process_loading_in;
			}
		}
		else if (strcmp(alg, "Next-Fit") == 0)
		{
			int count = 0;
			int first = first_pos_after_last_process;
			/*loop from first_pos_after_last_process too the end*/
			for (int i = first_pos_after_last_process; i < strlen(*Physical_Memory); i++)
			{
				if ((*Physical_Memory)[i] == '.')
				{
					if (count == 0)
					{
						first = i;
					}
					count++;
					//printf("Count is now %d\n", count);
				}
				else if ((*Physical_Memory)[i] != '.')
				{
					count = 0;
					//printf("Reset count\n");
				}
				if (count ==  process_loading_in->p_mem)
				{
					enough_room = 1;
					break;
				}
			}
			/*if not enough room found, start at the top and search until we've checked entire 
				physical memory*/
			if ((first_pos_after_last_process!=0) && (enough_room == 0))
			{
				count = 0;
				first = 0;
				//for (int i = 0; i < first_pos_after_last_process; i++)
				for (int i = 0; i < strlen(*Physical_Memory); i++)
				{
					if ((*Physical_Memory)[i] == '.')
					{
						if (count == 0)
						{
							first = i;
						}
						count++;
						//printf("Count is now %d\n", count);
					}
					else if ((*Physical_Memory)[i] != '.')
					{
						count = 0;
						//printf("Reset count\n");
					}
					if (count ==  process_loading_in->p_mem)
					{
						enough_room = 1;
						break;
					}
				}
			}
			if (enough_room == 1)
			{
				process_loading_in->mem_index = first;
				for (int i = 0; i < process_loading_in->p_mem; i++)
				{
					(*Physical_Memory)[first + i] = process_loading_in->process_id;
				}
				(*total_free_memory) -= process_loading_in->p_mem;
				/* add process to Mem_Processes */
				(*num_processes_in_memory)++;
				if ((*Mem_Processes) == NULL)
				{
					(*Mem_Processes) = calloc(*num_processes_in_memory, sizeof(int*));
				}
				else
				{
					(*Mem_Processes) = realloc((*Mem_Processes), (*num_processes_in_memory)*sizeof(Process *));
				}
				(*Mem_Processes)[(*num_processes_in_memory)-1] = process_loading_in;
				
				first_pos_after_last_process = process_loading_in->mem_index + process_loading_in->p_mem;
			}
		}
		/*else if (strcmp(alg, "Best Fit") == 0)
		{
		
		}*/
		/*
		else if (strcmp(alg, "Noncontiguous") == 0)
		*/
		return enough_room;
	}
}

/* attempt to put process in Memory */
/* will be a function that take in best fit, next fit, or first fit as variables*/
/* will also take Mem_Processes, ,and probably a pointer the process being loaded in*/
/* make it return indicator about whether process loaded in or not */

void update_times(Process*** Mem_Processes, int num_processes_in_memory, int time, Process** next_finishing_process)
{
	/*this function will loop through Mem_Processes and decrease their rem_run_times by appropriate amount*/
	int least_rem_run_time;
	if ((*next_finishing_process) == NULL)
	{
		if (num_processes_in_memory >= 1)
		{
			least_rem_run_time = ((*Mem_Processes)[0]->rem_run_time) + 1;
		}
		else
		{
			/*arbitrary value since it doesnt matter if no processes are in memory*/
			least_rem_run_time = 0;
		}
	}
	else
	{
		least_rem_run_time = ((*next_finishing_process) -> run_time) - (time - (*next_finishing_process) -> arrival_time);
	}
	for (int i = 0; i < num_processes_in_memory; i++)
	{
		(*Mem_Processes)[i]->rem_run_time = (*Mem_Processes)[i]->run_time - (time - (*Mem_Processes)[i]->arrival_time);
		//printf("time_passed is %d\n", time_passed); 
		#ifdef DEBUG_MODE
		printf("----Process %c has rem_run_time %d\n", (*Mem_Processes)[i]->process_id,(*Mem_Processes)[i]->rem_run_time);
		#endif
		if (least_rem_run_time > ((*Mem_Processes)[i]->rem_run_time))
		{
			(*next_finishing_process) = (*Mem_Processes)[i];
			least_rem_run_time = ((*Mem_Processes)[i]->rem_run_time);
		}
	}
}

void main_alg(Process** all_processes_array, int all_processes_size, int frames_per_line, int total_frames, int t_memmove, char* placement_alg)
{
	Process* all_processes = *all_processes_array;
	first_pos_after_last_process = 0;
	char* Physical_Memory = calloc(total_frames, sizeof(char*));
	for (int i = 0; i < total_frames; i++)
	{
		Physical_Memory[i] = '.';
	}
	#if DEBUG_MODE
	print_memory(Physical_Memory, frames_per_line, total_frames);
	#endif	
	
	/* this index will indicate the process in all_processes that is next to arrive */
	int next_arrival_index = 0;
	/* in our update rem_time function, we'll also update the value of next_finishinf_process */
	Process* next_finishing_process = NULL;
	/* this will be updated every time a function is placed in memory,
	Process* most_recently_placed_process = NULL; */
	/* will also set up an array of pointers to (structure in memory) so that we don't have to check if every process in all_processes is in memory when updating rem_time */
	Process** Mem_Processes = NULL;
	int num_processes_in_memory = 0;
	int time = 0;
	/*the following variable will be decreased when process is loaded into memory and increased when taken out of memory*/
	/*will be needed to see if we should do defragmentation or if we should skip process*/
	int total_free_memory = total_frames;
	int test = 0;
	
	printf("time %dms: Simulator started (Contiguous -- %s)\n", time, placement_alg);
	while ((next_arrival_index < all_processes_size) || (num_processes_in_memory > 0))
	//while (test < 6)
	{
		test++;
		//printf("HELLO\n");
		//int time_passed = 0;
		if ((next_arrival_index < all_processes_size) && ((next_finishing_process == NULL) || (all_processes[next_arrival_index].arrival_time < (next_finishing_process->rem_run_time + time))))
		{
			time = all_processes[next_arrival_index].arrival_time;
			printf("time %dms: Process %c arrived (requires %d frames)\n", time,  all_processes[next_arrival_index].process_id, all_processes[next_arrival_index].p_mem);
			//time_passed = all_processes[next_arrival_index].arrival_time - time;
			//put_process_into_memory(&Physical_Memory, &Mem_Processes, &num_processes_in_memory, "Next Fit", &total_free_memory, &all_processes[next_arrival_index]);
			int loaded = put_process_into_memory(&Physical_Memory, &Mem_Processes, &num_processes_in_memory, placement_alg, &total_free_memory, &all_processes[next_arrival_index]);
			/* if it didnt load in, check if there is enough total free memory */
			/* if so, defragment and then load in process into memory */
			/* if not, skip -> advance next arrival index without loading anything into memory */
			if (loaded == 0)
			{
				if (total_free_memory >= all_processes[next_arrival_index].p_mem)
				{
					printf("time %dms: Cannot place process %c -- starting defragmentation\n", time, all_processes[next_arrival_index].process_id);
					defragmentation(&Physical_Memory, &Mem_Processes, num_processes_in_memory, &time, t_memmove, &all_processes, all_processes_size, next_arrival_index);
					//printf("time %dms: Defragmentation complete\n", time);
					//put_process_into_memory(&Physical_Memory, &Mem_Processes, &num_processes_in_memory, "Next Fit", &total_free_memory, &all_processes[next_arrival_index]);
					loaded = put_process_into_memory(&Physical_Memory, &Mem_Processes, &num_processes_in_memory, placement_alg, &total_free_memory, &all_processes[next_arrival_index]);
					printf("time %dms: Placed process %c:\n", time, all_processes[next_arrival_index].process_id);
					print_memory(Physical_Memory, frames_per_line, total_frames);
					if (loaded == 0)
					{
						fprintf(stderr, "ERROR: 'loaded' return val and 'total_free_memory' val are inconsistent\n");
					}
					next_arrival_index ++;
				}
				else
				{
					printf("time %dms: Cannot place process %c -- skipped!\n", time, all_processes[next_arrival_index].process_id);
					next_arrival_index ++;
				}
			}
			else
			{
				printf("time %dms: Placed process %c:\n", time, all_processes[next_arrival_index].process_id);
				print_memory(Physical_Memory, frames_per_line, total_frames);
				next_arrival_index ++;
			}
		}
		else if ((next_arrival_index >= all_processes_size ) || (all_processes[next_arrival_index].arrival_time >= (next_finishing_process->rem_run_time + time)))
		{
			//time_passed = next_finishing_process->rem_run_time;
			time += next_finishing_process->rem_run_time;
			printf("time %dms: Process %c removed:\n", time, next_finishing_process->process_id);
			remove_process(&Physical_Memory, &Mem_Processes, &num_processes_in_memory, &total_free_memory, &next_finishing_process);
			#ifdef DEBUG_MODE
			printf("there are %d processes in memory\n", num_processes_in_memory); 
			#endif
			print_memory(Physical_Memory, frames_per_line, total_frames);
		}
		//printf("HELLO5\n");
		update_times(&Mem_Processes, num_processes_in_memory, time, &next_finishing_process);
		//printf("HELLO6\n");
		#ifdef DEBUG_MODE
		if (num_processes_in_memory >= 1)
		{
			printf("next_finishing_process is now %c\n", next_finishing_process->process_id);
		}
		printf("\n");
		#endif
	}
	printf("time %dms: Simulator ended (Contiguous -- %s)\n", time, placement_alg);
	
	free(Physical_Memory);
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
	#if 0
	char* Physical_Memory = calloc(total_frames, sizeof(char*));
	for (int i = 0; i < total_frames; i++)
	{
		Physical_Memory[i] = '.';
	}
	#if DEBUG_MODE
	print_memory(Physical_Memory, frames_per_line, total_frames);
	#endif
	#endif
	Process* all_processes = NULL;
	int all_processes_size = 0;
	
	FILE *fp;                  /* file pointer*/
   fp = fopen(input_file, "r");
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	while ((nread = getline(&line, &len, fp)) != -1) 
	{
		#if DEBUG_MODE
		printf("Retrieved line of length %zu:\n", nread);
		fwrite(line, nread, 1, stdout);
		#endif
		char current_process_id;
		int current_process_mem;
		char *ptr = strtok(line, " ");
		if (ptr !=NULL)
		{
			if ((ptr)[0] == '#')
			{
				#if DEBUG_MODE
				printf("this line is a comment, so ignore\n");
				#endif
				continue;
			}
			else if ((ptr)[0] == '\n')
			{
				#if DEBUG_MODE
				printf("this line is blank, so ignore\n");
				#endif
				continue;
			}
			else
			{
				current_process_id = (ptr)[0];
				#if DEBUG_MODE
				printf("this process is process %c\n", current_process_id);
				#endif
			}
			ptr = strtok(NULL, " ");
			current_process_mem = atoi(ptr);
			#if DEBUG_MODE
			printf("this process has fixed memory %d\n", current_process_mem);
			#endif
			ptr = strtok(NULL, "/");
		}
		while (ptr != NULL)
		{
			all_processes_size ++;
			if (all_processes == NULL)
			{
				all_processes = calloc(all_processes_size, sizeof(Process));
			}
			else
			{
				all_processes = realloc(all_processes, all_processes_size * sizeof(Process));
			}
			all_processes[all_processes_size - 1].process_id = current_process_id;
			all_processes[all_processes_size - 1].p_mem = current_process_mem;
			all_processes[all_processes_size - 1].arrival_time = atoi(ptr);
			all_processes[all_processes_size - 1].arrival_time_orig = atoi(ptr);
			#if DEBUG_MODE
			printf("this process has arrival_time %d\n", all_processes[all_processes_size - 1].arrival_time);
			#endif
			ptr = strtok(NULL, " ");
			all_processes[all_processes_size - 1].run_time  = atoi(ptr);
			all_processes[all_processes_size - 1].rem_run_time  = atoi(ptr);
			#if DEBUG_MODE
			printf("this process has run_time %d\n", all_processes[all_processes_size - 1].run_time);
			#endif
			ptr = strtok(NULL, "/");
		}
	}
	#if DEBUG_MODE
	printf("\nthere are %d total processes in this simulation\n", all_processes_size);
	print_processes(all_processes,all_processes_size);
	printf("\n");
	#endif

	qsort(all_processes, all_processes_size, sizeof(Process), compare_process);
	
	#if DEBUG_MODE
	print_processes(all_processes,all_processes_size);
	printf("\n");
	#endif
	
	main_alg(&all_processes, all_processes_size, frames_per_line, total_frames, t_memmove, "First-Fit");
	/*reset necessary values; call this before ever recall of main_alg*/
	for (int i = 0; i < all_processes_size; i++)
	{
		all_processes[i].rem_run_time = all_processes[i].run_time;
		all_processes[i].arrival_time = all_processes[i].arrival_time_orig;
	}
	printf("\n");
	main_alg(&all_processes, all_processes_size, frames_per_line, total_frames, t_memmove, "Next-Fit");
	
	free(all_processes);
	free(line);
	fclose(fp);
	return EXIT_SUCCESS;
}






















