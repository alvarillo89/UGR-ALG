//////////////////////////////////////////////////////////////////
// Álvaro Fernández García
// Alexandra-Diana Dumitru
// Ana María Romero Delgado
// Germán Castro Montes
// Marino Fajardo Torres
// NOTA: compilar con -std=c++11
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <vector>
#include <queue>
#include <limits>
#include <chrono>

using namespace std;
using namespace std::chrono;

//////////////////////////////////////////////////////////////////////////////////////////////////
//Representa una solución al problema del viajante de comercio:
struct Solucion{
  int k;                              //Representa el nivel por el que va el nodo en el árbol.
  double cota_local;                  //Representa la cota local del nodo.
  vector<int> recorrido;              //Representa el circuito solución.
  vector<int> ciudadesNoElegidas;     //Representa las ciudades que aún faltan por elegir.
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//Functor empleado para la priority_queue:
class Comparator{
public:
  bool operator()(const Solucion & s1, const Solucion & s2){
    return s1.cota_local > s2.cota_local;
  }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función para leer los puntos del archivo:
void leer_puntos(string & nombre, map<int,pair<double,double>> & M){
  ifstream datos;
  string s;
  pair<double,double> p;
  int n,act;

  datos.open(nombre.c_str());
  if(datos.is_open()) {
 	  datos >> s; // Leo "dimension:"
		datos >> n;
    int i=0;

	  while(i<n){
  	   datos >> act >> p.first >> p.second;
		   M[act] = p;
		   i++;
    }
 	  datos.close();
 }
 else{
   cout << "Error de Lectura en " << nombre << endl;
   exit(-1);
 }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función para calcular la distancia euclídea entre dos puntos:
double Distancia(pair<double,double> p1, pair<double,double> p2){
	return sqrt(pow(p2.first-p1.first,2) + pow(p2.second-p1.second,2));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función para calcular la matriz de distancias:
void CalculaMD(double** M, int n, map<int,pair<double,double>> & mapa){
	for(int i = 1; i<n; ++i)
		for(int j = 1; j<n; ++j)
			M[i][j] = Distancia(mapa[i], mapa[j]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Método para calcular el coste de una solución:
double calculaDistancia(vector<int> Solucion, double** MD){
  double coste = 0;
  for(unsigned int i = 0; i < Solucion.size()-1; ++i)
    coste += MD[Solucion[i]][Solucion[i+1]];

  coste += MD[Solucion.front()][Solucion.back()];

  return coste;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Sirve para calcular la cota global inicial mediante el vecino más cercano:
double VecinoMasCercano(vector<int> & candidatos, double** MD){

	//Variables para el calculo de la distancia;
	vector<int> S, Stemp, candidatos_tmp;
	double MejorDist = -1.0, distTemp, min_dist;
	int c_actual, c_siguiente;

	//Calcular la distancia partiendo de distintas ciudades:
	for(int i = 0; i < candidatos.size(); ++i){

		distTemp = 0.0;
		candidatos_tmp = candidatos;
		c_actual = candidatos_tmp[i];
		Stemp.push_back(c_actual); //Añadir a la lista de seleccionados.
		candidatos_tmp.erase(candidatos_tmp.begin()+i); //Borrar de entre los candidatos la ciudad.

		while(!candidatos_tmp.empty()){

			//Función Selección. El vecino más cercano:
			min_dist = MD[c_actual][candidatos_tmp[0]];
			c_siguiente = 0;
			for(int j = 1; j < candidatos_tmp.size(); ++j)
				if(MD[c_actual][candidatos_tmp[j]] < min_dist){
					c_siguiente = j;
					min_dist = MD[c_actual][candidatos_tmp[j]];
				}
			//Las ciudades elegidas son factibles, ya que las ya elegidas no figuran en candidatos.
			distTemp += min_dist;
			Stemp.push_back(candidatos_tmp[c_siguiente]);
			c_actual = candidatos_tmp[c_siguiente];
			candidatos_tmp.erase(candidatos_tmp.begin()+c_siguiente);
		}

		//Añadir a distTemp la distancia entre la primera y la última ciudad:
		distTemp += MD[Stemp.front()][Stemp.back()];

		//Comprobar si es mejor que la que ya había:
		if(MejorDist == -1.0){
			MejorDist = distTemp;
			S = Stemp;
		}
		else if(distTemp < MejorDist){
			MejorDist = distTemp;
			S = Stemp;
		}
		Stemp.clear(); // Vaciar la solución temporal.
	}

	return MejorDist;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función que calcula la cota local de un nodo:
void calculaCotaLocal(Solucion & sol, double **MD, int N){
  double cota = 0.0;
  //Calcular la distancia que lleva el recorrido:
  for(int i = 0; i<sol.k; ++i)
    cota += MD[sol.recorrido[i]][sol.recorrido[i+1]];

  //Calcular el mínimo recorrido para salir de cada una de las ciudades restantes:
  double min;
  for(int i = 0; i<sol.ciudadesNoElegidas.size(); ++i){
    min = numeric_limits<double>::max();
    for(int j = 1; j<N+1; ++j)
      if(sol.ciudadesNoElegidas[i] != j && MD[sol.ciudadesNoElegidas[i]][j] < min)
        min = MD[sol.ciudadesNoElegidas[i]][j];
    cota += min;
  }

  //Añadir el mínimo recorrido para salir de la última ciudad elegida:
  min = numeric_limits<double>::max();
  for(int i = 1; i<N+1; ++i)
    if(i != sol.recorrido[sol.k] && MD[i][sol.recorrido[sol.k]] < min)
      min = MD[i][sol.recorrido[sol.k]];
  cota += min;

  sol.cota_local = cota;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función que crea la raiz del árbol: (Actúa como constructor)
Solucion Raiz(int N, double** MD){
  Solucion sal;
  sal.k = 0;
  sal.recorrido.resize(N);
  sal.recorrido[sal.k] = 1; //Empezamos desde la primera ciudad.
  for(int i = 2; i<=N; ++i)
    sal.ciudadesNoElegidas.push_back(i);
  calculaCotaLocal(sal, MD, N);
  return sal;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función que crea el sucesor de un nodo:
Solucion generaHijo(const Solucion & padre, int ciudad, int N, double** MD){
  Solucion hijo = padre;
  hijo.k = padre.k + 1;
  hijo.recorrido[hijo.k] = ciudad;
  bool enc = false;
  for(auto it = hijo.ciudadesNoElegidas.begin(); it != hijo.ciudadesNoElegidas.end() && !enc; ++it)
    if(*it == ciudad){
      enc = true;
      hijo.ciudadesNoElegidas.erase(it);
    }
  calculaCotaLocal(hijo, MD, N);
  return hijo;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//Función B&B para el Viajante de Comercio:
void ViajanteComercioBandB(int N, double** MD, vector<int> & candidatos){
  int ciudad, nodos_exp = 0, num_podas = 0;
  int tam_max = numeric_limits<int>::min();
  double distancia = 0.0;
  Solucion e_node, hijo, mejorSolucion;
  priority_queue<Solucion, vector<Solucion>, Comparator> Q;
  double CG = VecinoMasCercano(candidatos, MD);
  e_node = Raiz(N, MD);
  Q.push(e_node);

  while(!Q.empty() && (Q.top().cota_local < CG)){
    e_node = Q.top();
    Q.pop();
    nodos_exp++;
    for(int i = 0; i<e_node.ciudadesNoElegidas.size(); ++i){
      ciudad = e_node.ciudadesNoElegidas[i];
      hijo = generaHijo(e_node, ciudad, N, MD);
      if(hijo.k == N-1){
        distancia = calculaDistancia(hijo.recorrido, MD);
        if(distancia < CG){
          CG = distancia;
          mejorSolucion = hijo;
        }
      }
      else if(hijo.cota_local < CG)
        Q.push(hijo);
      else
        num_podas++;
      if((int) Q.size() > tam_max) tam_max = (int) Q.size();
    }
  }
  //Imprimir resultados:
  cout << "Recorrido: ";
  for(int i = 0; i<mejorSolucion.recorrido.size(); ++i)
    cout << "[" << mejorSolucion.recorrido[i] << "]";
  cout << endl;
  cout << "Distancia: " << CG << endl;
  cout << "Nodos expandidos: " << nodos_exp << endl;
  cout << "Podas realizadas: " << num_podas << endl;
  cout << "Tamaño máximo de la cola: " << tam_max << endl;
}


int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "Modo de empleo: " << argv[0] << " <archivo.tsp>" << endl;
		exit(-1);
	}

  high_resolution_clock::time_point tantes, tdespues;
  duration<double> duracion;
	string archivo(argv[1]);
	map<int,pair<double,double>> puntos;

	//Leer los puntos:
	leer_puntos(archivo, puntos);
	int N = puntos.size();

	//Reservar matriz de distancia:
	double** D = new double*[N+1];
	for(int i = 0; i<N+1; ++i)
		D[i] = new double[N+1];

	//Calcular matriz de Distancias
	CalculaMD(D, N+1, puntos);

  //Crear el vector de candidatos para el greedy:
	vector<int> cand;
	for(int i = 1; i<=N; ++i)
		cand.push_back(i);

  tantes = high_resolution_clock::now();
  ViajanteComercioBandB(N, D, cand);
  tdespues = high_resolution_clock::now();

  duracion = duration_cast<duration<double>>(tdespues - tantes);
  cout << "Tiempo transcurrido: " << duracion.count() << " segundos" << endl;

  for(int i = 0; i<N+1; ++i)
    delete[] D[i];
  delete[] D;

  exit(0);
}
