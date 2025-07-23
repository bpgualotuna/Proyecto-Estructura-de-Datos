#include "MarquesinaBanner.h"
#include <chrono>
#include <iostream>
#include <atomic>
#include <windows.h> // Required for Windows API functions

void MarquesinaBanner::mostrarBanner() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 | 8); // Azul brillante
#endif
    std::cout << "BIENVENIDO AL SISTEMA BANCARIO" << std::endl;
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Color por defecto
#endif
    std::cout << std::endl;
} 