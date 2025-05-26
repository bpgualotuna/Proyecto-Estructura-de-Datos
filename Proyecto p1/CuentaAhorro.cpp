#include "CuentaAhorro.h"

CuentaAhorro::CuentaAhorro(const char* idCuenta, const char* idUsuario, const char* nombre, double saldoInicial,int diaNacimiento, int mesNacimiento, int anioNacimiento, double tasa)
    : Cuenta(idCuenta, idUsuario, nombre, saldoInicial,diaNacimiento, mesNacimiento, anioNacimiento), tasaInteres(tasa) {
    if (tasa < 0) throw "Tasa de interes invalida";
}

void CuentaAhorro::depositar(double monto) {
    if (monto <= 0) throw "Monto de deposito invalido";
    saldo += monto;
}

bool CuentaAhorro::retirar(double monto) {
    if (monto <= 0) throw "Monto de retiro invalido";
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
    saldo += monto;
    return *this;
}

bool CuentaAhorro::operator-=(double monto) {
    if (monto <= 0) throw "Monto de retiro invalido";
    if (monto > saldo) return false;
    saldo -= monto;
    return true;
}