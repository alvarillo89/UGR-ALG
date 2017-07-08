//////////////////////////////////////////////////////////////////
// Álvaro Fernández García
// Alexandra-Diana Dumitru
// Ana María Romero Delgado
// Germán Castro Montes
// Marino Fajardo Torres
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <list>


using namespace std;

list<int> cola;

double uniforme(){
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

// Determina si un nodo es hoja o no, recibe la matriz de adaycencia y el tamaño.
bool isLeaf(int x, int** MA, int n){
  for(int i = 0; i < n; ++i)
    if(MA[x][i] == 1)
      return false;

    return true;
}

// Poda un subárbol. Aplica los cambios sobre la matriz de adyacencia.
void Podar(int** & MA, int x, int n){
  for(int i = 0; i < n; ++i)
    MA[x][i] = 0;

  int j = x-1;
  bool continua = true;
  while(j >= 0 && continua){
    if(MA[j][x] == 1){
      MA[j][x] = 0;
      continua = false;
    }
    j--;
  }


}

//Algoritmo voraz que busca el recubrimiento minimal de un árbol.
list<int> RecubrimientoMinimal(list<int> Candidatos, int** MA, int nodes){
  list<int> Solucion;
  int x;

  while(!Candidatos.empty()){
    x = Candidatos.front();
    Candidatos.pop_front();

    if(!isLeaf(x, MA, nodes)){
      Solucion.push_back(x);
      Podar(MA, x, nodes);
    }
  }

  return Solucion;
}

int main(int argc, char * argv[]){
  if (argc != 3){
      cerr << "Formato " << argv[0] << " <num_nodes>" <<  " <num_hijos_max>" << endl;
      return -1;
  }

  int numnodes = atoi(argv[1]);
  int maxh=atoi(argv[2]);


  //genera un arbol aleatorio con un numero (numnodes) de nodos y un maximo (maxh) de hijos de cada nodo
  /*------------------------------------------------------------------------------------------------------*/
  srand(time(NULL));
  int **v;
  v =  new int * [numnodes];
  assert(v);

 for (int i = 0; i < numnodes; i++)
      v[i]= new int [numnodes];

  int n=1; //cuenta el número de nodos generados hasta ahora
  int i=0; //etiqueta del nodo
  cola.push_back(i); //es una cola FIFO
  while (n < numnodes) {
      i=cola.front();
      cola.pop_front();
      double u=uniforme();
      int ch=1+(int)(maxh*u); //entero aleatorio entre 1 y maxh
      if ((ch+n) > numnodes) ch=numnodes-n;
      for (int j=n; j<ch+n; j++) {
         v[i][j]=1; //v es la matriz de adyacencia del árbol
         cola.push_back(j);
      }
      n=n+ch;
  }
  while (!cola.empty())
    cola.pop_front();

/*------------------------------------------------------------------------------------------------------*/

  cout << "Matriz de adyacencia: " << endl;

  for (int i=0; i<numnodes; i++) {
   for (int j=0; j<numnodes; j++)
     cout << v[i][j] << " ";
   cout << " " << endl;
  }

  //Construir lista de candidatos:
  list<int> Candidatos;
  for(int i = n-1; i >= 0; --i)
    Candidatos.push_back(i);

  list<int> sal = RecubrimientoMinimal(Candidatos, v, numnodes);

  cout << "Nodos seleccionados: " << endl;
  for(auto it = sal.begin(); it != sal.end(); ++it)
    cout << *it << " ";

  cout << endl;
}
