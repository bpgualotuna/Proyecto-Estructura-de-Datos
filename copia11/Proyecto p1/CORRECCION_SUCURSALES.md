# Corrección: Números de Cuenta por Sucursal Específica

## Problema Identificado
El sistema estaba generando números de cuenta con códigos de sucursal incorrectos. Aunque el usuario seleccionaba diferentes sucursales (001, 002, 003), todos los números de cuenta se generaban con el código "000".

### Ejemplo del problema:
- **Sucursal seleccionada**: 001 (Sucursal Central)
- **Número generado**: `01-000-1-000001-X` ❌ (Incorrecto)
- **Número esperado**: `01-001-1-000001-X` ✅ (Correcto)

## Causa del Problema
En el código original, se creaba una única instancia del `Banco` con códigos fijos:
```cpp
// CÓDIGO PROBLEMÁTICO
Banco* bancoCompartido = new Banco("01", "000"); // ← Códigos fijos
arbol.insertar(new SucursalBancaria("01", "001", "Sucursal Central", bancoCompartido));
arbol.insertar(new SucursalBancaria("01", "002", "Sucursal Norte", bancoCompartido));
arbol.insertar(new SucursalBancaria("01", "003", "Sucursal Sur", bancoCompartido));
```

## Solución Implementada
Se modificó el `Main.cpp` para crear instancias separadas del `Banco` para cada sucursal:

```cpp
// CÓDIGO CORREGIDO
Banco* bancoSucursal001 = new Banco("01", "001");
Banco* bancoSucursal002 = new Banco("01", "002");
Banco* bancoSucursal003 = new Banco("01", "003");

arbol.insertar(new SucursalBancaria("01", "001", "Sucursal Central", bancoSucursal001));
arbol.insertar(new SucursalBancaria("01", "002", "Sucursal Norte", bancoSucursal002));
arbol.insertar(new SucursalBancaria("01", "003", "Sucursal Sur", bancoSucursal003));
```

## Resultados Esperados

### Sucursal 001 (Central):
- **Cuenta de Ahorro**: `01-001-1-000001-X`
- **Cuenta Corriente**: `01-001-2-000001-X`

### Sucursal 002 (Norte):
- **Cuenta de Ahorro**: `01-002-1-000001-X`
- **Cuenta Corriente**: `01-002-2-000001-X`

### Sucursal 003 (Sur):
- **Cuenta de Ahorro**: `01-003-1-000001-X`
- **Cuenta Corriente**: `01-003-2-000001-X`

## Detalles de la Estructura del Número

```
01-001-1-000001-8
│  │   │ │      │
│  │   │ │      └── Dígito verificador (calculado)
│  │   │ └────────── Número secuencial (por sucursal)
│  │   └───────────── Tipo cuenta (1=Ahorro, 2=Corriente)
│  └───────────────── Código sucursal (001, 002, 003)
└──────────────────── Código banco (01)
```

## Archivos Modificados

1. **`Main.cpp`** - Cambiado para crear instancias separadas del `Banco`
2. **`test_sucursales.cpp`** - Programa de prueba creado para verificar funcionamiento

## Ventajas de la Solución

✅ **Correcta identificación**: Cada cuenta refleja su sucursal real
✅ **Independencia**: Cada sucursal maneja su propio contador de cuentas
✅ **Escalabilidad**: Fácil agregar nuevas sucursales
✅ **Integridad**: Cada sucursal tiene su propio archivo de datos
✅ **Trazabilidad**: Número de cuenta identifica claramente la sucursal

## Gestión de Archivos por Sucursal

Cada sucursal ahora genera sus propios archivos:

### Sucursal 001:
- `cuentas_01_001.txt`
- `hash_cuentas_01_001.txt`
- `hash_copia_seguridad_01_001_*.txt`

### Sucursal 002:
- `cuentas_01_002.txt`
- `hash_cuentas_01_002.txt`
- `hash_copia_seguridad_01_002_*.txt`

### Sucursal 003:
- `cuentas_01_003.txt`
- `hash_cuentas_01_003.txt`
- `hash_copia_seguridad_01_003_*.txt`

## Pruebas Realizadas

1. **Compilación**: ✅ Sin errores
2. **Ejecución**: ✅ Programa funciona correctamente
3. **Generación de números**: ✅ Códigos de sucursal correctos
4. **Archivo de prueba**: ✅ `test_sucursales.cpp` verifica funcionamiento

**¡Corrección completada exitosamente!** 🎉
