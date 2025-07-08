#ifndef CIFRADO_CRC32_H
#define CIFRADO_CRC32_H

#include <string>
#include <cstdint>

class CifradoCRC32 {
private:
    static uint32_t crc32_table[256]; // Tabla precomputada para CRC32
    static void init_crc32_table();   // Inicializar la tabla
    static bool table_initialized;    // Bandera para inicialización

public:
    static uint32_t calcularCRC32(const std::string& datos); // Calcular hash CRC32
    static uint32_t calcularCRC32Archivo(const std::string& nombreArchivo); // Calcular hash de un archivo
    static bool verificarCRC32Archivo(const std::string& nombreArchivo, uint32_t hashEsperado); // Verificar hash
    static void guardarHash(const std::string& nombreArchivoHash, uint32_t hash); // Guardar hash en archivo
    static uint32_t leerHash(const std::string& nombreArchivoHash); // Leer hash de archivo
};

#endif