#pragma once
#include <string>

class GeneradorQR {
public:
    // Genera un código QR simple (formato PBM o ASCII) a partir de un string
    static bool generarQR(const std::string& data, const std::string& archivoSalida);
}; 