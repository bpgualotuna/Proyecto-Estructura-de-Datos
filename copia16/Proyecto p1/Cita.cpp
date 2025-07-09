#include "Cita.h"

Cita::Cita(const std::string& cedula, const FechaHora& fecha, int hora)
    : cedula(cedula), fecha(fecha), hora(hora), duracion(1) {}

std::string Cita::getCedula() const {
    return cedula;
}

FechaHora Cita::getFecha() const {
    return fecha;
}

int Cita::getHora() const {
    return hora;
}

int Cita::getDuracion() const {
    return duracion;
}

bool Cita::seSolapa(const Cita& otra) const {
    if (fecha.obtenerDia() != otra.fecha.obtenerDia() ||
        fecha.obtenerMes() != otra.fecha.obtenerMes() ||
        fecha.obtenerAnio() != otra.fecha.obtenerAnio()) {
        return false; // Diferente día, no se solapan
    }
    return (hora == otra.hora); // Solapan si tienen la misma hora
}