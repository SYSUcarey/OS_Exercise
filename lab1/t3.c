#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main () {
  pid_t pid;
  pid = fork();

  if (pid == 0) {
    //child process1
    printf("b\n");
  } 
  else if (pid > 0) {
    printf("a\n");
    pid_t pid2;
    pid2 = fork();
    if (pid2 == 0) {
      //child process2
      printf("c\n");
    }
  }
}
