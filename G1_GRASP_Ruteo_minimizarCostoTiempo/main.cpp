#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <limits>
#include <time.h>
#include "Grafo.h"

#define COSTO 0.12 // Costo de gasolina por km en euros
#define ITERACIONES 10
#define ALPHA 0.3
#define INF std::numeric_limits<double>::infinity()

using namespace std;

// Función para verificar el tamaño de la lista restringida de candidatAos (RCL)
int verifica(vector<Nodo> vecinos, double minrcl) {
    int cont = 0;
    for (int i = 0; i < vecinos.size(); i++) {
        double score = 1 / (vecinos[i].tiempo * vecinos[i].distancia * COSTO);
        if (minrcl >= score)
            cont++;
    }
    return cont;
}

// Función de comparación para ordenar los nodos según el criterio
bool compara(Nodo a, Nodo b) {
    return 1 / (a.tiempo * a.distancia * COSTO) > 1 / (b.tiempo * b.distancia * COSTO);
}

// Función para validar si un nodo tiene valores válidos
bool esValido(const Nodo& nodo) {
    return nodo.distancia != INF && nodo.tiempo != INF && nodo.tiempo > 0;
}

// Función principal del algoritmo GRASP
void solucionGRASP(const string &ciudad_inicio, const string &ciudad_destino, const Grafo &grafo, ofstream &reporte) {
    double min_ruta = INF, max_velocidad = 0, min_costo = INF;
    vector<string> min_visitados;
    srand(time(NULL));

    for (int i = 0; i < ITERACIONES; i++) {
        Grafo grafo_copia = grafo;
        double total = 0, velocidad = 0, costo = 0;
        string ciudad = ciudad_inicio;
        set<string> visitados;
        visitados.insert(ciudad);
        vector<string> ciudades_visitadas;
        ciudades_visitadas.push_back(ciudad);

        while (true) {
            vector<Nodo> vecinos = grafo_copia.vertices[ciudad];

            // Filtrar nodos ya visitados y nodos no válidos
            vecinos.erase(remove_if(vecinos.begin(), vecinos.end(),
                            [&visitados](const Nodo& nodo) {
                                return visitados.count(nodo.destino) > 0 || !esValido(nodo);
                            }),
                  vecinos.end());

            if (vecinos.empty()) {
                reporte << "No hay más vecinos disponibles desde " << ciudad << endl;
                break;
            }

            // Ordenar vecinos
            sort(vecinos.begin(), vecinos.end(), compara);

            // Calcular los valores de beta, tau y minrcl
            double beta = 1 / (vecinos[0].tiempo * vecinos[0].distancia * COSTO);
            double tau = 1 / (vecinos[vecinos.size() - 1].tiempo * vecinos[vecinos.size() - 1].distancia * COSTO);
            double minrcl = beta + ALPHA * (tau - beta);

            int indice_rcl = verifica(vecinos, minrcl);
            if (indice_rcl == 0) {
                reporte << "Error: Lista restringida de candidatos vacía en " << ciudad << endl;
                break;
            }
            int indice_aleatorio = rand() % indice_rcl;

            // Actualizar ciudad actual y registrar visita
            ciudad = vecinos[indice_aleatorio].destino;
            visitados.insert(ciudad);
            ciudades_visitadas.push_back(ciudad);
            total += vecinos[indice_aleatorio].distancia;
            velocidad += vecinos[indice_aleatorio].distancia / vecinos[indice_aleatorio].tiempo;
            costo += vecinos[indice_aleatorio].distancia * COSTO;

            if (ciudad == ciudad_destino) break;
        }

        // Actualizar resultados si la ruta actual es mejor
        if (total < min_ruta) {
            min_ruta = total;
            max_velocidad = velocidad;
            min_costo = costo;
            min_visitados = ciudades_visitadas;
        }
    }

    // Escribir resultados de la mejor solución al archivo de reporte
    if (min_ruta < INF) {
        for (string ciudad : min_visitados)
            reporte << ciudad << " ";
        reporte << endl;
        reporte << "La distancia de la mínima ruta es: " << min_ruta << endl;
        reporte << "La velocidad promedio de la mínima ruta es: " << max_velocidad / min_visitados.size() << endl;
        reporte << "El costo de la mínima ruta es: " << min_costo << endl;
    } else {
        reporte << "No se encontró una ruta válida de " << ciudad_inicio << " a " << ciudad_destino << endl;
    }
    reporte << "-------------------------------------" << endl;
}

int main(int argc, char** argv) {
    Grafo grafo;
    cargarDatosDeArchivo(grafo, "Mapa.csv");

    // Crear el archivo de reporte
    ofstream reporte("reporte.txt");
    if (!reporte.is_open()) {
        cerr << "Error: No se pudo crear el archivo de reporte." << endl;
        return 1;
    }

    for (string c : grafo.ciudades) {
        string ciudad_inicio = "Madrid";
        string ciudad_destino = c;
        if (ciudad_inicio != ciudad_destino) {
            reporte << "La ruta que minimiza el costo y maximiza la velocidad de " << ciudad_inicio
                    << " a " << ciudad_destino << " es:" << endl;
            solucionGRASP(ciudad_inicio, ciudad_destino, grafo, reporte);
        }
    }

    reporte.close();
    cout << "El reporte ha sido generado en el archivo 'reporte.txt'." << endl;

    return 0;
}
