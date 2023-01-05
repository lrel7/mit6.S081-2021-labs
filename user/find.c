#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  /*
  struct stat {
  int dev;     // /文件系统的磁盘设备 File system's disk device
  uint ino;    //节点数 Inode number
  short type;  // 文件类型 Type of file
  short nlink; // 指向文件的链接数 Number of links to file
  uint64 size; // 文件大小（字节） Size of file in bytes
  }; 

  struct dirent {
  ushort inum; //文件名长度
  char name[DIRSIZ]; //DIRSIZ: 最大的文件名长度
};
  */

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  
    
  //read the current filename
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    //add slash at the end of path
    *p++ = '/';

    if(de.inum == 0)
      continue;
    //add the current filename following '/'
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    if(stat(buf, &st) < 0){
      printf("ls: cannot stat %s\n", buf);
      continue;
    }

    switch(st.type){
      case T_FILE:
        if(strcmp(de.name, target)==0) printf("%s\n", buf);
        break;
      
      case T_DIR:
        if(strcmp(de.name, ".")!=0&&strcmp(de.name, "..")!=0) find(buf, target);
    }
  }
  close(fd);
  return;
}

int
main(int argc, char *argv[])
{
  if(argc!=3){
    fprintf(2, "Usage: find dir file\n");
    exit(1);
  }
  char *path=argv[1];
  char *target=argv[2];
  find(path, target);
  exit(0);
}