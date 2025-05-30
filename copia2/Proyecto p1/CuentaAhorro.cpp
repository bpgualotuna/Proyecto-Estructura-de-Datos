#include "CuentaAhorro.h"
#include <math.h>

CuentaAhorro::CuentaAhorro(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento, double tasa)
    : Cuenta(idCuenta, idUsuario, nombre, apellido, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento), tasaInteres(tasa) {
    if (tasa < 0) throw "Tasa de interes invalida";
}

void CuentaAhorro::depositar(double monto) {
    if (monto <= 0) throw "Monto de deposito invalido";
    monto = floor(monto * 100) / 100.0;
    saldo += monto;
}

bool CuentaAhorro::retirar(double monto) {
    if (monto <= 0) throw "Monto de retiro invalido";
    monto = floor(monto * 100) / 100.0;
    if (monto > saldo) return false;
    saldo -= monto;
    return true;
}

char* CuentaAhorro::obtenerTipo() const {
    char* tipo = new char[7];
    strcpy(tipo, "Ahorro");
    return tipo;
}

Cuenta& CuentaAhorro::operator+=(double monto) {
    if (monto <= 0) throw "Monto de deposito invalido";
     monto = floor(monto * 100) / 100.0;
    saldo += monto;
    return *this;
}

bool CuentaAhorro::operator-=(double monto) {
    if (monto <= 0) throw "Monto de retiro invalido";
    monto = floor(monto * 100) / 100.0;
    if (monto > saldo) return false;
    saldo -= monto;
    return true;
}