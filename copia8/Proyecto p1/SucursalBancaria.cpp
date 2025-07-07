#include "SucursalBancaria.h"
#include "Banco.h"

SucursalBancaria::SucursalBancaria(const char* codBanco, const char* codSucursal, const char* nombreSucursal, Banco* bancoCompartido) {
    if (!codBanco || strlen(codBanco) != 2) throw "Código de banco inválido";
    if (!codSucursal || strlen(codSucursal) != 3) throw "Código de sucursal inválido";
    if (!nombreSucursal || strlen(nombreSucursal) == 0) throw "Nombre de sucursal inválido";
    if (!bancoCompartido) throw "Instancia de Banco inválida";

    codigoBanco = new char[3];
    strcpy(codigoBanco, codBanco);
    codigoSucursal = new char[4];
    strcpy(codigoSucursal, codSucursal);
    nombre = new char[strlen(nombreSucursal) + 1];
    strcpy(nombre, nombreSucursal);
    banco = bancoCompartido; // Usar la instancia compartida de Banco
}

SucursalBancaria::~SucursalBancaria() {
    delete[] codigoBanco;
    delete[] codigoSucursal;
    delete[] nombre;
    // No eliminamos banco, ya que es compartido y se gestiona externamente
}

char* SucursalBancaria::obtenerCodigoBanco() const {
    char* copia = new char[3];
    strcpy(copia, codigoBanco);
    return copia;
}

char* SucursalBancaria::obtenerCodigoSucursal() const {
    char* copia = new char[4];
    strcpy(copia, codigoSucursal);
    return copia;
}

char* SucursalBancaria::obtenerNombre() const {
    char* copia = new char[strlen(nombre) + 1];
    strcpy(copia, nombre);
    return copia;
}

Banco* SucursalBancaria::obtenerBanco() const {
    return banco;
}