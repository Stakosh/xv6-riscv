#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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



//lógica de chmod
uint64
sys_chmod(void)
{
    char path[MAXPATH]; // Buffer para almacenar la ruta
    int mode;
    struct inode *ip;

    // Obtén los argumentos correctamente
    argstr(0, path, MAXPATH);
    argint(1, &mode);

    begin_op();
    if ((ip = namei(path)) == 0) { // Busca el inode por el nombre
        end_op();
        return -1; // Archivo no encontrado
    }
    ilock(ip);

    // Verifica si el archivo es inmutable
    if (ip->permissions == 5) {
        iunlockput(ip);
        end_op();
        return -1; // Archivo inmutable, no se pueden cambiar permisos
    }

    // Cambia los permisos
    ip->permissions = mode;
    iunlockput(ip);
    end_op();

    return 0; // Operación exitosa
}

