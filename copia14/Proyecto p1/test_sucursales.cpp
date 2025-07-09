#include "Banco.h"
#include <iostream>

int main() {
    try {
        std::cout << "=== PRUEBA DE NÚMEROS DE CUENTA POR SUCURSAL ===\n\n";
        
        // Crear instancias de Banco para diferentes sucursales
        Banco* bancoSucursal001 = new Banco("01", "001");
        Banco* bancoSucursal002 = new Banco("01", "002");
        Banco* bancoSucursal003 = new Banco("01", "003");
        
        std::cout << "=== SUCURSAL 001 (Central) ===\n";
        std::cout << "Creando cuenta de ahorro:\n";
        bancoSucursal001->agregarCuentaAhorro("1234567890", "Juan", "Pérez", 1000.0, 15, 8, 1990);
        std::cout << "Creando cuenta corriente:\n";
        bancoSucursal001->agregarCuentaCorriente("1234567890", "Juan", "Pérez", 5000.0, 15, 8, 1990);
        
        std::cout << "\n=== SUCURSAL 002 (Norte) ===\n";
        std::cout << "Creando cuenta de ahorro:\n";
        bancoSucursal002->agregarCuentaAhorro("0987654321", "María", "González", 2500.0, 20, 3, 1985);
        std::cout << "Creando cuenta corriente:\n";
        bancoSucursal002->agregarCuentaCorriente("0987654321", "María", "González", 7500.0, 20, 3, 1985);
        
        std::cout << "\n=== SUCURSAL 003 (Sur) ===\n";
        std::cout << "Creando cuenta de ahorro:\n";
        bancoSucursal003->agregarCuentaAhorro("1122334455", "Carlos", "Rodríguez", 500.0, 10, 12, 1995);
        std::cout << "Creando cuenta corriente:\n";
        bancoSucursal003->agregarCuentaCorriente("1122334455", "Carlos", "Rodríguez", 3000.0, 10, 12, 1995);
        
        std::cout << "\n=== RESULTADOS ESPERADOS ===\n";
        std::cout << "Sucursal 001: 01-001-1-000001-X (Ahorro) y 01-001-2-000001-X (Corriente)\n";
        std::cout << "Sucursal 002: 01-002-1-000001-X (Ahorro) y 01-002-2-000001-X (Corriente)\n";
        std::cout << "Sucursal 003: 01-003-1-000001-X (Ahorro) y 01-003-2-000001-X (Corriente)\n";
        
        // Liberar memoria
        delete bancoSucursal001;
        delete bancoSucursal002;
        delete bancoSucursal003;
        
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
    
    return 0;
}
