#ifndef ARBOL_AVL_SUCURSALES_H
#define ARBOL_AVL_SUCURSALES_H

    #include <functional>

class SucursalBancaria;

struct NodoAVL {
    SucursalBancaria* sucursal;
    NodoAVL* izquierda;
    NodoAVL* derecha;
    int altura;
    NodoAVL(SucursalBancaria* suc) : sucursal(suc), izquierda(nullptr), derecha(nullptr), altura(1) {}
};

class ArbolAVLSucursales {
private:
    NodoAVL* raiz;

    int obtenerAltura(NodoAVL* nodo);
    int obtenerFactorBalance(NodoAVL* nodo);
    NodoAVL* rotarDerecha(NodoAVL* y);
    NodoAVL* rotarIzquierda(NodoAVL* x);
    NodoAVL* balancear(NodoAVL* nodo, const char* codigoSucursal);
    NodoAVL* insertarRec(NodoAVL* nodo, SucursalBancaria* sucursal);
    NodoAVL* buscarRec(NodoAVL* nodo, const char* codigoSucursal);
    void destruirRec(NodoAVL* nodo);
    void recorrerInOrdenRec(NodoAVL* nodo, std::function<void(SucursalBancaria*)> func);

public:
    ArbolAVLSucursales();
    ~ArbolAVLSucursales();
    void insertar(SucursalBancaria* sucursal);
    SucursalBancaria* buscar(const char* codigoSucursal);
    void recorrerInOrden(std::function<void(SucursalBancaria*)> func);
};

#endif