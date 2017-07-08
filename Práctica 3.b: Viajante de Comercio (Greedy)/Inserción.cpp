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

using namespace std;

//Función para leer los puntos del archivo:
void leer_puntos(string & nombre, map<int,pair<double,double>> & M){
    ifstream datos;
    string s;
    pair<double,double> p;
    int n,act;

    datos.open(nombre.c_str());
    if (datos.is_open()) {
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
   else
      cout << "Error de Lectura en " << nombre << endl;
}


//Función para calcular la distancia euclídea entre dos puntos:
double Distancia(pair<double,double> p1, pair<double,double> p2){
	return sqrt(pow(p2.first-p1.first,2) + pow(p2.second-p1.second,2));
}


//Función para calcular la matriz de distancias:
void CalculaMD(double** M, int n, map<int,pair<double,double>> & mapa){
	for(int i = 1; i<n; ++i)
		for(int j = 1; j<n; ++j)
			M[i][j] = Distancia(mapa[i], mapa[j]);
}


//Método para calcular un recorrido para el viajante de comercio basado en la inserción:
pair<vector<int>,double> ViajanteDeComercio(vector<int> & candidatos, double** MD, 	map<int,pair<double,double>> puntos){
  vector<int> Sol;
  double distancia = 0;

  ///////////////////////////////////////////////////////////////////////////
  //Partiremos de un cuadrado inicial:
  ///////////////////////////////////////////////////////////////////////////
  double N = 0, S = 1.84467e+19, E = 0, O = 1.84467e+19; //Coordenadas
  int cn = 0, cs = 0, ce = 0, co = 0; //Índice ciudad.
  vector<bool> seleccionados(candidatos.size()+1, false);

  for(auto it = puntos.begin(); it != puntos.end(); ++it){
    if((it->second).second > N && !seleccionados[it->first]){
      N = (it->second).second;
      seleccionados[cn] = false;
      cn = it->first;
      seleccionados[cn] = true;
    }
    if((it->second).second < S && !seleccionados[it->first]){
      S = (it->second).second;
      seleccionados[cs] = false;
      cs = it->first;
      seleccionados[cs] = true;
    }
    if((it->second).first > E && !seleccionados[it->first]){
      E = (it->second).first;
      seleccionados[ce] = false;
      ce = it->first;
      seleccionados[ce] = true;
    }
    if((it->second).first < O && !seleccionados[it->first]){
      O = (it->second).first;
      seleccionados[co] = false;
      co = it->first;
      seleccionados[co] = true;
    }
  }

  //Añadir las cuatro ciudades elegidas:
  Sol.push_back(cn);
  Sol.push_back(ce);
  Sol.push_back(cs);
  Sol.push_back(co);

  //Calcular la distancia que llevamos hasta ahora:
  distancia = MD[cn][ce] + MD[ce][cs] + MD[cs][co] + MD[co][cn];
  ///////////////////////////////////////////////////////////////////////////
  double mejor_coste, distancia_aux;
  int mejor_ciudad;
  vector<int>::iterator mejor_pos;

  //Habrá que repetir el proceso las siguientes veces:
  for(unsigned int i = 0; i < candidatos.size()-4; ++i){
    mejor_coste = 1.84467e+19;
    //Para cada una de las ciudades en candidatos:
    for(unsigned int j = 0; j < candidatos.size(); ++j){
      //Si no he insertado la ciudad:
      if(!seleccionados[candidatos[j]]){
        for(auto it = Sol.begin(); it != Sol.end(); ++it){
          if((it+1) == Sol.end())
            distancia_aux = distancia - MD[Sol.front()][Sol.back()] + MD[Sol.front()][candidatos[j]] + MD[Sol.back()][candidatos[j]];
          else
            distancia_aux = distancia - MD[*it][*(it+1)] + MD[*it][candidatos[j]] + MD[*(it+1)][candidatos[j]];
          //Si es mejor que lo que tenía actualizo:
          if(distancia_aux < mejor_coste){
            mejor_coste = distancia_aux;
            mejor_ciudad = candidatos[j];
            mejor_pos = it+1;
          }
        }
      }
    }
    //Aplicar los cambios:
    Sol.insert(mejor_pos, mejor_ciudad);
    seleccionados[mejor_ciudad] = true;
    distancia = mejor_coste;
  }

  return pair<vector<int>,double>(Sol, distancia);
}

int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "Modo de empleo: " << argv[0] << " <archivo.tsp>" << endl;
		exit(-1);
	}

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

	//Construir vector de candidatos:
	vector<int> candidatos;
	for(int i = 0; i<N; ++i)
		candidatos.push_back(i+1);

	//Calcular recorrido:
	pair<vector<int>,double> res = ViajanteDeComercio(candidatos, D, puntos);

	//Imprimir resultados:
	cout << "Orden de recorrido: ";
	for(unsigned int i = 0; i < res.first.size(); ++i)
		cout << res.first[i] << " ";
	cout << endl;

	cout << "Distancia: " << res.second << endl;

	for(int i = 0; i<N+1; ++i)
		delete[] D[i];
	delete[] D;

	exit(0);
}
