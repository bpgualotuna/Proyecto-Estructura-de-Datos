#ifndef SUCURSAL_BANCARIA_H
#define SUCURSAL_BANCARIA_H

#include "ListaDobleCircular.h"
#include <cstring>
#include "Oficina.h"


// Declaración anticipada de Banco
class Banco;
class oficina;

class SucursalBancaria {
private:
    char* codigoBanco;   // 2 dígitos, e.g., "01"
    char* codigoSucursal; // 3 dígitos, e.g., "001"
    char* nombre;        // Nombre de la sucursal, e.g., "Sucursal Central"
    Banco* banco;        // Referencia a la instancia compartida de Banco
    ListaDobleCircular<Oficina*> oficinas; // Lista de oficinas (3 por sucursal)

public:
    SucursalBancaria(const char* codBanco, const char* codSucursal, const char* nombreSucursal, Banco* bancoCompartido);
    ~SucursalBancaria();
    char* obtenerCodigoBanco() const;
    char* obtenerCodigoSucursal() const;
    char* obtenerNombre() const;
    Banco* obtenerBanco() const;
    void inicializarOficinas(); // Inicializa las 3 oficinas
    ListaDobleCircular<Oficina*> obtenerOficinas() const;
    void registrarActividadEnOficina(const char* codOficina, const char* descripcion, const FechaHora& fecha);
    void mostrarOficinas() const;
};

#endif