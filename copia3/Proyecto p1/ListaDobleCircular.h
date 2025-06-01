#ifndef LISTA_DOBLE_CIRCULAR_H
#define LISTA_DOBLE_CIRCULAR_H

#include "Nodo.h"
#include <functional>

template <typename T>
class ListaDobleCircular {
private:
    Nodo<T>* cabeza;
    int tamano;

public:
    ListaDobleCircular();
    ~ListaDobleCircular();
    void insertar(const T& valor);
    void eliminar(Nodo<T>* nodo);
    Nodo<T>* buscar(std::function<bool(const T&)> predicado) const;
    void recorrer(std::function<void(const T&)> accion) const;
    int obtenerTamano() const { return tamano; }
};

template <typename T>
ListaDobleCircular<T>::ListaDobleCircular() : cabeza(nullptr), tamano(0) {}

template <typename T>
ListaDobleCircular<T>::~ListaDobleCircular() {
    if (!cabeza) return;
    Nodo<T>* actual = cabeza;
    do {
        Nodo<T>* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    } while (actual != cabeza);
}

template <typename T>
void ListaDobleCircular<T>::insertar(const T& valor) {
    Nodo<T>* nuevoNodo = new Nodo<T>(valor);
    if (!cabeza) {
        cabeza = nuevoNodo;
        cabeza->siguiente = cabeza;
        cabeza->anterior = cabeza;
    } else {
        nuevoNodo->siguiente = cabeza;
        nuevoNodo->anterior = cabeza->anterior;
        cabeza->anterior->siguiente = nuevoNodo;
        cabeza->anterior = nuevoNodo;
    }
    tamano++;
}

template <typename T>
void ListaDobleCircular<T>::eliminar(Nodo<T>* nodo) {
    if (!nodo || !cabeza) return;
    if (tamano == 1) {
        delete cabeza;
        cabeza = nullptr;
        tamano = 0;
        return;
    }
    nodo->anterior->siguiente = nodo->siguiente;
    nodo->siguiente->anterior = nodo->anterior;
    if (nodo == cabeza) cabeza = nodo->siguiente;
    delete nodo;
    tamano--;
}

template <typename T>
Nodo<T>* ListaDobleCircular<T>::buscar(std::function<bool(const T&)> predicado) const {
    if (!cabeza) return nullptr;
    Nodo<T>* actual = cabeza;
    do {
        if (predicado(actual->datos)) return actual;
        actual = actual->siguiente;
    } while (actual != cabeza);
    return nullptr;
}

template <typename T>
void ListaDobleCircular<T>::recorrer(std::function<void(const T&)> accion) const {
    if (!cabeza) return;
    Nodo<T>* actual = cabeza;
    do {
        accion(actual->datos);
        actual = actual->siguiente;
    } while (actual != cabeza);
}

#endif