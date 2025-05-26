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

void Banco::agregarCuentaAhorro(const char* idUsuario, const char* nombre, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento) {
    try {
        if (conteoAhorros >= 50) throw "Maximo de cuentas de ahorro alcanzado";
        std::string idUnico = generarIdUnico("AH", conteoAhorros + 1);
        cuentas.insertar(new CuentaAhorro(idUnico.c_str(), idUsuario, nombre, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento));
        conteoAhorros++;
        guardarEnArchivo();
        hacerCopiaSeguridadAutomatica();
        std::cout << "Cuenta de ahorro agregada exitosamente. ID Cuenta: " << idUnico << "\n";
    } catch (const char* e) {
        std::cerr << "Error: " << e << std::endl;
    }
}

void Banco::agregarCuentaCorriente(const char* idUsuario, const char* nombre, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento) {
    try {
        if (conteoCorrientes >= 1 && saldoInicial < 500) throw "Se requiere minimo $500 para cuenta corriente adicional";
        std::string idUnico = generarIdUnico("CO", conteoCorrientes + 1);
        cuentas.insertar(new CuentaCorriente(idUnico.c_str(), idUsuario, nombre, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento));
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
                std::cout << "Cuenta " << idCuenta << " (" << tipo << "), Saldo: $" << cta->obtenerSaldo() << "\n";
                delete[] idCuenta;
                delete[] tipo;
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
            std::cout << "\nID Cuenta: " << id << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << ", Edad: " << cta->obtenerEdad() << std::endl;
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
    if (!consulta || strlen(consulta) == 0) {
        std::cout << "Consulta invalida. Por favor, ingrese una consulta valida.\n";
        return;
    }

    bool encontrado = false;
    cuentas.recorrer([consulta, &encontrado](const Cuenta* cta) {
        char* idUsuario = cta->obtenerIdUsuario();
        char* nombre = cta->obtenerNombrePropietario();
        int edad = cta->obtenerEdad();
        char edadStr[4];
        snprintf(edadStr, sizeof(edadStr), "%d", edad);
        if (idUsuario && nombre && (strstr(idUsuario, consulta) || strstr(nombre, consulta) || strstr(edadStr, consulta))) {
            char* idCuenta = cta->obtenerIdCuenta();
            char* tipo = cta->obtenerTipo();
            std::cout << "ID Cuenta: " << idCuenta << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << ", Edad: " << edad << std::endl;
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
            std::cout << "ID Cuenta: " << id << ", ID Usuario: " << idUsuario << ", Nombre: " << nombre << ", Tipo: " << tipo << ", Saldo: $" << cta->obtenerSaldo() << ", Edad: " << cta->obtenerEdad() << std::endl;
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
        int edad = cta->obtenerEdad();
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
        archivo.write(reinterpret_cast<char*>(&edad), sizeof(int));
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

    try {
        while (archivo.peek() != EOF) {
            size_t lenId, lenIdUsuario, lenNombre, lenTipo;
            if (!archivo.read(reinterpret_cast<char*>(&lenId), sizeof(size_t))) break;
            if (lenId <= 0 || lenId > 1024) {
                std::cerr << "Longitud de ID invalida: " << lenId << ". Archivo corrupto.\n";
                break;
            }
            char* idCifrado = new char[lenId];
            archivo.read(idCifrado, lenId);

            if (!archivo.read(reinterpret_cast<char*>(&lenIdUsuario), sizeof(size_t))) {
                delete[] idCifrado;
                break;
            }
            if (lenIdUsuario <= 0 || lenIdUsuario > 1024) {
                std::cerr << "Longitud de ID de usuario invalida: " << lenIdUsuario << ". Archivo corrupto.\n";
                delete[] idCifrado;
                break;
            }
            char* idUsuarioCifrado = new char[lenIdUsuario];
            archivo.read(idUsuarioCifrado, lenIdUsuario);

            if (!archivo.read(reinterpret_cast<char*>(&lenNombre), sizeof(size_t))) {
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                break;
            }
            if (lenNombre <= 0 || lenNombre > 1024) {
                std::cerr << "Longitud de nombre invalida: " << lenNombre << ". Archivo corrupto.\n";
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                break;
            }
            char* nombreCifrado = new char[lenNombre];
            archivo.read(nombreCifrado, lenNombre);

            if (!archivo.read(reinterpret_cast<char*>(&lenTipo), sizeof(size_t))) {
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                break;
            }
            if (lenTipo <= 0 || lenTipo > 1024) {
                std::cerr << "Longitud de tipo invalida: " << lenTipo << ". Archivo corrupto.\n";
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                break;
            }
            char* tipoCifrado = new char[lenTipo];
            archivo.read(tipoCifrado, lenTipo);

            double saldo;
            int edad;
            if (!archivo.read(reinterpret_cast<char*>(&saldo), sizeof(double)) ||
                !archivo.read(reinterpret_cast<char*>(&edad), sizeof(int))) {
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                delete[] tipoCifrado;
                break;
            }

            int dia, mes, anio, hora, minuto, segundo;
            if (!archivo.read(reinterpret_cast<char*>(&dia), sizeof(int)) ||
                !archivo.read(reinterpret_cast<char*>(&mes), sizeof(int)) ||
                !archivo.read(reinterpret_cast<char*>(&anio), sizeof(int)) ||
                !archivo.read(reinterpret_cast<char*>(&hora), sizeof(int)) ||
                !archivo.read(reinterpret_cast<char*>(&minuto), sizeof(int)) ||
                !archivo.read(reinterpret_cast<char*>(&segundo), sizeof(int))) {
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                delete[] tipoCifrado;
                break;
            }

            char* id = Cifrado::descifrarMD5(idCifrado);
            char* idUsuario = Cifrado::descifrarMD5(idUsuarioCifrado);
            char* nombre = Cifrado::descifrarMD5(nombreCifrado);
            char* tipo = Cifrado::descifrarMD5(tipoCifrado);
            FechaHora fecha(dia, mes, anio, hora, minuto, segundo);

            if (!id || !idUsuario || !nombre || !tipo || strlen(id) == 0 || strlen(idUsuario) == 0 || strlen(nombre) == 0 || strlen(tipo) == 0) {
                std::cerr << "Datos descifrados invalidos para una cuenta. Archivo corrupto.\n";
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                delete[] tipoCifrado;
                delete[] id;
                delete[] idUsuario;
                delete[] nombre;
                delete[] tipo;
                break;
            }

            if (strcmp(tipo, "Ahorro") == 0) {
                cuentas.insertar(new CuentaAhorro(id, idUsuario, nombre, saldo, 1, 1, 2000)); // Fecha dummy, edad ya cargada
                conteoAhorros++;
            } else if (strcmp(tipo, "Corriente") == 0) {
                cuentas.insertar(new CuentaCorriente(id, idUsuario, nombre, saldo, 1, 1, 2000)); // Fecha dummy, edad ya cargada
                conteoCorrientes++;
            }

            cuentas.recorrer([&id, edad](Cuenta* cta) {
                char* cuentaId = cta->obtenerIdCuenta();
                if (strcmp(cuentaId, id) == 0) {
                    // Nota: La edad ya está cargada desde el archivo y no se modifica aquí
                }
                delete[] cuentaId;
            });

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
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error de asignacion de memoria: " << e.what() << ". Archivo corrupto o memoria insuficiente.\n";
        cuentas.recorrer([](Cuenta* cta) { delete cta; });
        cuentas = ListaDobleCircular<Cuenta*>();
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado al cargar archivo: " << e.what() << ". Archivo corrupto.\n";
        cuentas.recorrer([](Cuenta* cta) { delete cta; });
        cuentas = ListaDobleCircular<Cuenta*>();
    }

    archivo.close();
    if (cuentasLeidas > 0) {
        std::cout << "Carga completada. " << cuentasLeidas << " cuentas leidas.\n";
    } else {
        std::cout << "No se pudieron cargar cuentas. Archivo vacio o corrupto.\n";
    }
}

void Banco::hacerCopiaSeguridad(const char* nombreArchivo) {
    std::ifstream archivoOriginal(archivoCuentas, std::ios::binary);
    if (!archivoOriginal) {
        std::cerr << "Error al abrir " << archivoCuentas << " para copia.\n";
        return;
    }
    std::ofstream archivoCopia(nombreArchivo, std::ios::binary);
    if (!archivoCopia) {
        std::cerr << "Error al crear archivo de copia " << nombreArchivo << ".\n";
        archivoOriginal.close();
        return;
    }
    archivoCopia << archivoOriginal.rdbuf();
    archivoOriginal.close();
    archivoCopia.close();
    std::cout << "Copia de seguridad creada en " << nombreArchivo << ".\n";
}

void Banco::hacerCopiaSeguridadAutomatica() {
    FechaHora ahora;
    ahora.actualizarHoraActual();
    std::stringstream ss;
    ss << "copia_seguridad_"
       << ahora.obtenerAnio()
       << (ahora.obtenerMes() < 10 ? "0" : "") << ahora.obtenerMes()
       << (ahora.obtenerDia() < 10 ? "0" : "") << ahora.obtenerDia()
       << "_" << (ahora.obtenerHora() < 10 ? "0" : "") << ahora.obtenerHora()
       << (ahora.obtenerMinuto() < 10 ? "0" : "") << ahora.obtenerMinuto()
       << (ahora.obtenerSegundo() < 10 ? "0" : "") << ahora.obtenerSegundo()
       << ".bin";
    hacerCopiaSeguridad(ss.str().c_str());
}

void Banco::restaurar(const char* nombreArchivo) {
    std::ifstream archivoCopia(nombreArchivo, std::ios::binary);
    if (!archivoCopia) {
        std::cerr << "Error al abrir archivo de copia " << nombreArchivo << ".\n";
        return;
    }
    std::ofstream archivoOriginal(archivoCuentas, std::ios::binary);
    if (!archivoOriginal) {
        std::cerr << "Error al abrir " << archivoCuentas << " para restaurar.\n";
        archivoCopia.close();
        return;
    }
    archivoOriginal << archivoCopia.rdbuf();
    archivoCopia.close();
    archivoOriginal.close();
    cargarDesdeArchivo();
    std::cout << "Restauracion completada desde " << nombreArchivo << ".\n";
}

void Banco::cifrarArchivo(const char* archivo) {
    std::ifstream archivoOriginal(archivo, std::ios::binary);
    if (!archivoOriginal) {
        std::cerr << "Error al abrir " << archivo << " para cifrar.\n";
        return;
    }
    std::ofstream archivoCifrado((std::string(archivo) + ".cifrado").c_str(), std::ios::binary);
    if (!archivoCifrado) {
        std::cerr << "Error al crear archivo cifrado.\n";
        archivoOriginal.close();
        return;
    }
    char buffer[1024];
    while (archivoOriginal.read(buffer, sizeof(buffer))) {
        char* bufferCifrado = Cifrado::cifrarMD5(buffer);
        archivoCifrado.write(bufferCifrado, strlen(bufferCifrado) + 1);
        delete[] bufferCifrado;
    }
    archivoOriginal.close();
    archivoCifrado.close();
    std::cout << "Archivo " << archivo << " cifrado exitosamente.\n";
}

void Banco::descifrarArchivo(const char* archivo) {
    std::ifstream archivoCifrado(archivo, std::ios::binary);
    if (!archivoCifrado) {
        std::cerr << "Error al abrir " << archivo << " para descifrar.\n";
        return;
    }
    std::ofstream archivoDescifrado((std::string(archivo).substr(0, std::string(archivo).length() - 8)).c_str(), std::ios::binary);
    if (!archivoDescifrado) {
        std::cerr << "Error al crear archivo descifrado.\n";
        archivoCifrado.close();
        return;
    }
    char buffer[1024];
    while (archivoCifrado.read(buffer, sizeof(buffer))) {
        char* bufferDescifrado = Cifrado::descifrarMD5(buffer);
        archivoDescifrado.write(bufferDescifrado, strlen(bufferDescifrado) + 1);
        delete[] bufferDescifrado;
    }
    archivoCifrado.close();
    archivoDescifrado.close();
    std::cout << "Archivo " << archivo << " descifrado exitosamente.\n";
}