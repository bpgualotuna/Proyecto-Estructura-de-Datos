#include "Banco.h"
#include "Menu.h"
#include "Validaciones.h"

int main() {
    Banco banco;
    Menu menu(banco);
    menu.mostrar();
    return 0;
}
