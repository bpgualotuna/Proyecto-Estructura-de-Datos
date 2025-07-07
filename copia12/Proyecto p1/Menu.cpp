#include "Menu.h"
#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Validaciones.h"
#include "OrdenadorCuentas.h"
#include "BusquedaBinaria.h"
#include "Banco.h"
#include "FechaHora.h"
#include <iostream>
#include <conio.h>
#include <string>


Menu::Menu(ArbolAVLSucursales& arbol) : arbolSucursales(arbol), sucursalActual(nullptr) {
    horaActual.actualizarHoraActual();
    seleccionarSucursal();
}

void Menu::seleccionarSucursal() {
    system("cls");
    std::cout << "Seleccione una sucursal:\n\n";

    struct SucursalOpcion {
        SucursalBancaria* sucursal;
        char* codigo;
        char* nombre;
    };
    SucursalOpcion* opciones = nullptr;
    int conteoOpciones = 0;

    // Contar sucursales
    arbolSucursales.recorrerInOrden([&conteoOpciones](SucursalBancaria* suc) { 
        conteoOpciones++; 
    });

    // Crear arreglo de opciones
    opciones = new SucursalOpcion[conteoOpciones];
    int indice = 0;
    arbolSucursales.recorrerInOrden([&opciones, &indice](SucursalBancaria* suc) {
        opciones[indice].sucursal = suc;
        opciones[indice].codigo = suc->obtenerCodigoSucursal();
        opciones[indice].nombre = suc->obtenerNombre();
        indice++;
    });

    if (conteoOpciones == 0) {
        std::cerr << "No hay sucursales disponibles.\n";
        delete[] opciones;
        return;
    }

    int cursor = 0;
    while (true) {
        system("cls");
        std::cout << "\n=== SELECCION DE SUCURSAL ===\n";
        std::cout << "Use las flechas arriba/abajo para navegar, Enter para seleccionar:\n\n";
        for (int i = 0; i < conteoOpciones; i++) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << "Sucursal: " << opciones[i].nombre << " (Código: " << opciones[i].codigo << ")\n";
        }
        std::cout << "\nPresione Enter para seleccionar o use las flechas para navegar...\n";
        std::cout.flush();

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--;
            if (tecla == 80 && cursor < conteoOpciones - 1) cursor++;
        } else if (tecla == 13) {
            sucursalActual = opciones[cursor].sucursal;
            // Establecer la sucursal actual en el banco para la generación de números de cuenta
            sucursalActual->obtenerBanco()->establecerSucursalActual(opciones[cursor].codigo);
            std::cout << "Sucursal seleccionada: " << opciones[cursor].nombre << "\n";
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
            break;
        }
    }

    for (int i = 0; i < conteoOpciones; i++) {
        // No eliminar estos punteros porque apuntan a memoria manejada por SucursalBancaria
        // delete[] opciones[i].codigo;
        // delete[] opciones[i].nombre;
    }
    delete[] opciones;
}

void Menu::mostrar() {
    if (!sucursalActual) {
        std::cerr << "No se ha seleccionado ninguna sucursal. Saliendo...\n";
        return;
    }

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
        "Ordenar Cuentas",
        "Busqueda Binaria",
        "Hacer Copia de Seguridad",
        "Restaurar Copia de Seguridad",
        "Cifrar Archivo Binario",
        "Descifrar Archivo Binario",
        "Verificar Integridad del Archivo",
        "Busqueda Hash",
        "Cambiar Fecha del Sistema",
        "Busqueda AVL de cuentas",
        "Salir"};
    int conteoOpciones = 19;

    while (true) {
        system("cls");
        char* horaStr = horaActual.aCadena();
        char* nombreSucursal = sucursalActual->obtenerNombre();
        std::cout << "Sucursal: " << nombreSucursal << " | Hora Actual: " << horaStr << "\n\n";
        delete[] horaStr;
        delete[] nombreSucursal;

        for (int i = 0; i < conteoOpciones; i++) {
            if (i == cursor)
                std::cout << "> ";
            else
                std::cout << "  ";
            std::cout << opciones[i] << std::endl;
        }

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--;
            if (tecla == 80 && cursor < conteoOpciones - 1) cursor++;
        } else if (tecla == 13) {
            std::string id, nombre, apellido, consulta, archivoCopia;
            double monto, max;

            Banco* banco = sucursalActual->obtenerBanco();
            switch (cursor) {
            case 0: { // Agregar Cuenta de Ahorro
                std::string id = ingresar_cedula("Ingrese ID: ");
                char* nombre = nullptr;
                char* apellido = nullptr;
                int edad = 0;
                bool cedulaExistente = banco->obtenerDatosPorIdUsuario(id.c_str(), nombre, apellido, edad);

                if (cedulaExistente) {
                    std::cout << "Cedula existente encontrada. Usando datos existentes:\n";
                    std::cout << "Nombre: " << nombre << ", Apellido: " << apellido << ", Edad: " << edad << "\n";
                } else {
                    std::string tempNombre = ingresar_string("Ingrese Nombre: ");
                    std::string tempApellido = ingresar_apellido("Ingrese Apellido: ");
                    nombre = new char[tempNombre.length() + 1];
                    strcpy(nombre, tempNombre.c_str());
                    apellido = new char[tempApellido.length() + 1];
                    strcpy(apellido, tempApellido.c_str());
                }

                double monto = ingresar_saldo_inicial("Ingrese Saldo Inicial: ");
                int diaNacimiento = 1, mesNacimiento = 1, anioNacimiento = 2000;

                if (!cedulaExistente) {
                    FechaHora fechaNacimiento = ingresar_fecha_nacimiento_validada("Ingrese Fecha de Nacimiento (DD MM AAAA):\n");
                    diaNacimiento = fechaNacimiento.obtenerDia();
                    mesNacimiento = fechaNacimiento.obtenerMes();
                    anioNacimiento = fechaNacimiento.obtenerAnio();
                }

                banco->agregarCuentaAhorro(id.c_str(), nombre, apellido, monto, diaNacimiento, mesNacimiento, anioNacimiento);

                if (!cedulaExistente) {
                    delete[] nombre;
                    delete[] apellido;
                }
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 1: { // Agregar Cuenta Corriente
                std::string id = ingresar_cedula("Ingrese ID: ");
                char* nombre = nullptr;
                char* apellido = nullptr;
                int edad = 0;
                bool cedulaExistente = banco->obtenerDatosPorIdUsuario(id.c_str(), nombre, apellido, edad);

                if (cedulaExistente) {
                    std::cout << "Cedula existente encontrada. Usando datos existentes:\n";
                    std::cout << "Nombre: " << nombre << ", Apellido: " << apellido << ", Edad: " << edad << "\n";
                } else {
                    std::string tempNombre = ingresar_string("Ingrese Nombre: ");
                    std::string tempApellido = ingresar_apellido("Ingrese Apellido: ");
                    nombre = new char[tempNombre.length() + 1];
                    strcpy(nombre, tempNombre.c_str());
                    apellido = new char[tempApellido.length() + 1];
                    strcpy(apellido, tempApellido.c_str());
                }

                double monto = ingresar_saldo_inicial("Ingrese Saldo Inicial: ");
                int diaNacimiento = 1, mesNacimiento = 1, anioNacimiento = 2000;

                if (!cedulaExistente) {
                    FechaHora fechaNacimiento = ingresar_fecha_nacimiento_validada("Ingrese Fecha de Nacimiento (DD MM AAAA):\n");
                    diaNacimiento = fechaNacimiento.obtenerDia();
                    mesNacimiento = fechaNacimiento.obtenerMes();
                    anioNacimiento = fechaNacimiento.obtenerAnio();
                }

                banco->agregarCuentaCorriente(id.c_str(), nombre, apellido, monto, diaNacimiento, mesNacimiento, anioNacimiento);

                if (!cedulaExistente) {
                    delete[] nombre;
                    delete[] apellido;
                }
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 2: { // Depositar
                id = ingresar_cedula("Ingrese ID: ");
                monto = ingresar_float("Ingrese Monto: ");
                banco->depositar(id.c_str(), monto);
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 3: { // Retirar
                id = ingresar_cedula("Ingrese ID: ");
                monto = ingresar_float("Ingrese Monto: ");
                banco->retirar(id.c_str(), monto);
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 4: { // Consultar Saldo
                id = ingresar_cedula("Ingrese ID: ");
                banco->consultarSaldo(id.c_str());
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 5: { // Buscar por Rango de Fechas
                FechaHora inicio = ingresar_fecha_validada("Ingrese Fecha de Inicio (DD MM AAAA HH MM SS):\n");
                FechaHora fin = ingresar_fecha_validada("Ingrese Fecha de Fin (DD MM AAAA HH MM SS):\n");
                banco->buscarPorRangoFechas(inicio, fin);
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 6: { // Busqueda Personalizada
                std::cout << "Ingrese Consulta: ";
                std::cin.ignore();
                std::getline(std::cin, consulta);
                banco->buscarPersonalizado(consulta.c_str());
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 7: { // Buscar por Rango de Saldo
                monto = ingresar_float("Ingrese Saldo Minimo: ");
                max = ingresar_float("Ingrese Saldo Maximo: ");
                banco->buscarPorRangoSaldo(monto, max);
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 8: { // Ordenar Cuentas
                OrdenadorCuentas ordenador(*banco);
                ordenador.mostrarMenuOrdenamiento();
                break;
            }
            case 9: { // Búsqueda Binaria
                BusquedaBinaria busqueda(*banco);
                busqueda.mostrarMenu();
                break;
            }
            case 10: { // Hacer Copia de Seguridad
                banco->hacerCopiaSeguridadAutomatica();
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 11: { // Restaurar Copia de Seguridad
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo de Copia (.txt): ");
                banco->restaurar(archivoCopia.c_str());
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 12: { // Cifrar Archivo Binario
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Cifrar (.txt): ");
                banco->cifrarArchivo(archivoCopia.c_str());
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 13: { // Descifrar Archivo Binario
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Descifrar (.txt): ");
                banco->descifrarArchivo(archivoCopia.c_str());
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 14: { // Verificar Integridad del Archivo
                banco->verificarIntegridadArchivo();
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 15: { // Búsqueda Hash
                banco->busquedaHash();
                break;
            }
            case 16: { // Cambiar Fecha del Sistema
                FechaHora nuevaFecha = ingresar_fecha_validada("Ingrese Nueva Fecha y Hora (DD MM AAAA HH MM SS):\n");
                horaActual = nuevaFecha.obtenerCopia();
                std::cout << "Hora actual del programa actualizada a: ";
                char* horaStr = nuevaFecha.aCadena();
                std::cout << horaStr << "\n";
                delete[] horaStr;
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 17: { // Busqueda AVL de cuentas
                mostrarMenuBusquedaAVL();
                break;
            }
            case 18: { // Salir
                return;
            }
            }
        } else if (tecla == 0) {
            tecla = _getch();
            if (tecla == 60) { // F2
                std::ifstream archivoAyuda("ayuda.html");
                if (!archivoAyuda) {
                    std::cerr << "\nError: No se pudo encontrar el archivo ayuda.html.\n";
                    std::cout << "Presione cualquier tecla para continuar...";
                    _getch();
                } else {
                    archivoAyuda.close();
                    int resultado = system("start ayuda.html");
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

void Menu::mostrarMenuBusquedaAVL() {
    BusquedaAVL avl;
    avl.mostrarMenu();
}