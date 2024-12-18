#include <iostream>
#include <vector>
#include <climits>
#include <queue>

using namespace std;

struct Arista {
    int destino, capacidad, costo, flujo;
    Arista(int destino, int capacidad, int costo) : destino(destino), capacidad(capacidad), costo(costo), flujo(0) {}
};

class FlujoCostoMinimo {
public:
    int n; // Número de nodos (ciudades)
    vector<vector<Arista>> grafo;

    FlujoCostoMinimo(int n) : n(n), grafo(n) {}

    void agregarArista(int origen, int destino, int capacidad, int costo) {
        grafo[origen].emplace_back(destino, capacidad, costo);
        grafo[destino].emplace_back(origen, 0, -costo); // Arista inversa con capacidad 0 y costo negativo
    }

    int flujoMaximoCostoMinimo(int origen, int destino) {
        int flujoTotal = 0;
        int costoTotal = 0;

        while (true) {
            vector<int> dist(n, INT_MAX);
            vector<int> padre(n, -1);
            vector<int> aristaPadre(n, -1);
            vector<bool> enCola(n, false);

            dist[origen] = 0;
            queue<int> q;
            q.push(origen);
            enCola[origen] = true;

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                enCola[u] = false;

                for (int i = 0; i < grafo[u].size(); ++i) {
                    Arista &a = grafo[u][i];
                    if (a.capacidad > a.flujo && dist[a.destino] > dist[u] + a.costo) {
                        dist[a.destino] = dist[u] + a.costo;
                        padre[a.destino] = u;
                        aristaPadre[a.destino] = i;
                        if (!enCola[a.destino]) {
                            q.push(a.destino);
                            enCola[a.destino] = true;
                        }
                    }
                }
            }

            if (dist[destino] == INT_MAX) {
                break; // No hay más camino de costo mínimo
            }

            int flujoCamino = INT_MAX;
            for (int v = destino; v != origen; v = padre[v]) {
                int u = padre[v];
                int indiceArista = aristaPadre[v];
                flujoCamino = min(flujoCamino, grafo[u][indiceArista].capacidad - grafo[u][indiceArista].flujo);
            }

            for (int v = destino; v != origen; v = padre[v]) {
                int u = padre[v];
                int indiceArista = aristaPadre[v];
                grafo[u][indiceArista].flujo += flujoCamino;
                grafo[v][indiceArista ^ 1].flujo -= flujoCamino;
            }

            flujoTotal += flujoCamino;
            costoTotal += flujoCamino * dist[destino];
        }

        return costoTotal;
    }
};

int main() {
    int n = 6; // Número de nodos (ciudades)
    FlujoCostoMinimo fcm(n);

    // Añadir aristas (origen, destino, capacidad, costo)
    // Ciudad 0: Origen
    // Ciudad 5: Destino
    fcm.agregarArista(0, 1, 10, 5); // De ciudad 0 a ciudad 1, capacidad 10, costo 5
    fcm.agregarArista(0, 2, 15, 4); // De ciudad 0 a ciudad 2, capacidad 15, costo 4
    fcm.agregarArista(1, 3, 10, 2); // De ciudad 1 a ciudad 3, capacidad 10, costo 2
    fcm.agregarArista(2, 3, 10, 3); // De ciudad 2 a ciudad 3, capacidad 10, costo 3
    fcm.agregarArista(2, 4, 10, 2); // De ciudad 2 a ciudad 4, capacidad 10, costo 2
    fcm.agregarArista(3, 5, 10, 1); // De ciudad 3 a ciudad 5, capacidad 10, costo 1
    fcm.agregarArista(4, 5, 10, 3); // De ciudad 4 a ciudad 5, capacidad 10, costo 3

    int origen = 0; // Ciudad origen
    int destino = 5;   // Ciudad destino

    int costoMinimo = fcm.flujoMaximoCostoMinimo(origen, destino);
    cout << "Costo mínimo total: " << costoMinimo << endl;

    return 0;
}