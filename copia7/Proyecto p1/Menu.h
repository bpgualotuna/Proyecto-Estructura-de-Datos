#ifndef MENU_H
#define MENU_H

#include "Banco.h" // Elimina esta línea si ya no es necesaria
#include "FechaHora.h"
#include "Validaciones.h"
#include "ArbolAVLSucursales.h"
#include "Oficina.h"

// Declaración anticipada de Banco
class Banco;
class ArbolAVLSucursales;
class SucursalBancaria;

class Menu {
private:
    ArbolAVLSucursales& arbolSucursales;
    SucursalBancaria* sucursalActual;
    Oficina* oficinaActual; // Nueva variable para almacenar la oficina seleccionada
    FechaHora horaActual;

    void seleccionarSucursal();
    void seleccionarOficina(); // Modificado para no requerir parámetros

public:
   Menu(ArbolAVLSucursales& arbol);
    void mostrar();
};

#endif