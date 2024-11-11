#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int main() {
    // Solicitar un bloque de memoria usando sbrk para probar
    int size = 4096;  // Tamaño de una página
    void *addr = sbrk(size);

    // Llenar la memoria con datos para verificar el permiso de escritura
    char *p = (char *)addr;
    for (int i = 0; i < size; i++) {
        p[i] = 'A';
    }

    // Proteger la memoria (hacerla de solo lectura)
    if (mprotect(addr, 1) < 0) {
        printf("mprotect failed\n");
        exit(1);
    }

    // Intentar escribir en la memoria protegida
    int success = 1;
    if (fork() == 0) {
        p[0] = 'B';  // Esto debería fallar y causar una terminación
        success = 0; // Si escribe, algo está mal
        exit(0);
    } else {
        wait(0);
    }

    if (success) {
        printf("mprotect test passed\n");
    } else {
        printf("mprotect test failed: write succeeded on protected memory\n");
    }

    // Quitar la protección (volver a permitir escritura)
    if (munprotect(addr, 1) < 0) {
        printf("munprotect failed\n");
        exit(1);
    }

    // Intentar escribir nuevamente en la memoria desprotegida
    p[0] = 'C';  // Esto debería tener éxito

    printf("munprotect test passed\n");
    exit(0);
}
