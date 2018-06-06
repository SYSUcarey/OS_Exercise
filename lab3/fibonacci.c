#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *sequence;
void* runner(void *param);

int main(int argc, char *argv[]) {
	pthread_t tid;
	pthread_attr_t attr;

	if(argc != 2) {
		fprintf(stderr, "Error: Invalid parameter!\n");
		return -1;
	}
	int  param = atoi(argv[1]);
	if(param <= 0 || param > 40) {
		fprintf(stderr, "Error: It need a postive integer <= 40\n");
		return -1;
	}

	/* get the default attributes */
	pthread_attr_init(&attr);
	/* create the thread */
	pthread_create(&tid, &attr, runner, argv[1]);
	/* now wait for the thread to exit */
	pthread_join(tid, NULL);

	/* show the fibonacci sequence */
	printf("The fibonacci sequence are: \n");
	for (int i = 0; i < param; i++) {
		printf("%d ", sequence[i]);
	}
	printf("\n");
}

void *runner(void *param) {
	int size = atoi(param);
	sequence = (int *)malloc(sizeof(int) * size);

	if (size <= 1) {
		sequence[0] = 0;
		pthread_exit(0);
	}
	else if (size == 2) {
		sequence[0] = 0;
		sequence[1] = 1;
		pthread_exit(0);
	}
	else {
		sequence[0] = 0;
		sequence[1] = 1;
		for (int i = 2; i < size; i++) {
			sequence[i] = sequence[i-1] + sequence[i-2];
		}
		pthread_exit(0);
	}
	
}