//////////////////////////////////////////////////////////////////
// Señales Regulares:
// Álvaro Fernández García
// Alexandra-Diana Dumitru
// Ana María Romero Delgado
// Germán Castro Montes
// Marino Fajardo Torres
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <cassert>

using namespace std;

//Comentar y descomentar para medir tiempos o mostrar resultados:
//#define MIDE_TIEMPO
#define SALIDA

#ifdef MIDE_TIEMPO
  #include<chrono>
  using namespace std::chrono;
#endif

// Umbral a partil del cual se utilizará el método clásico en divide y vencerás.
#define UMBRAL 100

// Estructura que almacena la solución:
// Almacena la longitud de la secuencia, el M, donde empieza y donde acaba.
struct solucion{
  int longitud;
  int comienzo;
  int fin;
  int M;
};

// Método clásico para resolver el problema. O(n)
// Recibe el vector, y los límites donde aplicar el algoritmo.
solucion MetodoClasico_lims(int V[], int ini, int fin){
  solucion res;
  int com, M, i = ini, longMax = -1;

  M = abs(V[i]-V[i+1]);
  com = i;
  i++;

  while(i+1 <= fin){
    if(abs(V[i]-V[i+1]) != M){
      if((i-com)+1 > longMax){
        res.comienzo = com;
        res.fin = i;
        longMax = (i-com)+1;
        res.longitud = longMax;
        res.M = M;
      }
      com = i;
      M = abs(V[i] - V[i+1]);
    }
    else
      i++;
  }
  //Comprobar si la mayor secuencia es la última:
  if((i-com)+1 > longMax){
    res.comienzo = com;
    res.fin = i;
    longMax = (i-com)+1;
    res.longitud = longMax;
    res.M = M;
  }
  return res;
}

//Método clásico que recibe el vector y su tamaño:
solucion MetodoClasico(int V[], int n){
  return MetodoClasico_lims(V, 0, n-1);
}

//Calcula la primera subsecuencia de una secuencia dada por sus limites.
// Se utilizará en al método Divide y Vencerás para conectar posibles secuencias cortadas.
solucion PrimeraSubsecuencia(int V[], int ini, int fin){
  solucion res;
  int com, M, i = ini;
  bool continua = true;

  M = abs(V[i]-V[i+1]);
  com = i;
  i++;

  while(continua && i+1<=fin){
    if(abs(V[i]-V[i+1]) != M){
        res.comienzo = com;
        res.fin = i;
        res.longitud = (i-com)+1;
        res.M = M;
        continua = false;
    }
    else
      i++;
  }
  // Comprobar si toda la subsecuencia es en sí una señal regular:
  if(continua){
    res.comienzo = com;
    res.fin = i;
    res.longitud = (i-com)+1;
    res.M = M;
  }
  return res;
}

//Calcula la última subsecuencia de una secuencia dada por sus limites.
// Se utilizará en al método Divide y Vencerás para conectar posibles secuencias cortadas.
solucion UltimaSubsecuencia(int V[], int ini, int fin){
  solucion res;
  int com, M;
  int i = fin;
  bool continua = true;

  M = abs(V[i]-V[i-1]);
  com = i;
  i--;

  while(continua && i-1 >= ini){
    if(abs(V[i]-V[i-1]) != M){
        res.comienzo = i;
        res.fin = com;
        res.longitud = (com-i)+1;
        res.M = M;
        continua = false;
    }
    else
      i--;
  }
  // Comprobar si toda la subsecuencia es en sí una señal regular:
  if(continua){
    res.comienzo = i;
    res.fin = com;
    res.longitud = (com-i)+1;
    res.M = M;
  }
  return res;
}

// Método Divide y Vencerás para resolver el problema. O(nlog(n))
// Recibe el vector, y los límites donde aplicar el algoritmo.
solucion MetodoDivideVenceras_lims(int V[], int ini, int fin){
  solucion sal1, sal2, subcadena_inf, subcadena_sup, union_cadenas, res;
  int centro, max;
  union_cadenas.longitud = -1;
  union_cadenas.comienzo = -1;
  union_cadenas.fin = -1;
  union_cadenas.M = -1;

  if(fin - ini < UMBRAL)
    //Aplicar método clásico si esta por debajo del umbral:
    return MetodoClasico_lims(V, ini, fin);
  else{
    centro = (ini + fin)/2;
    sal1 = MetodoDivideVenceras_lims(V,ini, centro);
    //Calcula la ultima subsecuencia de la parte inferior:
    subcadena_sup = UltimaSubsecuencia(V, ini, centro);
    sal2 = MetodoDivideVenceras_lims(V, centro+1, fin);
    //Calcular la primera subsecuencia de la parte superior:
    subcadena_inf = PrimeraSubsecuencia(V, centro+1, fin);

    //Recombinar:
    //Calcular la M de donde hemos cortado las cadenas:
    union_cadenas.M = abs(V[subcadena_sup.fin] - V[subcadena_inf.comienzo]);

    //Comprobar si se ha cortado alguna serie por la mitad y si es así volver a unirlas:
    if(subcadena_inf.M == subcadena_sup.M && subcadena_sup.M == union_cadenas.M && subcadena_inf.M == union_cadenas.M){
      union_cadenas.longitud = subcadena_inf.longitud + subcadena_sup.longitud;
      union_cadenas.comienzo = subcadena_sup.comienzo;
      union_cadenas.fin = subcadena_inf.fin;
    }
    else if(subcadena_inf.M == union_cadenas.M){
      union_cadenas.longitud = subcadena_inf.longitud + 1;
      union_cadenas.comienzo = subcadena_sup.fin;
      union_cadenas.fin = subcadena_inf.fin;
    }
    else if(subcadena_sup.M == union_cadenas.M){
      union_cadenas.longitud = subcadena_sup.longitud + 1;
      union_cadenas.comienzo = subcadena_sup.comienzo;
      union_cadenas.fin = subcadena_inf.comienzo;
    }

    //Devolver la cadena con mayor longitud de las construidas:
    res = sal1;
    max = sal1.longitud;

    if(union_cadenas.longitud > max){
      res = union_cadenas;
      max = union_cadenas.longitud;
    }
    if(sal2.longitud > max)
      res = sal2;
  }
  return res;
}

//Método divide y vencerás que recibe el vector y su tamaño.
solucion MetodoDivideVenceras(int V[], int n){
  return MetodoDivideVenceras_lims(V, 0, n-1);
}

//Genera un número uniformemente distribuido en el intervalo [0,1) a partir de
//uno de los generadores disponibles en C.
double uniforme(){
  int t = rand();
  double f = ((double)RAND_MAX+1.0);
  return (double)t/f;
}

int main(int argc, char * argv[]){

  if(argc != 2){
    cerr << "Formato " << argv[0] << " <num_elem>" << endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * T = new int[n];
  assert(T);

  srand(time(0));

  for(int j=0; j<n; j++) {
    double u = uniforme();
    T[j]= (int) (21*u);
  }

  // Pruebas:
  //int v1[] = {0,1,3,5,3,5,0,10,9,10,11,12,11,11,0,0};
  //int v2[] = {4,2,4,6,8,6,4,2,0,1,2,10,2,0,3,5,3,0};
  //int v3[] = {0,15,1,28,10,15,20,15,10};

  #ifdef SALIDA
    for (int j=0; j<n; j++)
      cout << T[j] << " ";
    cout << endl;

    solucion res  = MetodoClasico(T, n);
    solucion res2 = MetodoDivideVenceras(T, n);

    cout << "(" << res.comienzo << "-" << res.fin << ") M = " << res.M << " L = " << res.longitud << endl;
    cout << "(" << res2.comienzo << "-" << res2.fin << ") M = " << res2.M << " L = " << res2.longitud << endl;
  #endif

  #ifdef MIDE_TIEMPO
    high_resolution_clock::time_point tantes, tdespues;
    duration<double> duracion;

    tantes = high_resolution_clock::now();
    MetodoClasico(T, n);
    //MetodoDivideVenceras(T, n);
    tdespues = high_resolution_clock::now();

    duracion = duration_cast<duration<double>>(tdespues - tantes);
    cout << n << " " << duracion.count() << endl;
  #endif

  delete[] T;
}
