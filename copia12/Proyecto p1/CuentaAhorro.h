#ifndef CUENTA_AHORRO_H
#define CUENTA_AHORRO_H

#include "Cuenta.h"

class CuentaAhorro : public Cuenta {
private:
    double tasaInteres;

public:
    CuentaAhorro(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento, double tasa = 0.02);
    CuentaAhorro(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial,
                 int diaNacimiento, int mesNacimiento, int anioNacimiento, const FechaHora& fechaCreacion, double tasa = 0.02);
    void depositar(double monto) override;
    bool retirar(double monto) override;
    char* obtenerTipo() const override;
    Cuenta& operator+=(double monto) override;
    bool operator-=(double monto) override;
};

#endif