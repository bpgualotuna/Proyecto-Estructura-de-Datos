#include "OrdenadorCuentas.h"
#include "Banco.h"
#include "Cuenta.h"
#include "FechaHora.h"

OrdenadorCuentas::OrdenadorCuentas(Banco& b) : banco(b) {}

void OrdenadorCuentas::mostrarMenuOrdenamiento() {
    const char* criterios[] = {"Nombre", "Apellido", "Fecha de Creacion"};
    int criterioCursor = 0;
    const int totalCriterios = 3;

    while (true) {
        system("cls");
        std::cout << "Criterios de ordenamiento:\n\n";
        for (int i = 0; i < totalCriterios; i++) {
            std::cout << (i == criterioCursor ? "> " : "  ") << criterios[i] << "\n";
        }

        int criterioTecla = _getch();
        if (criterioTecla == 224) {
            criterioTecla = _getch();
            if (criterioTecla == 72 && criterioCursor > 0) criterioCursor--; // Arriba
            if (criterioTecla == 80 && criterioCursor < totalCriterios - 1) criterioCursor++; // Abajo
        } else if (criterioTecla == 13) { // Enter
            std::function<bool(const Cuenta*, const Cuenta*)> comparador;
            if (criterioCursor == 0) { // Nombre
                comparador = [](const Cuenta* a, const Cuenta* b) {
                    char* nombreA = a->obtenerNombrePropietario();
                    char* nombreB = b->obtenerNombrePropietario();
                    int resultado = strcmp(nombreA, nombreB);
                    delete[] nombreA;
                    delete[] nombreB;
                    return resultado < 0;
                };
            } else if (criterioCursor == 1) { // Apellido
                comparador = [](const Cuenta* a, const Cuenta* b) {
                    char* apellidoA = a->obtenerApellidoPropietario();
                    char* apellidoB = b->obtenerApellidoPropietario();
                    int resultado = strcmp(apellidoA, apellidoB);
                    delete[] apellidoA;
                    delete[] apellidoB;
                    return resultado < 0;
                };
            } else { // Fecha de Creación
                comparador = [](const Cuenta* a, const Cuenta* b) {
                    FechaHora fechaA = a->obtenerFechaCreacion();
                    FechaHora fechaB = b->obtenerFechaCreacion();
                    return fechaA < fechaB;
                };
            }
            banco.ordenarCuentas(comparador);
            std::cout << "\n"; // Espacio para mejor legibilidad
            banco.mostrarCuentas(); // Mostrar las cuentas ordenadas
            
            _getch();
            break;
        }
    }
}

// const char* criterios[] = {"Nombre", "Apellido", "Fecha de Creacion", "Saldo"};
// int totalCriterios = 4;
// if (criterioCursor == 3) { // Saldo
//     comparador = [](const Cuenta* a, const Cuenta* b) {
//         return a->obtenerSaldo() < b->obtenerSaldo();
//     };
// }
