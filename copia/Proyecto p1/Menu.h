#ifndef MENU_H
#define MENU_H

#include "Banco.h"
#include "FechaHora.h"
#include "Validaciones.h"

class Menu {
private:
    Banco& banco;
    FechaHora horaActual;

public:
    Menu(Banco& b);
    void mostrar();
};

#endif