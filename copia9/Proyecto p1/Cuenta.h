#ifndef CUENTA_H
#define CUENTA_H

#include "FechaHora.h"
#include <cstring>

class Cuenta {
protected:
    char* idCuenta;          // Número de cuenta único generado
    char* idUsuario;         // ID proporcionado por el usuario (ej. cédula)
    char* nombrePropietario;
    char* apellidoPropietario;
    double saldo;
    FechaHora fechaCreacion;
    int edad;

public:
    Cuenta(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial,
         int diaNacimiento, int mesNacimiento, int anioNacimiento);
     Cuenta(const char* idCuenta, const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial,
           int diaNacimiento, int mesNacimiento, int anioNacimiento, const FechaHora& fechaCreacion);     
    virtual ~Cuenta();
    virtual void depositar(double monto) = 0;
    virtual bool retirar(double monto) = 0;
    virtual double obtenerSaldo() const;
    virtual char* obtenerTipo() const = 0;
    char* obtenerIdCuenta() const;
    char* obtenerIdUsuario() const;
    char* obtenerNombrePropietario() const;
    char* obtenerApellidoPropietario() const;
    FechaHora obtenerFechaCreacion() const;
    int obtenerEdad() const;

    // Operadores sobrecargados
    virtual Cuenta& operator+=(double monto) = 0;
    virtual bool operator-=(double monto) = 0;
    virtual double operator()() const;
};

#endif