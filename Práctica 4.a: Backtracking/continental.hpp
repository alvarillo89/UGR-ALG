///////////////////////////////////////////////
// Álvaro Fernández García
// Germán Castro Montes
// Alexandra-Diana Dumitru
// Ana María Romero Delgado
// Marino Fajardo Torres
///////////////////////////////////////////////

#ifndef _CONTINENTAL_H
#define _CONTINENTAL_H

  #include<iostream>
  #include<vector>

  using namespace std;

  class Continental{
  public:
    //Constructor: Inicializa el tablero.
    Continental();

    //Busca una solución al problema del Continental.
    void JuegoContinental();

    //Imprime la secuencia de pasos para resolver el juego:
    //Precondición: Se ha encontrado una solución.
    void ImprimeSolucion();

    //Imprime la solución paso por paso mostrando el tablero:
    //Precondición: Se ha encontrado una solución.
    void ImprimeSolucionPasos();

  private:
    //Struct que representa un movimiento:
    struct Movimiento{
      pair<int,int> posOriginal;
      pair<int,int> posFinal;
      pair<int,int> fichaComida;
    };

    //Atributos privados del algoritmo:
    char Tablero[7][7];
    vector<Movimiento> Camino;
    static const int MOVIMIENTOS = 31;
    bool encontrado;

    //Funciones privadas:
    //Comprueba si un movimiento se puede realizar o no (es factible)
    bool MovFactible(int fil, int col, int mov);

    //Aplica un movimiento al tablero.
    void ActualizaTablero(int k);

    //Deja el tablero en un estado anterior:
    void RestauraTablero(int pos);

    //Crea el struct movimiento correspondiente a ese nivel:
	// 1 = izquierda, 2 = arriba, 3 = derecha, 4 = abajo.    
	void ConstruyeMovimiento(int i, int j, int mov, int k);

    //Comprueba si una solución es válida o no:
    bool ProcesaSolucion();

    //Función recursiva para jugar al continental:
    //Método de backtracking:
    void JuegoContinental_rec(int k);

    //Imprime el Tablero:
    void ImprimeTablero();
  };

#endif
