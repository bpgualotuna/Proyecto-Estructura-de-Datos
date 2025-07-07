#include "Banco.h"
#include <iostream>

int main() {
    try {
        // Crear una instancia del banco
        Banco banco("01", "001");
        
        std::cout << "=== PRUEBA DE GENERACIÓN DE NÚMEROS DE CUENTA BANCARIA ===\n\n";
        
        // Crear algunas cuentas de prueba
        std::cout << "Creando cuentas de ahorro:\n";
        banco.agregarCuentaAhorro("1234567890", "Juan", "Pérez", 1000.0, 15, 8, 1990);
        banco.agregarCuentaAhorro("0987654321", "María", "González", 2500.0, 20, 3, 1985);
        banco.agregarCuentaAhorro("1122334455", "Carlos", "Rodríguez", 500.0, 10, 12, 1995);
        
        std::cout << "\nCreando cuentas corrientes:\n";
        banco.agregarCuentaCorriente("1234567890", "Juan", "Pérez", 5000.0, 15, 8, 1990);
        banco.agregarCuentaCorriente("0987654321", "María", "González", 7500.0, 20, 3, 1985);
        
        std::cout << "\n=== CUENTAS CREADAS ===\n";
        banco.mostrarCuentas();
        
        std::cout << "\n=== FORMATO DE NÚMERO DE CUENTA ===\n";
        std::cout << "Formato: BB-SSS-T-NNNNNN-V\n";
        std::cout << "BB: Código del banco (01-99)\n";
        std::cout << "SSS: Código de sucursal (001-999)\n";
        std::cout << "T: Tipo de cuenta (1=Ahorro, 2=Corriente)\n";
        std::cout << "NNNNNN: Número secuencial (000001-999999)\n";
        std::cout << "V: Dígito verificador (calculado con Módulo 10)\n";
        
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
    
    return 0;
}
