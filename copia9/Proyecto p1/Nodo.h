#ifndef NODO_H
#define NODO_H

template <typename T>
class Nodo {
public:
    T datos;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

    Nodo(const T& valor) : datos(valor), siguiente(nullptr), anterior(nullptr) {}
};

#endif