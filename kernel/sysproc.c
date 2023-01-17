#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va; //第一个参数: 第一页的第一个va
  int n; //第二个参数: 需要检查的页数
  uint64 buffer; //第三个参数: 储存结果的bitmask
  pte_t* pte;
  struct proc* p=myproc(); //当前进程
  uint64 abits=0;

  if(argaddr(0, &va)<0||argint(1, &n)<0||argaddr(2, &buffer)<0) //获取三个参数
    return -1;
  
  for(int i=0; i<n; i++, va+=PGSIZE){
    pte=walk(p->pagetable, va, 0);
    if(*pte&PTE_A){ //如果PTE的A位被置1(accessed)
      abits|=(1L<<i);
      *pte&=~PTE_A; //清空PTE_A位(置回0)
    }
  }

  copyout(p->pagetable, buffer, (char*)&abits, sizeof(uint64));

  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

