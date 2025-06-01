#include "CuentaCorriente.h"
#include <math.h>

CuentaCorriente::CuentaCorriente(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento, double sobregiro)
    : Cuenta(idCuenta, idUsuario, nombre, apellido, saldoInicial, diaNacimiento, mesNacimiento, anioNacimiento), limiteSobregiro(sobregiro) {
    if (sobregiro < 0) throw "Límite de sobregiro inválido";
}

void CuentaCorriente::depositar(double monto) {
    if (monto <= 0) throw "Monto de depósito inválido";
     monto = floor(monto * 100) / 100.0;
    saldo += monto;
}

bool CuentaCorriente::retirar(double monto) {
    if (monto <= 0) throw "Monto de retiro inválido";
     monto = floor(monto * 100) / 100.0;
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
     monto = floor(monto * 100) / 100.0;
    saldo += monto;
    return *this;
}

bool CuentaCorriente::operator-=(double monto) {
    if (monto <= 0) throw "Monto de retiro inválido";
     monto = floor(monto * 100) / 100.0;
    if (monto > saldo + limiteSobregiro) return false;
    saldo -= monto;
    return true;
}