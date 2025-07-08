#ifndef ORDENADOR_CUENTAS_H
#define ORDENADOR_CUENTAS_H

#include <functional>
#include <conio.h>
#include <iostream>

class Banco;

class OrdenadorCuentas {
private:
    Banco& banco; // Referencia a la instancia de Banco para llamar a ordenarCuentas

public:
    OrdenadorCuentas(Banco& b);
    void mostrarMenuOrdenamiento();
};

#endif