#include "Menu.h"
#include "ArbolAVLSucursales.h"
#include "SucursalBancaria.h"
#include "Validaciones.h"
#include "OrdenadorCuentas.h"
#include "BusquedaBinaria.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h> // Para SetConsoleOutputCP


Menu::Menu(ArbolAVLSucursales& arbol) : arbolSucursales(arbol), sucursalActual(nullptr), oficinaActual(nullptr) {
    SetConsoleOutputCP(CP_UTF8); // Establecer codificación UTF-8 para la consola
    horaActual.actualizarHoraActual();
    seleccionarSucursal();
    if (sucursalActual) {
        seleccionarOficina();
    }
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
    arbolSucursales.recorrerInOrden([&conteoOpciones](SucursalBancaria* suc) { conteoOpciones++; });

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
        std::cerr << "No hay sucursales disponibles. Presione cualquier tecla para salir...\n";
        _getch();
        delete[] opciones;
        return;
    }
    int cursor = 0;
    while (true) {
        system("cls");
        std::cout << "Seleccione una sucursal:\n\n";
        for (int i = 0; i < conteoOpciones; i++) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << "Sucursal: " << opciones[i].nombre << " (Código: " << opciones[i].codigo << ")\n";
        }

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--;
            if (tecla == 80 && cursor < conteoOpciones - 1) cursor++;
        } else if (tecla == 13) {
            sucursalActual = opciones[cursor].sucursal;
            std::cout << "Sucursal seleccionada: " << opciones[cursor].nombre << "\n";
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
            break;
        }
    }

    for (int i = 0; i < conteoOpciones; i++) {
        delete[] opciones[i].codigo;
        delete[] opciones[i].nombre;
    }
    delete[] opciones;
}

void Menu::seleccionarOficina() {
    system("cls");
    std::cout << "Seleccione una oficina:\n\n";

    struct OficinaOpcion {
        Oficina* oficina;
        char* codigo;
        char* nombre;
    };
    OficinaOpcion* opciones = nullptr;
    int conteoOpciones = 0;

    ListaDobleCircular<Oficina*> oficinas = sucursalActual->obtenerOficinas();
    oficinas.recorrer([&conteoOpciones](Oficina* of) { conteoOpciones++; });

    opciones = new OficinaOpcion[conteoOpciones];
    int indice = 0;
    oficinas.recorrer([&opciones, &indice](Oficina* of) {
        opciones[indice].oficina = of;
        opciones[indice].codigo = of->obtenerCodigoOficina();
        opciones[indice].nombre = of->obtenerNombre();
        indice++;
    });

    if (conteoOpciones == 0) {
        std::cerr << "No hay oficinas disponibles.\n";
        delete[] opciones;
        std::cout << "Presione cualquier tecla para regresar al menú de sucursales...";
        _getch();
        seleccionarSucursal(); // Volver a seleccionar sucursal si no hay oficinas
        return;
    }

    int cursor = 0;
    while (true) {
        system("cls");
        std::cout << "Seleccione una oficina:\n\n";
        for (int i = 0; i < conteoOpciones; i++) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << "Oficina: " << opciones[i].nombre << " (Código: " << opciones[i].codigo << ")\n";
        }

        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--;
            if (tecla == 80 && cursor < conteoOpciones - 1) cursor++;
        } else if (tecla == 13) {
            oficinaActual = opciones[cursor].oficina;
            std::cout << "Oficina seleccionada: " << opciones[cursor].nombre << "\n";
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
            break;
        }
    }

    for (int i = 0; i < conteoOpciones; i++) {
        delete[] opciones[i].codigo;
        delete[] opciones[i].nombre;
    }
    delete[] opciones;
}

void Menu::mostrar() {
    if (!sucursalActual || !oficinaActual) {
        std::cerr << "No se ha seleccionado ninguna sucursal u oficina. Saliendo...\n";
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
        "Cambiar Fecha del Sistema",
        "Actividad en Oficina",
        "Cambiar Sucursal",
        "Cambiar Oficina",
        "Salir"};
    int conteoOpciones = 19;

    while (true) {
        system("cls");
        char* horaStr = horaActual.aCadena();
        char* nombreSucursal = sucursalActual->obtenerNombre();
        char* nombreOficina = oficinaActual->obtenerNombre();
        std::cout << "Sucursal: " << nombreSucursal << " | Oficina: " << nombreOficina << " | Hora Actual: " << horaStr << "\n\n";
        delete[] horaStr;
        delete[] nombreSucursal;
        delete[] nombreOficina;

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
            char* codOficina = oficinaActual->obtenerCodigoOficina();
            Banco* banco = sucursalActual->obtenerBanco();
            FechaHora fechaActual;
            fechaActual.actualizarHoraActual();
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
                break;
            }
            case 2: { // Depositar
                id = ingresar_cedula("Ingrese ID: ");
                monto = ingresar_float("Ingrese Monto: ");
                banco->depositar(id.c_str(), monto);
                break;
            }
            case 3: { // Retirar
                id = ingresar_cedula("Ingrese ID: ");
                monto = ingresar_float("Ingrese Monto: ");
                banco->retirar(id.c_str(), monto);
                break;
            }
            case 4: { // Consultar Saldo
                id = ingresar_cedula("Ingrese ID: ");
                banco->consultarSaldo(id.c_str());
                break;
            }
            case 5: { // Buscar por Rango de Fechas
                FechaHora inicio = ingresar_fecha_validada("Ingrese Fecha de Inicio (DD MM AAAA HH MM SS):\n");
                FechaHora fin = ingresar_fecha_validada("Ingrese Fecha de Fin (DD MM AAAA HH MM SS):\n");
                banco->buscarPorRangoFechas(inicio, fin);
                break;
            }
            case 6: { // Busqueda Personalizada
                std::cout << "Ingrese Consulta: ";
                std::cin.ignore();
                std::getline(std::cin, consulta);
                banco->buscarPersonalizado(consulta.c_str());
                break;
            }
            case 7: { // Buscar por Rango de Saldo
                monto = ingresar_float("Ingrese Saldo Minimo: ");
                max = ingresar_float("Ingrese Saldo Maximo: ");
                banco->buscarPorRangoSaldo(monto, max);
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
                break;
            }
            case 11: { // Restaurar Copia de Seguridad
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo de Copia (.bin): ");
                banco->restaurar(archivoCopia.c_str());
                break;
            }
            case 12: { // Cifrar Archivo Binario
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Cifrar (.bin): ");
                banco->cifrarArchivo(archivoCopia.c_str());
                break;
            }
            case 13: { // Descifrar Archivo Binario
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Descifrar (.bin): ");
                banco->descifrarArchivo(archivoCopia.c_str());
                break;
            }
            case 14: { // Cambiar Fecha del Sistema
                FechaHora nuevaFecha = ingresar_fecha_validada("Ingrese Nueva Fecha y Hora (DD MM AAAA HH MM SS):\n");
                horaActual = nuevaFecha.obtenerCopia();
                std::cout << "Hora actual del programa actualizada a: ";
                char* horaStr = nuevaFecha.aCadena();
                std::cout << horaStr << "\n";
                delete[] horaStr;
                break;
            }
            case 15: { // Mostrar Actividades de Oficina
                char* codSucursal = sucursalActual->obtenerCodigoSucursal();
                std::string nombreArchivo = "Sucursal_" + std::string(codSucursal) + "_Oficina_" + std::string(codOficina) + ".txt";
                delete[] codSucursal;
                std::ifstream archivo(nombreArchivo);
                if (!archivo.is_open()) {
                    std::cout << "No hay actividades registradas para esta oficina.\n";
                } else {
                    std::cout << "Actividades registradas en " << nombreOficina << ":\n\n";
                    std::string linea;
                    while (std::getline(archivo, linea)) {
                        std::cout << linea << "\n";
                    }
                    archivo.close();
                }
                std::cout << "\nPresione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 16: { // Cambiar Sucursal
                seleccionarSucursal();
                break;
            }
            case 17: { // Cambiar Oficina
                seleccionarOficina();
                break;
            }
            case 18: { // Salir
                delete[] codOficina;
                return;
            }
            }
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
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