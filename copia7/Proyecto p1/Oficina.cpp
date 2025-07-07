#include "Oficina.h"
#include "SucursalBancaria.h"
#include <iostream>
#include <fstream>
#include <string>

Oficina::Oficina(const char* codOficina, const char* nombreOficina, SucursalBancaria* suc) {
    if (!codOficina || strlen(codOficina) != 3) throw "Código de oficina inválido";
    if (!nombreOficina || strlen(nombreOficina) == 0) throw "Nombre de oficina inválido";
    if (!suc) throw "Sucursal inválida";

    codigoOficina = new char[4];
    strcpy(codigoOficina, codOficina);
    nombre = new char[strlen(nombreOficina) + 1];
    strcpy(nombre, nombreOficina);
    sucursal = suc;
}

Oficina::~Oficina() {
    delete[] codigoOficina;
    delete[] nombre;
    actividades.recorrer([](RegistroActividad* reg) { delete reg; });
}

char* Oficina::obtenerCodigoOficina() const {
    char* copia = new char[4];
    strcpy(copia, codigoOficina);
    return copia;
}

char* Oficina::obtenerNombre() const {
    char* copia = new char[strlen(nombre) + 1];
    strcpy(copia, nombre);
    return copia;
}

SucursalBancaria* Oficina::obtenerSucursal() const {
    return sucursal;
}

void Oficina::registrarActividad(const char* descripcion, const FechaHora& fecha) {
    if (!descripcion || strlen(descripcion) == 0) throw "Descripción de actividad inválida";
    actividades.insertar(new RegistroActividad(descripcion, fecha));
    guardarActividadEnArchivo(descripcion, fecha); // Guardar en archivo automáticamente
}

void Oficina::guardarActividadEnArchivo(const char* descripcion, const FechaHora& fecha) {
    char* codSucursal = sucursal->obtenerCodigoSucursal();
    char* codOficina = obtenerCodigoOficina();
    std::string nombreArchivo = "Sucursal_" + std::string(codSucursal) + "_Oficina_" + std::string(codOficina) + ".txt";
    delete[] codSucursal;
    delete[] codOficina;

    std::ofstream archivo(nombreArchivo, std::ios::app);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo " << nombreArchivo << " para escritura.\n";
        return;
    }

    char* fechaStr = fecha.aCadena();
    archivo << "Fecha: " << fechaStr << " | Actividad: " << descripcion << "\n";
    delete[] fechaStr;
    archivo.close();
}

void Oficina::mostrarActividades() const {
    if (actividades.obtenerTamano() == 0) {
        std::cout << "No hay actividades registradas en esta oficina.\n";
        return;
    }
    actividades.recorrer([](const RegistroActividad* reg) {
        char* fechaStr = reg->fecha.aCadena();
        std::cout << "Actividad: " << reg->descripcion << ", Fecha: " << fechaStr << "\n";
        delete[] fechaStr;
    });
}

ListaDobleCircular<RegistroActividad*> Oficina::obtenerActividades() const {
    return actividades;
}