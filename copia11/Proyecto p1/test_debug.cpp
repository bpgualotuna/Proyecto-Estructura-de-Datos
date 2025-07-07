#include "Menu.h"
#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Banco.h"
#include <iostream>

int main() {
    try {
        std::cout << "PASO 1: Iniciando programa principal...\n";
        std::cout.flush();
        
        std::cout << "PASO 2: Creando banco compartido...\n";
        std::cout.flush();
        Banco* bancoCompartido = new Banco("01", "000");
        std::cout << "PASO 3: Banco creado exitosamente.\n";
        std::cout.flush();
        
        std::cout << "PASO 4: Creando árbol de sucursales...\n";
        std::cout.flush();
        ArbolAVLSucursales arbol;
        std::cout << "PASO 5: Árbol creado.\n";
        std::cout.flush();
        
        std::cout << "PASO 6: Insertando Sucursal Central...\n";
        std::cout.flush();
        arbol.insertar(new SucursalBancaria("01", "001", "Sucursal Central", bancoCompartido));
        std::cout << "PASO 7: Sucursal Central insertada.\n";
        std::cout.flush();
        
        std::cout << "PASO 8: Insertando Sucursal Norte...\n";
        std::cout.flush();
        arbol.insertar(new SucursalBancaria("01", "002", "Sucursal Norte", bancoCompartido));
        std::cout << "PASO 9: Sucursal Norte insertada.\n";
        std::cout.flush();
        
        std::cout << "PASO 10: Insertando Sucursal Sur...\n";
        std::cout.flush();
        arbol.insertar(new SucursalBancaria("01", "003", "Sucursal Sur", bancoCompartido));
        std::cout << "PASO 11: Sucursal Sur insertada.\n";
        std::cout.flush();
        
        std::cout << "PASO 12: Creando menú...\n";
        std::cout.flush();
        Menu menu(arbol);
        std::cout << "PASO 13: Menú creado.\n";
        std::cout.flush();
        
        delete bancoCompartido;
        std::cout << "PASO 14: Programa terminado exitosamente.\n";
        
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
