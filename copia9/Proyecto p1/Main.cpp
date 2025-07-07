#include "Menu.h"
#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Banco.h"
#include <iostream>

int main() {
    try {
        // Crear una única instancia de Banco
        Banco* bancoCompartido = new Banco("01", "000"); // "000" indica que es compartido
        ArbolAVLSucursales arbol;
        arbol.insertar(new SucursalBancaria("01", "001", "Sucursal Central", bancoCompartido));
        arbol.insertar(new SucursalBancaria("01", "002", "Sucursal Norte", bancoCompartido));
        arbol.insertar(new SucursalBancaria("01", "003", "Sucursal Sur", bancoCompartido));
        
        Menu menu(arbol);
        menu.mostrar();

        // Liberar la instancia compartida de Banco
        delete bancoCompartido;
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    }
    return 0;
}