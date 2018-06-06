#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

static sem_t mutex;			//use for changing the readcount
static sem_t mutex2;		//use for changing the writecount
static sem_t wrt;
static sem_t rd;
int readcount = 0;
int writecount = 0;

void* reader(void *param);
void* writer(void *param);

int main(void) {

	char** instruction = (char**)malloc(sizeof(char*)*10);
	for(int i = 0; i < 10; i++) {
		instruction[i] = (char*)malloc(sizeof(char)*10);
	}
	
	sem_init(&mutex,0,1);
	sem_init(&mutex2,0,1);
	sem_init(&wrt,0,1);
	sem_init(&rd,0,1);

	//need to create 5 pthread
	pthread_t tid[5];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (int i = 0; i < 5; i++) {
		//get the instruction input
		gets(instruction[i]);
	}

	//judge the instruction_thread_type and create the thread
	char thread_type;
	for (int i = 0; i < 5; i++) {
		thread_type = instruction[i][2];
		if(thread_type == 'R') {
			pthread_create(&tid[i], &attr, reader, instruction[i]);
		}
		else if(thread_type == 'W'){
			pthread_create(&tid[i], &attr, writer, instruction[i]);
		}
	}
	
	//join all the six pthread
	for (int i = 0; i < 5; i++) {
		pthread_join(tid[i],NULL);
	}

	sem_destroy(&mutex);
	sem_destroy(&wrt);
	return 0;
}

void* reader(void *param) {
	//do what a reader does
	
	char* instruction = (char*)malloc(sizeof(char)*10);
	instruction = (char*)param;

	int thread_number;
	int start;
	int time;

	thread_number = instruction[0] - '0';
	start = instruction[4] - '0';
	time = instruction[6] - '0';
	
	sleep(start);
	printf("reader %d is waiting!\n", thread_number);
	
	//if(writecount > 0) sem_wait(&rd);
	//if(writecount == 0) sem_post(&rd);
	sem_wait(&rd);
	sem_wait(&mutex);
	//sem_wait(&rd);
	readcount++;
	if (readcount == 1) {
		sem_wait(&wrt);
	}
	sem_post(&mutex);
	sem_post(&rd);

	printf("reader");
	printf("%d", thread_number);
	printf(" start reading!\n");
	sleep(time);
	printf("reader");
	printf("%d", thread_number);
	printf(" end reading!\n");

	
	sem_wait(&mutex);
	readcount--;
	if(readcount == 0) {
		sem_post(&wrt);
	}
	sem_post(&mutex);
}

void* writer(void *param) {
	//do what a writer does
	
	char* instruction = (char*)malloc(sizeof(char)*10);
	instruction = (char*)param;

	int thread_number;
	int start;
	int time;

	thread_number = instruction[0] - '0';
	start = instruction[4] - '0';
	time = instruction[6] - '0';

	sleep(start);
	printf("writer %d is waiting!\n", thread_number);
	sem_wait(&mutex2);
	writecount++;
	if(writecount == 1){
		sem_wait(&rd);
	}
	sem_post(&mutex2);
	
	sem_wait(&wrt);
	printf("writer %d start writing!\n", thread_number);
	sleep(time);
	printf("writer %d end writing!\n", thread_number);
	sem_post(&wrt);

	sem_wait(&mutex2);
	writecount--;
	if(writecount==0) {
		sem_post(&rd);
	}
	sem_post(&mutex2);
	
}
/*
Test input case:
1 R 3 5
2 W 4 5
3 R 5 2
4 R 6 5
5 W 7 3
*/


