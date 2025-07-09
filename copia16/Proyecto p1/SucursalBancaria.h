#ifndef SUCURSAL_BANCARIA_H
#define SUCURSAL_BANCARIA_H

#include <cstring>
#include <string>
#include "Banco.h"
#include "Cita.h"
#include <vector>

// Declaración anticipada de Banco
class Banco;

class SucursalBancaria {
private:
    char* codigoBanco;   // 2 dígitos, e.g., "01"
    char* codigoSucursal; // 3 dígitos, e.g., "001"
    char* nombre;        // Nombre de la sucursal, e.g., "Sucursal Central"
    Banco* banco;        // Referencia a la instancia compartida de Banco
    std::vector<Cita> citas;

public:
    SucursalBancaria(const char* codBanco, const char* codSucursal, const char* nombreSucursal, Banco* bancoCompartido);
    ~SucursalBancaria();
    char* obtenerCodigoBanco() const;
    char* obtenerCodigoSucursal() const;
    char* obtenerNombre() const;
    Banco* obtenerBanco() const;
    bool operator<(const SucursalBancaria& otra) const;
    bool operator==(const SucursalBancaria& otra) const;
    bool agendarCita(const std::string& cedula, const FechaHora& fecha, int hora);
    void mostrarCitas() const;
};

#endif