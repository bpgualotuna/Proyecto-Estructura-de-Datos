#include "Menu.h"
#include "Validaciones.h"
#include <iostream>
#include <conio.h>
#include <string>

Menu::Menu(Banco& b) : banco(b) {
    horaActual.actualizarHoraActual();
}

void Menu::mostrar() {
    int cursor = 0;
    const char* opciones[] = {
        "Agregar Cuenta de Ahorro",
        "Agregar Cuenta Corriente",
        "Depositar",
        "Retirar",
        "Consultar Saldo",
        "Buscar por Rango de Fechas",
        "Busqueda Personalizada",
        "Buscar por Rango de Saldo",
        "Hacer Copia de Seguridad",
        "Restaurar Copia de Seguridad",
        "Cifrar Archivo Binario",
        "Descifrar Archivo Binario",
        "Cambiar Fecha del Sistema",
        "Salir"
    };
    int conteoOpciones = 14;

    while (true) {
        system("cls");
        char* horaStr = horaActual.aCadena();
        std::cout << "Hora Actual: " << horaStr << "\n\n";
        delete[] horaStr;

        for (int i = 0; i < conteoOpciones; i++) {
            if (i == cursor) std::cout << "> ";
            else std::cout << "  ";
            std::cout << opciones[i] << std::endl;
        }

        

        int tecla = _getch();
        if (tecla == 224) { // Teclas de flecha
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--; // Arriba
            if (tecla == 80 && cursor < conteoOpciones - 1) cursor++; // Abajo
        } else if (tecla == 13) { // Enter
            std::string id, nombre, apellido, consulta, archivoCopia;
            double monto, max;
            int dia, mes, anio, hora, minuto, segundo;

            switch (cursor) {
                case 0: { // Agregar Cuenta de Ahorro
                    id = ingresar_cedula("Ingrese ID: ");
                    nombre = ingresar_string("Ingrese Nombre: ");
                    apellido = ingresar_apellido("Ingrese Apellido: ");
                    monto = ingresar_saldo_inicial("Ingrese Saldo Inicial: ");
                    FechaHora fechaNacimiento = ingresar_fecha_nacimiento_validada("Ingrese Fecha de Nacimiento (DD MM AAAA):\n");
                    banco.agregarCuentaAhorro(id.c_str(), nombre.c_str(), apellido.c_str(), monto, 
                                             fechaNacimiento.obtenerDia(), 
                                             fechaNacimiento.obtenerMes(), 
                                             fechaNacimiento.obtenerAnio());
                    break;
                }
                case 1: { // Agregar Cuenta Corriente
                    id = ingresar_cedula("Ingrese ID: ");
                    nombre = ingresar_string("Ingrese Nombre: ");
                    apellido = ingresar_apellido("Ingrese Apellido: ");
                    monto = ingresar_saldo_inicial("Ingrese Saldo Inicial: ");
                    FechaHora fechaNacimiento = ingresar_fecha_nacimiento_validada("Ingrese Fecha de Nacimiento (DD MM AAAA):\n");
                    banco.agregarCuentaCorriente(id.c_str(), nombre.c_str(), apellido.c_str(), monto, 
                                                fechaNacimiento.obtenerDia(), 
                                                fechaNacimiento.obtenerMes(), 
                                                fechaNacimiento.obtenerAnio());
                    break;
                }
                case 2: { // Depositar
                    id = ingresar_cedula("Ingrese ID: ");
                    monto = ingresar_float("Ingrese Monto: ");
                    banco.depositar(id.c_str(), monto);
                    break;
                }
                case 3: { // Retirar
                    id = ingresar_cedula("Ingrese ID: ");
                    monto = ingresar_float("Ingrese Monto: ");
                    banco.retirar(id.c_str(), monto);
                    break;
                }
                case 4: { // Consultar Saldo
                    id = ingresar_cedula("Ingrese ID: ");
                    banco.consultarSaldo(id.c_str());
                    break;
                }
                case 5: { // Buscar por Rango de Fechas
                   FechaHora inicio = ingresar_fecha_validada("Ingrese Fecha de Inicio (DD MM AAAA HH MM SS):\n");
                    FechaHora fin = ingresar_fecha_validada("Ingrese Fecha de Fin (DD MM AAAA HH MM SS):\n");
                    banco.buscarPorRangoFechas(inicio, fin);
                    break;
                }
                case 6: { // Busqueda Personalizada
                    std::cout << "Ingrese Consulta: ";
                    std::cin.ignore();
                    std::getline(std::cin, consulta);
                    banco.buscarPersonalizado(consulta.c_str());
                    break;
                }
                case 7: { // Buscar por Rango de Saldo
                    monto = ingresar_float("Ingrese Saldo Minimo: ");
                    max = ingresar_float("Ingrese Saldo Maximo: ");
                    banco.buscarPorRangoSaldo(monto, max);
                    break;
                }
                case 8: { // Hacer Copia de Seguridad
                    banco.hacerCopiaSeguridadAutomatica();
                    break;
                }
                case 9: { // Restaurar Copia de Seguridad
                    archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo de Copia (.bin): ");
                    banco.restaurar(archivoCopia.c_str());
                    break;
                }
                case 10: { // Cifrar Archivo Binario
                    archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Cifrar (.bin): ");
                    banco.cifrarArchivo(archivoCopia.c_str());
                    break;
                }
                case 11: { // Descifrar Archivo Binario
                    archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Descifrar (.bin): ");
                    banco.descifrarArchivo(archivoCopia.c_str());
                    break;
                }
                case 12: { // Cambiar Fecha del Sistema
                      FechaHora nuevaFecha = ingresar_fecha_validada("Ingrese Nueva Fecha y Hora (DD MM AAAA HH MM SS):\n");
                     horaActual = nuevaFecha.obtenerCopia(); // Actualizar horaActual localmente
                      std::cout << "Hora actual del programa actualizada a: ";
                     char* horaStr = nuevaFecha.aCadena();
                     std::cout << horaStr << "\n";
                     delete[] horaStr;
                     break;
                }
                case 13: { // Salir
                    return;
                }
            }
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
       } else if (tecla == 0) { // Teclas de función (F1-F12)
            tecla = _getch();
            if (tecla == 60) { // F2
                // Verificar si el archivo ayuda.html existe
                std::ifstream archivoAyuda("ayuda.html");
                if (!archivoAyuda) {
                    std::cerr << "\nError: No se pudo encontrar el archivo ayuda.html.\n";
                    std::cout << "Presione cualquier tecla para continuar...";
                    _getch();
                } else {
                    archivoAyuda.close();
                    int resultado = system("start ayuda.html"); // Abrir ayuda.html en el navegador predeterminado (Windows)
                    if (resultado != 0) {
                        std::cerr << "\nError: No se pudo abrir el archivo de ayuda. Asegúrese de que un navegador esté configurado.\n";
                        std::cout << "Presione cualquier tecla para continuar...";
                        _getch();
                    }
                }
            }
        }
    }
}
