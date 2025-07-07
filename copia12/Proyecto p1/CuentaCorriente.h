#ifndef CUENTA_CORRIENTE_H
#define CUENTA_CORRIENTE_H

#include "Cuenta.h"

class CuentaCorriente : public Cuenta {
private:
    double limiteSobregiro;

public:
    CuentaCorriente(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento, double sobregiro = 100.0);
    CuentaCorriente(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial,
                    int diaNacimiento, int mesNacimiento, int anioNacimiento, const FechaHora& fechaCreacion, double sobregiro = 100.0);
    void depositar(double monto) override;
    bool retirar(double monto) override;
    char* obtenerTipo() const override;
    Cuenta& operator+=(double monto) override;
    bool operator-=(double monto) override;
};

#endif