# Parte I Implementación Básica 'int getppid(void)'


1. Entendemos como funciona getpid()

Revisamos syscall.h, syscall.c, sysproc.c para entender como funciona getpid()




2. Implementación de la nueva llamada a sistema 'int getppid(void)'



Asignar un número de llamada al sistema en syscall.h

```

#define SYS_getppid  22
```


Agregar la entrada en 'syscall.c'

```

extern int sys_getppid(void);

[SYS_getppid] sys_getppid,
```
	

Implementar sys_getppid en sysproc.c

```

int
sys_getppid(void)
{
    return myproc()->parent->pid;
}
```

---
Declarar la Función en user.h

```

int getppid(void);
```


Luego agregar wrappers de Llamadas al Sistema en 'usys.pl'

```

entry("getppid");
```



3. Creación 'yosoytupadre.c'

En el directorio user, crear un archivo llamado yosoytupadre.c con lo siguiente:


```

#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    printf("Mi padre es: %d\n", getppid());
    exit(0);
}
```


Luego Modificar el Makefile 

```

UPROGS=\
    _cat\
    _echo\
    _forktest\
    ...
    _yosoytupadre\
```




# Parte II: Implementación Avanzada getancestor(int)

agregamos getancestor(int) de igual manera que lo hicimos con getpidd() con modificaciones en algunos códigos que ya se verán




se implementa en 'sysproc.c'


```

uint64
sys_getancestor(void)
{
    int n;
    struct proc *p = myproc();  // Obtiene el proceso actual

    argint(0, &n);  // Obtiene el argumento de entrada 'n'

    if (n < 0) {  // Verifica si el valor de 'n' es válido
        return -1;
    }

    // Bucle para navegar a través de los ancestros del proceso
    for (int i = 0; i < n; i++) {
        if (p->parent == 0)  // Si no hay más ancestros
            return -1;
        p = p->parent;       // Avanza al padre
    }
    return p->pid;           // Retorna el pid del ancestro
}
```


*Agregamos las funcion a syscall.h 

```
#define SYS_getancestor 23
```



*Agregamos las funcion a syscall.c

```

extern uint64 sys_getancestor(void);
```

```

[SYS_getancestor]  sys_getancestor,
```


*  agregar wrappers de Llamadas al Sistema en 'usys.pl'


```
entry("getancestor");
```

implementacion en user.h

```

int getancestor(int);   // Declaración para getancestor
```





Modificamos  'yosoytupadre.c'

printf("Ancestor 0 PID (self): %d\n", getancestor(0));
    printf("Ancestor 1 PID (parent): %d\n", getancestor(1));
    printf("Ancestor 2 PID (grandparent): %d\n", getancestor(2));




## Errores

* Makefile no reconoce informe 

```

fs.img: mkfs/mkfs README $(UPROGS)
	mkfs/mkfs fs.img README $(UPROGS)
	cp INFORME.md $(OBJDIR)
```

se cambia por 


```

fs.img: mkfs/mkfs README $(UPROGS)
	mkfs/mkfs fs.img README $(UPROGS)
	cp INFORME.md $(UPROGS)
```
