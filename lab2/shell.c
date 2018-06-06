#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define MAX_LINE 80
#define BUFFER_LEN 10
char *buffer[BUFFER_LEN];
int count = 0;

void setup(char inputBuffer[], char *args[], int *background);
void handle_SIGINT();

int main(void) {
	char inputBuffer[MAX_LINE];
	int background;
	char *args[MAX_LINE/2+1];

	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	sigaction(SIGINT, &handler, NULL);

	for(int i = 0; i < BUFFER_LEN; i++) {
		buffer[i] = (char*) malloc(50);
	}
	
	while (1) {
		background = 0;
		printf("COMMAND->");
		fflush(stdout);
		setup(inputBuffer, args, &background);

		for (int i = 0; i < BUFFER_LEN - 1; i++) {
			strcpy(buffer[i], buffer[i+1]);
		}
		//strcpy(buffer[BUFFER_LEN-1], inputBuffer);
		//if (count <= BUFFER_LEN) count++;
		int j = 0;
		strcpy(buffer[BUFFER_LEN-1], "");
			while (args[j] != NULL) {
				strcat(buffer[BUFFER_LEN-1], args[j]);
				strcat(buffer[BUFFER_LEN-1], " ");
				j++;
			}

		pid_t pid;
		pid = fork();
		if(pid == 0) {
			execvp(args[0], args);
		}
		if (pid > 0) {
			if (background == 0) 
				wait(0);
		}
	}
	return 0;
}

void setup(char inputBuffer[], char *args[], int *background) {
	int length, i, start, ct;
	ct = 0;

	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

	start = -1;
	if(length == 0) exit(0);
	/*
	if(length < 0) {
		perror("error reading the command");
		exit(-1);
	}
	*/
	for(i = 0; i < length; i++) {
		switch(inputBuffer[i]) {
			case ' ':
			case '\t':
				if(start != 1) {
					args[ct] = &inputBuffer[start];
					ct++;
				}
				inputBuffer[i] = '\0';
				start = -1;
				break;
			case '\n':
				if(start != -1) {
					args[ct] = &inputBuffer[start];
					ct++;
				}
				inputBuffer[i] = '\0';
				args[ct] = NULL;
				break;
			default:
				if(start == -1)
					start = i;
				if(inputBuffer[i] == '&') {
					*background = 1;
					inputBuffer[i] = '\0';
				}
		}
	}
	args[ct] = NULL;
}

void handle_SIGINT() {
	char* lineBreak = "\n";

	for (int i = BUFFER_LEN-1; i >= 0 ; i--) {
		write(STDOUT_FILENO, lineBreak, strlen(lineBreak));
		write(STDOUT_FILENO, buffer[i], strlen(buffer[i]));
		
	}
}
