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

void Banco::agregarCuentaAhorro(const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento) {
    try {
        if (conteoAhorros >= 50) throw "Maximo de cuentas de ahorro alcanzado";
        std::string idUnico = generarIdUnico("AH", conteoAhorros + 1);
        // Crear cuenta con los datos proporcionados
        cuentas.insertar(new CuentaAhorro(idUnico.c_str(), idUsuario, nombre, apellido, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento));
        conteoAhorros++;
        guardarEnArchivo();
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
        guardarEnArchivo();
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
            guardarEnArchivo();
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
                guardarEnArchivo(); 
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
            guardarEnArchivo();
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
                guardarEnArchivo();
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
        char* apellido = cta->obtenerApellidoPropietario();
        char* tipo = cta->obtenerTipo();
        double saldo = cta->obtenerSaldo();
        int edad = cta->obtenerEdad();
        FechaHora fecha = cta->obtenerFechaCreacion();

        char* idCifrado = Cifrado::cifrarMD5(id);
        char* idUsuarioCifrado = Cifrado::cifrarMD5(idUsuario);
        char* nombreCifrado = Cifrado::cifrarMD5(nombre);
        char* apellidoCifrado = Cifrado::cifrarMD5(apellido);
        char* tipoCifrado = Cifrado::cifrarMD5(tipo);

        size_t lenId = strlen(idCifrado) + 1;
        size_t lenIdUsuario = strlen(idUsuarioCifrado) + 1;
        size_t lenNombre = strlen(nombreCifrado) + 1;
        size_t lenApellido = strlen(apellidoCifrado) + 1;
        size_t lenTipo = strlen(tipoCifrado) + 1;
        archivo.write(reinterpret_cast<char*>(&lenId), sizeof(size_t));
        archivo.write(idCifrado, lenId);
        archivo.write(reinterpret_cast<char*>(&lenIdUsuario), sizeof(size_t));
        archivo.write(idUsuarioCifrado, lenIdUsuario);
        archivo.write(reinterpret_cast<char*>(&lenNombre), sizeof(size_t));
        archivo.write(nombreCifrado, lenNombre);
        archivo.write(reinterpret_cast<char*>(&lenApellido), sizeof(size_t));
        archivo.write(apellidoCifrado, lenApellido);
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
        delete[] apellido;
        delete[] tipo;
        delete[] idCifrado;
        delete[] idUsuarioCifrado;
        delete[] nombreCifrado;
        delete[] apellidoCifrado;
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
            size_t lenId, lenIdUsuario, lenNombre, lenApellido, lenTipo;
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

            if (!archivo.read(reinterpret_cast<char*>(&lenApellido), sizeof(size_t))) {
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                break;
            }
            if (lenApellido <= 0 || lenApellido > 1024) {
                std::cerr << "Longitud de apellido invalida: " << lenApellido << ". Archivo corrupto.\n";
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                break;
            }
            char* apellidoCifrado = new char[lenApellido];
            archivo.read(apellidoCifrado, lenApellido);

            if (!archivo.read(reinterpret_cast<char*>(&lenTipo), sizeof(size_t))) {
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                delete[] apellidoCifrado;
                break;
            }
            if (lenTipo <= 0 || lenTipo > 1024) {
                std::cerr << "Longitud de tipo invalida: " << lenTipo << ". Archivo corrupto.\n";
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                delete[] apellidoCifrado;
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
                delete[] apellidoCifrado;
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
                delete[] apellidoCifrado;
                delete[] tipoCifrado;
                break;
            }

            char* id = Cifrado::descifrarMD5(idCifrado);
            char* idUsuario = Cifrado::descifrarMD5(idUsuarioCifrado);
            char* nombre = Cifrado::descifrarMD5(nombreCifrado);
            char* apellido = Cifrado::descifrarMD5(apellidoCifrado);
            char* tipo = Cifrado::descifrarMD5(tipoCifrado);
            FechaHora fecha(dia, mes, anio, hora, minuto, segundo);

            if (!id || !idUsuario || !nombre || !apellido || !tipo || strlen(id) == 0 || strlen(idUsuario) == 0 ||
                strlen(nombre) == 0 || strlen(apellido) == 0 || strlen(tipo) == 0) {
                std::cerr << "Datos descifrados invalidos para una cuenta. Archivo corrupto.\n";
                delete[] idCifrado;
                delete[] idUsuarioCifrado;
                delete[] nombreCifrado;
                delete[] apellidoCifrado;
                delete[] tipoCifrado;
                delete[] id;
                delete[] idUsuario;
                delete[] nombre;
                delete[] apellido;
                delete[] tipo;
                break;
            }

            if (strcmp(tipo, "Ahorro") == 0) {
                cuentas.insertar(new CuentaAhorro(id, idUsuario, nombre, apellido, saldo, 1, 1, 2000, fecha));
                conteoAhorros++;
            } else if (strcmp(tipo, "Corriente") == 0) {
                cuentas.insertar(new CuentaCorriente(id, idUsuario, nombre, apellido, saldo, 1, 1, 2000, fecha));
                conteoCorrientes++;
            }

            cuentasLeidas++;
            std::cout << "Cuenta cargada: ID Cuenta=" << id << ", ID Usuario=" << idUsuario << ", Nombre=" << nombre << " " << apellido
                      << ", Tipo=" << tipo << ", Saldo=$" << saldo << ", Fecha Creacion=" << fecha.aCadena() << "\n";
            delete[] idCifrado;
            delete[] idUsuarioCifrado;
            delete[] nombreCifrado;
            delete[] apellidoCifrado;
            delete[] tipoCifrado;
            delete[] id;
            delete[] idUsuario;
            delete[] nombre;
            delete[] apellido;
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
    std::string nombreCopia = generarNombreCopiaSeguridad(); // e.g., copia_seguridad_20250622_210909.bin
    hacerCopiaSeguridad(nombreCopia.c_str());
    
    // Generar nombre para la copia de texto, reemplazando .bin por .txt
    std::string nombreCopiaTexto = nombreCopia.substr(0, nombreCopia.find(".bin")) + ".txt";
    hacerCopiaSeguridadTexto(nombreCopiaTexto.c_str());
}

std::string Banco::generarNombreCopiaSeguridad() {
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
    return ss.str();
}

// void Banco::hacerCopiaSeguridadAutomatica() {
  //  FechaHora ahora;
  //  ahora.actualizarHoraActual();
  //  std::stringstream ss;
  //  ss << "copia_seguridad_"
  //     << ahora.obtenerAnio()
  //     << (ahora.obtenerMes() < 10 ? "0" : "") << ahora.obtenerMes()
  //     << (ahora.obtenerDia() < 10 ? "0" : "") << ahora.obtenerDia()
  //     << "_" << (ahora.obtenerHora() < 10 ? "0" : "") << ahora.obtenerHora()
  //     << (ahora.obtenerMinuto() < 10 ? "0" : "") << ahora.obtenerMinuto()
  //     << (ahora.obtenerSegundo() < 10 ? "0" : "") << ahora.obtenerSegundo()
  //     << ".bin";
  //  hacerCopiaSeguridad(ss.str().c_str());
// }

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
    if (hashAlmacenado != 0 && !CifradoCRC32::verificarCRC32Archivo(archivoCuentasTexto, hashAlmacenado)) {
        std::cerr << "Error: Integridad de " << archivoCuentasTexto << " comprometida. No se cargaran cuentas desde texto.\n";
        return;
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
                << tipo << "," << saldo << "," << edad << fechaStr << "\n";
        delete[] id;
        delete[] idUsuario;
        delete[] nombre;
        delete[] apellido;
        delete[] tipo;
        delete[] fechaStr;
    });
    archivo.close();
    // Generar nombre del archivo de hash basado en el nombre del archivo de texto
    std::string nombreHash = std::string(prefijoHashCopiaSeguridad) + (nombreArchivo + strlen(prefijoCopiaSeguridadTexto));
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(nombreArchivo);
    CifradoCRC32::guardarHash(nombreHash, hash);
    std::cout << "Copia de seguridad de texto creada en " << nombreArchivo << ".\n";
}

void Banco::restaurarTexto(const char* nombreArchivo) {
    std::string nombreHash = std::string(prefijoHashCopiaSeguridad) + (nombreArchivo + strlen(prefijoCopiaSeguridadTexto));
    uint32_t hashAlmacenado = CifradoCRC32::leerHash(nombreHash);
    if (hashAlmacenado != 0 && !CifradoCRC32::verificarCRC32Archivo(nombreArchivo, hashAlmacenado)) {
        std::cerr << "Error: Integridad de " << nombreArchivo << " comprometida. No se restauraran cuentas.\n";
        return;
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
    if (hashAlmacenado != 0 && !CifradoCRC32::verificarCRC32Archivo(nombreArchivo, hashAlmacenado)) {
        std::cerr << "Error: Integridad de " << nombreArchivo << " comprometida.\n";
    }
}