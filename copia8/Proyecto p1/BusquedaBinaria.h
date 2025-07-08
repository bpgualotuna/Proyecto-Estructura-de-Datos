#ifndef BUSQUEDA_BINARIA_H
#define BUSQUEDA_BINARIA_H

#include "Banco.h"
#include "Validaciones.h"
#include "FechaHora.h"

class BusquedaBinaria {
private:
    Banco& banco; // Referencia al objeto Banco

    // Métodos privados para realizar las búsquedas binarias
    void buscarPorNombre();
    void buscarPorApellido();
    void buscarPorEdad();
    ListaDobleCircular<Cuenta*> buscarPorFechaCreacion(const FechaHora& fecha);

public:
    BusquedaBinaria(Banco& b);
    void mostrarMenu();
};

#endif