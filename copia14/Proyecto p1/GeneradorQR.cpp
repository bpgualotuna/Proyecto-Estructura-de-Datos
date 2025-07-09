#include "GeneradorQR.h"
#include "qrcodegen.hpp"
#include "lodepng.h"
#include <vector>
#include <string>
using qrcodegen::QrCode;

bool GeneradorQR::generarQR(const std::string& data, const std::string& archivoSalida) {
    QrCode qr = QrCode::encodeText(data.c_str(), QrCode::Ecc::LOW);
    int size = qr.getSize();
    int scale = 10; // Tamaño de cada módulo en píxeles
    int border = 4; // Bordes blancos alrededor del QR
    int imgSize = (size + border * 2) * scale;

    std::vector<unsigned char> image(imgSize * imgSize * 4, 255); // RGBA blanco

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            if (qr.getModule(x, y)) {
                for (int dy = 0; dy < scale; ++dy) {
                    for (int dx = 0; dx < scale; ++dx) {
                        int xx = (x + border) * scale + dx;
                        int yy = (y + border) * scale + dy;
                        int idx = (yy * imgSize + xx) * 4;
                        image[idx + 0] = 0;   // R
                        image[idx + 1] = 0;   // G
                        image[idx + 2] = 0;   // B
                        image[idx + 3] = 255; // A
                    }
                }
            }
        }
    }

    // Cambia la extensión a .png si es necesario
    std::string outFile = archivoSalida;
    if (outFile.size() < 4 || outFile.substr(outFile.size() - 4) != ".png")
        outFile += ".png";

    // Guardar como PNG usando lodepng header-only
    unsigned error = lodepng_encode32_file(outFile.c_str(), image.data(), imgSize, imgSize);
    return error == 0;
} 