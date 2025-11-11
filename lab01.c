/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Autor: Mauricio Beltrán Huertas
 * Fecha: 11 noviembre 2025
 * Descripción: Programa en C que solicita al usuario el nombre de un directorio y muestra 
 *              en pantalla los nombres y tamaños en bytes de todos los archivos regulares
 *              contenidos en dicho directorio o archivo . 
 *              Utiliza las funciones opendir(), readdir() y stat() para recorrer el sistema
 *              de archivos y obtener información detallada de cada fichero.
 ********************************************************************************************/

#include <sys/types.h> // Interfaz que define los tipos de datos usados por el sistema. 
#include <sys/stat.h> // Interfaz que permite obtener información de archivos, stat 
#include <sys/mman.h> // Interfaz para manejo de memoria
#include <fcntl.h>   // Interfaz que proporciona constantes para abrir archivos
#include <stdio.h> // Interfaz estándar de entrada/salida
#include <unistd.h>
#include <stdlib.h> // Interfaz estándar como por ejemplo malloc
#include <string.h> // Interfaz que ayuda al manejo de las cadenas 
#include <dirent.h> // Interfaz que ayuda a la manipulación de directorios, opendir 


int main() {
    DIR *d;  // Se crea un puntero al directorio (tipo DIR)
    char nomdir[90], nomfich[90]; // Se crean arreglos para guardar el nombre del directorio y de cada archivo
    struct stat datos; // Se crea una estructura donde se guarda la información de cada archivo
    struct dirent *direc; // Se crea un puntero a estructura que representa una entrada del directorio

    // Solicitar el nombre del directorio
    printf("Introduzca el nombre de un directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin); // Se lee el nombre del directorio desde teclado

    // Eliminar el salto de línea del final
    nomdir[strlen(nomdir) - 1] = '\0';

    // Abrir el directorio
    if ((d = opendir(nomdir)) == NULL) { // Si no se puede abrir el directorio devuelve null 
        printf("El directorio no existe o no se puede abrir.\n");
        return -1; // Se termina el programa con error 
    }

    // Leer los archivos del directorio
    while ((direc = readdir(d)) != NULL) { // Si hay archivos como entrada 
        strcpy(nomfich, nomdir); // Copia el nombre del directorio
        strcat(nomfich, "/"); // Agrega una barra para formar la ruta
        strcat(nomfich, direc->d_name); // Añade el nombre del archivo actual

        // Obtener información del archivo
        if (stat(nomfich, &datos) == 0) { // se confirma que el stat sea 0 para y asi se sabe que la información es valida
            if (S_ISREG(datos.st_mode)) { // Solo mostrar archivos regulares
                printf("Nombre: %-30s\t Tamaño: %ld bytes\n", direc->d_name, datos.st_size); // Se imprime el nombre y el tamaño en bytes del archivo 
            }
        }
    }

    // Se cierra el directorio para liberar memoria 
    closedir(d);
    return 0; // se finaliza el programa 
}

/********************************************************************************************
 * Conclusión:
 * Este programa permitió comprender el manejo básico de directorios en lenguaje C mediante
 * las funciones del sistema opendir(), readdir() y stat(). A través de su ejecución,
 * se logró recorrer un directorio e imprimir el nombre y tamaño de cada archivo regular.
 * 
 * Este laboratorio refuerza
 * los conocimientos sobre cómo interactúa un programa con el sistema de archivos 
 ********************************************************************************************/
