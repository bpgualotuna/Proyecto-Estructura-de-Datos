#include "Banco.h"
#include <iostream>

int main() {
    try {
        std::cout << "PASO 1: Iniciando programa...\n";
        std::cout.flush();
        
        std::cout << "PASO 2: Creando banco...\n";
        std::cout.flush();
        Banco* banco = new Banco("01", "000");
        std::cout << "PASO 3: Banco creado exitosamente.\n";
        std::cout.flush();
        
        std::cout << "PASO 4: Eliminando banco...\n";
        std::cout.flush();
        delete banco;
        std::cout << "PASO 5: Programa terminado exitosamente.\n";
        
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Error desconocido" << std::endl;
        return 1;
    }
    return 0;
}
