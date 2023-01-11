// a simple demonstration program
// for thread switching
// the output is that chars keep on
// being printed out
// and '/' & '\' are interlacing



#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int pid;
    char c;

    pid=fork(); // fork a child process
    if(pid==0){ // child process
        c='/';
    }else{ // parnet process
        printf("parent pid is %d, child is %d\n", getpid(), pid);
        c='\\';
    }

    // a dead loop
    for(int i=0; ; i++){
        if((i%10000000)==0)
            write(2, &c, 1);
    }

    exit(0);

}