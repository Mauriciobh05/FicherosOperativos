/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Mauricio Beltrán Huertas
 * Fecha: 11 noviembre 2025
 * Descripción: Programa en C que lee de teclado el nombre de un directorio y muestra        *
 *              en pantalla el nombre y tamaño de los ficheros que contiene.
 ********************************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


int main() {
    DIR *d;
    char nomdir[90], nomfich[90];
    struct stat datos;
    struct dirent *direc;

    // Solicitar el nombre del directorio
    printf("Introduzca el nombre de un directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin);

    // Eliminar el salto de línea del final
    nomdir[strlen(nomdir) - 1] = '\0';

    // Abrir el directorio
    if ((d = opendir(nomdir)) == NULL) {
        printf("El directorio no existe o no se puede abrir.\n");
        return -1;
    }

    // Leer los archivos del directorio
    while ((direc = readdir(d)) != NULL) {
        strcpy(nomfich, nomdir);
        strcat(nomfich, "/");
        strcat(nomfich, direc->d_name);

        // Obtener información del archivo
        if (stat(nomfich, &datos) == 0) {
            if (S_ISREG(datos.st_mode)) { // Solo mostrar archivos regulares
                printf("Nombre: %-30s\t Tamaño: %ld bytes\n", direc->d_name, datos.st_size);
            }
        }
    }

    // Cerrar el directorio
    closedir(d);
    return 0;
}
