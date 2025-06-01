#ifndef MENU_H
#define MENU_H

#include "Banco.h" // Elimina esta línea si ya no es necesaria
#include "FechaHora.h"
#include "Validaciones.h"

// Declaración anticipada de Banco
class Banco;

class Menu {
private:
    Banco& banco; // Referencia a Banco, no necesita la definición completa aquí
    FechaHora horaActual;

public:
    Menu(Banco& b); // Constructor que toma una referencia a Banco
    void mostrar();
};

#endif