#include "Cuenta.h"
#include <cstring>

Cuenta::Cuenta(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial,
 int diaNacimiento, int mesNacimiento, int anioNacimiento)
    : saldo(saldoInicial), fechaCreacion() {
    // Verificar que el idCuenta no sea nulo y tenga longitud razonable
    if (!idCuenta || strlen(idCuenta) == 0) throw "ID de cuenta invalido";
    // Verificar que el idUsuario no sea nulo y tenga longitud razonable
    if (!idUsuario || strlen(idUsuario) == 0) throw "ID de usuario invalido";
    // Verificar que el nombre no sea nulo y tenga longitud razonable
    if (!nombre || strlen(nombre) == 0) throw "Nombre de propietario invalido";
    // Verificar que el apellido no sea nulo y tenga longitud razonable
    if (!apellido || strlen(apellido) == 0) throw "Apellido de propietario invalido";
    // Verificar que el saldo inicial sea no negativo
    if (saldoInicial < 0) throw "Saldo inicial invalido";
    // Calcular edad
    FechaHora ahora;
    ahora.actualizarHoraActual();
    int anioActual = ahora.obtenerAnio();
    int mesActual = ahora.obtenerMes();
    int diaActual = ahora.obtenerDia();
    edad = anioActual - anioNacimiento;
    if (mesActual < mesNacimiento || (mesActual == mesNacimiento && diaActual < diaNacimiento)) {
        edad--; // Ajustar si aún no ha cumplido años este año
    }
    if (edad < 18) throw "El titular debe ser mayor de 18 años";

    this->idCuenta = new char[strlen(idCuenta) + 1];
    strcpy(this->idCuenta, idCuenta);
    this->idUsuario = new char[strlen(idUsuario) + 1];
    strcpy(this->idUsuario, idUsuario);
    nombrePropietario = new char[strlen(nombre) + 1];
    strcpy(nombrePropietario, nombre);
    apellidoPropietario = new char[strlen(apellido) + 1];
    strcpy(apellidoPropietario, apellido);
}

Cuenta::Cuenta(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial,
               int diaNacimiento, int mesNacimiento, int anioNacimiento, const FechaHora& fechaCreacion)
    : saldo(saldoInicial), fechaCreacion(fechaCreacion) {
    if (!idCuenta || strlen(idCuenta) == 0) throw "ID de cuenta invalido";
    if (!idUsuario || strlen(idUsuario) == 0) throw "ID de usuario invalido";
    if (!nombre || strlen(nombre) == 0) throw "Nombre de propietario invalido";
    if (!apellido || strlen(apellido) == 0) throw "Apellido de propietario invalido";
    if (saldoInicial < 0) throw "Saldo inicial invalido";

    FechaHora ahora;
    ahora.actualizarHoraActual();
    int anioActual = ahora.obtenerAnio();
    int mesActual = ahora.obtenerMes();
    int diaActual = ahora.obtenerDia();
    edad = anioActual - anioNacimiento;
    if (mesActual < mesNacimiento || (mesActual == mesNacimiento && diaActual < diaNacimiento)) {
        edad--;
    }
    if (edad < 18) throw "El titular debe ser mayor de 18 años";

    this->idCuenta = new char[strlen(idCuenta) + 1];
    strcpy(this->idCuenta, idCuenta);
    this->idUsuario = new char[strlen(idUsuario) + 1];
    strcpy(this->idUsuario, idUsuario);
    nombrePropietario = new char[strlen(nombre) + 1];
    strcpy(nombrePropietario, nombre);
    apellidoPropietario = new char[strlen(apellido) + 1];
    strcpy(apellidoPropietario, apellido);
}

Cuenta::~Cuenta() {
    delete[] idCuenta;
    delete[] idUsuario;
    delete[] nombrePropietario;
    delete[] apellidoPropietario;
}

double Cuenta::obtenerSaldo() const { return saldo; }

char* Cuenta::obtenerIdCuenta() const {
    char* copia = new char[strlen(idCuenta) + 1];
    strcpy(copia, idCuenta);
    return copia;
}

char* Cuenta::obtenerIdUsuario() const {
    char* copia = new char[strlen(idUsuario) + 1];
    strcpy(copia, idUsuario);
    return copia;
}

char* Cuenta::obtenerNombrePropietario() const {
    char* copia = new char[strlen(nombrePropietario) + 1];
    strcpy(copia, nombrePropietario);
    return copia;
}

char* Cuenta::obtenerApellidoPropietario() const {
    char* copia = new char[strlen(apellidoPropietario) + 1];
    strcpy(copia, apellidoPropietario);
    return copia;
}

FechaHora Cuenta::obtenerFechaCreacion() const { return fechaCreacion; }

int Cuenta::obtenerEdad() const { return edad; }

double Cuenta::operator()() const {
    return saldo;
}