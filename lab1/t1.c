#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


int main() {
	pid_t pid1 = fork();
	printf("**1**\n");
	pid_t pid2 = fork();
	printf("**2**\n");
	if (pid1 == 0) {
		pid_t pid3 = fork();
		printf("**3**\n");
	} else {
		printf("**4**\n");
	}
	return 0;
}
