#include "SucursalBancaria.h"
#include "Banco.h"

SucursalBancaria::SucursalBancaria(const char* codBanco, const char* codSucursal, const char* nombreSucursal, Banco* bancoCompartido) {
    if (!codBanco || strlen(codBanco) != 2) throw "Código de banco inválido";
    if (!codSucursal || strlen(codSucursal) != 3) throw "Código de sucursal inválido";
    if (!nombreSucursal || strlen(nombreSucursal) == 0) throw "Nombre de sucursal inválido";
    if (!bancoCompartido) throw "Instancia de Banco inválida";

    codigoBanco = new char[3];
    strcpy(codigoBanco, codBanco);
    codigoSucursal = new char[4];
    strcpy(codigoSucursal, codSucursal);
    nombre = new char[strlen(nombreSucursal) + 1];
    strcpy(nombre, nombreSucursal);
    banco = bancoCompartido; // Usar la instancia compartida de Banco
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