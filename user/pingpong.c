#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int pid;
    int p1[2]; //parent->child
    int p2[2]; //parent<-child
    char buf[]={'a'}; //the byte

    pipe(p1); //parent->child
    pipe(p2); //parent<-child

    pid=fork();

    //child
    if(pid==0){ 
        close(p1[1]); //pipe1's write end
        close(p2[0]); //pipe2's read end
        //read a byte from pipe1
        if(read(p1[0], buf, 1)!=1){
            printf("failed to read in child\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        //write a byte to pipe2
        if(write(p2[1], buf, 1)!=1){
            printf("failed to write in child\n");
            exit(1);
        }
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    //parent
    else{
        close(p1[0]);
        close(p2[1]);
        if(write(p1[1], buf, 1)!=1){
            printf("failed to write in parent\n");
            exit(1);
        }
        //now the child starts
        if(read(p2[0], buf, 1)!=1){
            printf("failed to read in parent\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(p1[1]);
        close(p2[0]);
        exit(0);
    }
}