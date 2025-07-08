#include "Banco.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== PRUEBA DE SUCURSAL DINÁMICA ===\n\n";
        
        // Crear banco compartido
        Banco* banco = new Banco("01", "000");
        
        std::cout << "Sucursal inicial: " << banco->obtenerSucursalActual() << "\n\n";
        
        // Simular selección de sucursal 001
        std::cout << "Cambiando a sucursal 001...\n";
        banco->establecerSucursalActual("001");
        std::cout << "Sucursal actual: " << banco->obtenerSucursalActual() << "\n";
        banco->agregarCuentaAhorro("1234567890", "Juan", "Pérez", 1000.0, 15, 8, 1990);
        
        // Simular selección de sucursal 002
        std::cout << "\nCambiando a sucursal 002...\n";
        banco->establecerSucursalActual("002");
        std::cout << "Sucursal actual: " << banco->obtenerSucursalActual() << "\n";
        banco->agregarCuentaAhorro("0987654321", "María", "González", 2500.0, 20, 3, 1985);
        
        // Simular selección de sucursal 003
        std::cout << "\nCambiando a sucursal 003...\n";
        banco->establecerSucursalActual("003");
        std::cout << "Sucursal actual: " << banco->obtenerSucursalActual() << "\n";
        banco->agregarCuentaCorriente("1122334455", "Carlos", "Rodríguez", 5000.0, 10, 12, 1995);
        
        std::cout << "\n=== NÚMEROS ESPERADOS ===\n";
        std::cout << "Sucursal 001: 01-001-1-000001-X\n";
        std::cout << "Sucursal 002: 01-002-1-000002-X\n";
        std::cout << "Sucursal 003: 01-003-2-000001-X\n";
        
        delete banco;
        
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
    
    return 0;
}
