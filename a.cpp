//Compilacion: mpicxx a.cpp -O3
//Ejecucion: mpirun -np 4 ./a.out
#include "mpi.h"
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int num_documentos, cant_terminos, cant_consultas, ID, nproc;
	int buffer_nodo[100];
	char titulo[1000];
	char termino[1000];
	char consulta[1000];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ID);
	MPI_Status var_status;



	if (ID == 0)
	{
		scanf("%d", &num_documentos);
		cout << "num_documentos = " << num_documentos << endl;
		MPI_Send( &num_documentos , 1, MPI_INT, 1,  0 , MPI_COMM_WORLD);

		for (int i = 0; i < num_documentos; ++i)
		{
			getchar();
			scanf("%s", titulo);
			cout << "titulo = " << titulo << endl;
		
			scanf("%d", &cant_terminos);
			cout << "cant_terminos = " << cant_terminos << endl;
			getchar();

			for (int j = 0; j < cant_terminos; ++j)
			{
				scanf("%s", termino);
				cout << "termino = " << termino << endl;
			}
		}

		scanf("%d", &cant_consultas);
		getchar();

		for (int i = 0; i < cant_consultas; ++i)
		{
			scanf("%s", consulta);
			cout << "consulta = " << consulta << endl;
		}

	}
	
	if (ID == 1){

		MPI_Recv(buffer_nodo , 1 , MPI_INT ,MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD, &var_status);
		printf("\n\nNODO[%d] :: Dato recibido = %d :: ID Emisor = %d ::\n\n", ID, buffer_nodo[0], var_status.MPI_SOURCE, var_status.MPI_TAG);
		int arreglo_documentos[buffer_nodo[0]];
	


	}
	

	MPI_Finalize();

	return 0;
}