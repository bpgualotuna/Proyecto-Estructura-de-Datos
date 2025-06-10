#ifndef FECHAHORA_H
#define FECHAHORA_H

#include <ctime>
#include <cstring>
#ifdef _WIN32
#include <windows.h>
#endif

class FechaHora {
private:
    int dia, mes, anio, hora, minuto, segundo;

public:
    FechaHora();
    FechaHora(int d, int m, int a, int h, int min, int s);
    void establecerHoraSistema();
    char* aCadena() const;
    bool estaEnRango(const FechaHora& inicio, const FechaHora& fin) const;
    void actualizarHoraActual();
    FechaHora obtenerCopia() const; // Nuevo método para obtener una copia
    bool operator<=(const FechaHora& otra) const;
    bool operator>=(const FechaHora& otra) const;
    bool operator<(const FechaHora& otra) const;
    int obtenerDia() const;
    int obtenerMes() const;
    int obtenerAnio() const;
    int obtenerHora() const;
    int obtenerMinuto() const;
    int obtenerSegundo() const;
    bool esBisiesto() const;
    int diasEnMes() const;
    bool esFechaValida() const;
};

#endif