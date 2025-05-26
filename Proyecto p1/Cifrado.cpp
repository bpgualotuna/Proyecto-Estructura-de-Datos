#include "Cifrado.h"

// Simulación simplificada de MD5 (el MD5 real requiere una biblioteca)
char* Cifrado::cifrarMD5(const char* datos) {
    size_t len = strlen(datos);
    char* resultado = new char[len + 1];
    for (size_t i = 0; i < len; i++) {
        resultado[i] = datos[i] ^ 0x5A; // Cifrado XOR simple
    }
    resultado[len] = '\0';
    return resultado;
}

char* Cifrado::descifrarMD5(const char* cifrado) {
    return cifrarMD5(cifrado); // XOR es reversible
}