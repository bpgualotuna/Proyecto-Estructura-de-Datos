#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include <cstring>
#include <algorithm>
#include <functional>

ArbolAVLSucursales::ArbolAVLSucursales() : raiz(nullptr) {}

ArbolAVLSucursales::~ArbolAVLSucursales() {
    destruirRec(raiz);
}

int ArbolAVLSucursales::obtenerAltura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int ArbolAVLSucursales::obtenerFactorBalance(NodoAVL* nodo) {
    return nodo ? obtenerAltura(nodo->izquierda) - obtenerAltura(nodo->derecha) : 0;
}

NodoAVL* ArbolAVLSucursales::rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierda;
    NodoAVL* T2 = x->derecha;
    x->derecha = y;
    y->izquierda = T2;
    y->altura = std::max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;
    x->altura = std::max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;
    return x;
}

NodoAVL* ArbolAVLSucursales::rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* T2 = y->izquierda;
    y->izquierda = x;
    x->derecha = T2;
    x->altura = std::max(obtenerAltura(x->izquierda), obtenerAltura(x->derecha)) + 1;
    y->altura = std::max(obtenerAltura(y->izquierda), obtenerAltura(y->derecha)) + 1;
    return y;
}

NodoAVL* ArbolAVLSucursales::balancear(NodoAVL* nodo, const char* codigoSucursal) {
    int factor = obtenerFactorBalance(nodo);
    char* cod = nodo->sucursal->obtenerCodigoSucursal();

    if (factor > 1) {
        char* codIzq = nodo->izquierda->sucursal->obtenerCodigoSucursal();
        if (strcmp(codigoSucursal, codIzq) < 0) {
            delete[] codIzq;
            delete[] cod;
            return rotarDerecha(nodo);
        } else {
            nodo->izquierda = rotarIzquierda(nodo->izquierda);
            delete[] codIzq;
            delete[] cod;
            return rotarDerecha(nodo);
        }
    }
    if (factor < -1) {
        char* codDer = nodo->derecha->sucursal->obtenerCodigoSucursal();
        if (strcmp(codigoSucursal, codDer) > 0) {
            delete[] codDer;
            delete[] cod;
            return rotarIzquierda(nodo);
        } else {
            nodo->derecha = rotarDerecha(nodo->derecha);
            delete[] codDer;
            delete[] cod;
            return rotarIzquierda(nodo);
        }
    }
    delete[] cod;
    return nodo;
}

NodoAVL* ArbolAVLSucursales::insertarRec(NodoAVL* nodo, SucursalBancaria* sucursal) {
    if (!nodo) {
        return new NodoAVL(sucursal);
    }
    char* codNodo = nodo->sucursal->obtenerCodigoSucursal();
    char* codSucursal = sucursal->obtenerCodigoSucursal();
    int comparacion = strcmp(codSucursal, codNodo);
    delete[] codNodo;
    delete[] codSucursal;

    if (comparacion < 0) {
        nodo->izquierda = insertarRec(nodo->izquierda, sucursal);
    } else if (comparacion > 0) {
        nodo->derecha = insertarRec(nodo->derecha, sucursal);
    } else {
        return nodo; // No insertar duplicados
    }

    nodo->altura = std::max(obtenerAltura(nodo->izquierda), obtenerAltura(nodo->derecha)) + 1;
    return balancear(nodo, codSucursal);
}

void ArbolAVLSucursales::insertar(SucursalBancaria* sucursal) {
    raiz = insertarRec(raiz, sucursal);
}

NodoAVL* ArbolAVLSucursales::buscarRec(NodoAVL* nodo, const char* codigoSucursal) {
    if (!nodo) return nullptr;
    char* codNodo = nodo->sucursal->obtenerCodigoSucursal();
    int comparacion = strcmp(codigoSucursal, codNodo);
    delete[] codNodo;

    if (comparacion == 0) return nodo;
    if (comparacion < 0) return buscarRec(nodo->izquierda, codigoSucursal);
    return buscarRec(nodo->derecha, codigoSucursal);
}

SucursalBancaria* ArbolAVLSucursales::buscar(const char* codigoSucursal) {
    NodoAVL* nodo = buscarRec(raiz, codigoSucursal);
    return nodo ? nodo->sucursal : nullptr;
}

void ArbolAVLSucursales::recorrerInOrdenRec(NodoAVL* nodo, std::function<void(SucursalBancaria*)> func) {
    if (nodo) {
        recorrerInOrdenRec(nodo->izquierda, func);
        func(nodo->sucursal);
        recorrerInOrdenRec(nodo->derecha, func);
    }
}

void ArbolAVLSucursales::recorrerInOrden(std::function<void(SucursalBancaria*)> func) {
    recorrerInOrdenRec(raiz, func);
}

void ArbolAVLSucursales::destruirRec(NodoAVL* nodo) {
    if (nodo) {
        destruirRec(nodo->izquierda);
        destruirRec(nodo->derecha);
        delete nodo->sucursal;
        delete nodo;
    }
}