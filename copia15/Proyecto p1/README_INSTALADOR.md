# Instalador Profesional (Inno Setup)

Este instalador te permite distribuir el sistema bancario con marquesina de forma profesional y autónoma.

## ¿Qué hace el instalador?
- Instala `banco.exe` y `marquesina_sfml.exe` en la carpeta elegida.
- Copia las DLLs de SFML si las necesitas (ajusta la línea en el script `.iss`).
- Solicita una clave de una lista de 20 claves válidas.
- Solo permite instalar si la clave es correcta.
- Crea acceso directo en el menú inicio.

## ¿Cómo compilar el instalador?
1. Instala [Inno Setup](https://jrsoftware.org/isdl.php) en tu PC (solo una vez).
2. Abre el archivo `Instalador_ProyectoP1.iss` con Inno Setup.
3. Haz clic en "Compilar".
4. El archivo generado (ejemplo: `BancoMarquesina_Instalador.exe`) es el instalador portable que puedes llevar a cualquier PC.

## ¿Cómo usar el instalador?
1. Ejecuta el instalador (`BancoMarquesina_Instalador.exe`).
2. Ingresa una de las claves válidas cuando se te solicite.
3. Elige la carpeta de instalación.
4. Finaliza la instalación y ejecuta el programa desde el acceso directo o desde la carpeta elegida.

## Lista de claves válidas (ejemplo)
- BANC-2024-001
- BANC-2024-002
- ...
- BANC-2024-020

Puedes ver y modificar la lista de claves en el script `.iss` en la sección `[Code]`. 