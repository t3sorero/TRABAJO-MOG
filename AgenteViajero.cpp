#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Número de vértices del grafo
int V;

// Matriz de adyacencia del grafo
vector<vector<int>> dist;

// Tabla de programación dinámica
vector<vector<int>> memo;

// Devuelve el coste mínimo de recorrer todos los vértices empezando desde `pos`
// y habiendo visitado los vértices indicados en el bitmask `visitados`
int tsp(int pos, int visitados) {
    // Caso base: todos los vértices han sido visitados
    if (visitados == (1 << V) - 1)
        return dist[pos][0]; // Regresamos al vértice inicial

    // Si ya se ha calculado esta subproblema, devolvemos el resultado almacenado
    if (memo[pos][visitados] != -1)
        return memo[pos][visitados];

    int res = INT_MAX; // Valor grande para inicializar el mínimo

    // Probar todas las opciones de vértices no visitados
    for (int i = 0; i < V; ++i) {
        // Si el vértice `i` no ha sido visitado
        if (!(visitados & (1 << i))) {
            res = min(res, dist[pos][i] + tsp(i, visitados | (1 << i)));
        }
    }

    // Guardar el resultado en la tabla de DP y devolverlo
    return memo[pos][visitados] = res;
}

int main() {
    // Definir las distancias entre las ciudades en una matriz
    dist = {
        {0, 12, 10, 19, 8},   // Distancias desde la ciudad A
        {12, 0, 3, 7, 6},     // Distancias desde la ciudad B
        {10, 3, 0, 2, 20},    // Distancias desde la ciudad C
        {19, 7, 2, 0, 4},     // Distancias desde la ciudad D
        {8, 6, 20, 4, 0}      // Distancias desde la ciudad E
    };

    // Inicializar el número de vértices
    V = dist.size();

    // Inicializar la tabla de DP con -1
    memo.assign(V, vector<int>(1 << V, -1));

    // Calcular el costo mínimo usando el TSP
    int costoMinimo = tsp(0, 1); // Comenzamos desde el vértice 0 y solo este ha sido visitado

    // Imprimir el costo mínimo
    cout << "Costo minimo: " << costoMinimo << endl;

    return 0;
}
