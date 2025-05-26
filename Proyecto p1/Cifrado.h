#ifndef CIFRADO_H
#define CIFRADO_H

#include <cstring>

class Cifrado {
public:
    static char* cifrarMD5(const char* datos);
    static char* descifrarMD5(const char* cifrado);
};

#endif