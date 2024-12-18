#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <climits>

using namespace std;

// Estructura para representar una arista
struct Arista {
    int destino, capacidad, costo, flujo, reversa;
};

class FlujoCostoMinimo {
private:
    vector<vector<Arista>> grafo;
    vector<int> distancia, potencial, padre, arco_padre;
    int num_nodos;

public:
    // Constructor para inicializar el grafo con el número de nodos
    FlujoCostoMinimo(int nodos) : num_nodos(nodos) {
        grafo.resize(nodos);
        grafo.assign(num_nodos, vector<Arista>());
        potencial.resize(nodos, 0);
        distancia.resize(nodos, 0);
        padre.resize(nodos, -1);
        arco_padre.resize(nodos, -1);
    }

    // Método para agregar una arista
    void agregarArista(int origen, int destino, int capacidad, int costo) {
        Arista arista = { destino, capacidad, costo, 0, (int)grafo[destino].size() };
        Arista arista_reversa = { origen, 0, -costo, 0, (int)grafo[origen].size() };
        grafo[origen].push_back(arista);
        grafo[destino].push_back(arista_reversa);
    }


    // Algoritmo Simplex de Redes para flujo de costo mínimo
    pair<int, int> flujoCostoMinimo(int fuente, int sumidero) {
        int flujo_total = 0, costo_total = 0;

        while (true) {
            // Inicialización de distancias y padres
            distancia.assign(num_nodos, INT_MAX);
            padre.assign(num_nodos, -1);
            arco_padre.assign(num_nodos, -1);
            distancia[fuente] = 0;

            // Cola de prioridad para Dijkstra
            priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> cola_prioridad;
            cola_prioridad.push({ 0, fuente });

            while (!cola_prioridad.empty()) {
                int distancia_actual = cola_prioridad.top().first;
                int nodo_actual = cola_prioridad.top().second;
                cola_prioridad.pop();

                if (distancia_actual > distancia[nodo_actual]) continue;

                for (int i = 0; i < grafo[nodo_actual].size(); ++i) {
                    Arista& arista = grafo[nodo_actual][i];
                    int costo_reducido = arista.costo + potencial[nodo_actual] - potencial[arista.destino];

                    if (arista.flujo < arista.capacidad &&
                        distancia[arista.destino] > distancia[nodo_actual] + costo_reducido) {

                        distancia[arista.destino] = distancia[nodo_actual] + costo_reducido;
                        padre[arista.destino] = nodo_actual;
                        arco_padre[arista.destino] = i;
                        cola_prioridad.push({ distancia[arista.destino], arista.destino });
                    }
                }
            }

            if (distancia[sumidero] == INT_MAX) break;

            // Actualizar potenciales
            for (int i = 0; i < num_nodos; ++i) {
                if (distancia[i] < INT_MAX) {
                    potencial[i] += distancia[i];
                }
            }

            // Encontrar el flujo máximo que se puede enviar
            int flujo_incremento = INT_MAX;
            for (int nodo = sumidero; nodo != fuente; nodo = padre[nodo]) {
                Arista& arista = grafo[padre[nodo]][arco_padre[nodo]];
                flujo_incremento = min(flujo_incremento, arista.capacidad - arista.flujo);
            }

            // Actualizar flujos y costos
            for (int nodo = sumidero; nodo != fuente; nodo = padre[nodo]) {
                Arista& arista = grafo[padre[nodo]][arco_padre[nodo]];
                arista.flujo += flujo_incremento;
                grafo[nodo][arista.reversa].flujo -= flujo_incremento;
                costo_total += flujo_incremento * arista.costo;
            }

            flujo_total += flujo_incremento;
        }

        return { flujo_total, costo_total };
    }
};

int main() {
    string nombre_archivo = "entrada.txt";
    int fuente, sumidero;

    cout << "Ingrese el nodo fuente: ";
    cin >> fuente;
    cout << "Ingrese el nodo sumidero: ";
    cin >> sumidero;
    ifstream archivo(nombre_archivo);
    if (!archivo) {
        cerr << "Error al abrir el archivo." << endl;
        exit(1);
    }

    int num_nodos;
    archivo >> num_nodos;
    FlujoCostoMinimo flujo(num_nodos);

    for (int i = 0; i < num_nodos; ++i) {
        int origen, destino, capacidad, costo;
        archivo >> origen >> destino >> capacidad >> costo;
        flujo.agregarArista(origen, destino, capacidad, costo);
    }

    archivo.close();

    pair<int, int> resultado = flujo.flujoCostoMinimo(fuente, sumidero);

    cout << "Flujo maximo: " << resultado.first << endl;
    cout << "Costo minimo: " << resultado.second << endl;

    return 0;
}