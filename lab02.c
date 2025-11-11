/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Mauricio Beltrán Huertas
 * Fecha: 11 noviembre 2025
 * Descripción: Programa en C que solicita el nombre de un directorio o archivo y analiza cada elemento
 *              que contiene, mostrando su tipo, permisos del 
 *              propietario y fecha de última modificación. 
 *              Emplea las funciones opendir(), readdir() y stat() para acceder a los 
 *              atributos de cada archivo, junto con estructuras del sistema como struct stat 
 *              y struct dirent. Además, determina si los archivos han sido modificados en los 
 *              últimos diez días utilizando operaciones con time_t.
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
    int er;  // Se crea esta variable para guardar el resultado de la función stat
    char nomdir[100], nomfich[100]; // se crean estos arreglos para el nombre del directorio y del archivo
    struct stat atr; // Se crea esta estructura donde se almacenan los atributos del archivo
    DIR *d;  // Se crea un puntero al directorio o archivo 
    struct dirent *rd1; // Se crea una estructura para leer las entradas del directorio o archivo 
    time_t fecha; // Se crea variable para guardar la fecha y hora actual del sistema

    printf("Nombre del directorio:\n"); // Se le solicita al usuario el nombre del directorio o archivo que desea analizar
    fgets(nomdir, sizeof(nomdir), stdin); // Lee el nombre del archivo o directorio desde el teclado 

    nomdir[strlen(nomdir) - 1] = '\0'; 
    fecha = time(&fecha); // Se guarda la fecha y hora 

    // Se intenta abrir el directorio
    if ((d = opendir(nomdir)) == NULL) {
        printf("No existe ese directorio.\n"); // si no se puede abrir imprime este mensaje 
        return -1; // retorna error 
    }

    // Leer cada entrada del directorio
    while ((rd1 = readdir(d)) != NULL) {
        if (strcmp(rd1->d_name, ".") != 0 && strcmp(rd1->d_name, "..") != 0) {
            strcpy(nomfich, nomdir); // Copia el nombre del directorio
            strcat(nomfich, "/");
            strcat(nomfich, rd1->d_name); // Añade el nombre del archivo actual

            printf("\nFichero: %s\n", nomfich); // Muestra el nombre completo del fichero actual 
            er = stat(nomfich, &atr);
            if (er != 0) {
                perror("Error obteniendo información del archivo");
                continue; // sigue al siguiente archivo 
            }

            printf("Modo: %lo\n", (unsigned long)atr.st_mode);  // Se muestra los permisos y tipo de archivo en formato ls -l
            if ((atr.st_mode & 0400) != 0) // // Se verifica si el propietario tiene permiso de lectura
                printf("Permiso R para propietario\n"); 
            else
                printf(" No permiso R para propietario\n");

            if (S_ISDIR(atr.st_mode))  // Se determina si la entrada es un directorio
                printf(" Es un directorio\n");
            if (S_ISREG(atr.st_mode)) // Se determina si la entrada es un archivo regular
                printf(" Es un fichero regular\n");

            // Archivos modificados en los últimos 10 días
            if ((fecha - 10 * 24 * 60 * 60) < atr.st_mtime) {
                printf("Modificado en los últimos 10 días\n");  // Se muestra la fecha y hora exacta de la última modificación
                printf("  Fecha de acceso: %s", ctime(&atr.st_mtime));
            }
        }
    }

    closedir(d); // Se cierra el directorio que esta abierto y se libera memoria o recursos 
    return 0; // finaliza el programa de 
}

/*------------------------------------------------------------------------------------------
 * Conclusión:
 * Este laboratorio profundiza en el uso de la estructura stat y las funciones del sistema
 * para obtener metadatos de archivos. Además de identificar los nombres de los ficheros,
 * el programa permite conocer sus permisos, tipo archivo o directorio y fecha de última
 * modificación, determinando si han sido editados recientemente.
 * 
 *------------------------------------------------------------------------------------------*/
