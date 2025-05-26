#include "CuentaCorriente.h"

CuentaCorriente::CuentaCorriente(const char* idCuenta, const char* idUsuario, const char* nombre, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento, double sobregiro)
    : Cuenta(idCuenta, idUsuario, nombre, saldoInicial,diaNacimiento, mesNacimiento, anioNacimiento), limiteSobregiro(sobregiro) {
    if (sobregiro < 0) throw "Límite de sobregiro inválido";
}

void CuentaCorriente::depositar(double monto) {
    if (monto <= 0) throw "Monto de depósito inválido";
    saldo += monto;
}

bool CuentaCorriente::retirar(double monto) {
    if (monto <= 0) throw "Monto de retiro inválido";
    if (monto > saldo + limiteSobregiro) return false;
    saldo -= monto;
    return true;
}

char* CuentaCorriente::obtenerTipo() const {
    char* tipo = new char[10];
    strcpy(tipo, "Corriente");
    return tipo;
}

Cuenta& CuentaCorriente::operator+=(double monto) {
    if (monto <= 0) throw "Monto de depósito inválido";
    saldo += monto;
    return *this;
}

bool CuentaCorriente::operator-=(double monto) {
    if (monto <= 0) throw "Monto de retiro inválido";
    if (monto > saldo + limiteSobregiro) return false;
    saldo -= monto;
    return true;
}