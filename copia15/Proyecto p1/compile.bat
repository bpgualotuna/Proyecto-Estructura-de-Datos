@echo off
echo Compilando proyecto bancario con marquesina SFML...

REM Buscar SFML en ubicaciones comunes
set SFML_FOUND=0

REM Verificar si SFML está en el PATH
where sfml-config >nul 2>&1
if %errorlevel% equ 0 (
    echo SFML encontrado en PATH
    set SFML_FOUND=1
    goto compile
)

REM Verificar ubicaciones comunes de SFML
if exist "C:\SFML-2.6.1\include\SFML" (
    echo SFML encontrado en C:\SFML-2.6.1
    set "SFML_DIR=C:\SFML-2.6.1"
    set SFML_FOUND=1
    goto compile
)

if exist "C:\SFML-2.5.1\include\SFML" (
    echo SFML encontrado en C:\SFML-2.5.1
    set "SFML_DIR=C:\SFML-2.5.1"
    set SFML_FOUND=1
    goto compile
)

if exist "C:\Program Files\SFML\include\SFML" (
    echo SFML encontrado en C:\Program Files\SFML
    set "SFML_DIR=C:\Program Files\SFML"
    set SFML_FOUND=1
    goto compile
)

if exist "C:\Program Files (x86)\SFML\include\SFML" (
    echo SFML encontrado en C:\Program Files (x86)\SFML
    set SFML_DIR=C:\Program Files (x86)\SFML
    set SFML_FOUND=1
    goto compile
)

echo ERROR: No se pudo encontrar SFML
echo Por favor instala SFML o ajusta las rutas en este script
pause
exit /b 1

:compile
if %SFML_FOUND% equ 1 (
    echo Compilando con SFML...
    
    REM Compilar todos los archivos fuente
    g++ -std=c++11 -Wall -Wextra -I"%SFML_DIR%\include" ^
        Main.cpp Menu.cpp Banco.cpp Cuenta.cpp CuentaAhorro.cpp CuentaCorriente.cpp ^
        ArbolAVL.cpp ArbolAVLSucursales.cpp SucursalBancaria.cpp FechaHora.cpp ^
        Validaciones.cpp OrdenadorCuentas.cpp BusquedaBinaria.cpp Cifrado.cpp ^
        CifradoCRC32.cpp GeneradorPDF.cpp GeneradorQR.cpp qrcodegen.cpp lodepng.cpp ^
        Marquesina.cpp ^
        -L"%SFML_DIR%\lib" -lsfml-graphics -lsfml-window -lsfml-system ^
        -o banco.exe
    
    if %errorlevel% equ 0 (
        echo Compilacion exitosa! Ejecutable creado: banco.exe
        echo.
        echo Para ejecutar el programa, ejecuta: banco.exe
    ) else (
        echo ERROR en la compilacion
    )
)

pause 