#ifndef CUENTA_H
#define CUENTA_H

#include "FechaHora.h"
#include <cstring>

class Cuenta {
protected:
    char* idCuenta;          // Número de cuenta único generado
    char* idUsuario;         // ID proporcionado por el usuario (ej. cédula)
    char* nombrePropietario;
    double saldo;
    FechaHora fechaCreacion;

public:
    Cuenta(const char* idCuenta, const char* idUsuario, const char* nombre, double saldoInicial);
    virtual ~Cuenta();
    virtual void depositar(double monto) = 0;
    virtual bool retirar(double monto) = 0;
    virtual double obtenerSaldo() const;
    virtual char* obtenerTipo() const = 0;
    char* obtenerIdCuenta() const;
    char* obtenerIdUsuario() const;
    char* obtenerNombrePropietario() const;
    FechaHora obtenerFechaCreacion() const;

    // Operadores sobrecargados
    virtual Cuenta& operator+=(double monto) = 0;
    virtual bool operator-=(double monto) = 0;
    virtual double operator()() const;
};

#endif