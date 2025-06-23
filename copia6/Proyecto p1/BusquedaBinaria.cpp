#include "BusquedaBinaria.h"
#include "Banco.h"
#include "FechaHora.h"
#include <iostream>
#include <conio.h>
#include <cstring>
#include <string>

BusquedaBinaria::BusquedaBinaria(Banco& b) : banco(b) {}

void BusquedaBinaria::mostrarMenu() {
    int cursor = 0;
    const char* opciones[] = {
        "Buscar por Nombre",
        "Buscar por Apellido",
        "Buscar por Edad",
        "Buscar por Fecha de Creacion",
        "Volver al Menu Principal"};
    int conteoOpciones = 5;

    while (true) {
        system("cls");
        std::cout << "Menu de Busqueda Binaria\n\n";
        for (int i = 0; i < conteoOpciones; i++) {
            if (i == cursor)
                std::cout << "> ";
            else
                std::cout << "  ";
            std::cout << opciones[i] << std::endl;
        }

        int tecla = _getch();
        if (tecla == 224) { // Teclas de flecha
            tecla = _getch();
            if (tecla == 72 && cursor > 0)
                cursor--; // Arriba
            if (tecla == 80 && cursor < conteoOpciones - 1)
                cursor++; // Abajo
        } else if (tecla == 13) { // Enter
            switch (cursor) {
            case 0: // Buscar por Nombre
                buscarPorNombre();
                break;
            case 1: // Buscar por Apellido
                buscarPorApellido();
                break;
            case 2: // Buscar por Edad
                buscarPorEdad();
                break;
            case 3: // Buscar por Fecha de Creación
               try {
                    std::cout << "Ingrese Fecha de Creacion a buscar (DD MM AAAA): ";
                    std::cout << "Ingrese dia entre: ";
                    int dia = ingresar_entero_limitado("", 1, 31);
                    std::cout << "Ingrese mes: ";
                    int mes = ingresar_entero_limitado("", 1, 12);
                    std::cout << "Ingrese anio: ";
                    int anio = ingresar_entero_limitado("", 1900, 9999);
                    FechaHora fechaBuscada(dia, mes, anio, 0, 0, 0);
                    ListaDobleCircular<Cuenta*> resultados = buscarPorFechaCreacion(fechaBuscada);
                    if (resultados.obtenerTamano() == 0) {
                        char* fechaStr = fechaBuscada.aCadena();
                        std::cout << "No se encontraron cuentas con la fecha de creacion " << fechaStr << ".\n";
                        delete[] fechaStr;
                    } else {
                        std::cout << "Cuentas encontradas para el " << dia << "/" << mes << "/" << anio << ":\n";
                        resultados.recorrer([](Cuenta* cta) {
                            char* idCuenta = cta->obtenerIdCuenta();
                            char* idUsuario = cta->obtenerIdUsuario();
                            char* nombre = cta->obtenerNombrePropietario();
                            char* apellido = cta->obtenerApellidoPropietario();
                            char* tipo = cta->obtenerTipo();
                            char* fechaStr = cta->obtenerFechaCreacion().aCadena();
                            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                                      << ", Nombre: " << nombre << " " << apellido
                                      << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo()
                                      << ", Fecha de Creacion: " << fechaStr << std::endl;
                            delete[] idCuenta;
                            delete[] idUsuario;
                            delete[] nombre;
                            delete[] apellido;
                            delete[] tipo;
                            delete[] fechaStr;
                        });
                    }
                } catch (const char* e) {
                    std::cerr << "Error: " << e << "\n";
                }
                break;
            case 4: // Volver
                return;
            }
            std::cout << "\nPresione cualquier tecla para continuar...";
            _getch();
        }
    }
}

void BusquedaBinaria::buscarPorNombre() {
    std::string nombreBuscado = ingresar_string("Ingrese el Nombre a buscar: ");
    
    // Ordenar cuentas por nombre
    banco.ordenarCuentas([](const Cuenta* a, const Cuenta* b) {
        char* nombreA = a->obtenerNombrePropietario();
        char* nombreB = b->obtenerNombrePropietario();
        int cmp = strcmp(nombreA, nombreB);
        delete[] nombreA;
        delete[] nombreB;
        return cmp < 0;
    });

    // Convertir la lista circular a una lista lineal temporal
    ListaDobleCircular<Cuenta*> cuentas = banco.buscarCuentasPorId(""); // Obtener todas las cuentas
    if (cuentas.obtenerTamano() == 0) {
        std::cout << "No hay cuentas registradas.\n";
        return;
    }

    // Obtener el tamaño y los nodos
    int tamano = cuentas.obtenerTamano();
    Nodo<Cuenta*>* inicio = cuentas.getCabeza();
    Nodo<Cuenta*>* ultimo = inicio->anterior;
    if (ultimo) ultimo->siguiente = nullptr;
    inicio->anterior = nullptr;

    // Realizar búsqueda binaria
    Nodo<Cuenta*>* izquierda = inicio;
    Nodo<Cuenta*>* derecha = inicio;
    for (int i = 0; i < tamano - 1 && derecha->siguiente; i++) {
        derecha = derecha->siguiente;
    }

    bool encontrado = false;
    while (izquierda != nullptr && derecha != nullptr && izquierda != derecha->siguiente) {
        Nodo<Cuenta*>* medio = izquierda;
        int pasos = 0;
        Nodo<Cuenta*>* temp = izquierda;
        while (temp != derecha && temp->siguiente != derecha) {
            temp = temp->siguiente;
            pasos++;
        }
        pasos = pasos / 2;
        for (int i = 0; i < pasos && medio->siguiente; i++) {
            medio = medio->siguiente;
        }

        char* nombreMedio = medio->datos->obtenerNombrePropietario();
        int cmp = strcmp(nombreMedio, nombreBuscado.c_str());
        delete[] nombreMedio;

        if (cmp == 0) {
            // Mostrar la cuenta encontrada
            char* idCuenta = medio->datos->obtenerIdCuenta();
            char* idUsuario = medio->datos->obtenerIdUsuario();
            char* nombre = medio->datos->obtenerNombrePropietario();
            char* apellido = medio->datos->obtenerApellidoPropietario();
            char* tipo = medio->datos->obtenerTipo();
            std::cout << "Cuenta encontrada:\n";
            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                      << ", Nombre: " << nombre << " " << apellido
                      << ", Tipo: " << tipo << ", Saldo: $" << medio->datos->obtenerSaldo()
                      << ", Edad: " << medio->datos->obtenerEdad() << std::endl;
            delete[] idCuenta;
            delete[] idUsuario;
            delete[] nombre;
            delete[] apellido;
            delete[] tipo;
            encontrado = true;

            // Buscar coincidencias adicionales hacia la izquierda
            Nodo<Cuenta*>* tempIzq = medio->anterior;
            while (tempIzq) {
                char* nombreTemp = tempIzq->datos->obtenerNombrePropietario();
                if (strcmp(nombreTemp, nombreBuscado.c_str()) == 0) {
                    char* idCuenta = tempIzq->datos->obtenerIdCuenta();
                    char* idUsuario = tempIzq->datos->obtenerIdUsuario();
                    char* nombre = tempIzq->datos->obtenerNombrePropietario();
                    char* apellido = tempIzq->datos->obtenerApellidoPropietario();
                    char* tipo = tempIzq->datos->obtenerTipo();
                    std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                              << ", Nombre: " << nombre << " " << apellido
                              << ", Tipo: " << tipo << ", Saldo: $" << tempIzq->datos->obtenerSaldo()
                              << ", Edad: " << tempIzq->datos->obtenerEdad() << std::endl;
                    delete[] idCuenta;
                    delete[] idUsuario;
                    delete[] nombre;
                    delete[] apellido;
                    delete[] tipo;
                } else {
                    delete[] nombreTemp;
                    break;
                }
                delete[] nombreTemp;
                tempIzq = tempIzq->anterior;
            }

            // Buscar coincidencias adicionales hacia la derecha
            Nodo<Cuenta*>* tempDer = medio->siguiente;
            while (tempDer) {
                char* nombreTemp = tempDer->datos->obtenerNombrePropietario();
                if (strcmp(nombreTemp, nombreBuscado.c_str()) == 0) {
                    char* idCuenta = tempDer->datos->obtenerIdCuenta();
                    char* idUsuario = tempDer->datos->obtenerIdUsuario();
                    char* nombre = tempDer->datos->obtenerNombrePropietario();
                    char* apellido = tempDer->datos->obtenerApellidoPropietario();
                    char* tipo = tempDer->datos->obtenerTipo();
                    std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                              << ", Nombre: " << nombre << " " << apellido
                              << ", Tipo: " << tipo << ", Saldo: $" << tempDer->datos->obtenerSaldo()
                              << ", Edad: " << tempDer->datos->obtenerEdad() << std::endl;
                    delete[] idCuenta;
                    delete[] idUsuario;
                    delete[] nombre;
                    delete[] apellido;
                    delete[] tipo;
                } else {
                    delete[] nombreTemp;
                    break;
                }
                delete[] nombreTemp;
                tempDer = tempDer->siguiente;
            }
            break;
        } else if (cmp < 0) {
            izquierda = medio->siguiente;
        } else {
            derecha = medio->anterior;
        }
    }

    // Restaurar la circularidad
    if (inicio && ultimo) {
        ultimo->siguiente = inicio;
        inicio->anterior = ultimo;
    }

    if (!encontrado) {
        std::cout << "No se encontraron cuentas con el nombre '" << nombreBuscado << "'.\n";
    }
}

void BusquedaBinaria::buscarPorApellido() {
    std::string apellidoBuscado = ingresar_apellido("Ingrese el Apellido a buscar: ");
    
    // Ordenar cuentas por apellido
    banco.ordenarCuentas([](const Cuenta* a, const Cuenta* b) {
        char* apellidoA = a->obtenerApellidoPropietario();
        char* apellidoB = b->obtenerApellidoPropietario();
        int cmp = strcmp(apellidoA, apellidoB);
        delete[] apellidoA;
        delete[] apellidoB;
        return cmp < 0;
    });

    // Convertir la lista circular a una lista lineal temporal
    ListaDobleCircular<Cuenta*> cuentas = banco.buscarCuentasPorId("");
    if (cuentas.obtenerTamano() == 0) {
        std::cout << "No hay cuentas registradas.\n";
        return;
    }

    int tamano = cuentas.obtenerTamano();
    Nodo<Cuenta*>* inicio = cuentas.getCabeza();
    Nodo<Cuenta*>* ultimo = inicio->anterior;
    if (ultimo) ultimo->siguiente = nullptr;
    inicio->anterior = nullptr;

    // Realizar búsqueda binaria
    Nodo<Cuenta*>* izquierda = inicio;
    Nodo<Cuenta*>* derecha = inicio;
    for (int i = 0; i < tamano - 1 && derecha->siguiente; i++) {
        derecha = derecha->siguiente;
    }

    bool encontrado = false;
    while (izquierda != nullptr && derecha != nullptr && izquierda != derecha->siguiente) {
        Nodo<Cuenta*>* medio = izquierda;
        int pasos = 0;
        Nodo<Cuenta*>* temp = izquierda;
        while (temp != derecha && temp->siguiente != derecha) {
            temp = temp->siguiente;
            pasos++;
        }
        pasos = pasos / 2;
        for (int i = 0; i < pasos && medio->siguiente; i++) {
            medio = medio->siguiente;
        }

        char* apellidoMedio = medio->datos->obtenerApellidoPropietario();
        int cmp = strcmp(apellidoMedio, apellidoBuscado.c_str());
        delete[] apellidoMedio;

        if (cmp == 0) {
            char* idCuenta = medio->datos->obtenerIdCuenta();
            char* idUsuario = medio->datos->obtenerIdUsuario();
            char* nombre = medio->datos->obtenerNombrePropietario();
            char* apellido = medio->datos->obtenerApellidoPropietario();
            char* tipo = medio->datos->obtenerTipo();
            std::cout << "Cuenta encontrada:\n";
            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                      << ", Nombre: " << nombre << " " << apellido
                      << ", Tipo: " << tipo << ", Saldo: $" << medio->datos->obtenerSaldo()
                      << ", Edad: " << medio->datos->obtenerEdad() << std::endl;
            delete[] idCuenta;
            delete[] idUsuario;
            delete[] nombre;
            delete[] apellido;
            delete[] tipo;
            encontrado = true;

            // Buscar coincidencias adicionales hacia la izquierda
            Nodo<Cuenta*>* tempIzq = medio->anterior;
            while (tempIzq) {
                char* apellidoTemp = tempIzq->datos->obtenerApellidoPropietario();
                if (strcmp(apellidoTemp, apellidoBuscado.c_str()) == 0) {
                    char* idCuenta = tempIzq->datos->obtenerIdCuenta();
                    char* idUsuario = tempIzq->datos->obtenerIdUsuario();
                    char* nombre = tempIzq->datos->obtenerNombrePropietario();
                    char* apellido = tempIzq->datos->obtenerApellidoPropietario();
                    char* tipo = tempIzq->datos->obtenerTipo();
                    std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                              << ", Nombre: " << nombre << " " << apellido
                              << ", Tipo: " << tipo << ", Saldo: $" << tempIzq->datos->obtenerSaldo()
                              << ", Edad: " << tempIzq->datos->obtenerEdad() << std::endl;
                    delete[] idCuenta;
                    delete[] idUsuario;
                    delete[] nombre;
                    delete[] apellido;
                    delete[] tipo;
                } else {
                    delete[] apellidoTemp;
                    break;
                }
                delete[] apellidoTemp;
                tempIzq = tempIzq->anterior;
            }

            // Buscar coincidencias adicionales hacia la derecha
            Nodo<Cuenta*>* tempDer = medio->siguiente;
            while (tempDer) {
                char* apellidoTemp = tempDer->datos->obtenerApellidoPropietario();
                if (strcmp(apellidoTemp, apellidoBuscado.c_str()) == 0) {
                    char* idCuenta = tempDer->datos->obtenerIdCuenta();
                    char* idUsuario = tempDer->datos->obtenerIdUsuario();
                    char* nombre = tempDer->datos->obtenerNombrePropietario();
                    char* apellido = tempDer->datos->obtenerApellidoPropietario();
                    char* tipo = tempDer->datos->obtenerTipo();
                    std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                              << ", Nombre: " << nombre << " " << apellido
                              << ", Tipo: " << tipo << ", Saldo: $" << tempDer->datos->obtenerSaldo()
                              << ", Edad: " << tempDer->datos->obtenerEdad() << std::endl;
                    delete[] idCuenta;
                    delete[] idUsuario;
                    delete[] nombre;
                    delete[] apellido;
                    delete[] tipo;
                } else {
                    delete[] apellidoTemp;
                    break;
                }
                delete[] apellidoTemp;
                tempDer = tempDer->siguiente;
            }
            break;
        } else if (cmp < 0) {
            izquierda = medio->siguiente;
        } else {
            derecha = medio->anterior;
        }
    }

    if (inicio && ultimo) {
        ultimo->siguiente = inicio;
        inicio->anterior = ultimo;
    }

    if (!encontrado) {
        std::cout << "No se encontraron cuentas con el apellido '" << apellidoBuscado << "'.\n";
    }
}

void BusquedaBinaria::buscarPorEdad() {
    int edadBuscada = ingresar_entero_limitado("Ingrese la Edad a buscar: ", 18, 150);
    
    // Ordenar cuentas por edad
    banco.ordenarCuentas([](const Cuenta* a, const Cuenta* b) {
        return a->obtenerEdad() < b->obtenerEdad();
    });

    // Convertir la lista circular a una lista lineal temporal
    ListaDobleCircular<Cuenta*> cuentas = banco.buscarCuentasPorId("");
    if (cuentas.obtenerTamano() == 0) {
        std::cout << "No hay cuentas registradas.\n";
        return;
    }

    int tamano = cuentas.obtenerTamano();
    Nodo<Cuenta*>* inicio = cuentas.getCabeza();
    Nodo<Cuenta*>* ultimo = inicio->anterior;
    if (ultimo) ultimo->siguiente = nullptr;
    inicio->anterior = nullptr;

    // Realizar búsqueda binaria
    Nodo<Cuenta*>* izquierda = inicio;
    Nodo<Cuenta*>* derecha = inicio;
    for (int i = 0; i < tamano - 1 && derecha->siguiente; i++) {
        derecha = derecha->siguiente;
    }

    bool encontrado = false;
    while (izquierda != nullptr && derecha != nullptr && izquierda != derecha->siguiente) {
        Nodo<Cuenta*>* medio = izquierda;
        int pasos = 0;
        Nodo<Cuenta*>* temp = izquierda;
        while (temp != derecha && temp->siguiente != derecha) {
            temp = temp->siguiente;
            pasos++;
        }
        pasos = pasos / 2;
        for (int i = 0; i < pasos && medio->siguiente; i++) {
            medio = medio->siguiente;
        }

        int edadMedio = medio->datos->obtenerEdad();

        if (edadMedio == edadBuscada) {
            char* idCuenta = medio->datos->obtenerIdCuenta();
            char* idUsuario = medio->datos->obtenerIdUsuario();
            char* nombre = medio->datos->obtenerNombrePropietario();
            char* apellido = medio->datos->obtenerApellidoPropietario();
            char* tipo = medio->datos->obtenerTipo();
            std::cout << "Cuenta encontrada:\n";
            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                      << ", Nombre: " << nombre << " " << apellido
                      << ", Tipo: " << tipo << ", Saldo: $" << medio->datos->obtenerSaldo()
                      << ", Edad: " << medio->datos->obtenerEdad() << std::endl;
            delete[] idCuenta;
            delete[] idUsuario;
            delete[] nombre;
            delete[] apellido;
            delete[] tipo;
            encontrado = true;

            // Buscar coincidencias adicionales hacia la izquierda
            Nodo<Cuenta*>* tempIzq = medio->anterior;
            while (tempIzq) {
                if (tempIzq->datos->obtenerEdad() == edadBuscada) {
                    char* idCuenta = tempIzq->datos->obtenerIdCuenta();
                    char* idUsuario = tempIzq->datos->obtenerIdUsuario();
                    char* nombre = tempIzq->datos->obtenerNombrePropietario();
                    char* apellido = tempIzq->datos->obtenerApellidoPropietario();
                    char* tipo = tempIzq->datos->obtenerTipo();
                    std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                              << ", Nombre: " << nombre << " " << apellido
                              << ", Tipo: " << tipo << ", Saldo: $" << tempIzq->datos->obtenerSaldo()
                              << ", Edad: " << tempIzq->datos->obtenerEdad() << std::endl;
                    delete[] idCuenta;
                    delete[] idUsuario;
                    delete[] nombre;
                    delete[] apellido;
                    delete[] tipo;
                } else {
                    break;
                }
                tempIzq = tempIzq->anterior;
            }

            // Buscar coincidencias adicionales hacia la derecha
            Nodo<Cuenta*>* tempDer = medio->siguiente;
            while (tempDer) {
                if (tempDer->datos->obtenerEdad() == edadBuscada) {
                    char* idCuenta = tempDer->datos->obtenerIdCuenta();
                    char* idUsuario = tempDer->datos->obtenerIdUsuario();
                    char* nombre = tempDer->datos->obtenerNombrePropietario();
                    char* apellido = tempDer->datos->obtenerApellidoPropietario();
                    char* tipo = tempDer->datos->obtenerTipo();
                    std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario
                              << ", Nombre: " << nombre << " " << apellido
                              << ", Tipo: " << tipo << ", Saldo: $" << tempDer->datos->obtenerSaldo()
                              << ", Edad: " << tempDer->datos->obtenerEdad() << std::endl;
                    delete[] idCuenta;
                    delete[] idUsuario;
                    delete[] nombre;
                    delete[] apellido;
                    delete[] tipo;
                } else {
                    break;
                }
                tempDer = tempDer->siguiente;
            }
            break;
        } else if (edadMedio < edadBuscada) {
            izquierda = medio->siguiente;
        } else {
            derecha = medio->anterior;
        }
    }

    if (inicio && ultimo) {
        ultimo->siguiente = inicio;
        inicio->anterior = ultimo;
    }

    if (!encontrado) {
        std::cout << "No se encontraron cuentas con la edad " << edadBuscada << ".\n";
    }
}

ListaDobleCircular<Cuenta*> BusquedaBinaria::buscarPorFechaCreacion(const FechaHora& fecha) {
    ListaDobleCircular<Cuenta*> resultados;

    // Ordenar cuentas por fecha de creación
    banco.ordenarCuentas([](const Cuenta* a, const Cuenta* b) {
        return a->obtenerFechaCreacion() < b->obtenerFechaCreacion();
    });

    // Convertir la lista circular a una lista lineal temporal
    ListaDobleCircular<Cuenta*> cuentas = banco.buscarCuentasPorId("");
    if (cuentas.obtenerTamano() == 0) {
        return resultados;
    }

    int tamano = cuentas.obtenerTamano();
    Nodo<Cuenta*>* inicio = cuentas.getCabeza();
    Nodo<Cuenta*>* ultimo = inicio->anterior;
    if (ultimo) ultimo->siguiente = nullptr;
    inicio->anterior = nullptr;

    // Normalizar la fecha de búsqueda al inicio del día
    FechaHora fechaInicio = fecha.normalizarDia();
    FechaHora fechaFin(fechaInicio.obtenerDia(), fechaInicio.obtenerMes(), fechaInicio.obtenerAnio(), 23, 59, 59);

    // Búsqueda binaria para encontrar el inicio del rango
    Nodo<Cuenta*>* izquierda = inicio;
    Nodo<Cuenta*>* derecha = inicio;
    for (int i = 0; i < tamano - 1 && derecha->siguiente; i++) {
        derecha = derecha->siguiente;
    }

    Nodo<Cuenta*>* puntoMedio = nullptr;
    while (izquierda != nullptr && derecha != nullptr && izquierda != derecha->siguiente) {
        Nodo<Cuenta*>* medio = izquierda;
        int pasos = 0;
        Nodo<Cuenta*>* temp = izquierda;
        while (temp != derecha && temp->siguiente != derecha) {
            temp = temp->siguiente;
            pasos++;
        }
        pasos = pasos / 2;
        for (int i = 0; i < pasos && medio->siguiente; i++) {
            medio = medio->siguiente;
        }

        FechaHora fechaMedio = medio->datos->obtenerFechaCreacion();
        if (fechaMedio.esIgualDia(fechaInicio)) {
            puntoMedio = medio;
            break;
        } else if (fechaMedio < fechaInicio) {
            izquierda = medio->siguiente;
        } else {
            derecha = medio->anterior;
        }
    }

    // Si no se encontró una cuenta en el día exacto
    if (!puntoMedio) {
        if (inicio && ultimo) {
            ultimo->siguiente = inicio;
            inicio->anterior = ultimo;
        }
        return resultados;
    }

    // Recolectar todas las cuentas del mismo día
    Nodo<Cuenta*>* actual = puntoMedio;
    while (actual) {
        FechaHora fechaActual = actual->datos->obtenerFechaCreacion();
        if (fechaActual.esIgualDia(fechaInicio)) {
            resultados.insertar(actual->datos);
        } else {
            break;
        }
        actual = actual->anterior;
    }

    actual = puntoMedio->siguiente;
    while (actual) {
        FechaHora fechaActual = actual->datos->obtenerFechaCreacion();
        if (fechaActual.esIgualDia(fechaInicio)) {
            resultados.insertar(actual->datos);
        } else {
            break;
        }
        actual = actual->siguiente;
    }

    // Restaurar la circularidad
    if (inicio && ultimo) {
        ultimo->siguiente = inicio;
        inicio->anterior = ultimo;
    }

    return resultados;
}