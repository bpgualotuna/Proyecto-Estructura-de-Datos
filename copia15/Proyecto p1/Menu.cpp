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
#include "ArbolAVL.h"
#include <SFML/Graphics.hpp>
#include "GeneradorPDF.h"
#include "GeneradorQR.h"
#include <windows.h>



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
        "Visualizacion de arbol AVL",
        "Convertir archivo a PDF",
        "Generar código QR de cuenta",
        "Cambiar Fecha del Sistema",
        "Salir"};
    int conteoOpciones = 21;

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
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo de Copia (.txt): ");
                banco->restaurar(archivoCopia.c_str());
                break;
            }
            case 12: { // Cifrar Archivo Binario
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Cifrar (.txt): ");
                banco->cifrarArchivo(archivoCopia.c_str());
                break;
            }
            case 13: { // Descifrar Archivo Binario
                archivoCopia = ingresar_nombre_archivo("Ingrese Nombre de Archivo a Descifrar (.txt): ");
                banco->descifrarArchivo(archivoCopia.c_str());
                break;
            }
            case 14: { // Verificar Integridad del Archivo
                banco->verificarIntegridadArchivo();
                break;
            }
            case 15: { // Búsqueda Hash
                banco->busquedaHash();
                break;
            }
            case 16: { // Visualización de árbol AVL
    int cursorAVL = 0;
    const char* opcionesAVL[] = {
        "Visualizar AVL por edad de duenios de cuentas (SFML)",
        "Visualizar AVL por distancias de sucursales (SFML)",
        "Visualizar AVL por edad de dueños de cuentas (Consola)",
        "Visualizar AVL por distancias de sucursales (Consola)",
        "Buscar edad en AVL (Consola)",
        "Buscar distancia en AVL (Consola)",
        "Volver al menú principal"
    };
    int conteoOpcionesAVL = 7;
    while (true) {
        system("cls");
        std::cout << "\n=== VISUALIZACION Y BUSQUEDA DE ARBOL AVL ===\n";
        for (int i = 0; i < conteoOpcionesAVL; i++) {
            std::cout << (i == cursorAVL ? "> " : "  ");
            std::cout << opcionesAVL[i] << std::endl;
        }
        int teclaAVL = _getch();
        if (teclaAVL == 224) {
            teclaAVL = _getch();
            if (teclaAVL == 72 && cursorAVL > 0) cursorAVL--;
            if (teclaAVL == 80 && cursorAVL < conteoOpcionesAVL - 1) cursorAVL++;
        } else if (teclaAVL == 13) {
            if (cursorAVL == 0) { // AVL por edad (SFML)
                ArbolAVL arbolEdad;
                char* codigoSucursalActual = banco->obtenerSucursalActual();
                auto cuentas = banco->buscarCuentasPorId(""); // Obtener todas las cuentas
                cuentas.recorrer([&arbolEdad, codigoSucursalActual](Cuenta* cta) {
                    char* idCuenta = cta->obtenerIdCuenta();
                    if (strncmp(idCuenta + 3, codigoSucursalActual, 3) == 0) {
                        int edad = cta->obtenerEdad();
                        arbolEdad.agregarDistancia(edad);
                    }
                    delete[] idCuenta;
                });
                delete[] codigoSucursalActual;
                system("cls");
                std::cout << "========================================" << std::endl;
                std::cout << "          MOSTRAR ARBOL AVL (EDAD)      " << std::endl;
                std::cout << "========================================" << std::endl;
                sf::RenderWindow ventana(sf::VideoMode(1000, 1000), "Arbol AVL por Edad");
                sf::Font fuente;
                if (!fuente.loadFromFile("C:\\Windows\\Fonts\\times.ttf")) {
                    std::cerr << "Error al cargar la fuente." << std::endl;
                    break;
                }
                while (ventana.isOpen()) {
                    sf::Event evento;
                    while (ventana.pollEvent(evento)) {
                        if (evento.type == sf::Event::Closed) {
                            ventana.close();
                        }
                    }
                    ventana.clear(sf::Color::Black);
                    sf::Text texto("Mostrando Arbol AVL por Edad", fuente, 24);
                    texto.setFillColor(sf::Color::White);
                    sf::FloatRect bounds = texto.getLocalBounds();
                    float centerX = (ventana.getSize().x - bounds.width) / 2;
                    float topY = 10;
                    texto.setPosition(centerX, topY);
                    ventana.draw(texto);
                    arbolEdad.mostrarArbol(ventana, fuente);
                    ventana.display();
                }
            } else if (cursorAVL == 1) { // AVL por distancia (SFML)
                ArbolAVL arbolDistancia;
                while (true) {
                    system("cls");
                    std::cout << "Ingrese una distancia (o -1 para terminar): ";
                    int distancia;
                    std::cin >> distancia;
                    if (distancia == -1) break;
                    arbolDistancia.agregarDistancia(distancia);
                }
                system("cls");
                std::cout << "========================================" << std::endl;
                std::cout << "          MOSTRAR ARBOL AVL (DISTANCIA) " << std::endl;
                std::cout << "========================================" << std::endl;
                sf::RenderWindow ventana(sf::VideoMode(1000, 1000), "Arbol AVL por Distancia");
                sf::Font fuente;
                if (!fuente.loadFromFile("C:\\Windows\\Fonts\\times.ttf")) {
                    std::cerr << "Error al cargar la fuente." << std::endl;
                    break;
                }
                while (ventana.isOpen()) {
                    sf::Event evento;
                    while (ventana.pollEvent(evento)) {
                        if (evento.type == sf::Event::Closed) {
                            ventana.close();
                        }
                    }
                    ventana.clear(sf::Color::Black);
                    sf::Text texto("Mostrando Arbol AVL por Distancia", fuente, 24);
                    texto.setFillColor(sf::Color::White);
                    sf::FloatRect bounds = texto.getLocalBounds();
                    float centerX = (ventana.getSize().x - bounds.width) / 2;
                    float topY = 10;
                    texto.setPosition(centerX, topY);
                    ventana.draw(texto);
                    arbolDistancia.mostrarArbol(ventana, fuente);
                    ventana.display();
                }
            } else if (cursorAVL == 2) { // AVL por edad (Consola)
                ArbolAVL arbolEdad;
                char* codigoSucursalActual = banco->obtenerSucursalActual();
                auto cuentas = banco->buscarCuentasPorId(""); // Obtener todas las cuentas
                cuentas.recorrer([&arbolEdad, codigoSucursalActual](Cuenta* cta) {
                    char* idCuenta = cta->obtenerIdCuenta();
                    if (strncmp(idCuenta + 3, codigoSucursalActual, 3) == 0) {
                        int edad = cta->obtenerEdad();
                        arbolEdad.agregarDistancia(edad);
                    }
                    delete[] idCuenta;
                });
                delete[] codigoSucursalActual;
                system("cls");
                std::cout << "========================================" << std::endl;
                std::cout << "      ARBOL AVL POR EDAD (CONSOLA)      " << std::endl;
                std::cout << "========================================" << std::endl;
                arbolEdad.printTree();
                std::cout << "\nPresione cualquier tecla para continuar...";
                _getch();
            } else if (cursorAVL == 3) { // AVL por distancia (Consola)
                ArbolAVL arbolDistancia;
                while (true) {
                    system("cls");
                    std::cout << "Ingrese una distancia (o -1 para terminar): ";
                    int distancia;
                    std::cin >> distancia;
                    if (distancia == -1) break;
                    arbolDistancia.agregarDistancia(distancia);
                }
                system("cls");
                std::cout << "========================================" << std::endl;
                std::cout << "    ARBOL AVL POR DISTANCIA (CONSOLA)   " << std::endl;
                std::cout << "========================================" << std::endl;
                arbolDistancia.printTree();
                std::cout << "\nPresione cualquier tecla para continuar...";
                _getch();
            } else if (cursorAVL == 4) { // Buscar edad en AVL (Consola)
                ArbolAVL arbolEdad;
                char* codigoSucursalActual = banco->obtenerSucursalActual();
                auto cuentas = banco->buscarCuentasPorId(""); // Obtener todas las cuentas
                cuentas.recorrer([&arbolEdad, codigoSucursalActual](Cuenta* cta) {
                    char* idCuenta = cta->obtenerIdCuenta();
                    if (strncmp(idCuenta + 3, codigoSucursalActual, 3) == 0) {
                        int edad = cta->obtenerEdad();
                        arbolEdad.agregarDistancia(edad);
                    }
                    delete[] idCuenta;
                });
                delete[] codigoSucursalActual;
                system("cls");
                std::cout << "========================================" << std::endl;
                std::cout << "      BUSCAR EDAD EN ÁRBOL AVL          " << std::endl;
                std::cout << "========================================" << std::endl;
                int edad = ingresar_entero("Ingrese la edad a buscar: ");
                arbolEdad.buscar(edad);
                std::cout << "\nPresione cualquier tecla para continuar...";
                _getch();
            } else if (cursorAVL == 5) { // Buscar distancia en AVL (Consola)
                ArbolAVL arbolDistancia;
                while (true) {
                    system("cls");
                    int distancia = ingresar_entero("Ingrese una distancia (o -1 para terminar): ");
                    if (distancia == -1) break;
                    arbolDistancia.agregarDistancia(distancia);
                }
                system("cls");
                std::cout << "========================================" << std::endl;
                std::cout << "      BUSCAR DISTANCIA EN ARBOL AVL     " << std::endl;
                std::cout << "========================================" << std::endl;
                int distancia = ingresar_entero("Ingrese la distancia a buscar: ");
                arbolDistancia.buscar(distancia);
                std::cout << "\nPresione cualquier tecla para continuar...";
                _getch();
            } else if (cursorAVL == 6) { // Volver al menú principal
                break;
            }
        }
    }
    break;
}
            case 17: { // Convertir archivo a PDF
                std::string archivoEntrada = ingresar_nombre_archivo("Ingrese el nombre del archivo de entrada (.txt): ");
                // Generar nombre de salida automáticamente
                std::string archivoSalida = archivoEntrada;
                size_t pos = archivoSalida.rfind(".txt");
                if (pos != std::string::npos) {
                    archivoSalida.replace(pos, 4, ".pdf");
                } else {
                    archivoSalida += ".pdf";
                }
                bool exito = GeneradorPDF::convertirATxtAPDF(archivoEntrada, archivoSalida);
                if (exito) {
                    std::cout << "Archivo convertido exitosamente a PDF (simple): " << archivoSalida << std::endl;
                } else {
                    std::cout << "Error al convertir el archivo a PDF." << std::endl;
                }
                std::cout << "Presione cualquier tecla para continuar...";
                _getch();
                break;
            }
            case 18: { // Generar código QR de cuenta
                std::string cedula = ingresar_cedula("Ingrese la cedula del titular: ");
                auto cuentas = banco->buscarCuentasPorId(cedula.c_str());
                int total = 0;
                cuentas.recorrer([&total](Cuenta*){ total++; });
                if (total == 0) {
                    std::cout << "No se encontraron cuentas para esa cedula.\nPresione cualquier tecla para continuar...";
                    _getch();
                    break;
                }
                int cursor = 0;
                while (true) {
                    system("cls");
                    std::cout << "Seleccione la cuenta para generar el QR:\n\n";
                    int idx = 0;
                    cuentas.recorrer([&](Cuenta* cta){
                        char* idCuenta = cta->obtenerIdCuenta();
                        char* tipo = cta->obtenerTipo();
                        char* nombre = cta->obtenerNombrePropietario();
                        std::cout << (idx == cursor ? "> " : "  ") << idCuenta << " (" << tipo << ") - " << nombre << std::endl;
                        delete[] idCuenta;
                        delete[] tipo;
                        delete[] nombre;
                        idx++;
                    });
                    std::cout << "\nPresione Enter para seleccionar, flechas para navegar...";
                    int tecla = _getch();
                    if (tecla == 224) {
                        tecla = _getch();
                        if (tecla == 72 && cursor > 0) cursor--;
                        if (tecla == 80 && cursor < total - 1) cursor++;
                    } else if (tecla == 13) {
                        int sel = 0;
                        cuentas.recorrer([&](Cuenta* cta){
                            if (sel == cursor) {
                                char* idCuenta = cta->obtenerIdCuenta();
                                char* nombre = cta->obtenerNombrePropietario();
                                // Solo incluir número de cuenta y nombre en el QR
                                std::string dataQR = std::string("Cuenta: ") + idCuenta + "\nNombre: " + nombre;
                                std::string archivoQR = std::string(idCuenta) + ".pbm";
                                bool exito = GeneradorQR::generarQR(dataQR, archivoQR);
                                if (exito) {
                                    std::cout << "QR generado exitosamente: " << archivoQR << std::endl;
                                    std::cout << "(El QR contiene solo el número de cuenta y el nombre del titular)" << std::endl;
                                    // Generar PDF con el texto del QR
                                    std::string archivoPDFTexto = std::string(idCuenta) + ".pdf";
                                    if (GeneradorPDF::crearPDFConTexto(dataQR, archivoPDFTexto)) {
                                        std::cout << "PDF de datos generado: " << archivoPDFTexto << std::endl;
                                    } else {
                                        std::cout << "Error al generar el PDF de datos." << std::endl;
                                    }
                                    // Generar PDF con la imagen del QR
                                    std::string archivoPDFImg = std::string(idCuenta) + "_img.pdf";
                                    std::string archivoQRPNG = archivoQR;
                                    if (archivoQRPNG.size() < 4 || archivoQRPNG.substr(archivoQRPNG.size() - 4) != ".png")
                                        archivoQRPNG += ".png";
                                    if (GeneradorPDF::crearPDFConImagen(archivoQRPNG, archivoPDFImg)) {
                                        std::cout << "PDF con QR generado: " << archivoPDFImg << std::endl;
                                    } else {
                                        std::cout << "Error al generar el PDF con QR." << std::endl;
                                    }
                                } else {
                                    std::cout << "Error al generar el QR." << std::endl;
                                }
                                delete[] idCuenta;
                                delete[] nombre;
                            }
                            sel++;
                        });
                        std::cout << "Presione cualquier tecla para continuar...";
                        _getch();
                        break;
                    }
                }
                break;
            }
            case 19: { // Cambiar Fecha del Sistema
                FechaHora nuevaFecha = ingresar_fecha_validada("Ingrese Nueva Fecha y Hora (DD MM AAAA HH MM SS):\n");
                horaActual = nuevaFecha.obtenerCopia();
                std::cout << "Hora actual del programa actualizada a: ";
                char* horaStr = nuevaFecha.aCadena();
                std::cout << horaStr << "\n";
                delete[] horaStr;
                break;
            }
            case 20: { // Salir
                return;
            }
            }
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
        } else if (tecla == 0) {
            tecla = _getch();
            if (tecla == 60) { // F2
                // Lanza el servidor http-server en una terminal nueva
                system("start cmd /k \"cd /d Creardocumentacionhtml && http-server -p 8080\"");
                // Espera 2 segundos para que el servidor arranque
                Sleep(2000);
                // Abre el navegador en la ayuda
                system("start http://localhost:8080/AyudaBanco.html");
            }
        }
    }
}