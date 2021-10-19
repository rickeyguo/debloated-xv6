#include "types.h"

#include "x86.h"

#include "defs.h"

#include "date.h"

#include "param.h"

#include "memlayout.h"

#include "mmu.h"

#include "proc.h"

int sys_fork(void)
{
  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use fork
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    return fork();
  }
  // unknown program, deny everything
  return -1;
}

int sys_exit(void)
{
  // programs are allowed to exit.
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use wait
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    return wait();
  }
  // unknown program, deny everything
  return -1;
}

int sys_kill(void)
{

  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // only kill program use fork
    if (strncmp(myproc()->name, "kill", 1) == 0) {
      int pid;
      if (argint(0, &pid) < 0)
        return -1;
      return kill(pid);
    }
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    int pid;
    if (argint(0, &pid) < 0)
      return -1;
    return kill(pid);
  }
  // unknown program, deny everything
  return -1;
}

int sys_getpid(void)
{

  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use getpid
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    return myproc()->pid;
  }
  // unknown program, deny everything
  return -1;
}

int sys_sbrk(void)
{

  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use sbrk
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    int addr;
    int n;

    if (argint(0, &n) < 0)
      return -1;
    addr = myproc()->sz;
    if (growproc(n) < 0)
      return -1;
    return addr;
  }
  // unknown program, deny everything
  return -1;
}

int sys_sleep(void)
{

  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use sleep
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);

    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
      return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n) {
      if (myproc()->killed) {
        release(&tickslock);
        return -1;
      }
      sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
  }
  // unknown program, deny everything
  return -1;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{

  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use uptime
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
  }
  // unknown program, deny everything
  return -1;
}

int sys_yield(void)
{

  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use yield
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    yield();
    return 0;
  }
  // unknown program, deny everything
  return -1;
}

int sys_shutdown(void)
{
  int gid = groupNum(myproc()->name);
  if (gid == 1) {
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    // none of group 1 programs use yield
    return -1;
  } else if (gid == 2) {
    // group 2 system program, allow everything
    // cprintf("[%s] is group %d application \n", myproc()->name, gid);
    shutdown();
    return 0;
  }
  // unknown program, deny everything
  return 0;
}
