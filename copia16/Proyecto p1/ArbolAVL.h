#pragma once

#include "NodoAVL.h"
#include <string>
#include <SFML/Graphics.hpp>

class ArbolAVL {
private:
    NodoAVLInt* raiz;

    int obtenerAltura(NodoAVLInt* nodo);
    int obtenerFactorBalance(NodoAVLInt* nodo);
    NodoAVLInt* rotarDerecha(NodoAVLInt* y);
    NodoAVLInt* rotarIzquierda(NodoAVLInt* x);
    NodoAVLInt* insertar(NodoAVLInt* nodo, int distancia);
    void recorridoInorden(NodoAVLInt* nodo);
    void dibujarNodo(NodoAVLInt* nodo, sf::RenderWindow &ventana, sf::Font &fuente, float x, float y, float offset);
    // Nuevo método privado para impresión en consola
    void printTree(NodoAVLInt* nodo, std::string prefix, bool isLeft);

public:
    ArbolAVL();
    NodoAVLInt* obtenerRaiz();
    void agregarDistancia(int distancia);
    void mostrarDistancias();  
    void mostrarArbol(sf::RenderWindow& ventana, sf::Font& fuente);  
    // Nuevo método público para impresión en consola
    void printTree();
    NodoAVLInt* buscar(int distancia); // Nuevo método de búsqueda
}; 