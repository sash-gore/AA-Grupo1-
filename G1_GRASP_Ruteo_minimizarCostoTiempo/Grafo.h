/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Grafo.h
 * Author: usuario
 *
 * Created on 23 de noviembre de 2024, 04:17 PM
 */

#ifndef GRAFO_H
#define GRAFO_H

#include "utils.h"
#include "Nodo.h"

struct Grafo{
    map<string,vector<Nodo>> vertices;
    vector<string> ciudades;
};

void cargarDatosDeArchivo(Grafo &grafo,const char *nombreArch);
vector<string> leeCiudades(ifstream &archDatosGrafo);
void procesarCelda(const string &celda, string &distancia, string &tiempo);
string leeCiudadOrigen(stringstream &ss);
void limpiaEspaciosEnBlanco(string &celda);
void muestraGrafo(const Grafo & grafo);

void testDeLectura();
void testDeEstructuraGRAFO();

#endif /* GRAFO_H */

