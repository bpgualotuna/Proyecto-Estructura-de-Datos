#ifndef CIFRADO_H
#define CIFRADO_H

#include <cstring>

class Cifrado {
public:
    static char* cifrarMD5(const char* datos, size_t len, size_t& outLen);
    static char* descifrarMD5(const char* cifrado, size_t len, size_t& outLen);
};

#endif