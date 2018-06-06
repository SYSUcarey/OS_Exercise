#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

typedef int buffer_item;
#define BUFFER_SIZE  5

/// shared buffer
buffer_item buffer[BUFFER_SIZE];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static sem_t empty;
static sem_t full;

int buffer_len = 0;
void* producer(void *param);
void* consumer(void *param);

int main(void) {

	char** instruction = (char**)malloc(sizeof(char*)*10);
	for(int i = 0; i < 10; i++) {
		instruction[i] = (char*)malloc(sizeof(char)*20);
	}
	
	sem_init(&empty,0,BUFFER_SIZE);
	sem_init(&full,0,0);



	//need to create 6 pthread
	pthread_t tid[6];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	
	
	for (int i = 0; i < 6; i++) {
		//get the instruction input
		gets(instruction[i]);
	}

	//judge the instruction_thread_type and create the thread
	char thread_type;
	for (int i = 0; i < 6; i++) {
		thread_type = instruction[i][2];
		if(thread_type == 'P') {
			pthread_create(&tid[i], &attr, producer, instruction[i]);
		}
		else if(thread_type == 'C'){
			pthread_create(&tid[i], &attr, consumer, instruction[i]);
		}
	}
	
	//join all the six pthread
	for (int i = 0; i < 6; i++) {
		pthread_join(tid[i],NULL);
	}

	//Show the end buffer
	printf("At last, the buffer is: \n");
	for (int i = 0; i < buffer_len; i++) {
		printf("%d " , buffer[i]);
	}
	if(buffer_len==0) printf("NULL\n");

	sem_destroy(&empty);
	sem_destroy(&full);
	pthread_mutex_destroy(&mutex);
	return 0;
}

int insert_item(buffer_item item) {
	if(buffer_len == BUFFER_SIZE) return 0;
	else {
		buffer[buffer_len] = item;
		buffer_len++;
	}
	return 1;
	
}

int remove_item() {
	if(buffer_len > 0) {
		buffer_len--;
		return 1;
	}
	return 0;
}

void* producer(void *param) {
	//do what a producer does
	
	char* instruction = (char*)malloc(sizeof(char)*20);
	instruction = (char*)param;

	
	int thread_number;
	char thread_type;
	int start;
	int time;
	int product_number;

	thread_number = instruction[0] - '0';
	thread_type = instruction[2];
	start = instruction[4] - '0';
	time = instruction[6] - '0';
	product_number = instruction[8] - '0';
	

	sleep(start);
	
	sem_wait(&empty);
	pthread_mutex_lock(&mutex);
	printf("producer");
	printf("%d", thread_number);
	printf(" start producing!\n");
	sleep(time);
	insert_item(product_number);
	printf("producer");
	printf("%d", thread_number);
	printf(" end producing!\n");
	printf("The buffer now is: \n");
	for (int i = 0; i < buffer_len; i++) {
		printf("%d " , buffer[i]);
	}
	if(buffer_len==0) printf("NULL");
	printf("\n");
	pthread_mutex_unlock(&mutex);
	sem_post(&full);
	
}

void* consumer(void *param) {
	//do what a consumer does
	
	char* instruction = (char*)malloc(sizeof(char)*20);
	instruction = (char*)param;

	
	int thread_number;
	char thread_type;
	int start;
	int time;
	int product_number;

	thread_number = instruction[0] - '0';
	thread_type = instruction[2];
	start = instruction[4] - '0';
	time = instruction[6] - '0';

	sleep(start);
	
	sem_wait(&full);
	pthread_mutex_lock(&mutex);
	printf("consumer");
	printf("%d", thread_number);
	printf(" start consuming!\n");
	sleep(time);
	remove_item();
	printf("consumer");
	printf("%d", thread_number);
	printf(" end consuming!\n");
	printf("The buffer now is: \n");
	for (int i = 0; i < buffer_len; i++) {
		printf("%d " , buffer[i]);
	}
	if(buffer_len==0) printf("NULL");
	printf("\n");
	pthread_mutex_unlock(&mutex);
	sem_post(&empty);
	
}
/*
Test input case:
1 C 3 5
2 P 4 5 1
3 C 5 2
4 C 6 5
5 P 7 3 2
6 P 8 4 3
*/


