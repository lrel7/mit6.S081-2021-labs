#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int p[]){
    int prime; //the first number from the left
    int n; //the number from the left
    close(p[1]);
    if(read(p[0], &prime, 4)!=4){
        printf("failed to read the first number\n");
        exit(1);
    }

    printf("prime %d\n", prime); //print the prime

    int newp[2];
    pipe(newp);

    //child
    if(fork()==0) sieve(newp);
    //parent
    else{
        close(newp[0]); //close the read end
        while(read(p[0], &n, 4)!=0){//if read returns 0, it means that it reaches the end
            if(n%prime!=0) write(newp[1], &n, 4); //send n to right
        } 
        close(newp[1]); //close the write end
        wait(0);
        exit(0);
    }
}

int main(int argc, char *argv[]){
    int p[2];
    pipe(p);

    //child: read from the left
    if(fork()==0) sieve(p);
    //parent: write 2~35 into the pipe
    else{
        close(p[0]);
        for(int i=2; i<=35; i++){
            if(write(p[1], &i, 4)!=4){
                printf("failed to write %d into the pipe\n", i);
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}