#pragma once

struct NodoAVLInt {
    int distancia;
    NodoAVLInt* izquierdo;
    NodoAVLInt* derecho;
    int altura;
    NodoAVLInt(int d) : distancia(d), izquierdo(nullptr), derecho(nullptr), altura(1) {}
}; 