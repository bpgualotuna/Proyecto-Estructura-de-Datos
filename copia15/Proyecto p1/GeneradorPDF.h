#pragma once
#include <string>

class GeneradorPDF {
public:
    // Convierte un archivo de texto o cualquier archivo a PDF
    static bool convertirATxtAPDF(const std::string& archivoEntrada, const std::string& archivoSalidaPDF);
    // Crea un PDF con texto plano
    static bool crearPDFConTexto(const std::string& texto, const std::string& archivoSalidaPDF);
    // Crea un PDF con una imagen PNG
    static bool crearPDFConImagen(const std::string& archivoPNG, const std::string& archivoSalidaPDF);
}; 