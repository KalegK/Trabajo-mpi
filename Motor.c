//Compilacion: mpicxx programa_mpi.c -O3
//Ejecucion: mpirun -np 12 ./a.out

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
main (int argc, char **argv) 
{
	FILE * archivo=fopen("in.txt","r");
    if (archivo==NULL){
        printf("no abrio");
    }else{
        printf("se leyo correctamente");
    }
}




