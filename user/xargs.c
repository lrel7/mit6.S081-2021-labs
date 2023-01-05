#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    //error
    if(argc<2){
        fprintf(2, "Usage: xargs command\n");
        exit(1);
    }

    char *paras[MAXARG]; //存放参数
    int idx=0; //参数数组的索引
    //读取命令行参数,存放到参数数组里(argv[0]是xargs,略去)
    for(int i=1; i<argc; i++){
        paras[idx]=(char*)malloc(sizeof(char)*(strlen(argv[i])+1));
        strcpy(paras[idx++], argv[i]);
    } 

    char buf[1024]={"\0"}; //存放从管道读取的数据
    char *pos=buf; //指示读到的位置
    //从管道读取数据到buf里
    while(read(0, pos, 1)>0){
        //读到换行符,将buf内容存入paras数组,并清空buf
        if(*pos=='\n'){
            *pos='\0';
            paras[idx]=(char*)malloc(sizeof(char)*(strlen(buf)+1));
            strcpy(paras[idx++], buf);
            pos=buf; //指示器复位到buf开头
            memset(buf, 0, 1024);
            continue;
        }
        pos++;
    }
    if(fork()==0){
        exec(argv[1], paras);
    }
    wait(0);
    exit(0);
}