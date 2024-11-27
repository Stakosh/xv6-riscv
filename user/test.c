#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void test_permissions() {
    int fd;
    char *filename = "testfile";

    printf("Creando archivo '%s' con permisos predeterminados...\n", filename);
    fd = open(filename, O_CREATE | O_RDWR);
    if (fd < 0) {
        printf("Error: No se pudo crear el archivo '%s'\n", filename);
        exit(1);
    }
    printf("Archivo '%s' creado exitosamente.\n", filename);

    printf("Escribiendo en el archivo '%s'...\n", filename);
    if (write(fd, "xv6", 3) != 3) {
        printf("Error: No se pudo escribir en el archivo '%s'\n", filename);
        exit(1);
    }
    printf("Escritura inicial exitosa.\n");
    close(fd);

    printf("Cambiando permisos de '%s' a solo lectura...\n", filename);
    if (chmod(filename, 1) < 0) {
        printf("Error: No se pudo cambiar permisos a solo lectura\n");
        exit(1);
    } else {
        printf("Permisos cambiados a solo lectura.\n");
    }

    printf("Intentando escribir en el archivo '%s' con permisos de solo lectura...\n", filename);
    fd = open(filename, O_WRONLY);
    if (fd >= 0) {
        if (write(fd, "fail", 4) == 4) {
            printf("Error: Escritura exitosa, pero no debería ser posible\n");
            exit(1);
        }
        close(fd);
    } else {
        printf("Correcto: No se pudo abrir el archivo '%s' en modo escritura\n", filename);
    }

    printf("Cambiando permisos de '%s' a lectura/escritura...\n", filename);
    if (chmod(filename, 3) < 0) {
        printf("Error: No se pudo cambiar permisos a lectura/escritura\n");
        exit(1);
    } else {
        printf("Permisos cambiados a lectura/escritura.\n");
    }

    printf("Intentando escribir en el archivo '%s' con permisos de lectura/escritura...\n", filename);
    fd = open(filename, O_WRONLY);
    if (fd >= 0) {
        if (write(fd, "pass", 4) == 4) {
            printf("Correcto: Escritura exitosa.\n");
        } else {
            printf("Error: No se pudo escribir en el archivo\n");
            exit(1);
        }
        close(fd);
    } else {
        printf("Error: No se pudo abrir el archivo en modo escritura\n");
        exit(1);
    }

    printf("Cambiando permisos de '%s' a inmutable...\n", filename);
    if (chmod(filename, 5) < 0) {
        printf("Error: No se pudo cambiar permisos a inmutable\n");
        exit(1);
    } else {
        printf("Permisos cambiados a inmutable.\n");
    }

    printf("Intentando cambiar permisos de '%s' (inmutable) a lectura/escritura...\n", filename);
    if (chmod(filename, 3) < 0) {
        printf("Correcto: No se pueden cambiar permisos de un archivo inmutable.\n");
    } else {
        printf("Error: Permisos cambiados, pero no debería ser posible\n");
        exit(1);
    }

    printf("Pruebas completadas exitosamente.\n");
    close(fd);
}

int main(int argc, char *argv[]) {
    test_permissions();
    exit(0);
}
