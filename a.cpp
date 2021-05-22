//Compilacion: mpicxx a.cpp -O3
//Ejecucion: mpirun -np 4 ./a.out <in.txt
#include "mpi.h"
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>

#define TAG_LENGTH 1
#define TAG_DATA 2


using namespace std;
map <string, vector<string> > mapa;

int main(int argc, char **argv)
{
	int num_documentos, cant_terminos, cant_consultas, ID, nproc;
	int buffer_nodo[100];
	int buffer_cantidad[100];
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
		// cout << "num_documentos = " << num_documentos << endl;
		MPI_Send(&num_documentos , 1, MPI_INT, 1,  0 , MPI_COMM_WORLD);

		for (int i = 0; i < num_documentos; ++i)
		{
			getchar();
			scanf("%s", titulo);
			// cout << "titulo = " << titulo << endl;
			MPI_Send(titulo, 100, MPI_CHAR, 1, TAG_LENGTH,MPI_COMM_WORLD);
			
		
			scanf("%d", &cant_terminos);
			// cout<<cant_terminos<< endl;
			// cout << "cant_terminos = " << cant_terminos << endl;
			MPI_Send( &cant_terminos , 1, MPI_INT, 1,  2, MPI_COMM_WORLD); // tag 2  envia la canitdad de terminos

	
			
			
			getchar();

			for (int j = 0; j < cant_terminos; ++j)
			{
				scanf("%s", termino);
				// cout << "termino = " << termino << endl;
				MPI_Send(termino, 100, MPI_CHAR, 1, TAG_LENGTH,MPI_COMM_WORLD);
			}
		}

		scanf("%d", &cant_consultas);
		getchar();

		for (int i = 0; i < cant_consultas; ++i)
		{
			scanf("%s", consulta);
			// cout << "consulta = " << consulta << endl;
		}

	}
	
	if (ID == 1){

		MPI_Recv(buffer_nodo , 1 , MPI_INT ,MPI_ANY_SOURCE , 0, MPI_COMM_WORLD, &var_status);
		printf("\n\nNODO[%d] :: Dato recibido = %d :: ID Emisor = %d ::\n\n", ID, buffer_nodo[0], var_status.MPI_SOURCE, var_status.MPI_TAG);
	


		
		for  (int i = 0; i < buffer_nodo[0]; ++i){
			MPI_Recv(buffer_cantidad, 1 , MPI_INT ,MPI_ANY_SOURCE ,2, MPI_COMM_WORLD, &var_status);
			printf(" Dato recibido = %d :: :\n\n", buffer_cantidad[0], var_status.MPI_SOURCE, var_status.MPI_TAG);
		}
		// MPI_Recv(buffer_cantidad, 1 , MPI_INT ,MPI_ANY_SOURCE ,0, MPI_COMM_WORLD, &var_status);
		// printf(" Dato recibido = %d :: :\n\n", buffer_cantidad[0], var_status.MPI_SOURCE, var_status.MPI_TAG);

		
		for (int i=0; i<buffer_nodo[0]; i++){

			MPI_Recv(titulo, 100, MPI_CHAR, 0, TAG_LENGTH, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				for(int i = 0;i< buffer_cantidad[0]; i++){
					MPI_Recv(termino, 100, MPI_CHAR, 0, TAG_LENGTH, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					mapa[titulo].push_back(termino);
				}
			
					
				
		}
		
		for (auto &el1: mapa)
		{
			cout << el1.first << endl;
			for (auto &el2: el1.second)
			{
				cout << el2 << " ";
			}
			cout << endl;
		}
		
	}
	

	MPI_Finalize();

	return 0;
}