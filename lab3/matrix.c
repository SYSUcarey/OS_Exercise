#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 3
#define K 2
#define N 3
int A [M] [K] = {{1,4}, {2,5}, {3,6}};
int B [K] [N] = {{8,7,6}, {5,4,3}};
int C [M] [N]; 

struct v
{
	int row;
	int column; 	
};

void* runner(void *param);

int main(void) {
	struct v *data = (struct v *) malloc(sizeof(struct v));
	pthread_t tid[M*N];
	pthread_attr_t attr;
	/* get the default attributes */
	pthread_attr_init(&attr);
	
	/* create the thread */
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			data -> row = i;
			data -> column = j;
			pthread_create(&tid[i*N+j], &attr, runner, data);
			/* now wait for the thread to exit */
			pthread_join(tid[i*N+j], NULL);
		}
	}

	/* show the matrix result */
	printf("The matrix result: \n");
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d ", C[i][j]);
		}
		printf("\n");
	}
}

void* runner(void *param) {
	int i = ((struct v*)param) -> row;
	int j = ((struct v*)param) -> column;
	C[i][j] = 0;
	for (int k = 0; k < K; k++) {
		C[i][j] += (A[i][k] * B[k][j]);
	}
}