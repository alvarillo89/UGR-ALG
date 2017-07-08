///////////////////////////////////////////////
// Álvaro Fernández García
// Germán Castro Montes
// Alexandra-Diana Dumitru
// Ana María Romero Delgado
// Marino Fajardo Torres
///////////////////////////////////////////////

#include "continental.hpp"

Continental::Continental(){
  Camino.resize(MOVIMIENTOS);
  for(unsigned i = 0; i<7; ++i)
    for(unsigned j = 0; j<7; ++j){
      if((i == 0 || i == 1 || i == 5 || i == 6) && (j == 0 || j == 1 || j == 5 || j == 6))
        Tablero[i][j] = 'X';
      else
        Tablero[i][j] = 'O';
    }
  Tablero[3][3] = '-';
  encontrado = false;
}


bool Continental::MovFactible(int fil, int col, int mov){
  switch (mov) {
    case 1: //izquierda
      if(col-2 > -1) //No estas fuera de la matriz
        if(Tablero[fil][col-2] != 'X') //pos valida.
          if(Tablero[fil][col-2] == '-' && Tablero[fil][col-1] == 'O')
              return true;
      break;
    case 2: //arriba
      if(fil-2 > -1) //No estas fuera de la matriz
        if(Tablero[fil-2][col] != 'X') //pos valida.
          if(Tablero[fil-2][col] == '-' && Tablero[fil-1][col] == 'O')
              return true;
      break;
    case 3: //derecha
      if(col+2 < 7) //No estas fuera de la matriz
        if(Tablero[fil][col+2] != 'X') //pos valida.
          if(Tablero[fil][col+2] == '-' && Tablero[fil][col+1] == 'O')
              return true;
      break;
    case 4: //abajo
      if(fil+2 < 7) //No estas fuera de la matriz
        if(Tablero[fil+2][col] != 'X') //pos valida.
          if(Tablero[fil+2][col] == '-' && Tablero[fil+1][col] == 'O')
              return true;
      break;
  }
  return false;
}


void Continental::ActualizaTablero(int k){
  Tablero[Camino[k].posOriginal.first][Camino[k].posOriginal.second] = '-';
  Tablero[Camino[k].posFinal.first][Camino[k].posFinal.second] = 'O';
  Tablero[Camino[k].fichaComida.first][Camino[k].fichaComida.second] = '-';
}


void Continental::RestauraTablero(int pos){
  Tablero[Camino[pos].posOriginal.first][Camino[pos].posOriginal.second] = 'O';
  Tablero[Camino[pos].posFinal.first][Camino[pos].posFinal.second] = '-';
  Tablero[Camino[pos].fichaComida.first][Camino[pos].fichaComida.second] = 'O';
}


void Continental::ConstruyeMovimiento(int i, int j, int mov, int k){
  Camino[k].posOriginal.first = i;
  Camino[k].posOriginal.second = j;

  switch (mov) {
    case 1:
      Camino[k].posFinal.first = i;
      Camino[k].posFinal.second = j-2;
      Camino[k].fichaComida.first = i;
      Camino[k].fichaComida.second = j-1;
      break;
    case 2:
      Camino[k].posFinal.first = i-2;
      Camino[k].posFinal.second = j;
      Camino[k].fichaComida.first = i-1;
      Camino[k].fichaComida.second = j;
      break;
    case 3:
      Camino[k].posFinal.first = i;
      Camino[k].posFinal.second = j+2;
      Camino[k].fichaComida.first = i;
      Camino[k].fichaComida.second = j+1;
      break;
    case 4:
      Camino[k].posFinal.first = i+2;
      Camino[k].posFinal.second = j;
      Camino[k].fichaComida.first = i+1;
      Camino[k].fichaComida.second = j;
      break;
  }
}


bool Continental::ProcesaSolucion(){
  int nfichas = 0;
  for(int i = 0; i<7; ++i)
    for(int j = 0; j<7; ++j)
      if(Tablero[i][j] == 'O')
        nfichas++;

  if(nfichas != 1)
    return false;

  for(int i = 0; i<7; ++i)
    for(int j = 0; j<7; ++j)
      if(Tablero[i][j] == 'O')
        if(!(i == 3 && j == 3))
          return false;

  return true;
}


void Continental::ImprimeSolucion(){
  for(unsigned i = 0; i < Camino.size(); ++i)
    cout << "[<" << Camino[i].posOriginal.first << "," << Camino[i].posOriginal.second << "><" << Camino[i].posFinal.first << "," << Camino[i].posFinal.second << ">]";
}


void Continental::ImprimeSolucionPasos(){
    Continental aux;
    aux.Camino = this->Camino;
    cout << endl << "Situación inicial: " << endl << endl;
    aux.ImprimeTablero();
    cin.get();
    cout << endl;
    for(unsigned i = 0; i < aux.Camino.size(); ++i){
      cout << "Paso " << i+1 << " de " << MOVIMIENTOS << " : ";
      cout << "[<" << aux.Camino[i].posOriginal.first << "," << aux.Camino[i].posOriginal.second << "><" << aux.Camino[i].posFinal.first << "," << aux.Camino[i].posFinal.second << ">]" << endl;
      cout << endl;
      aux.ActualizaTablero(i);
      aux.ImprimeTablero();
      cout << endl << "Pulsa enter para ver el siguiente paso..." << endl;
      cin.get();
    }
}


void Continental::ImprimeTablero(){
  for(int i = 0; i<7; ++i){
    for(int j = 0; j<7; ++j){
      cout << "[";
      if(Tablero[i][j] == 'X')
        cout << "\e[1;31mX\e[0m";
      else if(Tablero[i][j] == 'O')
        cout << "\e[1;34mO\e[0m";
      else
        cout << " ";
      cout << "]";
    }
    cout << endl;
  }
}


void Continental::JuegoContinental_rec(int k){
  if(ProcesaSolucion())
    encontrado = true;
  else{
    //Buscar la siguiente ficha:
    for(int i = 0; i<7 && !encontrado; ++i)
      for(int j = 0; j<7 && !encontrado; ++j)
        if(Tablero[i][j] == 'O')
          //Siguiente movimiento para esa ficha:
          for(int m = 1; m <= 4 && !encontrado; ++m)
            if(MovFactible(i,j,m)){
              ConstruyeMovimiento(i,j,m,k);
              ActualizaTablero(k);
              JuegoContinental_rec(k+1);
            }
  }
  if(!encontrado)
    RestauraTablero(k-1);

}

void Continental::JuegoContinental(){
  JuegoContinental_rec(0);
}
