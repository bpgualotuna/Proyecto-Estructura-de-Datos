#include "FechaHora.h"
#include <sstream>
#include <iostream>

FechaHora::FechaHora() {
    actualizarHoraActual();
}

FechaHora::FechaHora(int d, int m, int a, int h, int min, int s)
    : dia(d), mes(m), anio(a), hora(h), minuto(min), segundo(s) {}

void FechaHora::actualizarHoraActual() {
    time_t ahora = time(nullptr);
    tm* ltm = localtime(&ahora);
    dia = ltm->tm_mday;
    mes = ltm->tm_mon + 1;
    anio = ltm->tm_year + 1900;
    hora = ltm->tm_hour;
    minuto = ltm->tm_min;
    segundo = ltm->tm_sec;
}

void FechaHora::establecerHoraSistema() {
#ifdef _WIN32
    SYSTEMTIME st;
    GetSystemTime(&st);
    st.wYear = anio;
    st.wMonth = mes;
    st.wDay = dia;
    st.wHour = hora;
    st.wMinute = minuto;
    st.wSecond = segundo;
    st.wMilliseconds = 0;

    if (!SetSystemTime(&st)) {
        std::cerr << "Error: No se pudo establecer la hora del sistema. Verifique permisos de administrador.\n";
    }
#else
    std::cerr << "Establecer hora del sistema no implementado para esta plataforma.\n";
#endif
}

char* FechaHora::aCadena() const {
    char* resultado = new char[25];
    sprintf(resultado, "%02d-%02d-%04d_%02d-%02d-%02d", dia, mes, anio, hora, minuto, segundo);
    return resultado;
}

bool FechaHora::estaEnRango(const FechaHora& inicio, const FechaHora& fin) const {
    return (*this >= inicio && *this <= fin);
}

FechaHora FechaHora::obtenerCopia() const {
    return FechaHora(dia, mes, anio, hora, minuto, segundo);
}

bool FechaHora::operator<=(const FechaHora& otra) const {
    if (anio != otra.anio) return anio <= otra.anio;
    if (mes != otra.mes) return mes <= otra.mes;
    if (dia != otra.dia) return dia <= otra.dia;
    if (hora != otra.hora) return hora <= otra.hora;
    if (minuto != otra.minuto) return minuto <= otra.minuto;
    return segundo <= otra.segundo;
}

bool FechaHora::operator>=(const FechaHora& otra) const {
    if (anio != otra.anio) return anio >= otra.anio;
    if (mes != otra.mes) return mes >= otra.mes;
    if (dia != otra.dia) return dia >= otra.dia;
    if (hora != otra.hora) return hora >= otra.hora;
    if (minuto != otra.minuto) return minuto >= otra.minuto;
    return segundo >= otra.segundo;
}

int FechaHora::obtenerDia() const {
    return dia;
}

int FechaHora::obtenerMes() const {
    return mes;
}

int FechaHora::obtenerAnio() const {
    return anio;
}

int FechaHora::obtenerHora() const {
    return hora;
}

int FechaHora::obtenerMinuto() const {
    return minuto;
}

int FechaHora::obtenerSegundo() const {
    return segundo;
}

bool FechaHora::esBisiesto() const {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int FechaHora::diasEnMes() const {
    switch (mes) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return esBisiesto() ? 29 : 28;
        default:
            return 0; // Mes invalido
    }
}

bool FechaHora::esFechaValida() const {
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > diasEnMes()) return false;
    if (anio < 1970 || anio > 9999) return false;
    if (hora < 0 || hora > 23) return false;
    if (minuto < 0 || minuto > 59) return false;
    if (segundo < 0 || segundo > 59) return false;
    return true;
}