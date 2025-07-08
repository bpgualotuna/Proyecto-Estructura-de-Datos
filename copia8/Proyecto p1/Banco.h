#ifndef BANCO_H
#define BANCO_H

#include "ListaDobleCircular.h"
#include "Cuenta.h"
#include "FechaHora.h"
#include "OrdenadorCuentas.h"
#include "CifradoCRC32.h"
#include <fstream>
#include "Menu.h"
#include <string>
#include <functional>


class Banco {
private:
    ListaDobleCircular<Cuenta*> cuentas;
    int conteoAhorros;
    int conteoCorrientes;
    char* codigoBanco;   // 2 dígitos, e.g., "01"
    char* codigoSucursal; // 3 dígitos, e.g., "001"
    std::string archivoCuentas;      // e.g., "cuentas_01_001.bin"
    std::string archivoCuentasTexto; // e.g., "cuentas_01_001.txt"
    std::string hashCuentasTexto;    // e.g., "hash_cuentas_01_001.txt"
    std::string prefijoCopiaSeguridadTexto; // e.g., "copia_seguridad_01_001_"
    std::string prefijoHashCopiaSeguridad;  // e.g., "hash_copia_seguridad_01_001_"
    bool cuentaExiste(const char* idCuenta);
    
    void guardarEnArchivo();
    void cargarDesdeArchivo();
    void guardarEnArchivoTexto();
    void cargarDesdeArchivoTexto();
    void crearCopiaSeguridad(const char* nombreArchivo);
    void hacerCopiaSeguridadTexto(const char* nombreArchivo);
    void restaurarTexto(const char* nombreArchivo);
    void verificarIntegridadTexto(const std::string& nombreArchivo, const std::string& nombreHash);
    std::string generarNombreCopiaSeguridad();
    std::string generarIdUnico(const char* tipo, int conteo);
    //ListaDobleCircular<Cuenta*> buscarCuentasPorId(const char* idUsuario);

   
public:
    Banco(const char* codBanco, const char* codSucursal);
    ~Banco();
    bool obtenerDatosPorIdUsuario(const char* idUsuario, char*& nombre, char*& apellido, int& edad);
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
    void ordenarCuentas(std::function<bool(const Cuenta*, const Cuenta*)> comparador);
    void mostrarCuentas() const;
    ListaDobleCircular<Cuenta*> buscarCuentasPorId(const char* idUsuario);
};

#endif