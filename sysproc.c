#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

//#include "inc.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


int sys_pcbp(void) {
/*	int i = 0;
	for(i = 0; i < 20; i++) {
		cprintf("%d\n", i);
	}*/
	struct proc* t ;
	argint(0, (int*)&t) ;
	pcbp((int)t);
	return 1;
}

int sys_pcbload(void) {
	struct proc* t;
	void* pgtable;
	struct context* cptr;
	struct trapframe *tfptr;
	argint(0, (int*)&t);
	argint(1, (int*)&pgtable);
	argint(2, (int*)&cptr);
	argint(3, (int*)&tfptr);
	pcbload(t, pgtable, cptr, tfptr);
	return 1;
}
/*
int sys_contsave(void) {
	struct context* c;
	argint(0, (int*)&c);
	contsave(c);
	return 1;
}
*/
int sys_pgsave(void) {
	void* pgptr;
	void* cptr;
	struct trapframe *tfptr;
	argint(0, (int*)&pgptr);
	argint(1, (int*)&cptr);
	argint(2, (int*)&tfptr);
	pgsave(pgptr, cptr, tfptr);
	return 1;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
