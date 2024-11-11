

# Tarea 3: Protección de Memoria en xv6



1. Implementar las funciones mprotect y munprotect.
2. Modificación de la Tabla de Páginas

- En sistemas operativos, mprotect y munprotect son funciones que permiten cambiar los permisos de una página de memoria en un proceso. Estas funciones son útiles, por ejemplo, para proteger regiones de memoria contra escritura o lectura no deseadas.

- ```mprotect```: Cambia los permisos de una región de memoria para protegerla. Por ejemplo, podrías hacer que una página sea de solo lectura.

- ```munprotect```: Restaura los permisos de una región de memoria a su estado original, eliminando las restricciones de mprotect.

# Pasos para implementar ``` mprotect y munprotect ```

1. En el archivo syscall.h agregar

```
#define SYS_mprotect 22
#define SYS_munprotect 23 
``` 

2. en ```user.h``` agregar:

```
//nuevas para asignacion de memoria
int mprotect(void *addr, int len);
int munprotect(void *addr, int len);
```


3. En el archivo ```syscall.c```, agrega las funciones sys_mprotect y sys_munprotect, y mapea estas funciones a sus correspondientes entradas en la tabla de syscall

```
extern uint64 sys_mprotect(void);  //nueva
extern uint64 sys_munprotect(void); //nueva
```

```
[SYS_mprotect]    sys_mprotect, //nueva
[SYS_munprotect]  sys_munprotect, //nueva
```

```
uint64 sys_mprotect(void) {
    uint64 addr;
    int len;
    
    // Extraer los argumentos de la llamada de sistema
    argaddr(0, &addr);
    argint(1, &len);

    // Validar los valores extraídos (por ejemplo, verifica si `len` es negativo)
    if (len <= 0 || addr == 0)
        return -1;

    // Llamar a mprotect con los argumentos extraídos
    return mprotect((void *)addr, len);
}

uint64 sys_munprotect(void) {
    uint64 addr;
    int len;

    // Extraer los argumentos de la llamada de sistema
    argaddr(0, &addr);
    argint(1, &len);

    // Validar los valores extraídos (por ejemplo, verifica si `len` es negativo)
    if (len <= 0 || addr == 0)
        return -1;

    // Llamar a munprotect con los argumentos extraídos
    return munprotect((void *)addr, len);
}
```



4. Definir las funciones mprotect y munprotect en el archivo proc.c 

- La función mprotect debería tomar una dirección de inicio (addr) y un tamaño (len) como argumentos, y marcar las páginas en esa región como de solo lectura.

- Para munprotect, toma los mismos argumentos (addr y len), y restaura los permisos originales de las páginas (por ejemplo, de lectura y escritura).

codigo:

```
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "memlayout.h"
#include "proc.h"

// Define mprotect function
int
mprotect(void *addr, int len)
{
    struct proc *p = myproc();
    uint64 va = (uint64) addr;
    for (uint64 a = va; a < va + len; a += PGSIZE) {
        pte_t *pte = walk(p->pagetable, a, 0);
        if (pte == 0 || (*pte & PTE_V) == 0) {
            return -1;  // Invalid page
        }
        *pte &= ~PTE_W;  // Remove write permission
    }
    sfence_vma();  // Flush TLB
    return 0;
}

// Define munprotect function
int
munprotect(void *addr, int len)
{
    struct proc *p = myproc();
    uint64 va = (uint64) addr;
    for (uint64 a = va; a < va + len; a += PGSIZE) {
        pte_t *pte = walk(p->pagetable, a, 0);
        if (pte == 0 || (*pte & PTE_V) == 0) {
            return -1;  // Invalid page
        }
        *pte |= PTE_W;  // Restore write permission
    }
    sfence_vma();  // Flush TLB
    return 0;
}
```
- Antes de pasar al paso 5 vamos a comprobar que los cambios hechos funcionen, para ello crearemos mprotect_test.c para comprobarlo, luego lo añadiremos al Makefile para compilar y lo ejecutaremos 

```make qemu```

```$ mprotect_test```

si el programa  y los cambios fueron implementados correctamente, se deberia ver esto: 
mprotect test passed
munprotect test passed


error: falto incluir en ```proc.h``` :

```
// En proc.h
int mprotect(void *addr, int len);
int munprotect(void *addr, int len);
```

otro error: el archivo de prueba no reconoce las nuevas funciones. solucion:
Declara las funciones en ```user/user.h ```

```
int mprotect(void *addr, int len);
int munprotect(void *addr, int len);
```

y Agrega las llamadas de sistema en ```user/usys.pl```

```
entry("mprotect");
entry("munprotect");
```


5. Modificar los permisos en la tabla de páginas:

- En xv6, los codigos de ```mprotect``` y ```munprotect``` en ```proc.c``` usan la función ```walk()``` para obtener la entrada de la tabla de páginas correspondiente a la dirección de cada página en el rango de addr hasta addr + len.
Ademas, cambia los permisos de la entrada de la tabla de páginas usando los bits de permisos. Para hacer que una página sea de solo lectura, desactiva el bit PTE_W en la entrada de la tabla de páginas. (estos cambios ya estan en el codigo de ambas funciones nuevas)


- Fotos de los test ejecutados exitosamente

![Foto 1 ](img/tarea3-sistemas-testpass.png)
![Foto 2 ](img/tarea3-sistemas-testpass2.png)
