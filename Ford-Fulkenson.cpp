#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>
#include <stack>

using namespace std;

// Función para realizar una búsqueda en profundidad (DFS) en la red residual.
bool dfs(vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    stack<int> s;
    s.push(source);
    visited[source] = true;

    while (!s.empty()) {
        int u = s.top();
        s.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                s.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Función que implementa el algoritmo de Ford-Fulkerson.
int fordFulkerson(vector<vector<int>>& graph, int source, int sink) {
    int n = graph.size();

    // Crear una copia del grafo para trabajar con la red residual.
    vector<vector<int>> residualGraph = graph;

    // Vector para almacenar el camino aumentante.
    vector<int> parent(n, -1);

    int maxFlow = 0; // Flujo máximo inicial.

    // Mientras exista un camino aumentante desde la fuente hasta el sumidero.
    while (dfs(residualGraph, source, sink, parent)) {
        // Determinar la capacidad mínima en el camino aumentante encontrado.
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        // Actualizar la red residual reduciendo la capacidad y añadiendo flujo inverso.
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        // Incrementar el flujo máximo con el flujo del camino aumentante.
        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    // Definimos un grafo de prueba como matriz de adyacencia.
    // Ejemplo:
    // Nodo 0 = fuente, nodo 5 = sumidero.
    vector<vector<int>> graph = {
        {0, 16, 13, 0, 0, 0},
        {0, 0, 10, 12, 0, 0},
        {0, 4, 0, 0, 14, 0},
        {0, 0, 9, 0, 0, 20},
        {0, 0, 0, 7, 0, 4},
        {0, 0, 0, 0, 0, 0}
    };

    int source = 0; // Nodo fuente.
    int sink = 5;   // Nodo sumidero.

    cout << "El flujo máximo es: " << fordFulkerson(graph, source, sink) << endl;

    return 0;
}
