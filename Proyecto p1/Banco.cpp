#include "Banco.h"
#include "CuentaAhorro.h"
#include "CuentaCorriente.h"
#include "Cifrado.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <conio.h>

Banco::Banco() : conteoAhorros(0), conteoCorrientes(0) {
    cargarDesdeArchivo();
}

Banco::~Banco() {
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
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

void Banco::agregarCuentaAhorro(const char* idUsuario, const char* nombre, double saldoInicial) {
    try {
        if (conteoAhorros >= 50) throw "Maximo de cuentas de ahorro alcanzado";
        std::string idUnico = generarIdUnico("AH", conteoAhorros + 1);
        cuentas.insertar(new CuentaAhorro(idUnico.c_str(), idUsuario, nombre, saldoInicial));
        conteoAhorros++;
        guardarEnArchivo();
        hacerCopiaSeguridadAutomatica();
        std::cout << "Cuenta de ahorro agregada exitosamente. ID Cuenta: " << idUnico << "\n";
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::agregarCuentaCorriente(const char* idUsuario, const char* nombre, double saldoInicial) {
    try {
        if (conteoCorrientes >= 1 && saldoInicial < 500) throw "Se requiere minimo $500 para cuenta corriente adicional";
        std::string idUnico = generarIdUnico("CO", conteoCorrientes + 1);
        cuentas.insertar(new CuentaCorriente(idUnico.c_str(), idUsuario, nombre, saldoInicial));
        conteoCorrientes++;
        guardarEnArchivo();
        hacerCopiaSeguridadAutomatica();
        std::cout << "Cuenta corriente agregada exitosamente. ID Cuenta: " << idUnico << "\n";
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::depositar(const char* idUsuario, double monto) {
    try {
        ListaDobleCircular<Cuenta*> cuentasEncontradas = buscarCuentasPorId(idUsuario);
        if (cuentasEncontradas.obtenerTamano() == 0) throw "Cuenta no encontrada";

        if (cuentasEncontradas.obtenerTamano() == 1) {
            // Si solo hay una cuenta, usarla directamente
            Cuenta* cuentaSeleccionada = nullptr;
            cuentasEncontradas.recorrer([&cuentaSeleccionada](const Cuenta* cta) {
                cuentaSeleccionada = const_cast<Cuenta*>(cta);
            });
            *cuentaSeleccionada += monto;
            guardarEnArchivo();
            hacerCopiaSeguridadAutomatica();
            std::cout << "Deposito de $" << monto << " realizado exitosamente en cuenta " << cuentaSeleccionada->obtenerIdCuenta() << ".\n";
            return;
        }

        // Submenú con flechas y Enter
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
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Saldo: $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                indice++;
            });

            int tecla = _getch();
            if (tecla == 224) { // Teclas de flecha
                tecla = _getch();
                if (tecla == 72 && cursor > 0) cursor--; // Arriba
                if (tecla == 80 && cursor < totalCuentas - 1) cursor++; // Abajo
            } else if (tecla == 13) { // Enter
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
                guardarEnArchivo();
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
        ListaDobleCircular<Cuenta*> cuentasEncontradas = buscarCuentasPorId(idUsuario);
        if (cuentasEncontradas.obtenerTamano() == 0) throw "Cuenta no encontrada";

        if (cuentasEncontradas.obtenerTamano() == 1) {
            // Si solo hay una cuenta, usarla directamente
            Cuenta* cuentaSeleccionada = nullptr;
            cuentasEncontradas.recorrer([&cuentaSeleccionada](const Cuenta* cta) {
                cuentaSeleccionada = const_cast<Cuenta*>(cta);
            });
            if (!(*cuentaSeleccionada -= monto)) throw "Fondos insuficientes";
            guardarEnArchivo();
            hacerCopiaSeguridadAutomatica();
            std::cout << "Retiro de $" << monto << " realizado exitosamente de cuenta " << cuentaSeleccionada->obtenerIdCuenta() << ".\n";
            return;
        }

        // Submenú con flechas y Enter
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
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Saldo: $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                indice++;
            });

            int tecla = _getch();
            if (tecla == 224) { // Teclas de flecha
                tecla = _getch();
                if (tecla == 72 && cursor > 0) cursor--; // Arriba
                if (tecla == 80 && cursor < totalCuentas - 1) cursor++; // Abajo
            } else if (tecla == 13) { // Enter
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
                guardarEnArchivo();
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
            // Si solo hay una cuenta, mostrar su saldo directamente
            Cuenta* cuentaSeleccionada = nullptr;
            cuentasEncontradas.recorrer([&cuentaSeleccionada](const Cuenta* cta) {
                cuentaSeleccionada = const_cast<Cuenta*>(cta);
            });
            char* idCuenta = cuentaSeleccionada->obtenerIdCuenta();
            char* tipo = cuentaSeleccionada->obtenerTipo();
            std::cout << "Saldo de la cuenta " << idCuenta << " (" << tipo << "): $" << (*cuentaSeleccionada)() << "\n";
            delete[] idCuenta;
            delete[] tipo;
            return;
        }

        // Submenú con flechas y Enter, incluyendo "Todas las cuentas"
        int cursor = 0;
        int totalOpciones = cuentasEncontradas.obtenerTamano() + 1; // +1 para "Todas las cuentas"
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
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Saldo: $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
                indice++;
            });
            std::cout << (indice - 1 == cursor ? "> " : "  ") << "Todas las cuentas\n";

            int tecla = _getch();
            if (tecla == 224) { // Teclas de flecha
                tecla = _getch();
                if (tecla == 72 && cursor > 0) cursor--; // Arriba
                if (tecla == 80 && cursor < totalOpciones - 1) cursor++; // Abajo
            } else if (tecla == 13) { // Enter
                if (cursor == totalOpciones - 1) {
                    // Mostrar todas las cuentas
                    cuentasEncontradas.recorrer([](const Cuenta* cta) {
                        char* idCuenta = cta->obtenerIdCuenta();
                        char* tipo = cta->obtenerTipo();
                        std::cout << "Saldo de la cuenta " << idCuenta << " (" << tipo << "): $" << (*cta)() << "\n";
                        delete[] idCuenta;
                        delete[] tipo;
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
                std::cout << "Saldo de la cuenta " << idCuenta << " (" << tipo << "): $" << (*cuentaSeleccionada)() << "\n";
                delete[] idCuenta;
                delete[] tipo;
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
            char* tipo = cta->obtenerTipo();
            std::cout << "\nID Cuenta: " << id << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << std::endl;
            delete[] id;
            delete[] idUsuario;
            delete[] nombre;
            delete[] tipo;
            encontrado = true;
        }
    });
    if (!encontrado) std::cout << "\nNo se encontraron cuentas en el rango de fechas.\n";
}


void Banco::buscarPersonalizado(const char* consulta) {
    // Validar que la consulta no sea nula ni vacía
    if (!consulta || strlen(consulta) == 0) {
        std::cout << "Consulta invalida. Por favor, ingrese una consulta valida.\n";
        return;
    }

    bool encontrado = false;
    cuentas.recorrer([consulta, &encontrado](const Cuenta* cta) {
        char* idUsuario = cta->obtenerIdUsuario();
        char* nombre = cta->obtenerNombrePropietario();
        // Verificar que idUsuario y nombre no sean nulos
        if (idUsuario && nombre && (strstr(idUsuario, consulta) || strstr(nombre, consulta))) {
            char* idCuenta = cta->obtenerIdCuenta();
            char* tipo = cta->obtenerTipo();
            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << std::endl;
            delete[] idCuenta;
            delete[] tipo;
            encontrado = true;
        }
        delete[] idUsuario;
        delete[] nombre;
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
            char* tipo = cta->obtenerTipo();
            std::cout << "ID Cuenta: " << id << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << std::endl;
            delete[] id;
            delete[] idUsuario;
            delete[] nombre;
            delete[] tipo;
            encontrado = true;
        }
    });
    if (!encontrado) std::cout << "No se encontraron cuentas en el rango de saldo.\n";
}

void Banco::guardarEnArchivo() {
    std::ofstream archivo(archivoCuentas, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error al abrir archivo " << archivoCuentas << std::endl;
        return;
    }
    cuentas.recorrer([&archivo](const Cuenta* cta) {
        char* id = cta->obtenerIdCuenta();
        char* idUsuario = cta->obtenerIdUsuario();
        char* nombre = cta->obtenerNombrePropietario();
        char* tipo = cta->obtenerTipo();
        double saldo = cta->obtenerSaldo();
        FechaHora fecha = cta->obtenerFechaCreacion();

        char* idCifrado = Cifrado::cifrarMD5(id);
        char* idUsuarioCifrado = Cifrado::cifrarMD5(idUsuario);
        char* nombreCifrado = Cifrado::cifrarMD5(nombre);
        char* tipoCifrado = Cifrado::cifrarMD5(tipo);

        size_t lenId = strlen(idCifrado) + 1;
        size_t lenIdUsuario = strlen(idUsuarioCifrado) + 1;
        size_t lenNombre = strlen(nombreCifrado) + 1;
        size_t lenTipo = strlen(tipoCifrado) + 1;
        archivo.write(reinterpret_cast<char*>(&lenId), sizeof(size_t));
        archivo.write(idCifrado, lenId);
        archivo.write(reinterpret_cast<char*>(&lenIdUsuario), sizeof(size_t));
        archivo.write(idUsuarioCifrado, lenIdUsuario);
        archivo.write(reinterpret_cast<char*>(&lenNombre), sizeof(size_t));
        archivo.write(nombreCifrado, lenNombre);
        archivo.write(reinterpret_cast<char*>(&lenTipo), sizeof(size_t));
        archivo.write(tipoCifrado, lenTipo);
        archivo.write(reinterpret_cast<char*>(&saldo), sizeof(double));
        int dia = fecha.obtenerDia(), mes = fecha.obtenerMes(), anio = fecha.obtenerAnio();
        int hora = fecha.obtenerHora(), minuto = fecha.obtenerMinuto(), segundo = fecha.obtenerSegundo();
        archivo.write(reinterpret_cast<char*>(&dia), sizeof(int));
        archivo.write(reinterpret_cast<char*>(&mes), sizeof(int));
        archivo.write(reinterpret_cast<char*>(&anio), sizeof(int));
        archivo.write(reinterpret_cast<char*>(&hora), sizeof(int));
        archivo.write(reinterpret_cast<char*>(&minuto), sizeof(int));
        archivo.write(reinterpret_cast<char*>(&segundo), sizeof(int));

        delete[] id;
        delete[] idUsuario;
        delete[] nombre;
        delete[] tipo;
        delete[] idCifrado;
        delete[] idUsuarioCifrado;
        delete[] nombreCifrado;
        delete[] tipoCifrado;
    });
    archivo.close();
    std::cout << "Cuentas guardadas en " << archivoCuentas << ".\n";
}

void Banco::cargarDesdeArchivo() {
    std::ifstream archivo(archivoCuentas, std::ios::binary);
    if (!archivo) {
        std::cout << "No se encontro archivo " << archivoCuentas << ". Iniciando con lista vacia.\n";
        return;
    }
    std::cout << "Abriendo " << archivoCuentas << " para cargar cuentas.\n";
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
    cuentas = ListaDobleCircular<Cuenta*>();
    int cuentasLeidas = 0;

    while (archivo.peek() != EOF) {
        size_t lenId, lenIdUsuario, lenNombre, lenTipo;
        if (!archivo.read(reinterpret_cast<char*>(&lenId), sizeof(size_t))) break;
        char* idCifrado = new char[lenId];
        archivo.read(idCifrado, lenId);
        archivo.read(reinterpret_cast<char*>(&lenIdUsuario), sizeof(size_t));
        char* idUsuarioCifrado = new char[lenIdUsuario];
        archivo.read(idUsuarioCifrado, lenIdUsuario);
        archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(size_t));
        char* nombreCifrado = new char[lenNombre];
        archivo.read(nombreCifrado, lenNombre);
        archivo.read(reinterpret_cast<char*>(&lenTipo), sizeof(size_t));
        char* tipoCifrado = new char[lenTipo];
        archivo.read(tipoCifrado, lenTipo);
        double saldo;
        archivo.read(reinterpret_cast<char*>(&saldo), sizeof(double));
        int dia, mes, anio, hora, minuto, segundo;
        archivo.read(reinterpret_cast<char*>(&dia), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&mes), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&anio), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&hora), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&minuto), sizeof(int));
        archivo.read(reinterpret_cast<char*>(&segundo), sizeof(int));

        char* id = Cifrado::descifrarMD5(idCifrado);
        char* idUsuario = Cifrado::descifrarMD5(idUsuarioCifrado);
        char* nombre = Cifrado::descifrarMD5(nombreCifrado);
        char* tipo = Cifrado::descifrarMD5(tipoCifrado);
        FechaHora fecha(dia, mes, anio, hora, minuto, segundo);

        if (strcmp(tipo, "Ahorro") == 0) {
            cuentas.insertar(new CuentaAhorro(id, idUsuario, nombre, saldo));
            conteoAhorros++;
        } else if (strcmp(tipo, "Corriente") == 0) {
            cuentas.insertar(new CuentaCorriente(id, idUsuario, nombre, saldo));
            conteoCorrientes++;
        }
        cuentasLeidas++;
        std::cout << "Cuenta cargada: ID Cuenta=" << id << ", ID Usuario=" << idUsuario << ", Nombre=" << nombre << ", Tipo=" << tipo << ", Saldo=$" << saldo << "\n";

        delete[] idCifrado;
        delete[] idUsuarioCifrado;
        delete[] nombreCifrado;
        delete[] tipoCifrado;
        delete[] id;
        delete[] idUsuario;
        delete[] nombre;
        delete[] tipo;
    }
    archivo.close();
    std::cout << "Total de cuentas cargadas: " << cuentasLeidas << " desde " << archivoCuentas << ".\n";
}

void Banco::crearCopiaSeguridad(const char* nombreArchivo) {
    std::ifstream origen(archivoCuentas, std::ios::binary);
    if (!origen) {
        std::cerr << "Error al abrir archivo " << archivoCuentas << std::endl;
        return;
    }
    std::ofstream destino(nombreArchivo, std::ios::binary);
    if (!destino) {
        std::cerr << "Error al abrir archivo " << nombreArchivo << std::endl;
        origen.close();
        return;
    }
    destino << origen.rdbuf();
    origen.close();
    destino.close();
    std::cout << "Copia de seguridad creada en " << nombreArchivo << ".\n";
}

void Banco::restaurar(const char* nombreArchivo) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) {
        std::cerr << "Error al abrir archivo " << nombreArchivo << std::endl;
        return;
    }
    std::ofstream temp(archivoCuentas, std::ios::binary);
    if (!temp) {
        std::cerr << "Error al abrir archivo " << archivoCuentas << std::endl;
        archivo.close();
        return;
    }
    temp << archivo.rdbuf();
    archivo.close();
    temp.close();
    cargarDesdeArchivo();
    std::cout << "Cuentas restauradas desde " << nombreArchivo << ".\n";
}

std::string Banco::generarNombreCopiaSeguridad() {
    FechaHora ahora;
    ahora.actualizarHoraActual();
    std::stringstream ss;
    ss << "copia_seguridad_"
       << ahora.obtenerAnio() << "-"
       << (ahora.obtenerMes() < 10 ? "0" : "") << ahora.obtenerMes() << "-"
       << (ahora.obtenerDia() < 10 ? "0" : "") << ahora.obtenerDia() << "_"
       << (ahora.obtenerHora() < 10 ? "0" : "") << ahora.obtenerHora() << "-"
       << (ahora.obtenerMinuto() < 10 ? "0" : "") << ahora.obtenerMinuto() << "-"
       << (ahora.obtenerSegundo() < 10 ? "0" : "") << ahora.obtenerSegundo()
       << ".bin";
    return ss.str();
}

void Banco::hacerCopiaSeguridad(const char* nombreArchivo) {
    guardarEnArchivo();
    crearCopiaSeguridad(nombreArchivo);
}

void Banco::hacerCopiaSeguridadAutomatica() {
    std::string nombreCopia = generarNombreCopiaSeguridad();
    crearCopiaSeguridad(nombreCopia.c_str());
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