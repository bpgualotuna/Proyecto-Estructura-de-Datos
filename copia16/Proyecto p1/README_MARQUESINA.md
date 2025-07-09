# Sistema Bancario con Marquesina SFML

Este proyecto integra una marquesina gráfica usando SFML que se ejecuta al iniciar el programa del banco.

## Características de la Marquesina

- **Ventana gráfica**: Se abre una ventana SFML al iniciar el programa
- **Animación de texto**: El mensaje "SISTEMA BANCARIO - BIENVENIDO" aparece letra por letra
- **Ejecución en hilo separado**: La marquesina se ejecuta en paralelo con el programa principal
- **Cierre automático**: Se cierra automáticamente cuando el programa principal termina

## Requisitos

### SFML (Simple and Fast Multimedia Library)
Necesitas tener SFML instalado en tu sistema. Puedes descargarlo de:
- https://www.sfml-dev.org/download.php

### Versiones compatibles
- SFML 2.5.x o superior
- Compilador compatible con C++11

## Instalación de SFML en Windows

1. Descarga SFML para tu versión de compilador (MinGW o Visual Studio)
2. Extrae en `C:\SFML-2.6.1` (o la versión que descargues)
3. Asegúrate de que las DLLs de SFML estén en el PATH o en el directorio del ejecutable

## Compilación

### Opción 1: Usar el script batch (Recomendado)
```bash
compile.bat
```

### Opción 2: Usar Makefile
```bash
make
```

### Opción 3: Compilación manual
```bash
g++ -std=c++11 -Wall -Wextra -I"C:\SFML-2.6.1\include" \
    Main.cpp Menu.cpp Banco.cpp Cuenta.cpp CuentaAhorro.cpp CuentaCorriente.cpp \
    ArbolAVL.cpp ArbolAVLSucursales.cpp SucursalBancaria.cpp FechaHora.cpp \
    Validaciones.cpp OrdenadorCuentas.cpp BusquedaBinaria.cpp Cifrado.cpp \
    CifradoCRC32.cpp GeneradorPDF.cpp GeneradorQR.cpp qrcodegen.cpp lodepng.cpp \
    Marquesina.cpp \
    -L"C:\SFML-2.6.1\lib" -lsfml-graphics -lsfml-window -lsfml-system \
    -o banco.exe
```

## Ejecución

```bash
banco.exe
```

## Archivos Nuevos

- `Marquesina.h`: Cabecera de la clase Marquesina
- `Marquesina.cpp`: Implementación de la marquesina con SFML
- `compile.bat`: Script de compilación automática
- `README_MARQUESINA.md`: Este archivo de documentación

## Modificaciones Realizadas

### Main.cpp
- Agregado `#include "Marquesina.h"`
- Creada instancia de `Marquesina` al inicio
- La marquesina se inicia en un hilo separado
- Se detiene automáticamente al salir del programa

### Marquesina.h
- Clase que encapsula la funcionalidad de la marquesina
- Manejo de hilos con `std::thread`
- Control de ejecución con `std::atomic<bool>`

### Marquesina.cpp
- Implementación de la ventana SFML
- Animación de texto letra por letra
- Manejo de eventos de ventana
- Cierre limpio del hilo

## Solución de Problemas

### Error: "No se pudo encontrar SFML"
- Verifica que SFML esté instalado en una de las rutas esperadas
- Ajusta la ruta en `compile.bat` si es necesario

### Error: "No se pudo cargar la fuente"
- La marquesina intenta cargar fuentes de Windows
- Si falla, verifica que Arial o Calibri estén disponibles

### Error de compilación con SFML
- Asegúrate de usar la versión correcta de SFML para tu compilador
- Verifica que las rutas de include y lib sean correctas

## Personalización

### Cambiar el mensaje
Edita `Marquesina.cpp` línea 47:
```cpp
const std::string mensaje = "TU MENSAJE PERSONALIZADO";
```

### Cambiar colores
Edita `Marquesina.cpp` línea 49:
```cpp
const sf::Color colorAzul = sf::Color(R, G, B); // RGB values
```

### Cambiar velocidad
Edita `Marquesina.cpp` línea 58:
```cpp
float delay = 0.15f; // segundos entre letras
```

## Notas Técnicas

- La marquesina usa `std::thread` para ejecución paralela
- Se usa `std::atomic<bool>` para comunicación segura entre hilos
- La ventana SFML se cierra automáticamente al terminar el programa
- El programa principal continúa normalmente mientras la marquesina se ejecuta 