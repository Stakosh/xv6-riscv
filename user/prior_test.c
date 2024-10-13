#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prior_test() {
    printf("Iniciando la prueba de prioridades con 20 procesos.\n");
    printf("Creando 20 procesos usando fork()...\n");

    int i;

    // Crear 20 procesos usando fork
    for (i = 0; i < 20; i++) {
        printf("Creando proceso %d...\n", i);  // Mensaje antes del fork
        int pid = fork();

        if (pid == 0) {  // Proceso hijo
            // Asignar una prioridad inversa al índice del proceso
            int priority = i;
            printf("Proceso hijo creado: %d con PID: %d y prioridad: %d\n", 
                   i, getpid(), priority);

            // Simular ejecución: procesos menos prioritarios duermen más tiempo
            printf("Proceso %d con PID: %d durmiendo por %d segundos.\n", 
                   i, getpid(), priority * 5);
            sleep(priority * 5);  // Sleep proporcional a la "prioridad"

            printf("Proceso %d con PID: %d ha terminado.\n", i, getpid());
            exit(0);  // Terminar el proceso hijo
        }
    }

    // Código del proceso padre: Espera a que todos los hijos terminen
    printf("Esperando a que todos los procesos hijos terminen...\n");
    for (i = 0; i < 20; i++) {
        wait(0);  // Esperar que cada hijo termine
        printf("Proceso hijo terminado. Quedan %d procesos por esperar.\n", 19 - i);
    }

    printf("Todos los procesos han terminado.\n");
}

int main(int argc, char *argv[]) {
    printf("Iniciando el programa prior_test...\n");
    prior_test();  // Ejecutar la prueba
    printf("Saliendo del programa prior_test...\n");
    exit(0);  // Salir del programa
}
