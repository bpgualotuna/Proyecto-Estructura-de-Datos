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
    char* sucursalActual; // 3 dígitos para la sucursal en uso actual
    std::string archivoCuentas;      // e.g., "cuentas_01_001.bin"
    std::string hashCuentasTexto;    // e.g., "hash_cuentas_01_001.txt"
    std::string prefijoHashCopiaSeguridad;  // e.g., "hash_copia_seguridad_01_001_"
    bool cuentaExiste(const char* idCuenta);
    std::time_t ultimaModificacion;
    void guardarEnArchivo();
    void cargarDesdeArchivo();
    void verificarIntegridadTexto(const std::string& nombreArchivo, const std::string& nombreHash);
    std::string generarNombreCopiaSeguridad();
    std::string generarIdUnico(const char* tipo, int conteo);
    int calcularDigitoVerificador(const std::string& numeroCuenta);
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
    void verificarIntegridadArchivo();
    void verificarIntegridadSilencioso();
    void busquedaHash(); // Nueva funcionalidad de búsqueda por hash
    void establecerSucursalActual(const char* codigoSucursal);
    char* obtenerSucursalActual() const;
    
    ListaDobleCircular<Cuenta*> buscarCuentasPorId(const char* idUsuario);
};

#endif