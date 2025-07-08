#include "Banco.h"
#include "CuentaAhorro.h"
#include "CuentaCorriente.h"
#include "Cifrado.h"
#include "OrdenadorCuentas.h"
#include "CifradoCRC32.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <conio.h>
#include <math.h>

Banco::Banco(const char* codBanco, const char* codSucursal) : conteoAhorros(0), conteoCorrientes(0) {
    if (!codBanco || strlen(codBanco) != 2) throw "Código de banco inválido";
    if (!codSucursal || strlen(codSucursal) != 3) throw "Código de sucursal inválido";

    codigoBanco = new char[3];
    strcpy(codigoBanco, codBanco);
    codigoSucursal = new char[4];
    strcpy(codigoSucursal, codSucursal);

    // Generar nombres de archivos con código de banco y sucursal
    archivoCuentasTexto = "cuentas_" + std::string(codBanco) + "_" + std::string(codSucursal) + ".txt";
    hashCuentasTexto = "hash_cuentas_" + std::string(codBanco) + "_" + std::string(codSucursal) + ".txt";
    prefijoCopiaSeguridadTexto = "copia_seguridad_" + std::string(codBanco) + "_" + std::string(codSucursal) + "_";
    prefijoHashCopiaSeguridad = "hash_copia_seguridad_" + std::string(codBanco) + "_" + std::string(codSucursal) + "_";

    cargarDesdeArchivoTexto();
}

Banco::~Banco() {
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
    delete[] codigoBanco;
    delete[] codigoSucursal;
}


std::string Banco::generarIdUnico(const char* tipo, int conteo) {
    FechaHora ahora;
    ahora.actualizarHoraActual();
    std::stringstream ss;
    ss << tipo << "-"
       << ahora.obtenerAnio()
       << (ahora.obtenerMes() < 10 ? "0" : "") << ahora.obtenerMes()
       << (ahora.obtenerDia() < 10 ? "0" : "") << ahora.obtenerDia()
       << "-" << conteo;
    return ss.str();
}

ListaDobleCircular<Cuenta*> Banco::buscarCuentasPorId(const char* idUsuario) {
    ListaDobleCircular<Cuenta*> cuentasEncontradas;
    cuentas.recorrer([&cuentasEncontradas, idUsuario](const Cuenta* cta) {
        char* id = cta->obtenerIdUsuario();
        if (strstr(id, idUsuario)) {
            cuentasEncontradas.insertar(const_cast<Cuenta*>(cta));
        }
        delete[] id;
    });
    return cuentasEncontradas;
}

void Banco::agregarCuentaAhorro(const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento) {
    try {
        if (conteoAhorros >= 50) throw "Maximo de cuentas de ahorro alcanzado";
        std::string idUnico = generarIdUnico("AH", conteoAhorros + 1);
        // Crear cuenta con los datos proporcionados
        cuentas.insertar(new CuentaAhorro(idUnico.c_str(), idUsuario, nombre, apellido, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento));
        conteoAhorros++;
        guardarEnArchivoTexto();
        hacerCopiaSeguridadAutomatica();
        std::cout << "Cuenta de ahorro agregada exitosamente. ID Cuenta: " << idUnico << "\n";
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

bool Banco::obtenerDatosPorIdUsuario(const char* idUsuario, char*& nombre, char*& apellido, int& edad) {
    bool encontrado = false;
    cuentas.recorrer([&idUsuario, &nombre, &apellido, &edad, &encontrado](const Cuenta* cta) {
        char* id = cta->obtenerIdUsuario();
        if (strcmp(id, idUsuario) == 0) {
            nombre = cta->obtenerNombrePropietario();
            apellido = cta->obtenerApellidoPropietario();
            edad = cta->obtenerEdad();
            encontrado = true;
        } else {
            delete[] id;
        }
    });
    return encontrado;
}

void Banco::agregarCuentaCorriente(const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento) {
    try {
        if (conteoCorrientes >= 1 && saldoInicial < 500) throw "Se requiere minimo $500 para cuenta corriente adicional";
        std::string idUnico = generarIdUnico("CO", conteoCorrientes + 1);
        // Crear cuenta con los datos proporcionados
        cuentas.insertar(new CuentaCorriente(idUnico.c_str(), idUsuario, nombre, apellido, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento));
        conteoCorrientes++;
        guardarEnArchivoTexto();
        hacerCopiaSeguridadAutomatica();
        std::cout << "Cuenta corriente agregada exitosamente. ID Cuenta: " << idUnico << "\n";
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::depositar(const char* idUsuario, double monto) {
    try {
         // Truncar el monto a dos decimales (centavos)
        monto = floor(monto * 100) / 100.0;
        if (monto <= 0) throw "Monto de deposito invalido";
        ListaDobleCircular<Cuenta*> cuentasEncontradas = buscarCuentasPorId(idUsuario);
        if (cuentasEncontradas.obtenerTamano() == 0) throw "Cuenta no encontrada";

        if (cuentasEncontradas.obtenerTamano() == 1) {
            Cuenta* cuentaSeleccionada = nullptr;
            cuentasEncontradas.recorrer([&cuentaSeleccionada](const Cuenta* cta) {
                cuentaSeleccionada = const_cast<Cuenta*>(cta);
            });
            *cuentaSeleccionada += monto;
            guardarEnArchivoTexto();
            hacerCopiaSeguridadAutomatica();
            std::cout << "Deposito de $" << monto << " realizado exitosamente en cuenta " << cuentaSeleccionada->obtenerIdCuenta() << ".\n";
            return;
        }

        int cursor = 0;
        int totalCuentas = cuentasEncontradas.obtenerTamano();
        ListaDobleCircular<Cuenta*> cuentasParaSeleccion;
        cuentasEncontradas.recorrer([&cuentasParaSeleccion](const Cuenta* cta) {
            cuentasParaSeleccion.insertar(const_cast<Cuenta*>(cta));
        });

        while (true) {
            system("cls");
            std::cout << "Cuentas asociadas al ID " << idUsuario << ":\n\n";
            int indice = 1;
            cuentasEncontradas.recorrer([&indice, cursor](const Cuenta* cta) {
                std::cout << (indice - 1 == cursor ? "> " : "  ");
                char* idCuenta = cta->obtenerIdCuenta();
                char* tipo = cta->obtenerTipo();
                char* nombre = cta->obtenerNombrePropietario();
                char* apellido = cta->obtenerApellidoPropietario();
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Propietario: " << nombre << " " << apellido << ", Saldo: $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                delete[] nombre;
                delete[] apellido;
                indice++;
            });

            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72 && cursor > 0) cursor--;
                if (tecla == 80 && cursor < totalCuentas - 1) cursor++;
            } else if (tecla == 13) {
                Cuenta* cuentaSeleccionada = nullptr;
                int contador = 1;
                cuentasParaSeleccion.recorrer([&cuentaSeleccionada, &contador, cursor](const Cuenta* cta) {
                    if (contador - 1 == cursor) {
                        cuentaSeleccionada = const_cast<Cuenta*>(cta);
                    }
                    contador++;
                });

                if (!cuentaSeleccionada) throw "Seleccion invalida.";

                *cuentaSeleccionada += monto;
                guardarEnArchivoTexto();
                hacerCopiaSeguridadAutomatica();
                std::cout << "Deposito de $" << monto << " realizado exitosamente en cuenta " << cuentaSeleccionada->obtenerIdCuenta() << ".\n";
                break;
            }
        }
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::retirar(const char* idUsuario, double monto) {
    try {
        // Truncar el monto a dos decimales (centavos)
        monto = floor(monto * 100) / 100.0;
        if (monto <= 0) throw "Monto de retiro invalido";
        
        ListaDobleCircular<Cuenta*> cuentasEncontradas = buscarCuentasPorId(idUsuario);
        if (cuentasEncontradas.obtenerTamano() == 0) throw "Cuenta no encontrada";

        if (cuentasEncontradas.obtenerTamano() == 1) {
            Cuenta* cuentaSeleccionada = nullptr;
            cuentasEncontradas.recorrer([&cuentaSeleccionada](const Cuenta* cta) {
                cuentaSeleccionada = const_cast<Cuenta*>(cta);
            });
            if (!(*cuentaSeleccionada -= monto)) throw "Fondos insuficientes";
            guardarEnArchivoTexto();
            hacerCopiaSeguridadAutomatica();
            std::cout << "Retiro de $" << monto << " realizado exitosamente de cuenta " << cuentaSeleccionada->obtenerIdCuenta() << ".\n";
            return;
        }

        int cursor = 0;
        int totalCuentas = cuentasEncontradas.obtenerTamano();
        ListaDobleCircular<Cuenta*> cuentasParaSeleccion;
        cuentasEncontradas.recorrer([&cuentasParaSeleccion](const Cuenta* cta) {
            cuentasParaSeleccion.insertar(const_cast<Cuenta*>(cta));
        });

        while (true) {
            system("cls");
            std::cout << "Cuentas asociadas al ID " << idUsuario << ":\n\n";
            int indice = 1;
            cuentasEncontradas.recorrer([&indice, cursor](const Cuenta* cta) {
                std::cout << (indice - 1 == cursor ? "> " : "  ");
                char* idCuenta = cta->obtenerIdCuenta();
                char* tipo = cta->obtenerTipo();
                char* nombre = cta->obtenerNombrePropietario();
                char* apellido = cta->obtenerApellidoPropietario();
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Propietario: " << nombre << " " << apellido << ", Saldo: $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                delete[] nombre;
                delete[] apellido;
                indice++;
            });

            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72 && cursor > 0) cursor--;
                if (tecla == 80 && cursor < totalCuentas - 1) cursor++;
            } else if (tecla == 13) {
                Cuenta* cuentaSeleccionada = nullptr;
                int contador = 1;
                cuentasParaSeleccion.recorrer([&cuentaSeleccionada, &contador, cursor](const Cuenta* cta) {
                    if (contador - 1 == cursor) {
                        cuentaSeleccionada = const_cast<Cuenta*>(cta);
                    }
                    contador++;
                });

                if (!cuentaSeleccionada) throw "Seleccion invalida.";

                if (!(*cuentaSeleccionada -= monto)) throw "Fondos insuficientes";
                guardarEnArchivoTexto();
                hacerCopiaSeguridadAutomatica();
                std::cout << "Retiro de $" << monto << " realizado exitosamente de cuenta " << cuentaSeleccionada->obtenerIdCuenta() << ".\n";
                break;
            }
        }
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::consultarSaldo(const char* idUsuario) {
    try {
        ListaDobleCircular<Cuenta*> cuentasEncontradas = buscarCuentasPorId(idUsuario);
        if (cuentasEncontradas.obtenerTamano() == 0) throw "Cuenta no encontrada";

        if (cuentasEncontradas.obtenerTamano() == 1) {
            Cuenta* cuentaSeleccionada = nullptr;
            cuentasEncontradas.recorrer([&cuentaSeleccionada](const Cuenta* cta) {
                cuentaSeleccionada = const_cast<Cuenta*>(cta);
            });
            char* idCuenta = cuentaSeleccionada->obtenerIdCuenta();
            char* tipo = cuentaSeleccionada->obtenerTipo();
            char* nombre = cuentaSeleccionada->obtenerNombrePropietario();
            char* apellido = cuentaSeleccionada->obtenerApellidoPropietario();
            std::cout << "Saldo de la cuenta " << idCuenta << " (" << tipo << "), Propietario: " << nombre << " " << apellido << ": $" << (*cuentaSeleccionada)() << "\n";
            delete[] idCuenta;
            delete[] tipo;
            delete[] nombre;
            delete[] apellido;
            return;
        }

        int cursor = 0;
        int totalOpciones = cuentasEncontradas.obtenerTamano() + 1;
        ListaDobleCircular<Cuenta*> cuentasParaSeleccion;
        cuentasEncontradas.recorrer([&cuentasParaSeleccion](const Cuenta* cta) {
            cuentasParaSeleccion.insertar(const_cast<Cuenta*>(cta));
        });

        while (true) {
            system("cls");
            std::cout << "Cuentas asociadas al ID " << idUsuario << ":\n\n";
            int indice = 1;
            cuentasEncontradas.recorrer([&indice, cursor](const Cuenta* cta) {
                std::cout << (indice - 1 == cursor ? "> " : "  ");
                char* idCuenta = cta->obtenerIdCuenta();
                char* tipo = cta->obtenerTipo();
                char* nombre = cta->obtenerNombrePropietario();
                char* apellido = cta->obtenerApellidoPropietario();
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Propietario: " << nombre << " " << apellido << ": $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                delete[] nombre;
                delete[] apellido;
                indice++;
            });
            std::cout << (indice - 1 == cursor ? "> " : "  ") << "Todas las cuentas\n";

            int tecla = _getch();
            if (tecla == 224) {
                tecla = _getch();
                if (tecla == 72 && cursor > 0) cursor--;
                if (tecla == 80 && cursor < totalOpciones - 1) cursor++;
            } else if (tecla == 13) {
                if (cursor == totalOpciones - 1) {
                    cuentasEncontradas.recorrer([](const Cuenta* cta) {
                        char* idCuenta = cta->obtenerIdCuenta();
                        char* tipo = cta->obtenerTipo();
                        char* nombre = cta->obtenerNombrePropietario();
                        char* apellido = cta->obtenerApellidoPropietario();
                        std::cout << "Saldo de la cuenta " << idCuenta << " (" << tipo << "), Propietario: " << nombre << " " << apellido << ": $" << (*cta)() << "\n";
                        delete[] idCuenta;
                        delete[] tipo;
                        delete[] nombre;
                        delete[] apellido;
                    });
                    break;
                }

                Cuenta* cuentaSeleccionada = nullptr;
                int contador = 1;
                cuentasParaSeleccion.recorrer([&cuentaSeleccionada, &contador, cursor](const Cuenta* cta) {
                    if (contador - 1 == cursor) {
                        cuentaSeleccionada = const_cast<Cuenta*>(cta);
                    }
                    contador++;
                });

                if (!cuentaSeleccionada) throw "Seleccion invalida.";

                char* idCuenta = cuentaSeleccionada->obtenerIdCuenta();
                char* tipo = cuentaSeleccionada->obtenerTipo();
                char* nombre = cuentaSeleccionada->obtenerNombrePropietario();
                char* apellido = cuentaSeleccionada->obtenerApellidoPropietario();
                std::cout << "Saldo de la cuenta " << idCuenta << " (" << tipo << "), Propietario: " << nombre << " " << apellido << ": $" << (*cuentaSeleccionada)() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                delete[] nombre;
                delete[] apellido;
                break;
            }
        }
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::buscarPorRangoFechas(const FechaHora& inicio, const FechaHora& fin) {
    bool encontrado = false;
    cuentas.recorrer([&inicio, &fin, &encontrado](const Cuenta* cta) {
        if (cta->obtenerFechaCreacion().estaEnRango(inicio, fin)) {
            char* id = cta->obtenerIdCuenta();
            char* idUsuario = cta->obtenerIdUsuario();
            char* nombre = cta->obtenerNombrePropietario();
            char* apellido = cta->obtenerApellidoPropietario();
            char* tipo = cta->obtenerTipo();
            std::cout << "\nID Cuenta: " << id << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << " " << apellido << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << ", Edad: " << cta->obtenerEdad() << std::endl;
            delete[] id;
            delete[] idUsuario;
            delete[] nombre;
            delete[] apellido;
            delete[] tipo;
            encontrado = true;
        }
    });
    if (!encontrado) std::cout << "\nNo se encontraron cuentas en el rango de fechas.\n";
}

void Banco::buscarPersonalizado(const char* consulta) {
    if (!consulta || strlen(consulta) == 0) {
        std::cout << "Consulta invalida. Por favor, ingrese una consulta valida.\n";
        return;
    }

    bool encontrado = false;
    cuentas.recorrer([consulta, &encontrado](const Cuenta* cta) {
        char* idUsuario = cta->obtenerIdUsuario();
        char* nombre = cta->obtenerNombrePropietario();
        char* apellido = cta->obtenerApellidoPropietario();
        int edad = cta->obtenerEdad();
        char edadStr[4];
        snprintf(edadStr, sizeof(edadStr), "%d", edad);
        if (idUsuario && nombre && apellido && (strstr(idUsuario, consulta) || strstr(nombre, consulta) || strstr(apellido, consulta) || strstr(edadStr, consulta))) {
            char* idCuenta = cta->obtenerIdCuenta();
            char* tipo = cta->obtenerTipo();
            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << " " << apellido << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << ", Edad: " << edad << std::endl;
            delete[] idCuenta;
            delete[] tipo;
            encontrado = true;
        }
        delete[] idUsuario;
        delete[] nombre;
        delete[] apellido;
    });
    if (!encontrado) {
        std::cout << "No se encontraron cuentas que coincidan con la consulta '" << consulta << "'.\n";
    }
}

void Banco::buscarPorRangoSaldo(double min, double max) {
    bool encontrado = false;
    cuentas.recorrer([min, max, &encontrado](const Cuenta* cta) {
        if (cta->obtenerSaldo() >= min && cta->obtenerSaldo() <= max) {
            char* id = cta->obtenerIdCuenta();
            char* idUsuario = cta->obtenerIdUsuario();
            char* nombre = cta->obtenerNombrePropietario();
            char* apellido = cta->obtenerApellidoPropietario();
            char* tipo = cta->obtenerTipo();
            std::cout << "ID Cuenta: " << id << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << " " << apellido << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << ", Edad: " << cta->obtenerEdad() << std::endl;
            delete[] id;
            delete[] idUsuario;
            delete[] nombre;
            delete[] apellido;
            delete[] tipo;
            encontrado = true;
        }
    });
    if (!encontrado) std::cout << "No se encontraron cuentas en el rango de saldo.\n";
}

void Banco::hacerCopiaSeguridadAutomatica() {
    std::string nombreCopiaTexto = generarNombreCopiaSeguridad();
    hacerCopiaSeguridadTexto(nombreCopiaTexto.c_str());
}

std::string Banco::generarNombreCopiaSeguridad() {
    FechaHora ahora;
    ahora.actualizarHoraActual();
    std::stringstream ss;
    ss << prefijoCopiaSeguridadTexto
       << ahora.obtenerAnio()
       << (ahora.obtenerMes() < 10 ? "0" : "") << ahora.obtenerMes()
       << (ahora.obtenerDia() < 10 ? "0" : "") << ahora.obtenerDia()
       << "_" << (ahora.obtenerHora() < 10 ? "0" : "") << ahora.obtenerHora()
       << (ahora.obtenerMinuto() < 10 ? "0" : "") << ahora.obtenerMinuto()
       << (ahora.obtenerSegundo() < 10 ? "0" : "") << ahora.obtenerSegundo()
       << ".txt";
    return ss.str();
}




void Banco::cifrarArchivo(const char* archivo) {
    std::ifstream entrada(archivo, std::ios::binary | std::ios::ate);
    if (!entrada) {
        std::cerr << "Error al abrir archivo " << archivo << std::endl;
        return;
    }
    std::streamsize tamano = entrada.tellg();
    entrada.seekg(0, std::ios::beg);
    char* buffer = new char[tamano];
    entrada.read(buffer, tamano);
    entrada.close();

    // Cifrar buffer con XOR
    for (std::streamsize i = 0; i < tamano; i++) {
        buffer[i] = buffer[i] ^ 0x5A;
    }

    std::ofstream salida(archivo, std::ios::binary);
    if (!salida) {
        std::cerr << "Error al abrir archivo " << archivo << std::endl;
        delete[] buffer;
        return;
    }
    salida.write(buffer, tamano);
    salida.close();
    delete[] buffer;
    std::cout << "Archivo " << archivo << " cifrado exitosamente.\n";
}

void Banco::descifrarArchivo(const char* archivo) {
    std::ifstream entrada(archivo, std::ios::binary | std::ios::ate);
    if (!entrada) {
        std::cerr << "Error al abrir archivo " << archivo << std::endl;
        return;
    }
    std::streamsize tamano = entrada.tellg();
    entrada.seekg(0, std::ios::beg);
    char* buffer = new char[tamano];
    entrada.read(buffer, tamano);
    entrada.close();

    // Descifrar buffer con XOR
    for (std::streamsize i = 0; i < tamano; i++) {
        buffer[i] = buffer[i] ^ 0x5A;
    }

    std::ofstream salida(archivo, std::ios::binary);
    if (!salida) {
        std::cerr << "Error al abrir archivo " << archivo << std::endl;
        delete[] buffer;
        return;
    }
    salida.write(buffer, tamano);
    salida.close();
    delete[] buffer;
    std::cout << "Archivo " << archivo << " descifrado exitosamente.\n";
}

void Banco::ordenarCuentas(std::function<bool(const Cuenta*, const Cuenta*)> comparador) {
    // Convertir la lista doblemente circular en una lista lineal temporalmente
    if (cuentas.obtenerTamano() <= 1) return; // No hay nada que ordenar

    // Función auxiliar para dividir la lista
    auto dividirLista = [](Nodo<Cuenta*>* inicio, Nodo<Cuenta*>** izquierda, Nodo<Cuenta*>** derecha, int tamano) {
        Nodo<Cuenta*>* rapido = inicio;
        Nodo<Cuenta*>* lento = inicio;
        int mitad = tamano / 2;
        for (int i = 0; i < mitad; i++) {
            rapido = rapido->siguiente;
        }
        *izquierda = inicio;
        *derecha = rapido;
        if (rapido->anterior) rapido->anterior->siguiente = nullptr;
        if (rapido) rapido->anterior = nullptr;
    };

    // Función auxiliar para fusionar dos listas ordenadas
    auto fusionar = [comparador](Nodo<Cuenta*>* izquierda, Nodo<Cuenta*>* derecha) -> Nodo<Cuenta*>* {
        Nodo<Cuenta*>* resultado = nullptr;
        Nodo<Cuenta*>* ultimo = nullptr;

        while (izquierda != nullptr && derecha != nullptr) {
            if (comparador(izquierda->datos, derecha->datos)) { // Cambiado 'dato' por 'datos'
                if (resultado == nullptr) {
                    resultado = izquierda;
                    ultimo = resultado;
                } else {
                    ultimo->siguiente = izquierda;
                    izquierda->anterior = ultimo;
                    ultimo = izquierda;
                }
                izquierda = izquierda->siguiente;
            } else {
                if (resultado == nullptr) {
                    resultado = derecha;
                    ultimo = resultado;
                } else {
                    ultimo->siguiente = derecha;
                    derecha->anterior = ultimo;
                    ultimo = derecha;
                }
                derecha = derecha->siguiente;
            }
        }

        if (izquierda != nullptr) {
            ultimo->siguiente = izquierda;
            izquierda->anterior = ultimo;
        }
        if (derecha != nullptr) {
            ultimo->siguiente = derecha;
            derecha->anterior = ultimo;
        }

        // Encontrar el último nodo
        while (ultimo->siguiente != nullptr) {
            ultimo = ultimo->siguiente;
        }

        return resultado;
    };

    // Función auxiliar para obtener el tamaño de una sublista
    auto obtenerTamanoSublista = [](Nodo<Cuenta*>* inicio) -> int {
        int tamano = 0;
        Nodo<Cuenta*>* actual = inicio;
        while (actual != nullptr) {
            tamano++;
            actual = actual->siguiente;
        }
        return tamano;
    };

    // Función principal de Merge Sort
    std::function<Nodo<Cuenta*>* (Nodo<Cuenta*>*)> mergeSort;
    mergeSort = [&](Nodo<Cuenta*>* inicio) -> Nodo<Cuenta*>* {
        if (inicio == nullptr || inicio->siguiente == nullptr) {
            return inicio;
        }

        Nodo<Cuenta*>* izquierda = nullptr;
        Nodo<Cuenta*>* derecha = nullptr;
        int tamano = obtenerTamanoSublista(inicio);
        dividirLista(inicio, &izquierda, &derecha, tamano);

        izquierda = mergeSort(izquierda);
        derecha = mergeSort(derecha);

        return fusionar(izquierda, derecha);
    };

    // Obtener el primer nodo y romper la circularidad
    Nodo<Cuenta*>* inicio = cuentas.getCabeza(); // Cambiado 'obtenerPrimero' por 'getCabeza'
    if (inicio == nullptr) return;

    Nodo<Cuenta*>* ultimo = inicio->anterior;
    if (ultimo) ultimo->siguiente = nullptr;
    inicio->anterior = nullptr;

    // Ordenar la lista
    inicio = mergeSort(inicio);

    // Reconstruir la lista circular
    if (inicio) {
        Nodo<Cuenta*>* nuevoUltimo = inicio;
        while (nuevoUltimo->siguiente != nullptr) {
            nuevoUltimo = nuevoUltimo->siguiente;
        }
        nuevoUltimo->siguiente = inicio;
        inicio->anterior = nuevoUltimo;
    }

    // Actualizar la lista en el objeto cuentas
    cuentas.setCabeza(inicio); // Cambiado 'establecerPrimero' por 'setCabeza'
    std::cout << "Cuentas ordenadas exitosamente.\n";
}
void Banco::mostrarCuentas() const {
    if (cuentas.obtenerTamano() == 0) {
        std::cout << "No hay cuentas registradas.\n";
        return;
    }

    std::cout << "Lista de cuentas:\n";
    cuentas.recorrer([](const Cuenta* cta) {
        char* idCuenta = cta->obtenerIdCuenta();
        char* tipo = cta->obtenerTipo();
        char* nombre = cta->obtenerNombrePropietario();
        char* apellido = cta->obtenerApellidoPropietario();
        FechaHora fechaCreacion = cta->obtenerFechaCreacion();
        char* fechaStr = fechaCreacion.aCadena();
        std::cout << "ID: " << idCuenta << ", Tipo: " << tipo
                  << ", Propietario: " << nombre << " " << apellido
                  << ", Saldo: $" << cta->obtenerSaldo()
                  << ", Fecha de Creacion: " << fechaStr << "\n";
        delete[] idCuenta;
        delete[] tipo;
        delete[] nombre;
        delete[] apellido;
        delete[] fechaStr;
    });
}

void Banco::guardarEnArchivoTexto() {
    std::ofstream archivo(archivoCuentasTexto);
    if (!archivo) {
        std::cerr << "Error al abrir archivo " << archivoCuentasTexto << std::endl;
        return;
    }
    cuentas.recorrer([&archivo](const Cuenta* cta) {
        char* id = cta->obtenerIdCuenta();
        char* idUsuario = cta->obtenerIdUsuario();
        char* nombre = cta->obtenerNombrePropietario();
        char* apellido = cta->obtenerApellidoPropietario();
        char* tipo = cta->obtenerTipo();
        double saldo = cta->obtenerSaldo();
        int edad = cta->obtenerEdad();
        FechaHora fecha = cta->obtenerFechaCreacion();
        char* fechaStr = fecha.aCadena();
        archivo << id << "," << idUsuario << "," << nombre << "," << apellido << ","
                << tipo << "," << saldo << "," << edad << fechaStr << "\n";
        delete[] id;
        delete[] idUsuario;
        delete[] nombre;
        delete[] apellido;
        delete[] tipo;
        delete[] fechaStr;
    });
    archivo.close();
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(archivoCuentasTexto);
    CifradoCRC32::guardarHash(hashCuentasTexto, hash);
    std::cout << "Cuentas guardadas en " << archivoCuentasTexto << ".\n";
}

void Banco::cargarDesdeArchivoTexto() {
    uint32_t hashAlmacenado = CifradoCRC32::leerHash(hashCuentasTexto);
    uint32_t hashCalculado = CifradoCRC32::calcularCRC32Archivo(archivoCuentasTexto);
    if (hashAlmacenado != 0 && hashCalculado != hashAlmacenado) {
        std::cout << "Advertencia: El archivo " << archivoCuentasTexto << " ha sido modificado manualmente. Actualizando hash.\n";
        CifradoCRC32::guardarHash(hashCuentasTexto, hashCalculado);
    }
    std::ifstream archivo(archivoCuentasTexto);
    if (!archivo) {
        std::cout << "No se encontro archivo " << archivoCuentasTexto << ". Continuando sin cargar cuentas de texto.\n";
        return;
    }
    std::string linea;
    int cuentasLeidas = 0;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string idCuenta, idUsuario, nombre, apellido, tipo, fechaStr;
        double saldo;
        int edad, diaNacimiento, mesNacimiento, anioNacimiento;
        std::getline(ss, idCuenta, ',');
        std::getline(ss, idUsuario, ',');
        std::getline(ss, nombre, ',');
        std::getline(ss, apellido, ',');
        std::getline(ss, tipo, ',');
        ss >> saldo;
        ss.ignore(1); // Ignorar coma
        ss >> edad;
        ss.ignore(1); // Ignorar coma
        ss >> diaNacimiento;
        ss.ignore(1); // Ignorar coma
        ss >> mesNacimiento;
        ss.ignore(1); // Ignorar coma
        ss >> anioNacimiento;
        ss.ignore(1); // Ignorar coma
        std::getline(ss, fechaStr);
        int dia, mes, anio, hora, minuto, segundo;
        sscanf(fechaStr.c_str(), "%d-%d-%d_%d-%d-%d", &dia, &mes, &anio, &hora, &minuto, &segundo);
        FechaHora fecha(dia, mes, anio, hora, minuto, segundo);
        Cuenta* cuenta = nullptr;
        if (tipo == "Ahorro") {
            cuenta = new CuentaAhorro(idCuenta.c_str(), idUsuario.c_str(), nombre.c_str(), apellido.c_str(), saldo, diaNacimiento, mesNacimiento, anioNacimiento, fecha);
            conteoAhorros++;
        } else if (tipo == "Corriente") {
            cuenta = new CuentaCorriente(idCuenta.c_str(), idUsuario.c_str(), nombre.c_str(), apellido.c_str(), saldo, diaNacimiento, mesNacimiento, anioNacimiento, fecha);
            conteoCorrientes++;
        }
        if (cuenta) {
            cuentas.insertar(cuenta);
            cuentasLeidas++;
        }
    }
    archivo.close();
    if (cuentasLeidas > 0) {
        std::cout << "Carga completada desde texto. " << cuentasLeidas << " cuentas leidas.\n";
    }
}

void Banco::hacerCopiaSeguridadTexto(const char* nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "Error al abrir archivo " << nombreArchivo << std::endl;
        return;
    }
    cuentas.recorrer([&archivo](const Cuenta* cta) {
        char* id = cta->obtenerIdCuenta();
        char* idUsuario = cta->obtenerIdUsuario();
        char* nombre = cta->obtenerNombrePropietario();
        char* apellido = cta->obtenerApellidoPropietario();
        char* tipo = cta->obtenerTipo();
        double saldo = cta->obtenerSaldo();
        int edad = cta->obtenerEdad();
        FechaHora fecha = cta->obtenerFechaCreacion();
        char* fechaStr = fecha.aCadena();
        archivo << id << "," << idUsuario << "," << nombre << "," << apellido << ","
                << tipo << "," << saldo << "," << edad << "," << fechaStr << "\n";
        delete[] id;
        delete[] idUsuario;
        delete[] nombre;
        delete[] apellido;
        delete[] tipo;
        delete[] fechaStr;
    });
    archivo.close();
    std::string nombreHash = prefijoHashCopiaSeguridad + std::string(nombreArchivo).substr(prefijoCopiaSeguridadTexto.length());
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(nombreArchivo);
    CifradoCRC32::guardarHash(nombreHash, hash);
    std::cout << "Copia de seguridad de texto creada en " << nombreArchivo << ".\n";
}

void Banco::restaurarTexto(const char* nombreArchivo) {
    std::string nombreHash = prefijoHashCopiaSeguridad + std::string(nombreArchivo).substr(prefijoCopiaSeguridadTexto.length());
    uint32_t hashAlmacenado = CifradoCRC32::leerHash(nombreHash);
    uint32_t hashCalculado = CifradoCRC32::calcularCRC32Archivo(nombreArchivo);
   if (hashAlmacenado != 0 && hashCalculado != hashAlmacenado) {
        std::cout << "Advertencia: El archivo " << nombreArchivo << " ha sido modificado manualmente. Actualizando hash.\n";
        CifradoCRC32::guardarHash(nombreHash, hashCalculado);
    }
    std::ifstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "Error al abrir archivo de copia " << nombreArchivo << ".\n";
        return;
    }
    std::ofstream archivoTexto(archivoCuentasTexto);
    if (!archivoTexto) {
        std::cerr << "Error al abrir " << archivoCuentasTexto << " para restaurar.\n";
        archivo.close();
        return;
    }
    archivoTexto << archivo.rdbuf();
    archivo.close();
    archivoTexto.close();
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
    cuentas = ListaDobleCircular<Cuenta*>();
    conteoAhorros = 0;
    conteoCorrientes = 0;
    cargarDesdeArchivoTexto();
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(archivoCuentasTexto);
    CifradoCRC32::guardarHash(hashCuentasTexto, hash);
    std::cout << "Restauracion de texto completada desde " << nombreArchivo << ".\n";
}

void Banco::verificarIntegridadTexto(const std::string& nombreArchivo, const std::string& nombreHash) {
    uint32_t hashAlmacenado = CifradoCRC32::leerHash(nombreHash);
    uint32_t hashCalculado = CifradoCRC32::calcularCRC32Archivo(nombreArchivo);
   if (hashAlmacenado != 0 && hashCalculado != hashAlmacenado) {
        std::cerr << "Error: Integridad de " << nombreArchivo << " comprometida. Actualizando hash.\n";
        CifradoCRC32::guardarHash(nombreHash, hashCalculado);
    }
}

bool Banco::cuentaExiste(const char* idCuenta) {
    bool existe = false;
    cuentas.recorrer([idCuenta, &existe](const Cuenta* cuenta) {
        char* id = cuenta->obtenerIdCuenta();
        if (strcmp(id, idCuenta) == 0) {
            existe = true;
        }
        delete[] id;
    });
    return existe;
}