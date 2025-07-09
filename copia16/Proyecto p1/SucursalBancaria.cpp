#include "SucursalBancaria.h"
#include "Banco.h"
#include <fstream>
#include <sstream>

SucursalBancaria::SucursalBancaria(const char* codBanco, const char* codSucursal, const char* nombreSucursal, Banco* bancoCompartido) {
    if (!codBanco || strlen(codBanco) != 2) throw "Codigo de banco invalido";
    if (!codSucursal || strlen(codSucursal) != 3) throw "Codigo de sucursal invalido";
    if (!nombreSucursal || strlen(nombreSucursal) == 0) throw "Nombre de sucursal invalido";
    if (!bancoCompartido) throw "Instancia de Banco invalida";

    codigoBanco = new char[3];
    strcpy(codigoBanco, codBanco);
    codigoSucursal = new char[4];
    strcpy(codigoSucursal, codSucursal);
    nombre = new char[strlen(nombreSucursal) + 1];
    strcpy(nombre, nombreSucursal);
    banco = bancoCompartido; // Usar la instancia compartida de Banco
    cargarCitasDesdeArchivo();
}

SucursalBancaria::~SucursalBancaria() {
    delete[] codigoBanco;
    delete[] codigoSucursal;
    delete[] nombre;
    // No eliminamos banco, ya que es compartido y se gestiona externamente
}

char* SucursalBancaria::obtenerCodigoBanco() const {
    char* copia = new char[3];
    strcpy(copia, codigoBanco);
    return copia;
}

char* SucursalBancaria::obtenerCodigoSucursal() const {
    char* copia = new char[4];
    strcpy(copia, codigoSucursal);
    return copia;
}

char* SucursalBancaria::obtenerNombre() const {
    char* copia = new char[strlen(nombre) + 1];
    strcpy(copia, nombre);
    return copia;
}

Banco* SucursalBancaria::obtenerBanco() const {
    return banco;
}

bool SucursalBancaria::agendarCita(const std::string& cedula, const FechaHora& fecha, int hora) {
    // Validar hora (8:00 a 16:00)
    if (hora < 8 || hora > 16) {
        std::cout << "Error: La hora debe estar entre 8:00 y 16:00.\n";
        return false;
    }

    // Crear cita temporal
    Cita nuevaCita(cedula, fecha, hora);

    // Verificar solapamiento
    for (const Cita& cita : citas) {
        if (cita.seSolapa(nuevaCita)) {
            std::cout << "Error: Ya existe una cita en ese horario.\n";
            return false;
        }
    }

    // Agregar cita
    citas.push_back(nuevaCita);
    std::cout << "Cita agendada exitosamente para el " << fecha.obtenerDia() << "/"
              << fecha.obtenerMes() << "/" << fecha.obtenerAnio() << " a las " << hora << ":00.\n";
    guardarCitasEnArchivo();
    return true;
}

void SucursalBancaria::mostrarCitas() const {
    if (citas.empty()) {
        std::cout << "No hay citas agendadas en " << nombre << ".\n";
        return;
    }
    std::cout << "Citas en " << nombre << ":\n";
    for (const Cita& cita : citas) {
        std::cout << "Cedula: " << cita.getCedula() << ", Fecha: "
                  << cita.getFecha().obtenerDia() << "/" << cita.getFecha().obtenerMes() << "/"
                  << cita.getFecha().obtenerAnio() << ", Hora: " << cita.getHora() << ":00\n";
    }
}

void SucursalBancaria::guardarCitasEnArchivo() const {
    std::string nombreArchivo = std::string("citas_") + codigoBanco + "_" + codigoSucursal + ".txt";
    std::ofstream archivo(nombreArchivo);
    if (!archivo) {
        std::cerr << "Error al abrir archivo de citas: " << nombreArchivo << std::endl;
        return;
    }
    for (const Cita& cita : citas) {
        archivo << cita.getCedula() << ","
                << cita.getFecha().obtenerDia() << "-" << cita.getFecha().obtenerMes() << "-" << cita.getFecha().obtenerAnio() << ","
                << cita.getHora() << "\n";
    }
    archivo.close();
}

void SucursalBancaria::cargarCitasDesdeArchivo() {
    std::string nombreArchivo = std::string("citas_") + codigoBanco + "_" + codigoSucursal + ".txt";
    std::ifstream archivo(nombreArchivo);
    if (!archivo) return;
    citas.clear();
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string cedula, fechaStr, horaStr;
        if (!std::getline(ss, cedula, ',')) continue;
        if (!std::getline(ss, fechaStr, ',')) continue;
        if (!std::getline(ss, horaStr)) continue;
        int dia, mes, anio, hora;
        if (sscanf(fechaStr.c_str(), "%d-%d-%d", &dia, &mes, &anio) != 3) continue;
        hora = std::stoi(horaStr);
        FechaHora fecha(dia, mes, anio, 0, 0, 0);
        citas.emplace_back(cedula, fecha, hora);
    }
    archivo.close();
}