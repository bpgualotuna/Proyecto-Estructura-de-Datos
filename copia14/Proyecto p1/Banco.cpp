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
#include <limits>
#include <sys/stat.h> // Para obtener metadatos del archivo

Banco::Banco(const char* codBanco, const char* codSucursal) : conteoAhorros(0), conteoCorrientes(0) {
    if (!codBanco || strlen(codBanco) != 2) throw "Código de banco inválido";
    if (!codSucursal || strlen(codSucursal) != 3) throw "Código de sucursal inválido";

    codigoBanco = new char[3];
    strcpy(codigoBanco, codBanco);
    codigoSucursal = new char[4];
    strcpy(codigoSucursal, codSucursal);
    
    // Inicializar sucursal actual con el código de sucursal por defecto
    sucursalActual = new char[4];
    strcpy(sucursalActual, codSucursal);

    // Generar nombres de archivos con código de banco y sucursal
    archivoCuentas = "cuentas_" + std::string(codBanco) + "_" + std::string(codSucursal) + ".txt";
    hashCuentasTexto = "hash_cuentas_" + std::string(codBanco) + "_" + std::string(codSucursal) + ".txt";
    prefijoHashCopiaSeguridad = "hash_copia_seguridad_" + std::string(codBanco) + "_" + std::string(codSucursal) + "_";

    struct stat fileInfo;
    if (stat(archivoCuentas.c_str(), &fileInfo) == 0) {
        ultimaModificacion = fileInfo.st_mtime;
    }

    cargarDesdeArchivo();
}

Banco::~Banco() {
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
    delete[] codigoBanco;
    delete[] codigoSucursal;
    delete[] sucursalActual;
}


int Banco::calcularDigitoVerificador(const std::string& numeroCuenta) {
    int suma = 0;
    int peso = 2;
    
    // Procesar de derecha a izquierda
    for (int i = numeroCuenta.length() - 1; i >= 0; i--) {
        if (numeroCuenta[i] >= '0' && numeroCuenta[i] <= '9') {
            int digito = numeroCuenta[i] - '0';
            int producto = digito * peso;
            
            // Si el producto es mayor a 9, restar 9
            if (producto > 9) {
                producto -= 9;
            }
            
            suma += producto;
            
            // Alternar peso entre 2 y 1
            peso = (peso == 2) ? 1 : 2;
        }
    }
    
    // Calcular dígito verificador
    int digitoVerificador = (10 - (suma % 10)) % 10;
    return digitoVerificador;
}

std::string Banco::generarIdUnico(const char* tipo, int conteo) {
    // Validar parámetros de entrada
    if (!tipo || (strcmp(tipo, "AH") != 0 && strcmp(tipo, "CO") != 0)) {
        throw "Tipo de cuenta inválido. Debe ser 'AH' (Ahorro) o 'CO' (Corriente)";
    }
    
    if (conteo < 1 || conteo > 999999) {
        throw "Número secuencial fuera de rango (1-999999)";
    }
    
    // Validar códigos de banco y sucursal
    if (!codigoBanco || !sucursalActual) {
        throw "Códigos de banco o sucursal no inicializados";
    }
    
    // Construir el número de cuenta
    std::stringstream ss;
    
    // Prefijo Banco / Sucursal: BB-SSS (usar sucursal actual)
    ss << codigoBanco << "-" << sucursalActual;
    
    // Tipo de cuenta / Producto: T (1 para ahorros, 2 para corriente)
    char tipoDigito = (strcmp(tipo, "AH") == 0) ? '1' : '2';
    ss << "-" << tipoDigito;
    
    // Número secuencial: NNNNNN (6 dígitos con ceros a la izquierda)
    ss << "-";
    if (conteo < 10) ss << "00000";
    else if (conteo < 100) ss << "0000";
    else if (conteo < 1000) ss << "000";
    else if (conteo < 10000) ss << "00";
    else if (conteo < 100000) ss << "0";
    ss << conteo;
    
    // Obtener la cadena sin el dígito verificador
    std::string numeroParcial = ss.str();
    
    // Calcular el dígito verificador
    std::string numeroParaDigito = "";
    for (char c : numeroParcial) {
        if (c >= '0' && c <= '9') {
            numeroParaDigito += c;
        }
    }
    
    int digitoVerificador = calcularDigitoVerificador(numeroParaDigito);
    
    // Agregar el dígito verificador
    ss << "-" << digitoVerificador;
    
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
    std::ofstream archivo(archivoCuentas);
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

        archivo << id << "," << idUsuario << "," << nombre << "," << apellido << ","
                << tipo << "," << saldo << "," << edad << ","
                << fecha.obtenerDia() << "-" << fecha.obtenerMes() << "-" << fecha.obtenerAnio() << "_"
                << fecha.obtenerHora() << "-" << fecha.obtenerMinuto() << "-" << fecha.obtenerSegundo() << "\n";

        delete[] id;
        delete[] idUsuario;
        delete[] nombre;
        delete[] apellido;
        delete[] tipo;
    });
    archivo.close();

    struct stat fileInfo;
    if (stat(archivoCuentas.c_str(), &fileInfo) == 0) {
        ultimaModificacion = fileInfo.st_mtime;
    }
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(archivoCuentas);
    CifradoCRC32::guardarHash(hashCuentasTexto, hash);
    std::cout << "Cuentas guardadas en " << archivoCuentas << ".\n";
}

void Banco::cargarDesdeArchivo() {
    // Verificar integridad antes de cargar
    verificarIntegridadArchivo();

    std::ifstream archivo(archivoCuentas);
    if (!archivo) {
        std::cout << "No se encontro archivo " << archivoCuentas << ". Iniciando con lista vacia.\n";
        return;
    }
    std::cout << "Abriendo " << archivoCuentas << " para cargar cuentas.\n";
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
    cuentas = ListaDobleCircular<Cuenta*>();
    int cuentasLeidas = 0;

    try {
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::stringstream ss(linea);
            std::string id, idUsuario, nombre, apellido, tipo, fechaStr;
            double saldo;
            int edad;

            std::getline(ss, id, ',');
            std::getline(ss, idUsuario, ',');
            std::getline(ss, nombre, ',');
            std::getline(ss, apellido, ',');
            std::getline(ss, tipo, ',');
            ss >> saldo;
            ss.ignore(1); // Ignorar coma
            ss >> edad;
            ss.ignore(1); // Ignorar coma
            std::getline(ss, fechaStr);

            if (id.empty() || idUsuario.empty() || nombre.empty() || apellido.empty() || tipo.empty()) {
                std::cerr << "Datos inválidos en línea: " << linea << ". Archivo corrupto.\n";
                continue;
            }

            int dia, mes, anio, hora, minuto, segundo;
            if (sscanf(fechaStr.c_str(), "%d-%d-%d_%d-%d-%d", &dia, &mes, &anio, &hora, &minuto, &segundo) != 6) {
                std::cerr << "Formato de fecha inválido en línea: " << linea << ". Archivo corrupto.\n";
                continue;
            }

            FechaHora fecha(dia, mes, anio, hora, minuto, segundo);

            if (tipo == "Ahorro") {
                cuentas.insertar(new CuentaAhorro(id.c_str(), idUsuario.c_str(), nombre.c_str(), apellido.c_str(), saldo, 1, 1, 2000, fecha));
                conteoAhorros++;
            } else if (tipo == "Corriente") {
                cuentas.insertar(new CuentaCorriente(id.c_str(), idUsuario.c_str(), nombre.c_str(), apellido.c_str(), saldo, 1, 1, 2000, fecha));
                conteoCorrientes++;
            } else {
                std::cerr << "Tipo de cuenta inválido: " << tipo << ". Omitiendo cuenta.\n";
                continue;
            }

            cuentasLeidas++;
            std::cout << "Cuenta cargada: ID Cuenta=" << id << ", ID Usuario=" << idUsuario << ", Nombre=" << nombre << " " << apellido
                      << ", Tipo=" << tipo << ", Saldo=$" << saldo << ", Fecha Creacion=" << fecha.aCadena() << "\n";
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error de asignación de memoria: " << e.what() << ". Archivo corrupto o memoria insuficiente.\n";
        cuentas.recorrer([](Cuenta* cta) { delete cta; });
        cuentas = ListaDobleCircular<Cuenta*>();
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado al cargar archivo: " << e.what() << ". Archivo corrupto.\n";
        cuentas.recorrer([](Cuenta* cta) { delete cta; });
        cuentas = ListaDobleCircular<Cuenta*>();
    }

    archivo.close();
    if (cuentasLeidas > 0) {
        std::cout << "Carga completada. " << cuentasLeidas << " cuentas leídas.\n";
    } else {
        std::cout << "No se pudieron cargar cuentas. Archivo vacío o corrupto.\n";
    }
}

void Banco::hacerCopiaSeguridad(const char* nombreArchivo) {
    std::ifstream archivoOriginal(archivoCuentas);
    if (!archivoOriginal) {
        std::cerr << "Error al abrir " << archivoCuentas << " para copia.\n";
        return;
    }
    std::ofstream archivoCopia(nombreArchivo);
    if (!archivoCopia) {
        std::cerr << "Error al crear archivo de copia " << nombreArchivo << ".\n";
        archivoOriginal.close();
        return;
    }
    archivoCopia << archivoOriginal.rdbuf();
    archivoOriginal.close();
    archivoCopia.close();
    std::string nombreHash = prefijoHashCopiaSeguridad + std::string(nombreArchivo).substr(prefijoHashCopiaSeguridad.length());
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(nombreArchivo);
    CifradoCRC32::guardarHash(nombreHash, hash);
    std::cout << "Copia de seguridad creada en " << nombreArchivo << ".\n";
}

void Banco::hacerCopiaSeguridadAutomatica() {
    // Generar el nombre del archivo de respaldo de cuentas
    std::string nombreCopiaCuentas = generarNombreCopiaSeguridad();

    // Generar el nombre del archivo de respaldo de hash con la misma marca temporal
    FechaHora ahora;
    ahora.actualizarHoraActual();
    std::stringstream ssHash;
    ssHash << prefijoHashCopiaSeguridad
           << ahora.obtenerAnio()
           << (ahora.obtenerMes() < 10 ? "0" : "") << ahora.obtenerMes()
           << (ahora.obtenerDia() < 10 ? "0" : "") << ahora.obtenerDia()
           << "_" << (ahora.obtenerHora() < 10 ? "0" : "") << ahora.obtenerHora()
           << (ahora.obtenerMinuto() < 10 ? "0" : "") << ahora.obtenerMinuto()
           << (ahora.obtenerSegundo() < 10 ? "0" : "") << ahora.obtenerSegundo()
           << ".txt";
    std::string nombreCopiaHash = ssHash.str();

    // Crear respaldo del archivo de cuentas
    hacerCopiaSeguridad(nombreCopiaCuentas.c_str());

    // Crear respaldo del archivo de hash
    std::ifstream hashOriginal(hashCuentasTexto);
    if (!hashOriginal) {
        std::cerr << "Error al abrir " << hashCuentasTexto << " para copia.\n";
        return;
    }
    std::ofstream hashCopia(nombreCopiaHash);
    if (!hashCopia) {
        std::cerr << "Error al crear archivo de copia " << nombreCopiaHash << ".\n";
        hashOriginal.close();
        return;
    }
    hashCopia << hashOriginal.rdbuf();
    hashOriginal.close();
    hashCopia.close();
    std::cout << "Copia de seguridad del hash creada en " << nombreCopiaHash << ".\n";
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
       << ".txt"; // Cambiado a .txt para consistencia
    return ss.str();
}

void Banco::restaurar(const char* nombreArchivo) {
    std::string nombreHash = prefijoHashCopiaSeguridad + std::string(nombreArchivo).substr(prefijoHashCopiaSeguridad.length());
    uint32_t hashAlmacenado = CifradoCRC32::leerHash(nombreHash);
    uint32_t hashCalculado = CifradoCRC32::calcularCRC32Archivo(nombreArchivo);
    if (hashAlmacenado != 0 && hashCalculado != hashAlmacenado) {
        std::cout << "Advertencia: El archivo " << nombreArchivo << " ha sido modificado manualmente. Actualizando hash.\n";
        CifradoCRC32::guardarHash(nombreHash, hashCalculado);
    }

    std::ifstream archivoCopia(nombreArchivo);
    if (!archivoCopia) {
        std::cerr << "Error al abrir archivo de copia " << nombreArchivo << ".\n";
        return;
    }
    std::ofstream archivoOriginal(archivoCuentas);
    if (!archivoOriginal) {
        std::cerr << "Error al abrir " << archivoCuentas << " para restaurar.\n";
        archivoCopia.close();
        return;
    }
    archivoOriginal << archivoCopia.rdbuf();
    archivoCopia.close();
    archivoOriginal.close();
    cuentas.recorrer([](Cuenta* cta) { delete cta; });
    cuentas = ListaDobleCircular<Cuenta*>();
    conteoAhorros = 0;
    conteoCorrientes = 0;
    cargarDesdeArchivo();
    uint32_t hash = CifradoCRC32::calcularCRC32Archivo(archivoCuentas);
    CifradoCRC32::guardarHash(hashCuentasTexto, hash);
    std::cout << "Restauración completada desde " << nombreArchivo << ".\n";
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

void Banco::verificarIntegridadArchivo() {
    // Verificar si el archivo existe
    std::ifstream archivo(archivoCuentas);
    if (!archivo) {
        std::cerr << "Error: El archivo " << archivoCuentas << " no existe o ha sido eliminado.\n";
        return;
    }
    archivo.close();

    // Obtener la fecha de última modificación actual
    struct stat fileInfo;
    std::time_t modificacionActual = 0;
    if (stat(archivoCuentas.c_str(), &fileInfo) == 0) {
        modificacionActual = fileInfo.st_mtime;
    } else {
        std::cerr << "Error: No se pudieron obtener los metadatos del archivo " << archivoCuentas << ".\n";
        return;
    }

    // Comparar la fecha de última modificación
    bool modificadoManualmente = (modificacionActual != ultimaModificacion);

    // Comparar hashes
    uint32_t hashAlmacenado = CifradoCRC32::leerHash(hashCuentasTexto);
    uint32_t hashCalculado = CifradoCRC32::calcularCRC32Archivo(archivoCuentas);

    // Mostrar resultados
    std::cout << "Verificación de integridad para " << archivoCuentas << ":\n";
    std::cout << "Hash almacenado: " << hashAlmacenado << "\n";
    std::cout << "Hash calculado: " << hashCalculado << "\n";

    if (hashAlmacenado == 0) {
        std::cout << "Advertencia: No se encontró un hash almacenado en " << hashCuentasTexto << ".\n";
    } else if (hashAlmacenado != hashCalculado) {
        std::cout << "Error: El archivo " << archivoCuentas << " ha sido modificado manualmente (hash no coincide).\n";
        CifradoCRC32::guardarHash(hashCuentasTexto, hashCalculado); // Actualizar hash
        std::cout << "Hash actualizado con el valor calculado.\n";
    } else {
        std::cout << "El hash coincide. El contenido del archivo no ha sido modificado.\n";
    }

    if (modificadoManualmente) {
        std::cout << "Advertencia: La fecha de última modificación del archivo ha cambiado desde la última operación.\n";
        std::cout << "Última modificación conocida: " << (ultimaModificacion == 0 ? "No registrada" : std::ctime(&ultimaModificacion));
        std::cout << "Modificación actual: " << std::ctime(&modificacionActual);
        std::cout << "El archivo pudo haber sido abierto o manipulado directamente.\n";
        ultimaModificacion = modificacionActual; // Actualizar la última modificación conocida
    } else {
        std::cout << "La fecha de última modificación no ha cambiado desde la última operación.\n";
    }
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

void Banco::busquedaHash() {
    int cursor = 0;
    const char* opciones[] = {
        "Buscar datos por Hash",
        "Generar Hash de datos específicos",
        "Volver al menú principal"
    };
    int conteoOpciones = 3;

    while (true) {
        system("cls");
        std::cout << "\n=== BÚSQUEDA HASH ===\n";
        std::cout << "Seleccione una opción:\n\n";
        
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
            switch (cursor) {
            case 0: { // Buscar datos por Hash
                uint32_t hashNumerico = ingresar_entero_32("\nIngrese el hash a buscar (solo números): ");
                
                bool encontrado = false;
                cuentas.recorrer([&hashNumerico, &encontrado](const Cuenta* cta) {
                    // Generar hash para diferentes campos de la cuenta
                    char* nombre = cta->obtenerNombrePropietario();
                    char* apellido = cta->obtenerApellidoPropietario();
                    char* idUsuario = cta->obtenerIdUsuario();
                    char* idCuenta = cta->obtenerIdCuenta();
                    
                    std::string edadStr = std::to_string(cta->obtenerEdad());
                    std::string saldoStr = std::to_string(cta->obtenerSaldo());
                    
                    uint32_t hashNombre = CifradoCRC32::calcularCRC32(std::string(nombre));
                    uint32_t hashApellido = CifradoCRC32::calcularCRC32(std::string(apellido));
                    uint32_t hashIdUsuario = CifradoCRC32::calcularCRC32(std::string(idUsuario));
                    uint32_t hashIdCuenta = CifradoCRC32::calcularCRC32(std::string(idCuenta));
                    uint32_t hashEdad = CifradoCRC32::calcularCRC32(edadStr);
                    uint32_t hashSaldo = CifradoCRC32::calcularCRC32(saldoStr);
                    
                    if (hashNombre == hashNumerico || hashApellido == hashNumerico || 
                        hashIdUsuario == hashNumerico || hashIdCuenta == hashNumerico ||
                        hashEdad == hashNumerico || hashSaldo == hashNumerico) {
                        
                        std::cout << "\n=== CUENTA ENCONTRADA ===\n";
                        std::cout << "ID Cuenta: " << idCuenta << " (Hash: " << hashIdCuenta << ")\n";
                        std::cout << "ID Usuario: " << idUsuario << " (Hash: " << hashIdUsuario << ")\n";
                        std::cout << "Nombre: " << nombre << " (Hash: " << hashNombre << ")\n";
                        std::cout << "Apellido: " << apellido << " (Hash: " << hashApellido << ")\n";
                        std::cout << "Edad: " << cta->obtenerEdad() << " (Hash: " << hashEdad << ")\n";
                        std::cout << "Saldo: $" << cta->obtenerSaldo() << " (Hash: " << hashSaldo << ")\n";
                        encontrado = true;
                    }
                    
                    delete[] nombre;
                    delete[] apellido;
                    delete[] idUsuario;
                    delete[] idCuenta;
                });
                
                if (!encontrado) {
                    std::cout << "\nNo se encontraron datos que coincidan con el hash: " << hashNumerico << "\n";
                }
                break;
            }
            case 1: { // Generar Hash de datos específicos
                std::cout << "\nIngrese el dato para generar su hash: ";
                std::string dato;
                // Solo limpiar el buffer si hay algo pendiente
                if (std::cin.peek() == '\n') {
                    std::cin.ignore();
                }
                std::getline(std::cin, dato);
                
                if (!dato.empty()) {
                    uint32_t hashGenerado = CifradoCRC32::calcularCRC32(dato);
                    std::cout << "\nDato: \"" << dato << "\"\n";
                    std::cout << "Hash CRC32: " << hashGenerado << "\n";
                    
                    // Buscar si este dato existe en alguna cuenta
                    bool encontradoEnCuentas = false;
                    cuentas.recorrer([&dato, &encontradoEnCuentas](const Cuenta* cta) {
                        char* nombre = cta->obtenerNombrePropietario();
                        char* apellido = cta->obtenerApellidoPropietario();
                        char* idUsuario = cta->obtenerIdUsuario();
                        char* idCuenta = cta->obtenerIdCuenta();
                        
                        if (std::string(nombre) == dato || std::string(apellido) == dato ||
                            std::string(idUsuario) == dato || std::string(idCuenta) == dato ||
                            std::to_string(cta->obtenerEdad()) == dato) {
                            
                            if (!encontradoEnCuentas) {
                                std::cout << "\nEste dato se encontró en las siguientes cuentas:\n";
                                encontradoEnCuentas = true;
                            }
                            
                            std::cout << "- Cuenta: " << idCuenta << ", Propietario: " << nombre << " " << apellido << "\n";
                        }
                        
                        delete[] nombre;
                        delete[] apellido;
                        delete[] idUsuario;
                        delete[] idCuenta;
                    });
                    
                    if (!encontradoEnCuentas) {
                        std::cout << "Este dato no se encontró en ninguna cuenta registrada.\n";
                    }
                } else {
                    std::cout << "No se ingresó ningún dato.\n";
                }
                break;
            }
            case 2: { // Volver al menú principal
                return;
            }
            }
            std::cout << "\nPresione cualquier tecla para continuar...";
            _getch();
        }
    }
}

void Banco::establecerSucursalActual(const char* codigoSucursal) {
    if (!codigoSucursal || strlen(codigoSucursal) != 3) {
        throw "Código de sucursal inválido";
    }
    
    // Actualizar sucursal actual
    delete[] sucursalActual;
    sucursalActual = new char[4];
    strcpy(sucursalActual, codigoSucursal);
}

char* Banco::obtenerSucursalActual() const {
    char* copia = new char[4];
    strcpy(copia, sucursalActual);
    return copia;
}