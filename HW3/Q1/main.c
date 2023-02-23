// gcc -o main.o main.c Practical_Q_1f.o -lm
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include <sys/wait.h>  
#include<unistd.h>
extern long fun(long a,long b);
int main(){
    char read_buf[256];
    int the_main_pipe[2], queue;
    FILE *file_pointer = fopen("Practical_Q_1numbers.txt", "r");
    mkfifo("some_name", 0666);
    pipe(the_main_pipe); 
    if(fork() == 0){
        queue = open("some_name", O_WRONLY);
        fork();fork();fork();
        int number, number_2;
        read(the_main_pipe[0],&number,sizeof(number));
        for(int i=0; i<39999 ;i++){
            read(the_main_pipe[0],&number_2,sizeof(number_2));
            number = fun(number,number_2);
        }
        int result = write(queue, &number, sizeof(number));
        printf("PID:%d -> %d\n", getpid(), result);
    }else{
        queue = open("some_name", O_RDONLY);
        while(fgets(read_buf, 256, file_pointer)){
            int atoi_result = atoi(read_buf);
            write(the_main_pipe[1],&atoi_result,sizeof(atoi_result));
        }
        int to_return, the_pid;
        while ((the_pid = wait(&to_return)) > 0);
        int first,result;
        for(int i=0;i<7;i++){
            printf("parent -> read %d\n", read(queue, &first, sizeof(int)));
            result = fun(result,first);
        }
        printf("result -> %d\n", result);
    }
}