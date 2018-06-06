#include <stdio.h>
#include <stdlib.h>

int main(void){
	char input[30], output[30];
	FILE *rfile = NULL, *wfile = NULL;
	char buffer;

	scanf("%s",input);
	scanf("%s",output);
	//Acquire input and output file name

	rfile = fopen(input, "r");
	if(rfile == NULL) {
		printf("The input file doesn't exist, abort\n");
		return 0;
	}
	
	wfile = fopen(output, "r");
	if (wfile != NULL) {
		printf("The output file exists, abort\n");
		return 0;
	}

	wfile = fopen(output, "w");
	buffer = fgetc(rfile);
	while (buffer != EOF) {
		fputc(buffer,wfile);
		printf("%c",buffer);
		buffer = fgetc(rfile);

	}

	return 0;

}
