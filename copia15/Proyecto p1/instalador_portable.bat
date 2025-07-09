@echo off
setlocal enabledelayedexpansion

REM === Lista de claves válidas ===
set CLAVES=BANC-2024-001 BANC-2024-002 BANC-2024-003 BANC-2024-004 BANC-2024-005 BANC-2024-006 BANC-2024-007 BANC-2024-008 BANC-2024-009 BANC-2024-010 BANC-2024-011 BANC-2024-012 BANC-2024-013 BANC-2024-014 BANC-2024-015 BANC-2024-016 BANC-2024-017 BANC-2024-018 BANC-2024-019 BANC-2024-020

:inicio
cls
echo =====================================
echo Instalador Banco con Marquesina (Portable)
echo =====================================
echo.
echo 1. Instalar
echo 2. Ver lista de claves
set /p opcion=Elija una opcion [1-2]: 
if "%opcion%"=="2" goto mostrarclaves
if not "%opcion%"=="1" goto inicio

echo.
set /p clave=Ingrese su clave de instalacion: 
set valido=0
for %%C in (%CLAVES%) do (
    if /I "%%C"=="%clave%" set valido=1
)
if not %valido%==1 (
    echo Clave incorrecta. Intente de nuevo.
    pause
    goto inicio
)

echo Clave correcta!
set "DESTINO="
set /p DESTINO=Ingrese la ruta de instalacion (ejemplo: C:\BancoMarquesina): 
if "%DESTINO%"=="" goto inicio
if not exist "%DESTINO%" mkdir "%DESTINO%"

REM Copiar archivos principales
echo Copiando banco.exe...
copy /Y banco.exe "%DESTINO%" >nul
echo Copiando marquesina_sfml.exe...
copy /Y marquesina_sfml.exe "%DESTINO%" >nul

REM Copiar DLLs de SFML si existen
for %%D in (*.dll) do copy /Y "%%D" "%DESTINO%" >nul

echo Instalacion completada en: %DESTINO%
echo.
echo Para ejecutar el sistema, abre banco.exe en la carpeta destino.
pause
goto fin

:mostrarclaves
cls
echo === Lista de claves validas ===
for %%C in (%CLAVES%) do echo   %%C
echo.
pause
goto inicio

:fin
endlocal 