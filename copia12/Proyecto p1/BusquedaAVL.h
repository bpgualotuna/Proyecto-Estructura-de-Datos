#ifndef BUSQUEDA_AVL_H
#define BUSQUEDA_AVL_H

#include <string>
#include <vector>
#include "ListaDobleCircular.h"

struct CuentaSimple {
    std::string id;
    std::string nombre;
    std::string apellido;
    int edad;
    std::string tipo;
    double saldo;
    std::string fechaCreacion;
};

class BusquedaAVL {
public:
    void mostrarMenu();
};

template <typename T, typename Compare>
class AVLNode {
public:
    T data;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const T& d) : data(d), left(nullptr), right(nullptr), height(1) {}
};

template <typename T, typename Compare>
class AVLTree {
public:
    AVLNode<T, Compare>* root;
    Compare cmp;
    AVLTree() : root(nullptr) {}
    void insert(const T& value) { root = insertRec(root, value); }
    AVLNode<T, Compare>* insertRec(AVLNode<T, Compare>* node, const T& value);
    AVLNode<T, Compare>* find(const T& value);
    void printTree();
    void printTreeHelper(AVLNode<T, Compare>* node, int space);
};

// Comparadores para AVL
struct CompararPorNombre {
    bool operator()(const CuentaSimple& a, const CuentaSimple& b) const {
        return a.nombre < b.nombre;
    }
};
struct CompararPorApellido {
    bool operator()(const CuentaSimple& a, const CuentaSimple& b) const {
        return a.apellido < b.apellido;
    }
};
struct CompararPorEdad {
    bool operator()(const CuentaSimple& a, const CuentaSimple& b) const {
        return a.edad < b.edad;
    }
};
struct CompararPorFecha {
    bool operator()(const CuentaSimple& a, const CuentaSimple& b) const {
        return a.fechaCreacion < b.fechaCreacion;
    }
};

// Funciones para construir y graficar AVL
void construirYGraficarAVLPorNombre();
void construirYGraficarAVLPorApellido();
void construirYGraficarAVLPorEdad();
void construirYGraficarAVLPorFecha();
void buscarEnAVLPorCampo();

// Funciones de lectura de archivo
ListaDobleCircular<CuentaSimple> leerCuentasDesdeArchivo(const std::string& nombreArchivo);

#endif 