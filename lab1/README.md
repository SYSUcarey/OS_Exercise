# OS_Exercise/lab1

***

## 实验要求：

1. 将下面的程序编译运行，并解释现象。
```
#include < sys/types.h >
#include < stdio.h >
#include < unistd.h >

  int main(){

     int pid1=fork();
    
     printf(“**1**\n”);
    
     int pid2=fork();
    
     printf(“**2**\n”);
    
     if(pid1==0){int pid3=fork();printf(“**3**\n”);}
    
     else printf(“**4**\n”);

     return 0;

  }
```
***
2. 通过实验完成书本第三章习题3.4。
***
3. 编写一段程序，使用系统调用fork()创建两个子进程。当此程序运行时，在系统中有一个父进程和两个子进程活动。让每一个进程在屏幕上显示一个字符；父进程显示字符“a”；子进程分别显示字符“b”和字符“c”。试观察记录屏幕上的显示结果，并分析原因。

