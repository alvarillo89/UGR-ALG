#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace::std;
using namespace::std::chrono;

/* REPRESENTACIÓN DE LAS TUPLAS: Vector de 0 o 1, 1 indica que se
selecciona dicho número, 0 que no se selecciona. */

void ProcesaVector(vector<int> & Tupla, vector<int> subconjunto, int sumaBuscada){
	int suma = 0;	
	
	for(int i = 0; i<subconjunto.size(); ++i)
		if(Tupla[i] == 1)
			suma+=subconjunto[i];
/*
	if(suma == sumaBuscada){
		for(int i = 0; i<subconjunto.size(); ++i)
		if(Tupla[i] == 1)
			//cout << subconjunto[i] << " ";
		//cout << endl;
	}*/
}

void completa_binario(vector<int> & V, vector<int>subconj, int pos, int sumaBuscada){ 
	
	if(pos==V.size())
		ProcesaVector(V, subconj, sumaBuscada);
	else{
		V[pos]=0;
		completa_binario(V,subconj, pos+1, sumaBuscada);
		V[pos]=1;
		completa_binario(V,subconj, pos+1, sumaBuscada);
	}
}

void SumaConjuntosFuerzaBruta(vector<int> subconjunto, int sumaBuscada){
	vector<int> tuplaIni;
	tuplaIni.resize(subconjunto.size());
	completa_binario(tuplaIni, subconjunto, 0, sumaBuscada);
}

int main(int argc, char* argv[]){

	if(argc < 3){
		cout << "Modo de empleo <" << argv[0] << "> <Tamaño> <Suma>" << endl;
		exit(-1);
	}

	int N = atoi(argv[1]);
	int sum = atoi(argv[2]);

	//Generar Vector:
	vector<int> sub;

	for(int i = 0; i<N; ++i){
		//cout << i*2+1 << " ";
		sub.push_back(i*2+1);
	}

	//cout << endl;

	high_resolution_clock::time_point tantes, tdespues;
  	duration<double> duracion;

  	tantes = high_resolution_clock::now();
  	SumaConjuntosFuerzaBruta(sub, sum);
  	tdespues = high_resolution_clock::now();

  	duracion = duration_cast<duration<double>>(tdespues - tantes);
  	cout << N << " " << duracion.count() << endl;

}


