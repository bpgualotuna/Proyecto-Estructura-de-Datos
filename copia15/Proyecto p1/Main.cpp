#include "Menu.h"
#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Banco.h"
#include <iostream>

int main() {
    try {
        // Crear una única instancia de Banco
        Banco* bancoCompartido = new Banco("01", "000");
        
        // Crear árbol de sucursales
        ArbolAVLSucursales arbol;
        
        // Insertar sucursales
        arbol.insertar(new SucursalBancaria("01", "001", "Sucursal Central", bancoCompartido));
        arbol.insertar(new SucursalBancaria("01", "002", "Sucursal Norte", bancoCompartido));
        arbol.insertar(new SucursalBancaria("01", "003", "Sucursal Sur", bancoCompartido));
        
        // Crear y mostrar menú
        Menu menu(arbol);
        menu.mostrar();

        // Liberar memoria
        delete bancoCompartido;
    } catch (const char* e) {
        std::cerr << "Error capturado: " << e << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error std::exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Error desconocido capturado" << std::endl;
        return 1;
    }
    return 0;
}