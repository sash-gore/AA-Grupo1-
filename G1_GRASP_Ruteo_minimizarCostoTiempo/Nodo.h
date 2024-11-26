/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Nodo.h
 * Author: usuario
 *
 * Created on 23 de noviembre de 2024, 04:18 PM
 */

#ifndef NODO_H
#define NODO_H

#include "utils.h"

struct Nodo{
    double distancia; // v = d/t
    double tiempo;    // costo = distancia * 0.8 <- 0.8 es una constante que se puede cambiar
    string destino;
};

#endif /* NODO_H */

