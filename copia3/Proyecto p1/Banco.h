#ifndef BANCO_H
#define BANCO_H

#include "ListaDobleCircular.h"
#include "Cuenta.h"
#include "FechaHora.h"
#include <fstream>
#include "Menu.h"

class Banco {
private:
    ListaDobleCircular<Cuenta*> cuentas;
    int conteoAhorros;
    int conteoCorrientes;
    const char* archivoCuentas = "cuentas.bin";

    
    void guardarEnArchivo();
    void cargarDesdeArchivo();
    void crearCopiaSeguridad(const char* nombreArchivo);
    std::string generarNombreCopiaSeguridad();
    std::string generarIdUnico(const char* tipo, int conteo);
    ListaDobleCircular<Cuenta*> buscarCuentasPorId(const char* idUsuario);

public:
    Banco();
    ~Banco();
    bool obtenerDatosPorIdUsuario(const char* idUsuario, char*& nombre, char*& apellido);
    void agregarCuentaAhorro(const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento);
    void agregarCuentaCorriente(const char* idUsuario, const char* nombre, const char* apellido, double saldoInicial, int diaNacimiento, int mesNacimiento, int anioNacimiento);
    void depositar(const char* idUsuario, double monto);
    void retirar(const char* idUsuario, double monto);
    void consultarSaldo(const char* idUsuario);
    void buscarPorRangoFechas(const FechaHora& inicio, const FechaHora& fin);
    void buscarPersonalizado(const char* consulta);
    void buscarPorRangoSaldo(double min, double max);
    void hacerCopiaSeguridad(const char* nombreArchivo);
    void hacerCopiaSeguridadAutomatica();
    void restaurar(const char* nombreArchivo);
    void cifrarArchivo(const char* archivo);
    void descifrarArchivo(const char* archivo);
};

#endif