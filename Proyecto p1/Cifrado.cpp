#include "Cifrado.h"

char* Cifrado::cifrarMD5(const char* datos, size_t len, size_t& outLen) {
    char* resultado = new char[len];
    for (size_t i = 0; i < len; i++) {
        resultado[i] = datos[i] ^ 0x5A; // Cifrado XOR simple
    }
    outLen = len;
    return resultado;
}

char* Cifrado::descifrarMD5(const char* cifrado, size_t len, size_t& outLen) {
    char* resultado = new char[len];
    for (size_t i = 0; i < len; i++) {
        resultado[i] = cifrado[i] ^ 0x5A; // Descifrado XOR simple (reversible)
    }
    outLen = len;
    return resultado;
}