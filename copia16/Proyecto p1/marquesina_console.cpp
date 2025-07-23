#include <iostream>
#include <thread>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// Cambia el color de la consola a azul (solo en Windows)
void setBlue() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1 | 8); // Azul brillante
#endif
}

void resetColor() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Color por defecto
#endif
}

void mostrarBanner() {
    system("cls");
    setBlue();
    cout << "BIENVENIDO AL SISTEMA BANCARIO" << endl;
    resetColor();
    cout << endl;
}

int main() {
    mostrarBanner();
    return 0;
} 