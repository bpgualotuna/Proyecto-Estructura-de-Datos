#ifndef MENU_H
#define MENU_H

#include "FechaHora.h"
#include "Validaciones.h"

#include "ArbolAVLSucursales.h"
#include "BusquedaAVL.h"

// Declaración anticipada de Banco
class busquedaAVL;
class Banco;
class ArbolAVLSucursales;
class SucursalBancaria;

class Menu {
private:
    ArbolAVLSucursales& arbolSucursales;
    SucursalBancaria* sucursalActual;
    FechaHora horaActual;

    void seleccionarSucursal();
    void mostrarMenuBusquedaAVL();

public:
   Menu(ArbolAVLSucursales& arbol);
    void mostrar();
};

#endif