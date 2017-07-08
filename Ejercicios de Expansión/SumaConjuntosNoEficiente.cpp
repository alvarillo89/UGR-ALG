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

public:
	
	Solucion(const int tam_max, const vector<int> & sub, const int sum){
		Subconjunto = sub;
		X.resize(tam_max);
		sumaBuscada = sum;
	} 
	
	int size() const{
		return X.size();
	}

	void IniciaComp(int k){
		X[k] = 2; // Valor NULO 
	}

	void SigValComp(int k){ 
		X[k]--; // Siguiente valor del dominio. 
	}

	bool TodosGenerados(int k){
		return X[k]== -1; //END
	}

	bool Factible(int k){
		int pesoActual = 0, pesoRestante = 0;
		
		for(int i = 0; i<=k; ++i)
			if(X[i] == 1)
				pesoActual+=Subconjunto[i];
	
		for(int i = k+1; i<Subconjunto.size(); ++i)
			pesoRestante+=Subconjunto[i];

		if((pesoActual == sumaBuscada) || ((pesoActual+pesoRestante >= sumaBuscada) && (pesoActual+Subconjunto[k+1] <= sumaBuscada)))
			return true;

		return false;
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
			if(Sol.Factible(k))
				back_recursivo(Sol, k+1);
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
