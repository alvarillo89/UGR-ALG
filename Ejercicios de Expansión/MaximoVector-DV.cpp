/**
   @file Ordenación por inserción
*/

   
#include <iostream>
using namespace std;
#include <ctime>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <chrono>

using namespace std::chrono;

int MaximoDV(int T[], int ini, int fin){
	int n1, n2, medio;
	if(ini == fin)
		return T[ini];
	else if(ini == fin-1){
		if(T[ini] > T[fin]) return T[ini];
		else return T[fin];
	}
	else{
		medio = (fin + ini) / 2;
		n1 = MaximoDV(T,ini, medio);
		n2 = MaximoDV(T, medio+1, fin);
		if(n1 < n2) return n2;
		else return n1;
	}
}



int main(int argc, char * argv[])
{

	
	high_resolution_clock::time_point tantes, tdespues;
	duration<double> transcurrido;  

    if (argc != 2)
    {
      cerr << "Formato " << argv[0] << " <num_elem>" << endl;
      return -1;
    }

  int n = atoi(argv[1]);

  int * T = new int[n];
  assert(T);

  srandom(time(0));

  for (int i = 0; i < n; i++)
    {
      T[i] = random();
    };

  tantes = high_resolution_clock::now();
  MaximoDV(T, 0, n);
  tdespues = high_resolution_clock::now();
  transcurrido = duration_cast<duration<double>>(tdespues - tantes);
  cout << n << " " << transcurrido.count() << endl;

  delete [] T;

  return 0;
};
 
