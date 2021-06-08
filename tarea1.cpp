//Compilacion: mpicxx a.cpp -O3
//Ejecucion: mpirun -np 4 ./a.out <in.txt
#include "mpi.h"
#include <stdio.h>
#include <map>
#include <string>
#include <iostream>
#include <vector>


using namespace std;



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
	map <string, vector<string>>::iterator iter;
	vector<string> consultas;



	if (ID == 0)
	{	
		//se realiza el envio de los datos leidos del archivo a otro nodo para la creacion del indice
		scanf("%d", &num_documentos);
		MPI_Send(&num_documentos , 1, MPI_INT, 1,  0 , MPI_COMM_WORLD); // tag 0 envia la canitdad de documentos a nodo 1

		for (int i = 0; i < num_documentos; ++i)
		{
			getchar();
			scanf("%s", titulo);
			MPI_Send(titulo, 100, MPI_CHAR, 1, 1,MPI_COMM_WORLD); // tag 1  envia los titulos  a nodo 1
			
		
			scanf("%d", &cant_terminos);
			MPI_Send( &cant_terminos , 1, MPI_INT, 1,  2, MPI_COMM_WORLD); // tag 2  envia la cantidad  de terminos

			getchar();

			for (int j = 0; j < cant_terminos; ++j)
			{
				scanf("%s", termino);
				MPI_Send(termino, 100, MPI_CHAR, 1, 3,MPI_COMM_WORLD);  // tag 3 envia   los terminos a cada documento
			}
		}

		scanf("%d", &cant_consultas);
		MPI_Send( &cant_consultas , 1, MPI_INT, 1,  4, MPI_COMM_WORLD); // tag 4 envia la cantidad de consultas
		getchar();

		for (int i = 0; i < cant_consultas; ++i)
		{
			scanf("%s", consulta);
			//cout << "consulta = " << consulta << endl;
			MPI_Send(consulta, 100, MPI_CHAR, 1, 5,MPI_COMM_WORLD);//tag 5 envia las consultas
		}

	}
	
	if (ID == 1){
		
		MPI_Recv(buffer_nodo , 1 , MPI_INT ,MPI_ANY_SOURCE , 0, MPI_COMM_WORLD, &var_status); // recibe cantidad de documentos
	
		//Recibe los terminos y crea el indice invertido usando un map con un string y un vector de string
		for (int i=0; i<buffer_nodo[0]; i++){
			MPI_Recv(buffer_cantidad, 1 , MPI_INT ,MPI_ANY_SOURCE ,2, MPI_COMM_WORLD, &var_status); // recibe cantidad de terminos
			MPI_Recv(titulo, 100, MPI_CHAR, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recibe titulos
			for(int i = 0;i< buffer_cantidad[0]; i++){
				MPI_Recv(termino, 100, MPI_CHAR, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);// recibe cada termino
				mapa[termino].push_back(titulo);
			}							
		}
		

		// // Impresion de el indice invertido
		cout<<"\n****Indice invertido****\n"<<endl;
		for (auto &el1: mapa)
		{
			cout << el1.first << endl;
			for (auto &el2: el1.second)
			{
				cout << el2 << " ";
			}
			cout << endl << endl;
		}

		MPI_Recv(buffer_cantidad_consultas, 1 , MPI_INT ,MPI_ANY_SOURCE ,4, MPI_COMM_WORLD, &var_status);// recibe la cantidad de las consultas
	
		for(int i = 0;i< buffer_cantidad_consultas[0]; i++){
			MPI_Recv(consulta, 100 , MPI_CHAR ,0 ,5, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // recibe las consultas
 			consultas.insert(consultas.begin()+i,consulta); //crea un vector con las consultas
		}

		cout<<"\n****Resultado consultas****\n";

		//realiza la busqueda de la consulta en el indice invertido
		for(int i = 0; i< consultas.size(); i++){
			contador=0;
			for (iter = mapa.begin(); iter != mapa.end(); iter++){
            	if(consultas[i] == iter->first){
					cout << endl;	
					cout << "Resultados para " << iter->first << ":" << endl;
					for(int j=0; j<iter->second.size();j++){	
						cout << iter->second[j]<<" ";	
					}
					cout << endl;	
				}
		    	else{
					contador=contador+1;
					if(contador==mapa.size()){
						cout << endl;	
						cout << "Para "<<consultas[i]<<" no existe resultado."<<endl;
					}
				}
			}
		}	
	}
	

	MPI_Finalize();

	return 0;
}