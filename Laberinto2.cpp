/*
* Algoritmo para hallar la meta de un laberinto
*/
#include <iostream>
#include <vector> // Biblioteca para usar vectores
#include <stack> // Biblioteca para usar pilas
#include <queue> // Biblioteca para usar colas
#include "windows.h" // Biblioteca para imprimir caracteres especiales (tildes, eñes...)
#include <chrono> // Biblioteca para calcular tiempo de ejecución

using namespace std;
using namespace chrono;

void mostrarLaberinto(int* matriz, int cantFilas, int cantCols) {
    for (int i = 0; i < cantFilas; ++i) {
        for (int j = 0; j < cantCols; ++j) {
            // Obtiene e imprime el elemento correspondiente para formar una matriz
            cout << matriz[(i * cantCols) + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

class Laberinto {
public:

    // Constructor
    Laberinto(int* labeParam, int &numHori, int &numVert, int filaI, int coluI, int &filaF, int &coluF) {
        labeClase = labeParam;
        filaInicio = filaI;
        coluInicio = coluI;
        filaFinal = filaF;
        coluFinal = coluF;
        numFilas = numHori;
        numCols = numVert;
        visitados = vector<vector<bool>>(numFilas, vector<bool>(numCols, false));
    }

    // Funciones públicas

    int encontrarSalidaDFS(int filaInicio, int coluInicio) {
        return buscarProfundidad(filaInicio, coluInicio);
    }

    int encontrarSalidaBFS(int filaInicio, int& coluInicio) {
        return buscarAnchura(filaInicio, coluInicio);
    }

private:
    // Variables privadas, solo se usan en esta clase
    vector<vector<bool>> visitados;
    stack<pair<int, int>> pilaElegidos;
    queue<pair<int, int>> colaElegidos;
    int* labeClase;
    int filaInicio;
    int coluInicio;
    int filaFinal;
    int coluFinal;
    int numFilas;
    int numCols;

    // Funciones privadas

    bool verificarMovimiento(int fila, int columna) {
        int pasable = 1;

        // Los valores deben estar dentro del rango de la matriz 10*10
        if (fila < numFilas && fila >= 0 && columna < numCols && columna >= 0) {
            // Si el elemento está como "false" en el vector visitados, entonces se puede visitar
            if (labeClase[(fila * numCols) + columna] == pasable && !visitados[fila][columna]) {
                return true;
            }
        }
        return false;
    }

    int buscarProfundidad(int filActual, int colActual) {
        // Número de movimientos
        int mov = 0;

        // Insertamos el primer elemento (el de inicio)
        pilaElegidos.push({ filActual, colActual });

        // Imprimimos el elemento
        pair<int, int> anterior = pilaElegidos.top();
        std::cout << "(" << anterior.first << ", " << anterior.second << ")" << endl;

        // Bucle hasta que los elementos correspondan con los de la meta
        while (!(filActual == filaFinal && colActual == coluFinal)) {
            // Cada iteración es un movimiento que se hace en el laberinto
            ++mov;

            // Marcamos el elemento como visitado antes de comprobar posibles movimientos más
            visitados[filActual][colActual] = true;

            // Variable para comprobar si hay movimientos posibles o no
            bool caminosDisponibles = false;

            // Variable para comprobar si ya se imprimió el movimiento o no
            bool impreso = false;

            // Izquierda
            if (verificarMovimiento(filActual, colActual - 1)) {
                --colActual;
                pilaElegidos.push({ filActual,colActual });
                caminosDisponibles = true;
            }

            // Arriba
            else if (verificarMovimiento(filActual - 1, colActual)) {
                --filActual;
                pilaElegidos.push({ filActual,colActual });
                caminosDisponibles = true;
            }

            // Derecha
            else if (verificarMovimiento(filActual, colActual + 1)) {
                ++colActual;
                pilaElegidos.push({ filActual,colActual });
                caminosDisponibles = true;
            }

            // Abajo
            else if (verificarMovimiento(filActual + 1, colActual)) {
                ++filActual;
                pilaElegidos.push({ filActual,colActual });
                caminosDisponibles = true;
            }

            // If para retroceder al nodo anterior y buscar más posibles movimientos
            if (!caminosDisponibles) {
                if (!pilaElegidos.empty()) {
                    // Quitamos el último nodo para ir al anterior
                    pilaElegidos.pop();
                    
                    // Actualizamos filActual y colActual
                    pair<int, int> anterior = pilaElegidos.top(); 
                    filActual = anterior.first;
                    colActual = anterior.second;

                    // Imprimimos nodo actual, por más que sea uno ya visitado
                    std::cout << "(" << anterior.first << ", " << anterior.second << ")" << endl;
                    impreso = true;
                }
            }

            // Si no se imprimió en el if de arriba, se imprime el nodo que será uno nuevo
            if (!impreso) {
                pair<int, int> anterior = pilaElegidos.top();
                std::cout << "(" << anterior.first << ", " << anterior.second << ")" << endl;
            }
        }

        return mov;
    }

    int buscarAnchura(int& filAhora, int& colAhora) {
        // Número de movimientos
        int numMov = 0;

        // Marcamos el nodo actual como visitado
        visitados[filAhora][colAhora] = true;

        // Agregamos nodo a la cola
        colaElegidos.push({ filAhora, colAhora });
        
        // Bucle que almacenerá los nodos disponibles por niveles
        while (!(filAhora == filaFinal && colAhora == coluFinal)) {
            // Imprimimos el movimiento realizado
            pair<int, int> primero = colaElegidos.front();
            std::cout << "(" << primero.first << ", " << primero.second << ")" << endl;

            // Aumentamos el número de movimientos
            ++numMov;

            // Variables auxiliares para añadir a la cola y que no se malogre el programa
            int colita = 0;
            int filita = 0;

            // Izquierda
            if (verificarMovimiento(filAhora, colAhora - 1)) {
                colita = colAhora - 1;
                colaElegidos.push({ filAhora, colita });
                visitados[filAhora][colita] = true;
            }

            // Arriba
            if (verificarMovimiento(filAhora - 1, colAhora)) {
                filita = filAhora - 1;
                colaElegidos.push({ filita, colAhora });
                visitados[filita][colAhora] = true;
            }

            // Derecha
            if (verificarMovimiento(filAhora, colAhora + 1)) {
                colita = colAhora + 1;
                colaElegidos.push({ filAhora, colita });
                visitados[filAhora][colita] = true;
            }

            // Abajo
            if (verificarMovimiento(filAhora + 1, colAhora)) {
                filita = filAhora + 1;
                colaElegidos.push({ filita, colAhora });
                visitados[filita][colAhora] = true;
            }

            // Quitamos el nodo ya impreso y visitado de la cola para actualizar el siguiente nodo a visitar
            colaElegidos.pop();
            pair<int, int> siguiente = colaElegidos.front();
            filAhora = siguiente.first;
            colAhora = siguiente.second;
        }

        // Imprimimos el último movimiento realizado, el nodo de la meta
        pair<int, int> primero = colaElegidos.front();
        std::cout << "(" << primero.first << ", " << primero.second << ")" << endl;

        return numMov;
    }
};

int main()
{
    // Código para imprimir caracteres especiales (tildes, eñes...)
    SetConsoleOutputCP(CP_UTF8);

    // Puntero a un "array" unidimensional de 100 elementos
    int* laberintoPtr;
    laberintoPtr = new int[10 * 10] {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
        0, 0, 1, 1, 1, 1, 1, 1, 0, 1,
        0, 0, 0, 0, 1, 0, 0, 1, 1, 1,
        0, 0, 0, 0, 1, 0, 0, 1, 0, 1,
        1, 1, 1, 1, 1, 0, 0, 1, 0, 1,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
        0, 0, 1, 0, 0, 0, 0, 0, 1, 1,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0
    };
        
    // Posición inicial (comienzo)
    int iFila = 9;
    int iColu = 3;

    // Posición final (meta)
    int fFila = 0;
    int fColu = 9;

    // Número de filas y columnas
    int nFilas = 10;
    int nCols = 10;

    // Imprimimos el laberinto para que se visualice
    cout << "|---------------------|" << endl;
    cout << "|------LABERINTO------|" << endl;
    cout << "|---------------------|" << endl;
    mostrarLaberinto(laberintoPtr, nFilas, nCols);


    cout << "****************************************************************" << endl << endl;


    /* Búsqueda por profundidad */

    // Registramos el tiempo de inicio
    auto start = high_resolution_clock::now();

    Laberinto labeDFS(laberintoPtr, nFilas, nCols, iFila, iColu, fFila, fColu);
    cout << "|--------------------------------------------------------------|" << endl;
    cout << "|--------------------Búsqueda por profundidad------------------|" << endl;
    cout << "|--------------------------------------------------------------|" << endl;
    cout << "Este es el camino que se toma para llegar a la meta (" << fFila << ", " << fColu << "):" << endl;
    int movimientosDFS = labeDFS.encontrarSalidaDFS(iFila, iColu);
    cout << endl << "Movimientos realizados: " << movimientosDFS << endl;

    // Registramos el tiempo de finalización
    auto stop = high_resolution_clock::now();
    // Calculamos la duración del algoritmo DFS
    auto duration = duration_cast<milliseconds>(stop - start);
    // Imprimimos el tiempo de ejecución del algoritmo DFS
    cout << "Tiempo de ejecución de DFS: " << duration.count() << " milisegundos" << endl << endl << endl;


    cout << "****************************************************************" << endl << endl;


    /* Búsqueda por anchura */
        
    // Registramos el tiempo de inicio
    auto start2 = high_resolution_clock::now();

    Laberinto labeBFS(laberintoPtr, nFilas, nCols, iFila, iColu, fFila, fColu);
    cout << "|--------------------------------------------------------------|" << endl;
    cout << "|----------------------Búsqueda por anchura--------------------|" << endl;
    cout << "|--------------------------------------------------------------|" << endl;
    cout << "Este es el camino que se toma para llegar a la meta (" << fFila << ", " << fColu << "):" << endl;
    int movimientosBFS = labeBFS.encontrarSalidaBFS(iFila, iColu);
    cout << endl << "Movimientos realizados: " << movimientosBFS << endl;

    // Registramos el tiempo de finalización
    auto stop2 = high_resolution_clock::now();
    // Calculamos la duración del algoritmo DFS
    auto duration2 = duration_cast<milliseconds>(stop2 - start2);
    // Imprimimos el tiempo de ejecución del algoritmo DFS
    cout << "Tiempo de ejecución de BFS: " << duration2.count() << " milisegundos" << endl;
    
    // Liberación de la memoria asignada dinámicamente
    delete laberintoPtr;

    return 0;
}