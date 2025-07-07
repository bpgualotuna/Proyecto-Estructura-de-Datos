#ifndef MENU_H
#define MENU_H

#include "Banco.h" // Elimina esta línea si ya no es necesaria
#include "FechaHora.h"
#include "Validaciones.h"
#include "ArbolAVLSucursales.h"

// Declaración anticipada de Banco
class Banco;
class ArbolAVLSucursales;
class SucursalBancaria;

class Menu {
private:
    ArbolAVLSucursales& arbolSucursales;
    SucursalBancaria* sucursalActual;
    FechaHora horaActual;

    void seleccionarSucursal();

public:
   Menu(ArbolAVLSucursales& arbol);
    void mostrar();
};

#endif