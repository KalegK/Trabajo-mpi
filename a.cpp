//Compilacion: mpicxx a.cpp -O3
//Ejecucion: mpirun -np 4 ./a.out <in.txt
#include "mpi.h"
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>


using namespace std;

bool buscar(vector<string> a, char* b){

		for (int i = 0; i < a.size(); ++i)
		{
			if (a[i] == b)
			{
				return true;
			}
		}

		return false;
	}


int main(int argc, char **argv)
{
	int num_documentos, cant_terminos, cant_consultas, ID, nproc;
	int buffer_nodo[100];
	int buffer_cantidad[100];
	int buffer_cantidad_consultas[100];
	char titulo[1000];
	char termino[1000];
	char consulta[1000];
	int contador=0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &ID);
	MPI_Status var_status;

	map <string, vector<string> > mapa;
	map <string, vector<string> > mapa2;
	map <string, vector<string>>::iterator iter;
	vector<string> consultas;

	bool buscar(vector<string> a, char* b);
	


	if (ID == 0)
	{	
		
		
		scanf("%d", &num_documentos);
		// cout << "num_documentos = " << num_documentos << endl;
		MPI_Send(&num_documentos , 1, MPI_INT, 1,  0 , MPI_COMM_WORLD); // tag 0 envia la canitdad de documentos a nodo 1

		for (int i = 0; i < num_documentos; ++i)
		{
			getchar();
			scanf("%s", titulo);
			cout << "titulo = " << titulo << endl;
			MPI_Send(titulo, 100, MPI_CHAR, 1, 1,MPI_COMM_WORLD); // tag 1  envia los titulos  a nodo 1
			
		
			scanf("%d", &cant_terminos);
			// cout<<cant_terminos<< endl;
			cout << "cant_terminos = " << cant_terminos << endl;
			MPI_Send( &cant_terminos , 1, MPI_INT, 1,  2, MPI_COMM_WORLD); // tag 2  envia la cantidad  de terminos

	
			
			
			getchar();

			for (int j = 0; j < cant_terminos; ++j)
			{
				scanf("%s", termino);
				cout << "termino = " << termino << endl;
				MPI_Send(termino, 100, MPI_CHAR, 1, 3,MPI_COMM_WORLD);  // tag 3 envia   los terminos a cada documento
			}
		}

		scanf("%d", &cant_consultas);
		MPI_Send( &cant_consultas , 1, MPI_INT, 1,  4, MPI_COMM_WORLD);
		getchar();

		for (int i = 0; i < cant_consultas; ++i)
		{
			scanf("%s", consulta);
			cout << "consulta = " << consulta << endl;
			MPI_Send(consulta, 100, MPI_CHAR, 1, 5,MPI_COMM_WORLD);//envia las consultas
		}

	}
	
	if (ID == 1){
		

		MPI_Recv(buffer_nodo , 1 , MPI_INT ,MPI_ANY_SOURCE , 0, MPI_COMM_WORLD, &var_status); // recibe cantidad de documentos
		// printf("\n\nNODO[%d] :: Dato recibido = %d :: ID Emisor = %d ::\n\n", ID, buffer_nodo[0], var_status.MPI_SOURCE, var_status.MPI_TAG); 
	


		
	
		
		for (int i=0; i<buffer_nodo[0]; i++){
			MPI_Recv(buffer_cantidad, 1 , MPI_INT ,MPI_ANY_SOURCE ,2, MPI_COMM_WORLD, &var_status); // recibe cantidad de terminos
			MPI_Recv(titulo, 100, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recibe titulos
				for(int i = 0;i< buffer_cantidad[0]; i++){
					
					MPI_Recv(termino, 100, MPI_CHAR, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);// recibe cada termino
					mapa[titulo].push_back(termino);
				}
			
					
				
		}
		

		// Impresion de los documentos
		for (auto &el1: mapa)
		{
			cout << el1.first << endl;
			for (auto &el2: el1.second)
			{
				cout << el2 << " ";
			}
			cout << endl;
		}

		MPI_Recv(buffer_cantidad_consultas, 1 , MPI_INT ,MPI_ANY_SOURCE ,4, MPI_COMM_WORLD, &var_status);// recibe la cantidad de las consultas
		for(int i = 0;i< buffer_cantidad_consultas[0]; i++){
			MPI_Recv(consulta, 100 , MPI_CHAR ,0 ,5, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recibe las consultas
			consultas.insert(consultas.begin()+i,consulta); //crea un vector con las consultas
		}
		
		
		// for (iter = mapa.begin(); iter != mapa.end(); iter++)
		// {
		// 	for (int i = 0; i < consultas.size(); ++i)
		// 	{	
				
			
		// 		if (buscar(iter->second, const_cast<char*>(consultas[i].c_str())) == true)
		// 		{
		// 			mapa2[consultas[i]].push_back(iter->first);
		// 			contador=0;
		// 		}
		// 		else{		
		// 			cout<< "contador I" <<contador << endl;	 
		// 			contador=contador+1;
		// 			cout<< "contador F" <<contador << endl;
		// 			if(contador==buffer_nodo[0]){
		// 				mapa2[consultas[i]].push_back("No existe resultado");
		// 				contador=0;
						
		// 			}else{
		// 				contador=0;
		// 			}
		// 		}
		// 	}
		// }

		for (int i = 0; i < consultas.size(); ++i){
			for (iter = mapa.begin(); iter != mapa.end(); iter++){

				if (buscar(iter->second, const_cast<char*>(consultas[i].c_str())) == true)
				{		
					mapa2[consultas[i]].push_back(iter->first);
					contador=0;
					
				}
				else{		
					contador=contador+1;
					if(contador==buffer_nodo[0]){
						mapa2[consultas[i]].push_back("No existe resultado");
						contador=0;
					}
			}


		}
		}


		for (auto &el1: mapa2)
		{
			cout << "resultado para: " << el1.first << " -->";
			for (auto &el2: el1.second)
			{
				cout << " " <<el2 << " ";
			}
			cout << endl;
		}

		
	
	}
	

	MPI_Finalize();

	return 0;
}