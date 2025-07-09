# Sistema de Generación de Números de Cuenta Bancaria - Ecuador

## Descripción
Este sistema genera números de cuenta bancaria compatibles con los estándares bancarios de Ecuador, siguiendo el formato estructurado y validado.

## Formato del Número de Cuenta
**Formato:** `BB-SSS-T-NNNNNN-V`

### Componentes:
1. **BB** (2 dígitos): Código del banco (01-99)
2. **SSS** (3 dígitos): Código de sucursal (001-999)
3. **T** (1 dígito): Tipo de cuenta
   - **1**: Cuenta de Ahorro
   - **2**: Cuenta Corriente
4. **NNNNNN** (6 dígitos): Número secuencial (000001-999999)
5. **V** (1 dígito): Dígito verificador calculado con Módulo 10

## Algoritmo de Dígito Verificador
Utiliza el algoritmo **Módulo 10** con pesos alternados:

1. **Pesos**: Se alternan entre 2 y 1, procesando de derecha a izquierda
2. **Producto**: Cada dígito se multiplica por su peso correspondiente
3. **Ajuste**: Si el producto es mayor a 9, se resta 9
4. **Suma**: Se suman todos los productos ajustados
5. **Dígito verificador**: `(10 - (suma % 10)) % 10`

## Ejemplos de Números de Cuenta

### Cuentas de Ahorro:
- `01-001-1-000001-8`: Primera cuenta de ahorro
- `01-001-1-000002-6`: Segunda cuenta de ahorro
- `01-001-1-000003-4`: Tercera cuenta de ahorro

### Cuentas Corrientes:
- `01-001-2-000001-6`: Primera cuenta corriente
- `01-001-2-000002-4`: Segunda cuenta corriente
- `01-001-2-000003-2`: Tercera cuenta corriente

## Validaciones Implementadas

### 1. Validación de Tipo de Cuenta
- Solo acepta "AH" (Ahorro) o "CO" (Corriente)
- Lanza excepción para tipos inválidos

### 2. Validación de Número Secuencial
- Rango permitido: 1 a 999,999
- Lanza excepción fuera del rango

### 3. Validación de Códigos
- Código de banco: exactamente 2 dígitos
- Código de sucursal: exactamente 3 dígitos
- Verificación de inicialización

## Métodos Implementados

### `calcularDigitoVerificador(const std::string& numeroCuenta)`
Calcula el dígito verificador usando el algoritmo Módulo 10.

**Parámetros:**
- `numeroCuenta`: Cadena con solo los dígitos del número de cuenta

**Retorna:**
- `int`: Dígito verificador (0-9)

### `generarIdUnico(const char* tipo, int conteo)`
Genera un número de cuenta único siguiendo el formato bancario.

**Parámetros:**
- `tipo`: "AH" para ahorro, "CO" para corriente
- `conteo`: Número secuencial (1-999999)

**Retorna:**
- `std::string`: Número de cuenta completo con formato BB-SSS-T-NNNNNN-V

## Características del Sistema

✅ **Compatibilidad**: Cumple con estándares bancarios ecuatorianos
✅ **Unicidad**: Garantiza números únicos por banco, sucursal y tipo
✅ **Validación**: Robusta validación de parámetros de entrada
✅ **Dígito verificador**: Algoritmo Módulo 10 para detección de errores
✅ **Formato estándar**: Estructura clara y legible
✅ **Sin dependencias**: No requiere librerías externas

## Casos de Uso

### Crear Cuenta de Ahorro
```cpp
banco.agregarCuentaAhorro("1234567890", "Juan", "Pérez", 1000.0, 15, 8, 1990);
// Genera: 01-001-1-000001-8
```

### Crear Cuenta Corriente
```cpp
banco.agregarCuentaCorriente("1234567890", "Juan", "Pérez", 5000.0, 15, 8, 1990);
// Genera: 01-001-2-000001-6
```

## Manejo de Errores

El sistema lanza excepciones para:
- Tipo de cuenta inválido
- Número secuencial fuera de rango
- Códigos de banco/sucursal no inicializados
- Parámetros nulos o vacíos

## Ventajas del Sistema

1. **Trazabilidad**: Cada número identifica banco, sucursal y tipo
2. **Escalabilidad**: Hasta 999,999 cuentas por tipo y sucursal
3. **Integridad**: Dígito verificador detecta errores de transcripción
4. **Compatibilidad**: Formato estándar bancario ecuatoriano
5. **Mantenibilidad**: Código limpio y bien documentado
