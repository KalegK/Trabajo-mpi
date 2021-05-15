#include "mpi.h"
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
	int num_documentos, cant_terminos, cant_consultas, ID, nproc;
	char titulo[1000];
	char termino[1000];
	char consulta[1000];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ID);

	if (ID == 0)
	{
		scanf("%d", &num_documentos);
		cout << "num_documentos = " << num_documentos << endl;

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

	MPI_Finalize();

	return 0;
}