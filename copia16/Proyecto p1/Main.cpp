#include <thread>
#include <cstdlib>
#include "Menu.h"
#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Banco.h"
#include <iostream>
#include "Marquesina.h"
#include <mutex>

std::mutex coutMutex;

int main() {
    Marquesina banner("BIENVENIDO AL SISTEMA BANCARIO");
    banner.start();

    // Lanzar marquesina SFML en hilo aparte
    std::thread hiloMarquesinaSFML([](){
        system("start marquesina_sfml.exe");
    });
    
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
        banner.stop();
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error std::exception: " << e.what() << std::endl;
        banner.stop();
        return 1;
    } catch (...) {
        std::cerr << "Error desconocido capturado" << std::endl;
        banner.stop();
        return 1;
    }
    banner.stop();
    // Esperar a que el hilo termine (opcional)
    if (hiloMarquesinaSFML.joinable()) {
        hiloMarquesinaSFML.join();
    }
    return 0;
}