#include<iostream>
#include"Validaciones.h"
#include <conio.h>
#include <stdlib.h>
#include<string>

using namespace std;


int ingresar_entero(const char *mensaje) {
    char num[10]; // Buffer para almacenar el numero
    char c;
    int i = 0;
    bool tiene_digito = false; // Bandera para verificar si se ingreso al menos un digito
    int valor;

    cout << mensaje;

    while (true) {
        c = getch();

        if (c >= '0' && c <= '9') { // Si es un numero
            if (i < 9) { // Verifica que no exceda el tamaño del buffer
                cout << c; // Muestra el caracter
                num[i++] = c; // Agrega al arreglo
                tiene_digito = true; // Marca que se ingreso al menos un digito
            }
        } else if (c == 8 && i > 0) { // Si se presiona Backspace y hay algo que borrar
            cout << "\b \b"; // Retrocede, borra el caracter en pantalla
            i--; // Reduce el indice para eliminar el ultimo caracter ingresado
            if (i == 0) {
                tiene_digito = false; // Si no quedan caracteres, resetea la bandera
            }
        } else if (c == 13) { // Si se presiona Enter
            if (tiene_digito) { // Permitir Enter solo si se ingreso al menos un digito
                break;
            } else {
                cout << '\a'; // Beep para indicar error
            }
        }
    }

    num[i] = '\0'; // Termina la cadena
    valor = atoi(num); // Convierte la cadena a entero

    return valor;
}

float ingresar_float(const char* msj) {
    char cad[20]; 
    char c;
    int i = 0;
    bool decimal_point = false; 

    printf("%s", msj);

    while ((c = _getch()) != 13) { // Mientras no se presione Enter (ASCII 13)
        if ((c >= '0' && c <= '9') || (c == '.' && !decimal_point)) {
            if (c == '.') {
                decimal_point = true; // Marcar que se ingreso un punto decimal
            }
            printf("%c", c);       // Mostrar el caracter ingresado
            cad[i++] = c;         // Guardarlo en el arreglo
        }
        else if (c == 8 && i > 0) { // Si se presiona Backspace y hay algo que borrar
            i--; // Reduce el indice para eliminar el ultimo caracter ingresado
            if (cad[i] == '.') {
                decimal_point = false; // Si se borro un punto decimal, permitir otro
            }
            printf("\b \b"); // Retrocede, borra el caracter en pantalla
        }
    }

    // Agregar .00 si no hay punto decimal
    if (!decimal_point) {
        cad[i++] = '.';
        cad[i++] = '0';
        cad[i++] = '0';
    }

    cad[i] = '\0'; // Cerrar la cadena con null terminator
    printf("\n"); // Salto de linea para mantener formato
    return atof(cad); 
}
string ingresar_string(const char* mensaje) {
    char cadena[100]; // Buffer para la cadena
    char c;
    int i = 0;
    bool tiene_letra = false; // Bandera para verificar si se ingreso al menos una letra

    cout << mensaje;

    while (true) {
        c = _getch();

        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ') {
            if (i < 99) { // Verificar que no exceda el tamaño del buffer
                cout << c; // Muestra el caracter
                cadena[i++] = c; // Agrega el caracter al arreglo
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    tiene_letra = true; // Marca que se ingreso al menos una letra
                }
            }
        } else if (c == 8 && i > 0) { // Si se presiona Backspace y hay algo que borrar
            cout << "\b \b"; // Retrocede y borra el caracter en pantalla
            if ((cadena[i - 1] >= 'A' && cadena[i - 1] <= 'Z') || 
                (cadena[i - 1] >= 'a' && cadena[i - 1] <= 'z')) {
                tiene_letra = false; // Puede ser necesario revisar si quedan letras
                for (int j = 0; j < i - 1; j++) {
                    if ((cadena[j] >= 'A' && cadena[j] <= 'Z') || 
                        (cadena[j] >= 'a' && cadena[j] <= 'z')) {
                        tiene_letra = true;
                        break;
                    }
                }
            }
            i--; // Reduce el indice
        } else if (c == 13) { // Si se presiona Enter
            if (tiene_letra) { // Permitir Enter solo si se ingreso al menos una letra
                break;
            } else {
                cout << '\a'; // Beep para indicar error
            }
        }
    }

    cadena[i] = '\0'; // Termina la cadena con el null terminator
    cout << endl; // Salto de linea al finalizar

    return string(cadena); // Retorna como un objeto de tipo string
}
string ingresar_cedula(const char* mensaje) {
    char cedula[11];
    char c;
    int i;

    auto validarCedula = [](const char* ced) -> bool {
        // Convertir la cedula a un arreglo de enteros
        int digitos[10];
        for (int j = 0; j < 10; j++) {
            digitos[j] = ced[j] - '0';
        }

        // Validar primeros dos digitos 
        int provincia = digitos[0] * 10 + digitos[1];
        if (provincia < 1 || provincia > 24) {
            return false;
        }

        // Validar tercer digito (< 6)
        if (digitos[2] >= 6) {
            return false;
        }

        // Algoritmo del modulo 10
        int suma = 0;
        for (int j = 0; j < 9; j++) {
            int valor = digitos[j];
            if (j % 2 == 0) { // Posiciones impares (0, 2, 4, 6, 8)
                valor *= 2;
                if (valor > 9) valor -= 9;
            }
            suma += valor;
        }

        int modulo = suma % 10;
        int verificador = (modulo == 0) ? 0 : 10 - modulo;
        return verificador == digitos[9];
    };

    while (true) {
        i = 0;
        cout << mensaje;

        // Capturar 10 dígitos
        while (i < 10) {
            c = _getch();

            if (c >= '0' && c <= '9') {
                cout << c;
                cedula[i++] = c;
            } else if (c == 8 && i > 0) {
                cout << "\b \b";
                i--;
            }
        }

        // Esperar Enter
        while (true) {
            c = _getch();
            if (c == 13) {
                break;
            } else if (c == 8 && i > 0) {
                cout << "\b \b";
                i--;
            }
        }

        cedula[i] = '\0';

        // Validar cedula
        if (validarCedula(cedula)) {
            cout << endl;
            return string(cedula);
        } else {
            cout << "\nCedula invalida. Intente nuevamente.\n";
            _getch(); // Esperar tecla antes de limpiar
        }
    }
}


std::string ingresar_nombre_archivo(const char* mensaje) {
    std::string entrada;
    bool valido = false;
    do {
        std::cout << mensaje;
        std::getline(std::cin, entrada);
        valido = true;
        for (char c : entrada) {
            if (!std::isalnum(c) && c != '.' && c != '_' && c != '-') {
                valido = false;
                break;
            }
        }
        if (!valido || entrada.empty()) {
            std::cout << "Por favor, ingrese solo letras, numeros, '.', '_', o '-'.\n";
        }
        // Verificar que termine en .bin
        if (valido && (entrada.length() < 4 || entrada.substr(entrada.length() - 4) != ".bin")) {
            valido = false;
            std::cout << "El archivo debe tener extension .bin.\n";
        }
    } while (!valido || entrada.empty());
    return entrada;
}