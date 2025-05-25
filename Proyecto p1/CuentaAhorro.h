#ifndef CUENTA_AHORRO_H
#define CUENTA_AHORRO_H

#include "Cuenta.h"

class CuentaAhorro : public Cuenta {
private:
    double tasaInteres;

public:
    CuentaAhorro(const char* idCuenta, const char* idUsuario, const char* nombre, double saldoInicial, double tasa = 0.02);
    void depositar(double monto) override;
    bool retirar(double monto) override;
    char* obtenerTipo() const override;
    Cuenta& operator+=(double monto) override;
    bool operator-=(double monto) override;
};

#endif