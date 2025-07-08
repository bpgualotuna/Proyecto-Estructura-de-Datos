#ifndef OFICINA_H
#define OFICINA_H

#include "ListaDobleCircular.h"
#include "FechaHora.h"
#include <cstring>

class SucursalBancaria;

struct RegistroActividad {
    char* descripcion;
    FechaHora fecha;
    RegistroActividad(const char* desc, const FechaHora& f) : fecha(f) {
        descripcion = new char[strlen(desc) + 1];
        strcpy(descripcion, desc);
    }
    ~RegistroActividad() { delete[] descripcion; }
};

class Oficina {
private:
    char* codigoOficina; // Ejemplo: "001", "002", "003"
    char* nombre;        // Nombre descriptivo, ej. "Oficina Principal"
    SucursalBancaria* sucursal; // Referencia a la sucursal padre
    ListaDobleCircular<RegistroActividad*> actividades; // Registro de actividades

public:
    Oficina(const char* codOficina, const char* nombreOficina, SucursalBancaria* suc);
    ~Oficina();
    char* obtenerCodigoOficina() const;
    char* obtenerNombre() const;
    SucursalBancaria* obtenerSucursal() const;
    void registrarActividad(const char* descripcion, const FechaHora& fecha);
    void mostrarActividades() const;
    void guardarActividadEnArchivo(const char* descripcion, const FechaHora& fecha); 
    ListaDobleCircular<RegistroActividad*> obtenerActividades() const;
};

#endif