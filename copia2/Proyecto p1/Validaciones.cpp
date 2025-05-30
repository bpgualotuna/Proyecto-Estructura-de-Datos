#include <iostream>
#include "Validaciones.h"
#include <conio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

using namespace std;

int ingresar_entero(const char *mensaje)
{
    char num[10]; // Buffer para almacenar el numero
    char c;
    int i = 0;
    bool tiene_digito = false; // Bandera para verificar si se ingreso al menos un digito
    int valor;

    cout << mensaje;

    while (true)
    {
        c = getch();

        if (c >= '0' && c <= '9')
        { // Si es un numero
            if (i < 9)
            {                        // Verifica que no exceda el tamaño del buffer
                cout << c;           // Muestra el caracter
                num[i++] = c;        // Agrega al arreglo
                tiene_digito = true; // Marca que se ingreso al menos un digito
            }
        }
        else if (c == 8 && i > 0)
        {                    // Si se presiona Backspace y hay algo que borrar
            cout << "\b \b"; // Retrocede, borra el caracter en pantalla
            i--;             // Reduce el indice para eliminar el ultimo caracter ingresado
            if (i == 0)
            {
                tiene_digito = false; // Si no quedan caracteres, resetea la bandera
            }
        }
        else if (c == 13)
        { // Si se presiona Enter
            if (tiene_digito)
            { // Permitir Enter solo si se ingreso al menos un digito
                break;
            }
            else
            {
                cout << '\a'; // Beep para indicar error
            }
        }
    }

    num[i] = '\0';     // Termina la cadena
    valor = atoi(num); // Convierte la cadena a entero

    return valor;
}

float ingresar_float(const char *msj)
{
    char cad[20];
    char c;
    int i = 0;
    bool decimal_point = false;

    printf("%s", msj);

    while ((c = _getch()) != 13)
    { // Mientras no se presione Enter (ASCII 13)
        if ((c >= '0' && c <= '9') || (c == '.' && !decimal_point))
        {
            if (c == '.')
            {
                decimal_point = true; // Marcar que se ingreso un punto decimal
            }
            printf("%c", c); // Mostrar el caracter ingresado
            cad[i++] = c;    // Guardarlo en el arreglo
        }
        else if (c == 8 && i > 0)
        {        // Si se presiona Backspace y hay algo que borrar
            i--; // Reduce el indice para eliminar el ultimo caracter ingresado
            if (cad[i] == '.')
            {
                decimal_point = false; // Si se borro un punto decimal, permitir otro
            }
            printf("\b \b"); // Retrocede, borra el caracter en pantalla
        }
    }

    // Agregar .00 si no hay punto decimal
    if (!decimal_point)
    {
        cad[i++] = '.';
        cad[i++] = '0';
        cad[i++] = '0';
    }

    cad[i] = '\0'; // Cerrar la cadena con null terminator
    printf("\n");  // Salto de linea para mantener formato
    return atof(cad);
}

float ingresar_saldo_inicial(const char *msj)
{
    while (true)
    {
        char cad[20];
        char c;
        int i = 0;
        bool decimal_point = false;
        int decimal_count = 0; // Contador para limitar a dos decimales

        printf("%s", msj);

        while ((c = _getch()) != 13)
        { // Mientras no se presione Enter (ASCII 13)
            if ((c >= '0' && c <= '9') || (c == '.' && !decimal_point))
            {
                if (c == '.')
                {
                    decimal_point = true; // Marcar que se ingresó un punto decimal
                }
                else if (decimal_point)
                {
                    decimal_count++; // Contar dígitos después del punto decimal
                    if (decimal_count > 2)
                        continue; // Ignorar más de dos decimales
                }
                printf("%c", c); // Mostrar el caracter ingresado
                cad[i++] = c;    // Guardarlo en el arreglo
            }
            else if (c == 8 && i > 0)
            {        // Si se presiona Backspace y hay algo que borrar
                i--; // Reduce el índice para eliminar el último caracter ingresado
                if (cad[i] == '.')
                {
                    decimal_point = false; // Si se borró un punto decimal, permitir otro
                    decimal_count = 0;     // Reiniciar el contador de decimales
                }
                else if (decimal_point)
                {
                    decimal_count--; // Reducir el contador si se borra un dígito decimal
                }
                printf("\b \b"); // Retrocede, borra el caracter en pantalla
            }
        }

        // Agregar .00 si no hay punto decimal
        if (!decimal_point)
        {
            cad[i++] = '.';
            cad[i++] = '0';
            cad[i++] = '0';
        }
        else if (decimal_count == 1)
        {
            cad[i++] = '0'; // Añadir un cero si solo hay un dígito decimal
        }

        cad[i] = '\0'; // Cerrar la cadena con null terminator
        printf("\n");  // Salto de línea para mantener formato
        float valor = atof(cad);

        // Truncar a dos decimales (centavos) sin redondear
        valor = floor(valor * 100) / 100.0;

        // Validar que el saldo sea al menos 20
        if (valor >= 20)
        {
            return valor; // Retorna el valor válido
        }
        else
        {
            std::cout << "\nError: Saldo inicial inválido, debe ser al menos $20\n";
            std::cout << "Presione cualquier tecla para continuar...";
            _getch(); // Esperar tecla antes de reintentar
            std::cout << std::endl;
        }
    }
}

string ingresar_string(const char *mensaje)
{
    while (true)
    {
        char cadena[100]; // Buffer para la cadena
        char c;
        int i = 0;
        bool tiene_letra = false; // Bandera para verificar si se ingresó al menos una letra
        int longitud_letras = 0;  // Contador de letras (a-z, A-Z)

        cout << mensaje;

        while (true)
        {
            c = _getch();

            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
            {
                if (i < 10 && longitud_letras < 10)
                {                       // Límite máximo de 10 letras
                    cout << c;          // Muestra el caracter
                    cadena[i++] = c;    // Agrega el caracter al arreglo
                    tiene_letra = true; // Marca que se ingresó al menos una letra
                    longitud_letras++;  // Incrementa el contador de letras
                }
                else
                {
                    cout << '\a'; // Beep si se excede el límite
                }
            }
            else if (c == 8 && i > 0)
            {                    // Si se presiona Backspace y hay algo que borrar
                cout << "\b \b"; // Retrocede y borra el caracter en pantalla
                if ((cadena[i - 1] >= 'A' && cadena[i - 1] <= 'Z') ||
                    (cadena[i - 1] >= 'a' && cadena[i - 1] <= 'z'))
                {
                    longitud_letras--;   // Decrementa el contador de letras
                    tiene_letra = false; // Revisar si quedan letras
                    for (int j = 0; j < i - 1; j++)
                    {
                        if ((cadena[j] >= 'A' && cadena[j] <= 'Z') ||
                            (cadena[j] >= 'a' && cadena[j] <= 'z'))
                        {
                            tiene_letra = true;
                            break;
                        }
                    }
                }
                i--; // Reduce el índice
            }
            else if (c == 13)
            { // Si se presiona Enter
                if (tiene_letra && longitud_letras >= 4)
                { // Al menos una letra y mínimo 4 letras
                    break;
                }
                else
                {
                    cout << '\a'; // Beep para indicar error
                }
            }
            else if (c == ' ')
            {                 // No permitir espacios
                cout << '\a'; // Beep para indicar error
            }
        }

        cadena[i] = '\0'; // Termina la cadena con el null terminator
        cout << endl;     // Salto de línea al finalizar

        // Contar palabras (debe ser exactamente 1)
        string entrada(cadena);
        int numPalabras = 0;
        bool enPalabra = false;
        for (char c : entrada)
        {
            if (c == ' ')
            {
                enPalabra = false;
            }
            else if (!enPalabra)
            {
                enPalabra = true;
                numPalabras++;
            }
        }

        if (numPalabras == 1 && longitud_letras >= 4 && longitud_letras <= 10)
        {
            return entrada; // Retorna como un objeto de tipo string
        }
        else
        {
            cout << "\nError: El nombre debe ser una sola palabra sin espacios, tener al menos 4 letras y máximo 10 letras (ejemplo: alexander).\n";
            cout << "Intente de nuevo.\n";
            _getch(); // Esperar tecla antes de limpiar
        }
    }
}

string ingresar_apellido(const char *mensaje)
{
    while (true)
    {
        char cadena[100]; // Buffer para la cadena
        char c;
        int i = 0;
        bool tiene_letra = false; // Bandera para verificar si se ingresó al menos una letra
        int longitud_letras = 0;  // Contador de letras (a-z, A-Z)

        cout << mensaje;

        while (true)
        {
            c = _getch();

            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')
            {
                if (i < 99)
                {                    // Verificar que no exceda el tamaño del buffer
                    cout << c;       // Muestra el caracter
                    cadena[i++] = c; // Agrega el caracter al arreglo
                    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
                    {
                        tiene_letra = true; // Marca que se ingresó al menos una letra
                        longitud_letras++;  // Incrementa el contador de letras
                    }
                }
            }
            else if (c == 8 && i > 0)
            {                    // Si se presiona Backspace y hay algo que borrar
                cout << "\b \b"; // Retrocede y borra el caracter en pantalla
                if ((cadena[i - 1] >= 'A' && cadena[i - 1] <= 'Z') ||
                    (cadena[i - 1] >= 'a' && cadena[i - 1] <= 'z'))
                {
                    longitud_letras--;   // Decrementa el contador de letras
                    tiene_letra = false; // Revisar si quedan letras
                    for (int j = 0; j < i - 1; j++)
                    {
                        if ((cadena[j] >= 'A' && cadena[j] <= 'Z') ||
                            (cadena[j] >= 'a' && cadena[j] <= 'z'))
                        {
                            tiene_letra = true;
                            break;
                        }
                    }
                }
                i--; // Reduce el índice
            }
            else if (c == 13)
            { // Si se presiona Enter
                if (tiene_letra && longitud_letras >= 4)
                { // Al menos una letra y mínimo 4 letras
                    break;
                }
                else
                {
                    cout << '\a'; // Beep para indicar error
                }
            }
        }

        cadena[i] = '\0'; // Termina la cadena con el null terminator
        cout << endl;     // Salto de línea al finalizar

        // Contar palabras (debe ser entre 1 y 4)
        string entrada(cadena);
        int numPalabras = 0;
        bool enPalabra = false;
        for (char c : entrada)
        {
            if (c == ' ')
            {
                enPalabra = false;
            }
            else if (!enPalabra)
            {
                enPalabra = true;
                numPalabras++;
            }
        }

        if (numPalabras >= 1 && numPalabras <= 4 && longitud_letras >= 4)
        {
            return entrada; // Retorna como un objeto de tipo string
        }
        else
        {
            cout << "\nError: El apellido debe contener de 1 a 4 palabras y tener al menos 4 letras (ejemplo: ulco, de la Cruz).\n";
            cout << "Intente de nuevo.\n";
            _getch(); // Esperar tecla antes de limpiar
        }
    }
}

string ingresar_cedula(const char *mensaje)
{
    char cedula[11];
    char c;
    int i;

    auto validarCedula = [](const char *ced) -> bool
    {
        // Convertir la cedula a un arreglo de enteros
        int digitos[10];
        for (int j = 0; j < 10; j++)
        {
            digitos[j] = ced[j] - '0';
        }

        // Validar primeros dos digitos
        int provincia = digitos[0] * 10 + digitos[1];
        if (provincia < 1 || provincia > 24)
        {
            return false;
        }

        // Validar tercer digito (< 6)
        if (digitos[2] >= 6)
        {
            return false;
        }

        // Algoritmo del modulo 10
        int suma = 0;
        for (int j = 0; j < 9; j++)
        {
            int valor = digitos[j];
            if (j % 2 == 0)
            { // Posiciones impares (0, 2, 4, 6, 8)
                valor *= 2;
                if (valor > 9)
                    valor -= 9;
            }
            suma += valor;
        }

        int modulo = suma % 10;
        int verificador = (modulo == 0) ? 0 : 10 - modulo;
        return verificador == digitos[9];
    };

    while (true)
    {
        i = 0;
        cout << mensaje;

        // Capturar 10 dígitos
        while (i < 10)
        {
            c = _getch();

            if (c >= '0' && c <= '9')
            {
                cout << c;
                cedula[i++] = c;
            }
            else if (c == 8 && i > 0)
            {
                cout << "\b \b";
                i--;
            }
        }

        // Esperar Enter
        while (true)
        {
            c = _getch();
            if (c == 13)
            {
                break;
            }
            else if (c == 8 && i > 0)
            {
                cout << "\b \b";
                i--;
            }
        }

        cedula[i] = '\0';

        // Validar cedula
        if (validarCedula(cedula))
        {
            cout << endl;
            return string(cedula);
        }
        else
        {
            cout << "\nCedula invalida. Intente nuevamente.\n";
            _getch(); // Esperar tecla antes de limpiar
        }
    }
}

std::string ingresar_nombre_archivo(const char *mensaje)
{
    std::string entrada;
    bool valido = false;
    do
    {
        std::cout << mensaje;
        std::getline(std::cin, entrada);
        valido = true;
        for (char c : entrada)
        {
            if (!std::isalnum(c) && c != '.' && c != '_' && c != '-')
            {
                valido = false;
                break;
            }
        }
        if (!valido || entrada.empty())
        {
            std::cout << "Por favor, ingrese solo letras, numeros, '.', '_', o '-'.\n";
        }
        // Verificar que termine en .bin
        if (valido && (entrada.length() < 4 || entrada.substr(entrada.length() - 4) != ".bin"))
        {
            valido = false;
            std::cout << "El archivo debe tener extension .bin.\n";
        }
    } while (!valido || entrada.empty());
    return entrada;
}

FechaHora ingresar_fecha_validada(const char *mensaje)
{
    int dia, mes, anio, hora, minuto, segundo;
    bool fechaValida = false;
    std::cout << mensaje;

    while (!fechaValida)
    {
        // Ingresar y validar anioo
        anio = ingresar_entero("\nAnio (1970-9999): ");
        while (anio < 1970 || anio > 9999)
        {
            std::cerr << "\nError: Anio debe estar entre 1970 y 9999.\n";
            anio = ingresar_entero("\nAnio (1970-9999): ");
        }

        // Ingresar y validar mes
        mes = ingresar_entero("\nMes (1-12): ");
        while (mes < 1 || mes > 12)
        {
            std::cerr << "\nError: Mes debe estar entre 1 y 12.\n";
            mes = ingresar_entero("\nMes (1-12): ");
        }

        // Crear una fecha temporal para obtener el número máximo de días
        FechaHora tempFecha(1, mes, anio, 0, 0, 0);
        int maxDias = tempFecha.diasEnMes();
        bool esBisiesto = tempFecha.esBisiesto();
        std::string mensajeDia = "\nDia (1-" + std::to_string(maxDias) + "): ";

        // Ingresar y validar día
        dia = ingresar_entero(mensajeDia.c_str());
        while (dia < 1 || dia > maxDias)
        {
            std::cerr << "\nError: Dia debe estar entre 1 y " << maxDias << " para este mes.\n";
            dia = ingresar_entero(mensajeDia.c_str());
        }

        // Ingresar y validar hora
        hora = ingresar_entero("\nHora (0-23): ");
        while (hora < 0 || hora > 23)
        {
            std::cerr << "\nError: Hora debe estar entre 0 y 23.\n";
            hora = ingresar_entero("\nHora (0-23): ");
        }

        // Ingresar y validar minuto
        minuto = ingresar_entero("\nMinuto (0-59): ");
        while (minuto < 0 || minuto > 59)
        {
            std::cerr << "\nError: Minuto debe estar entre 0 y 59.\n";
            minuto = ingresar_entero("\nMinuto (0-59): ");
        }

        // Ingresar y validar segundo
        segundo = ingresar_entero("\nSegundo (0-59): ");
        while (segundo < 0 || segundo > 59)
        {
            std::cerr << "\nError: Segundo debe estar entre 0 y 59.\n";
            segundo = ingresar_entero("\nSegundo (0-59): ");
        }

        // Crear objeto FechaHora y validar
        FechaHora nuevaFecha(dia, mes, anio, hora, minuto, segundo);
        if (nuevaFecha.esFechaValida())
        {
            // Mostrar si el año es bisiesto
            std::cout << "\nEl anio " << anio << (esBisiesto ? " es bisiesto.\n" : " no es bisiesto.\n");
            return nuevaFecha;
        }
        else
        {
            std::cerr << "\nError: Fecha u hora invalida. Intente de nuevo.\n";
        }
    }
    return FechaHora(); // Nunca se alcanza, pero evita advertencias del compilador
}

FechaHora ingresar_fecha()
{
    return ingresar_fecha_validada("Ingrese Nueva Fecha y Hora (DD MM AAAA HH MM SS):\n");
}

FechaHora ingresar_fecha_nacimiento_validada(const char *mensaje)
{
    int dia, mes, anio;
    bool fechaValida = false;
    std::cout << mensaje;

    // Obtener el año actual para validar que la fecha de nacimiento sea razonable
    FechaHora ahora;
    ahora.actualizarHoraActual();
    int anioActual = ahora.obtenerAnio();

    while (!fechaValida)
    {
        // Ingresar y validar año
        anio = ingresar_entero(("\nAnio (1900-" + std::to_string(anioActual) + "): ").c_str());
        while (anio < 1900 || anio > anioActual)
        {
            std::cerr << "\nError: Anio debe estar entre 1900 y " << anioActual << ".\n";
            anio = ingresar_entero(("\nAnio (1900-" + std::to_string(anioActual) + "): ").c_str());
        }

        // Ingresar y validar mes
        mes = ingresar_entero("\nMes (1-12): ");
        while (mes < 1 || mes > 12)
        {
            std::cerr << "\nError: Mes debe estar entre 1 y 12.\n";
            mes = ingresar_entero("\nMes (1-12): ");
        }

        // Crear una fecha temporal para obtener el número máximo de días
        FechaHora tempFecha(1, mes, anio, 0, 0, 0);
        int maxDias = tempFecha.diasEnMes();
        std::string mensajeDia = "\nDia (1-" + std::to_string(maxDias) + "): ";

        // Ingresar y validar día
        dia = ingresar_entero(mensajeDia.c_str());
        while (dia < 1 || dia > maxDias)
        {
            std::cerr << "\nError: Dia debe estar entre 1 y " << maxDias << " para este mes.\n";
            dia = ingresar_entero(mensajeDia.c_str());
        }

        // Crear objeto FechaHora para validar la fecha
        FechaHora fechaNacimiento(dia, mes, anio, 0, 0, 0);
        if (fechaNacimiento.esFechaValida())
        {
            // Verificar si la edad es al menos 18 años (esto también lo verifica el constructor de Cuenta, pero se incluye aquí para feedback inmediato)
            int edad = anioActual - anio;
            if (ahora.obtenerMes() < mes || (ahora.obtenerMes() == mes && ahora.obtenerDia() < dia))
            {
                edad--;
            }
            if (edad < 18)
            {
                std::cerr << "\nError: El titular debe tener al menos 18 anos. Intente de nuevo.\n";
            }
            else
            {
                std::cout << "\nFecha de nacimiento valida: " << dia << "/" << mes << "/" << anio << "\n";
                return fechaNacimiento;
            }
        }
        else
        {
            std::cerr << "\nError: Fecha invalida. Intente de nuevo.\n";
        }
    }
    return FechaHora(); // Nunca se alcanza, pero evita advertencias del compilador
}