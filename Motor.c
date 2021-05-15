//Compilacion: mpicxx Motor.c -O3
//Ejecucion: mpirun -np 12 ./a.out

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

main (int argc, char **argv) 
{   
    char *palabra;

	FILE * archivo=fopen("in.txt","r");   /* Abre el archivo archivo.txt para lectura de texto */
        if (archivo==NULL){
            printf("no abrio");
            exit(1);
        }
 
}




