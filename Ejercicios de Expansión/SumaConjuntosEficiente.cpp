#include <iostream>
#include <chrono>
#include <vector>

using namespace::std;
using namespace::std::chrono;

class Solucion{
private:
	vector<int> X;
	vector<int> Subconjunto;
	int sumaBuscada;
	int s;
	int r;

public:
	
	Solucion(const int tam_max, const vector<int> & sub, const int sum){
		Subconjunto = sub;
		X.resize(tam_max);
		sumaBuscada = sum;
		s = r = 0;
		for(int i = 0; i<Subconjunto.size(); ++i)
			r+=Subconjunto[i];

	} 
	
	int size() const{
		return X.size();
	}

	void IniciaComp(int k){
		X[k] = 2; // Valor NULO 
	}

	void SigValComp(int k){ 
		// Orden de los valores 2 -> 1 -> 0
		X[k]--;
		if (X[k] == 1) { s = s + Subconjunto[k]; r = r-Subconjunto[k]; }
		if (X[k] == 0) s = s - Subconjunto[k]; // Descontamos el valor
	}

	bool TodosGenerados(int k){
		return X[k]== -1; //END
	}

	bool Factible(int k){
		bool fact = false;
		if(((s+Subconjunto[k+1] <= sumaBuscada) && (s+r >= sumaBuscada) ) || (s==sumaBuscada) ) fact = true;
		return fact;
	}

	void VueltaAtras( int pos ){ 
		if (pos==X.size()) { return;}
		r = r+Subconjunto[pos];
		X[pos] = 2;
	}
		
	void ProcesaSolucion(){
		int suma = 0;	
	
		for(int i = 0; i<size(); ++i)
			if(X[i] == 1)
				suma+=Subconjunto[i];
/*
		if(suma == sumaBuscada){
			for(int i = 0; i<size(); ++i)
			if(X[i] == 1)
				cout << Subconjunto[i] << " ";
			cout << endl;
		}*/
	}
};

void back_recursivo(Solucion & Sol, int k){
	if (k == Sol.size())
		Sol.ProcesaSolucion();
	else{
		Sol.IniciaComp(k);
		Sol.SigValComp(k);
		while (!Sol.TodosGenerados(k)){
			if(Sol.Factible(k)){
				back_recursivo(Sol, k+1);
				Sol.VueltaAtras(k+1);
			}
			Sol.SigValComp(k);
		}
	}
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

	Solucion sol(N, sub, sum);

	high_resolution_clock::time_point tantes, tdespues;
  	duration<double> duracion;

  	tantes = high_resolution_clock::now();
	back_recursivo(sol, 0);
  	tdespues = high_resolution_clock::now();

  	duracion = duration_cast<duration<double>>(tdespues - tantes);
  	cout << N << " " << duracion.count() << endl;

}
