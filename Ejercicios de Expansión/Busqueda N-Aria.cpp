#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

int BusquedaBinaria(int T[], int N, int busca){
  int inf = 0;
  int sup = N-1;
  int centro;

  while(inf <= sup){
    centro = (inf+sup)/2;
    if(T[centro] == busca)
      return centro;
    else{
      if(T[centro] < busca)
        inf = centro + 1;
      else
        sup = centro -1;
    }
  }
  return -1;
}

int BusquedaTernaria(int T[], int N, int busca){
  int inf = 0;
  int sup = N-1;
  int tercio;

  while(inf <= sup){
    tercio = (sup-inf+1)/3;
    if(T[inf+tercio] == busca)
      return (inf+tercio);
    if(T[sup-tercio] == busca)
      return (sup-tercio);
    else{
      if(busca < T[inf+tercio])
        sup = inf+tercio-1;
      else if(T[inf+tercio] < busca && busca < T[sup-tercio]){
        inf = inf+tercio+1;
        sup = sup-tercio-1;
      }
      else
        inf = sup-tercio+1;
    }
  }
  return -1;
}

int main(int argc, char* argv[]){
  if(argc < 2){
    cout << "Modo de empleo " << argv[0] << " <tamaño>" << endl;
    exit(-1);
  }

  int N = atoi(argv[1]);

  //Generar vector ordenado:
  int* V = new int[N];
  for(int i = 0; i < N; ++i)
    V[i] = i;

  //Generar elemento aleatorio que buscar:
  srand(time(NULL));
  int buscar = 1;

  //Medir tiempo
  high_resolution_clock::time_point tantes, tdespues;
  duration<double> duracion;

  tantes = high_resolution_clock::now();
  //BusquedaBinaria(V, N, buscar);
  BusquedaTernaria(V, N, buscar);
  tdespues = high_resolution_clock::now();

  duracion = duration_cast<duration<double>>(tdespues - tantes);
  cout << N << " " << duracion.count() << endl;

  delete[] V;
}
