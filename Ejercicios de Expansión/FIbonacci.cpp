#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

int fiborec(int n){
	if(n <= 1)
		return n;
	else
		return (fiborec(n-1) + fiborec(n-2));
}


int fibomap_rec(map<int,int> & sol, int k){
	int fib, f1, f2;
	if(k < 2)
		fib = k;
	else{
		auto it1 = sol.find(k-1);
		if(it1 == sol.end())
			f1 = fibomap_rec(sol, k-1);
		else
			f1 = it1->second;
		
		auto it2 = sol.find(k-2);
		if(it2 == sol.end())
			f2 = fibomap_rec(sol, k-2);
		else
			f2 = it2->second;

		fib = f1 + f2;
	}

 	sol[k] = fib;
	return fib;
}


int fibomap(int n){
	map<int,int> soluciones;
	return fibomap_rec(soluciones, n);
}


int fibolineal(int n){
	int* T = new int[n+1];
	if(n <= 1) 
		return n;
	else{
		T[0]=0; 
		T[1]=1;
		for(int i = 2; i <= n; ++i)
			T[i]=T[i-1]+T[i-2];
	}
	int res = T[n];
	delete[] T;
	return res;
}

int main(int argc, char* argv[]){
  if(argc < 2){
    cout << "Modo de empleo " << argv[0] << " <tamaño>" << endl;
    exit(-1);
  }

  int N = atoi(argv[1]);

  
  //Medir tiempo
  high_resolution_clock::time_point tantes, tdespues;
  duration<double> duracion;

  tantes = high_resolution_clock::now();
  //fiborec(N);
  //fibolineal(N);
  fibomap(N);
  tdespues = high_resolution_clock::now();

  duracion = duration_cast<duration<double>>(tdespues - tantes);
  cout << N << " " << duracion.count() << endl;

  
}
