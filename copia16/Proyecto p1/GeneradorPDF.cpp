#include "GeneradorPDF.h"
#include <hpdf.h>
#include <fstream>
#include <string>
#include <sstream>

bool GeneradorPDF::convertirATxtAPDF(const std::string& archivoEntrada, const std::string& archivoSalidaPDF) {
    std::ifstream in(archivoEntrada);
    if (!in) return false;

    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) return false;

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 12);

    float y = HPDF_Page_GetHeight(page) - 30;
    std::string linea;
    while (std::getline(in, linea)) {
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 30, y, linea.c_str());
        HPDF_Page_EndText(page);
        y -= 15;
        if (y < 30) {
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 12);
            y = HPDF_Page_GetHeight(page) - 30;
        }
    }
    in.close();

    HPDF_SaveToFile(pdf, archivoSalidaPDF.c_str());
    HPDF_Free(pdf);
    return true;
}

// NUEVA: Crear PDF con texto plano
bool GeneradorPDF::crearPDFConTexto(const std::string& texto, const std::string& archivoSalidaPDF) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) return false;
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 12);
    float y = HPDF_Page_GetHeight(page) - 30;
    std::istringstream iss(texto);
    std::string linea;
    while (std::getline(iss, linea)) {
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 30, y, linea.c_str());
        HPDF_Page_EndText(page);
        y -= 15;
        if (y < 30) {
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 12);
            y = HPDF_Page_GetHeight(page) - 30;
        }
    }
    HPDF_SaveToFile(pdf, archivoSalidaPDF.c_str());
    HPDF_Free(pdf);
    return true;
}

// NUEVA: Crear PDF con imagen PNG
bool GeneradorPDF::crearPDFConImagen(const std::string& archivoPNG, const std::string& archivoSalidaPDF) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) return false;
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetWidth(page, 300);
    HPDF_Page_SetHeight(page, 300);
    HPDF_Image img = HPDF_LoadPngImageFromFile(pdf, archivoPNG.c_str());
    if (!img) {
        HPDF_Free(pdf);
        return false;
    }
    HPDF_Page_DrawImage(page, img, 0, 0, 300, 300);
    HPDF_SaveToFile(pdf, archivoSalidaPDF.c_str());
    HPDF_Free(pdf);
    return true;
} 