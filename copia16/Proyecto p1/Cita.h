#ifndef CITA_H
#define CITA_H

#include <string>
#include "FechaHora.h"

class Cita {
private:
    std::string cedula;
    FechaHora fecha; // Usar FechaHora en lugar de string
    int hora; // Hora de inicio (8 a 16, formato 24 horas)
    int duracion; // Fija en 1 hora

public:
    Cita(const std::string& cedula, const FechaHora& fecha, int hora);
    std::string getCedula() const;
    FechaHora getFecha() const;
    int getHora() const;
    int getDuracion() const;
    bool seSolapa(const Cita& otra) const;
};

#endif