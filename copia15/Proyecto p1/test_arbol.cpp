#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Banco.h"
#include <iostream>

int main() {
    try {
        std::cout << "PASO 1: Creando banco...\n";
        Banco* banco = new Banco("01", "000");
        std::cout << "PASO 2: Banco creado.\n";
        
        std::cout << "PASO 3: Creando árbol...\n";
        ArbolAVLSucursales arbol;
        
        std::cout << "PASO 4: Insertando sucursal 1...\n";
        arbol.insertar(new SucursalBancaria("01", "001", "Central", banco));
        std::cout << "PASO 5: Sucursal 1 insertada.\n";
        
        std::cout << "PASO 6: Insertando sucursal 2...\n";
        arbol.insertar(new SucursalBancaria("01", "002", "Norte", banco));
        std::cout << "PASO 7: Sucursal 2 insertada.\n";
        
        std::cout << "PASO 8: Insertando sucursal 3...\n";
        arbol.insertar(new SucursalBancaria("01", "003", "Sur", banco));
        std::cout << "PASO 9: Sucursal 3 insertada.\n";
        
        std::cout << "PASO 10: Recorriendo árbol...\n";
        int contador = 0;
        arbol.recorrerInOrden([&contador](SucursalBancaria* suc) {
            contador++;
            char* codigo = suc->obtenerCodigoSucursal();
            char* nombre = suc->obtenerNombre();
            std::cout << "Sucursal " << contador << ": " << nombre << " (" << codigo << ")\n";
            delete[] codigo;
            delete[] nombre;
        });
        
        std::cout << "PASO 11: Programa terminado exitosamente.\n";
        delete banco;
        
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Error desconocido." << std::endl;
        return 1;
    }
    return 0;
}
