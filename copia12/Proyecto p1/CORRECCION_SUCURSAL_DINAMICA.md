# Corrección: Números de Cuenta con Sucursal Dinámica

## Problema Identificado
El sistema generaba números de cuenta con código de sucursal incorrecto (000) independientemente de la sucursal seleccionada, manteniendo el banco compartido para permitir que todas las cuentas se carguen desde cualquier sucursal.

### Ejemplo del problema:
- **Sucursal seleccionada**: 002 (Norte)
- **Número generado**: `01-000-2-000003-8` ❌ (Incorrecto)  
- **Número esperado**: `01-002-2-000003-X` ✅ (Correcto)

## Solución Implementada (SIN MODIFICAR Main.cpp)

### 1. Nueva Variable en Banco.h
```cpp
char* sucursalActual; // 3 dígitos para la sucursal en uso actual
```

### 2. Métodos Agregados
```cpp
void establecerSucursalActual(const char* codigoSucursal);
char* obtenerSucursalActual() const;
```

### 3. Funcionamiento
1. **Inicio**: Banco se inicializa con sucursal "000"
2. **Selección**: Usuario selecciona una sucursal (ej: 002)  
3. **Actualización**: Se llama `establecerSucursalActual("002")`
4. **Generación**: Nuevas cuentas usan código "002"
5. **Compartido**: Todas las cuentas siguen siendo accesibles

## Ventajas
✅ **Banco compartido**: Mantiene acceso a todas las cuentas
✅ **Números correctos**: Refleja la sucursal real
✅ **Dinámico**: Cambio de sucursal sin reiniciar
✅ **Compatible**: No rompe funcionalidad existente

**¡Corrección completada exitosamente!** 🎉
