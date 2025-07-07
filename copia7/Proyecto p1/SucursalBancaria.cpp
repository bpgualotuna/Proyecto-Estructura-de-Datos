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
    inicializarOficinas(); // Inicializar las 3 oficinas al crear la sucursal
}

SucursalBancaria::~SucursalBancaria() {
    delete[] codigoBanco;
    delete[] codigoSucursal;
    delete[] nombre;
    // No eliminamos banco, ya que es compartido y se gestiona externamente
    oficinas.recorrer([](Oficina* of) { delete of; });
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

void SucursalBancaria::inicializarOficinas() {
    char codigo[4];
    strcpy(codigo, "001");
    oficinas.insertar(new Oficina(codigo, "Oficina Principal", this));
    strcpy(codigo, "002");
    oficinas.insertar(new Oficina(codigo, "Oficina Secundaria", this));
    strcpy(codigo, "003");
    oficinas.insertar(new Oficina(codigo, "Oficina de Atención al Cliente", this));
}

ListaDobleCircular<Oficina*> SucursalBancaria::obtenerOficinas() const {
    return oficinas;
}

void SucursalBancaria::registrarActividadEnOficina(const char* codOficina, const char* descripcion, const FechaHora& fecha) {
    bool encontrado = false;
    oficinas.recorrer([&codOficina, &descripcion, &fecha, &encontrado](Oficina* oficina) {
        char* codigo = oficina->obtenerCodigoOficina();
        if (strcmp(codigo, codOficina) == 0) {
            oficina->registrarActividad(descripcion, fecha);
            encontrado = true;
        }
        delete[] codigo;
    });
    if (!encontrado) throw "Oficina no encontrada";
}