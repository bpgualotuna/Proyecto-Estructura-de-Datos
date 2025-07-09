# Instalador Portable por Batch

Este instalador es una solución de emergencia, 100% portable, que no requiere ningún software adicional para funcionar.

## ¿Qué hace el instalador?
- Solicita una clave de una lista de 20 claves válidas.
- Si la clave es correcta, pide la carpeta de destino.
- Copia `banco.exe`, `marquesina_sfml.exe` y todas las DLLs del directorio actual a la carpeta elegida.
- Permite ver la lista de claves válidas antes de instalar.

## ¿Cómo usar el instalador?
1. Coloca `instalador_portable.bat`, `banco.exe`, `marquesina_sfml.exe` y las DLLs necesarias en la misma carpeta.
2. Ejecuta `instalador_portable.bat` (doble clic o desde consola).
3. Elige la opción 1 para instalar o 2 para ver la lista de claves.
4. Si eliges instalar, ingresa una clave válida cuando se te solicite.
5. Escribe la ruta de la carpeta donde quieres instalar el programa (ejemplo: `C:\BancoMarquesina`).
6. El instalador copiará los archivos y te indicará cuando termine.

## Lista de claves válidas (ejemplo)
- BANC-2024-001
- BANC-2024-002
- ...
- BANC-2024-020

Puedes ver la lista completa eligiendo la opción 2 en el menú del batch. 