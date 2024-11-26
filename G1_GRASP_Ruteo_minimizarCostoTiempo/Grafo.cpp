/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Grafo.h"

void cargarDatosDeArchivo(Grafo &grafo, const char *nombreArch) {
    cout << "Leyendo archivo " << nombreArch << "..." << endl;

    ifstream archDatosGrafo(nombreArch);
    if (not archDatosGrafo.is_open()) {
        cout << "No se pudo abrir el archivo " << nombreArch << endl;
        exit(1);
    }

    vector<string> ciudades = leeCiudades(archDatosGrafo);
    grafo.ciudades = ciudades;
    string linea;
    // Leer el resto de las líneas
    while (getline(archDatosGrafo, linea)) {
        stringstream ss(linea);
        string origen = leeCiudadOrigen(ss);

        // Leer las conexiones
        string celda;
        int indiceDestino = 0;
        while (getline(ss, celda, ',')) {
            limpiaEspaciosEnBlanco(celda);
            if (celda != "inf" && !celda.empty()) {
                try {
                    // Extraer distancia y tiempo
                    string distancia, tiempo;
                    procesarCelda(celda, distancia, tiempo);

                    // Crear nodo y agregarlo al grafo
                    Nodo nodo;
                    nodo.distancia = stod(distancia);
                    nodo.tiempo = stod(tiempo);
                    nodo.destino = ciudades[indiceDestino];
                    grafo.vertices[origen].push_back(nodo);
                } catch (const invalid_argument &e) {
                    cerr << "Error al procesar la celda: " << celda << endl;
                } catch (const out_of_range &e) {
                    cerr << "Valor fuera de rango en la celda: " << celda << endl;
                }
            }
            indiceDestino++;
        }
    }

    archDatosGrafo.close();
    cout << "Datos cargados correctamente." << endl;
}

vector<string> leeCiudades(ifstream &archDatosGrafo) {
    string linea;
    vector<string> ciudades;

    // Leer la primera línea de encabezado (título)
    getline(archDatosGrafo, linea);

    // Leer la segunda línea de encabezado (nombres de ciudades)
    getline(archDatosGrafo, linea);
    stringstream ss(linea);
    string ciudad;
    getline(ss, ciudad, ','); // Saltar la primera celda vacía
    while (getline(ss, ciudad, ',')) {
        // Eliminar espacios adicionales
        limpiaEspaciosEnBlanco(ciudad);
        ciudades.push_back(ciudad);
    }

    return ciudades;
}

// Función para procesar una celda con delimitador ';' dentro de los paréntesis

void procesarCelda(const string &celda, string &distancia, string &tiempo) {
    string contenido = celda.substr(1, celda.size() - 2); // Quitar los paréntesis
    stringstream ss(contenido);
    getline(ss, distancia, ';'); // Separar por ';'
    getline(ss, tiempo, ';'); // Leer el tiempo

    // Asegurarnos de que los valores estén en el formato correcto
    if (distancia.empty()) distancia = "0.0";
    if (tiempo.empty()) tiempo = "0.0";
}

string leeCiudadOrigen(stringstream &ss) {
    string origen;
    getline(ss, origen, ',');
    limpiaEspaciosEnBlanco(origen);
    return origen;
}

void limpiaEspaciosEnBlanco(string &cadena) {
    cadena.erase(remove(cadena.begin(), cadena.end(), ' '), cadena.end());
}

void muestraGrafo(const Grafo & grafo){
    for (const auto &vertice : grafo.vertices) {
        cout << "Desde " << vertice.first << ":\n";
        for (const auto &nodo : vertice.second) {
            cout << "  A " << nodo.destino << " -> Distancia(km): " << nodo.distancia 
                    << ", Tiempo(h): " << nodo.tiempo << endl;
        }
    }
}

void testDeLectura(){
    Grafo grafo;
    cargarDatosDeArchivo(grafo, "Mapa_1.csv");
    
    muestraGrafo(grafo);
}

void testDeEstructuraGRAFO(){
    Grafo grafo;
    
    grafo.vertices["Ejemplo1"] = {
        {251.0, 2.6, "Ejemplo2"},
        {141.0, 1.3, "Ejemplo3"}
    };
    
    vector<Nodo> v = grafo.vertices["Ejemplo1"];
    
    for(Nodo n : v){
        cout << "distancia: " << n.distancia << endl;
        cout << "tiempo: " << n.tiempo << endl;
        cout << "destino: " << n.destino << endl;
    }
}