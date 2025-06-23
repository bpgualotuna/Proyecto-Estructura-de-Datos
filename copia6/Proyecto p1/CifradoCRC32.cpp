#include "CifradoCRC32.h"
#include <fstream>
#include <sstream>

bool CifradoCRC32::table_initialized = false;
uint32_t CifradoCRC32::crc32_table[256];

void CifradoCRC32::init_crc32_table() {
    const uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            crc = (crc & 1) ? ((crc >> 1) ^ polynomial) : (crc >> 1);
        }
        crc32_table[i] = crc;
    }
    table_initialized = true;
}

uint32_t CifradoCRC32::calcularCRC32(const std::string& datos) {
    if (!table_initialized) init_crc32_table();
    uint32_t crc = 0xFFFFFFFF;
    for (char c : datos) {
        crc = (crc >> 8) ^ crc32_table[(crc ^ static_cast<uint8_t>(c)) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
}

uint32_t CifradoCRC32::calcularCRC32Archivo(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo.is_open()) return 0;
    std::stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    return calcularCRC32(buffer.str());
}

bool CifradoCRC32::verificarCRC32Archivo(const std::string& nombreArchivo, uint32_t hashEsperado) {
    uint32_t hashCalculado = calcularCRC32Archivo(nombreArchivo);
    return hashCalculado == hashEsperado;
}

void CifradoCRC32::guardarHash(const std::string& nombreArchivoHash, uint32_t hash) {
    std::ofstream archivo(nombreArchivoHash);
    if (archivo.is_open()) {
        archivo << hash;
        archivo.close();
    }
}

uint32_t CifradoCRC32::leerHash(const std::string& nombreArchivoHash) {
    std::ifstream archivo(nombreArchivoHash);
    uint32_t hash = 0;
    if (archivo.is_open()) {
        archivo >> hash;
        archivo.close();
    }
    return hash;
}