/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Mauricio Beltrán Huertas
 * Fecha: 11 noviembre 2025
 * Descripción: Programa que pide el nombre de un directorio y muestra información sobre
 *              sus ficheros: tipo, permisos y fecha de modificación reciente.
 ********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    int er;
    char nomdir[100], nomfich[100];
    struct stat atr;
    DIR *d;
    struct dirent *rd1;
    time_t fecha;

    printf("Nombre del directorio:\n");
    fgets(nomdir, sizeof(nomdir), stdin);

    // Quitar salto de línea al final
    nomdir[strlen(nomdir) - 1] = '\0';
    fecha = time(&fecha);

    // Intentar abrir el directorio
    if ((d = opendir(nomdir)) == NULL) {
        printf("No existe ese directorio.\n");
        return -1;
    }

    // Leer cada entrada del directorio
    while ((rd1 = readdir(d)) != NULL) {
        if (strcmp(rd1->d_name, ".") != 0 && strcmp(rd1->d_name, "..") != 0) {
            strcpy(nomfich, nomdir);
            strcat(nomfich, "/");
            strcat(nomfich, rd1->d_name);

            printf("\nFichero: %s\n", nomfich);
            er = stat(nomfich, &atr);
            if (er != 0) {
                perror("Error obteniendo información del archivo");
                continue;
            }

            printf("Modo: %lo\n", (unsigned long)atr.st_mode);
            if ((atr.st_mode & 0400) != 0)
                printf("→ Permiso R para propietario\n");
            else
                printf("→ No permiso R para propietario\n");

            if (S_ISDIR(atr.st_mode))
                printf("→ Es un directorio\n");
            if (S_ISREG(atr.st_mode))
                printf("→ Es un fichero regular\n");

            // Archivos modificados en los últimos 10 días
            if ((fecha - 10 * 24 * 60 * 60) < atr.st_mtime) {
                printf("→ Modificado en los últimos 10 días\n");
                printf("   Fecha de acceso: %s", ctime(&atr.st_mtime));
            }
        }
    }

    closedir(d);
    return 0;
}
