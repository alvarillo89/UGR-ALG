///////////////////////////////////////////////
// Álvaro Fernández García
// Germán Castro Montes
// Alexandra-Diana Dumitru
// Ana María Romero Delgado
// Marino Fajardo Torres
///////////////////////////////////////////////

#include "continental.hpp"
#include <iostream>
#include <chrono>

using namespace std::chrono;
using namespace std;

int main(){
  Continental con;
  high_resolution_clock::time_point tantes, tdespues;
  duration<double> duracion;
  char res;

  cout << "Calculando una solución..." << endl;
  tantes = high_resolution_clock::now();
  con.JuegoContinental();
  tdespues = high_resolution_clock::now();

  duracion = duration_cast<duration<double>>(tdespues - tantes);
  cout << "Solución encontrada. Tiempo transcurrido: " << duracion.count() << " segundos" << endl;

  cout << "Desea ver la solución paso por paso? (s/n): ";
  cin >> res;
  res = tolower(res);
  while(res != 's' && res != 'n')
    cin >> res;

  if(res == 's'){
    con.ImprimeSolucionPasos();
    cout << endl;
  }
  else{
    cout << "Los pasos son:" << endl;
    con.ImprimeSolucion();
    cout << endl;
  }
}
