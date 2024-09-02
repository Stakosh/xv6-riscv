#include "kernel/types.h"  // Incluye tipos básicos utilizados en xv6
#include "user/user.h"     // Incluye las declaraciones de funciones del usuario

int main(void)
{
    int ppid = getppid();  // Llama a getppid() y almacena el ID del proceso padre
    printf("Parent PID: %d\n", ppid);  // Imprime el ID del proceso padre

    // Pruebas adicionales para getancestor
    printf("Ancestor 0 PID (self): %d\n", getancestor(0));  // Debe retornar el PID del proceso mismo
    printf("Ancestor 1 PID (parent): %d\n", getancestor(1));  // Debe retornar el PID del padre
    printf("Ancestor 2 PID (grandparent): %d\n", getancestor(2));  // Debe retornar el PID del abuelo, o -1 si no existe

    exit(0);  // Termina el programa con código de salida 0
}
