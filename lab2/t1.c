#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_SEQUENCE 10

typedef struct 
{
	long fib_sequence[MAX_SEQUENCE];
	int sequence_size;
} shared_data;

int main(int argc, char* argv[]) {
	int seq_size;

	/* ---Check the parameter. ---*/
	if (argc != 2) {
		printf("Invalid parameter\n");
		return -1;
	}

	seq_size = atoi(argv[1]);
	if (seq_size > MAX_SEQUENCE || seq_size <= 0) {
		printf("sequence size must be < 10 AND > 0\n");
		return -1;
	}

	/**
	 * Allocate the shared memory segment.
	 * The size is the same of structure shared_data.
	 * Attach the shared memory segment into shared_memory.
	 */
	int segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR | S_IWUSR);
	shared_data* shared_memory = (shared_data*)shmat(segment_id, NULL, 0);
	
	/* ---Creat the child process--- */
	pid_t pid;
	pid = fork();

	if(pid == 0) { 
	/* Child process
	 * Create the Fibonacci in shared_data
	 */
		shared_memory -> sequence_size = seq_size;
		shared_memory -> fib_sequence[0] = 0;
		shared_memory -> fib_sequence[1] = 1;
		for (int i = 2; i < seq_size; i++) {
			shared_memory -> fib_sequence[i] = shared_memory -> fib_sequence[i-1] + shared_memory -> fib_sequence[i-2];
		}
	}
	else if(pid > 0){
		/*
		 * Parent process
		 * Show the fibonacci sequence
		 */
		wait();
		for(int i = 0; i < shared_memory -> sequence_size; i++) {
			printf("%d : %ld\n", i, shared_memory -> fib_sequence[i]);
		}

		//remove the shared memory segment
		shmctl(segment_id, IPC_RMID, NULL);
		return 0;
	}



}