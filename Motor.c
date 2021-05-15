//Compilacion: mpicxx programa_mpi.c -O3
//Ejecucion: mpirun -np 12 ./a.out

#include "mpi.h"
#include <stdio.h>

main (int argc, char **argv) 
{
	int nproc; /* Nro. de procesos */
	int ID; /* Mi Identificador  0<= ID <=(nproc-1) */

    int var_envio = 150, var_envio2 = 222;
    int buffer_nodo[100];
    int cant_datos_recibidos=0, flag=0;

    MPI_Status var_status;
    MPI_Request req;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ID);

	printf("Hola Mundoo :: Soy el Nodo con ID = %d de un total de %d nodos.\n", ID, nproc);

//    MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
//    MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status);
//    MPI_Get_count(MPI_Status *status, MPI_Datatype datatype, int *count);


    //////////  Envio y Recepcion Bloqueantes
    
    if (ID == 0)
        MPI_Send(&var_envio, 1, MPI_INT, 2, 111, MPI_COMM_WORLD);

    if (ID == 2)
    {
        MPI_Recv(buffer_nodo, 100, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &var_status);

        MPI_Get_count(&var_status, MPI_INT, &cant_datos_recibidos);
        
        printf("\n\nNODO[%d] :: Dato recibido = %d :: Cant. Datos Recibidos = %d :: ID Emisor = %d :: Tag del mensaje recibido = %d\n\n", ID, buffer_nodo[0], cant_datos_recibidos, var_status.MPI_SOURCE, var_status.MPI_TAG);
    }


    //////////  Envio Bloqueante y Recepcion No Bloqueante

    if (ID == 1)
        MPI_Send(&var_envio2, 1, MPI_INT, 3, 112, MPI_COMM_WORLD);

    if (ID == 3)
    {
        MPI_Irecv(buffer_nodo, 100, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);

        while(flag == 0)
        {
            MPI_Test(&req, &flag, &var_status);

            //Procesamiento
        }

        MPI_Get_count(&var_status, MPI_INT, &cant_datos_recibidos);

        printf("\n\nNODO[%d] :: Dato recibido = %d :: Cant. Datos Recibidos = %d :: ID Emisor = %d :: Tag del mensaje recibido = %d\n\n", ID, buffer_nodo[0], cant_datos_recibidos, var_status.MPI_SOURCE, var_status.MPI_TAG);

    }

    ///////////// Envio con buffer y Recepcion del mensaje por encuesta  /////

    int arr_Buffer[1000];
    
    MPI_Buffer_attach(&(arr_Buffer[0]), sizeof(int)*1000);

    //MPI_Bsend(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
    if (ID == 4)
        MPI_Bsend(&var_envio, 1, MPI_INT, 5, 111, MPI_COMM_WORLD);

    if (ID == 5)
    {
        //MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);

        while (flag == 0)
        {
            MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &var_status);
        }

        //El tag del mensaje es status.MPI_TAG y el emisor es el nodo con ID = status.MPI_SOURCE

        if (var_status.MPI_TAG == 111)
        {
            MPI_Recv(buffer_nodo, 100, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &var_status);
            MPI_Get_count(&var_status, MPI_INT, &cant_datos_recibidos);
        }
        /*
        else if (status.MPI_TAG == 222)
        {
            MPI_Recv(buffer_recepcion, 100, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &var_status);
            MPI_Get_count(&var_status, MPI_CHAR, &cant_datos_recibidos);
        }
         */
        
        printf("\n\nNODO[%d] :: Dato recibido = %d :: Cant. Datos Recibidos = %d :: ID Emisor = %d :: Tag del mensaje recibido = %d\n\n", ID, buffer_nodo[0], cant_datos_recibidos, var_status.MPI_SOURCE, var_status.MPI_TAG);
    }

    //int MPI_Barrier(MPI_Comm comm);

    MPI_Barrier(MPI_COMM_WORLD);


	MPI_Finalize();
}




