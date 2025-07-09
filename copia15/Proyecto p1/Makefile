# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++11 -Wall -Wextra

# Directorios de SFML (ajusta según tu instalación)
SFML_DIR = C:/SFML-2.6.1
SFML_INCLUDE = $(SFML_DIR)/include
SFML_LIB = $(SFML_DIR)/lib

# Flags de SFML
SFML_FLAGS = -I$(SFML_INCLUDE) -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system

# Archivos fuente
SOURCES = Main.cpp \
          Menu.cpp \
          Banco.cpp \
          Cuenta.cpp \
          CuentaAhorro.cpp \
          CuentaCorriente.cpp \
          ArbolAVL.cpp \
          ArbolAVLSucursales.cpp \
          SucursalBancaria.cpp \
          FechaHora.cpp \
          Validaciones.cpp \
          OrdenadorCuentas.cpp \
          BusquedaBinaria.cpp \
          Cifrado.cpp \
          CifradoCRC32.cpp \
          GeneradorPDF.cpp \
          GeneradorQR.cpp \
          qrcodegen.cpp \
          lodepng.cpp \
          Marquesina.cpp

# Archivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Ejecutable principal
TARGET = banco.exe

# Regla por defecto
all: $(TARGET)

# Compilar el ejecutable principal
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SFML_FLAGS)

# Reglas de compilación para archivos objeto
Main.o: Main.cpp Menu.h ArbolAVLSucursales.h SucursalBancaria.h Banco.h Marquesina.h
	$(CXX) $(CXXFLAGS) -c Main.cpp

Menu.o: Menu.cpp Menu.h ArbolAVLSucursales.h SucursalBancaria.h Banco.h Validaciones.h OrdenadorCuentas.h BusquedaBinaria.h ArbolAVL.h GeneradorPDF.h GeneradorQR.h
	$(CXX) $(CXXFLAGS) -c Menu.cpp

Banco.o: Banco.cpp Banco.h Cuenta.h CuentaAhorro.h CuentaCorriente.h ArbolAVL.h FechaHora.h
	$(CXX) $(CXXFLAGS) -c Banco.cpp

Cuenta.o: Cuenta.cpp Cuenta.h FechaHora.h
	$(CXX) $(CXXFLAGS) -c Cuenta.cpp

CuentaAhorro.o: CuentaAhorro.cpp CuentaAhorro.h Cuenta.h
	$(CXX) $(CXXFLAGS) -c CuentaAhorro.cpp

CuentaCorriente.o: CuentaCorriente.cpp CuentaCorriente.h Cuenta.h
	$(CXX) $(CXXFLAGS) -c CuentaCorriente.cpp

ArbolAVL.o: ArbolAVL.cpp ArbolAVL.h NodoAVL.h
	$(CXX) $(CXXFLAGS) -c ArbolAVL.cpp

ArbolAVLSucursales.o: ArbolAVLSucursales.cpp ArbolAVLSucursales.h ArbolAVL.h SucursalBancaria.h
	$(CXX) $(CXXFLAGS) -c ArbolAVLSucursales.cpp

SucursalBancaria.o: SucursalBancaria.cpp SucursalBancaria.h Banco.h Cuenta.h
	$(CXX) $(CXXFLAGS) -c SucursalBancaria.cpp

FechaHora.o: FechaHora.cpp FechaHora.h
	$(CXX) $(CXXFLAGS) -c FechaHora.cpp

Validaciones.o: Validaciones.cpp Validaciones.h
	$(CXX) $(CXXFLAGS) -c Validaciones.cpp

OrdenadorCuentas.o: OrdenadorCuentas.cpp OrdenadorCuentas.h Cuenta.h
	$(CXX) $(CXXFLAGS) -c OrdenadorCuentas.cpp

BusquedaBinaria.o: BusquedaBinaria.cpp BusquedaBinaria.h Cuenta.h
	$(CXX) $(CXXFLAGS) -c BusquedaBinaria.cpp

Cifrado.o: Cifrado.cpp Cifrado.h
	$(CXX) $(CXXFLAGS) -c Cifrado.cpp

CifradoCRC32.o: CifradoCRC32.cpp CifradoCRC32.h
	$(CXX) $(CXXFLAGS) -c CifradoCRC32.cpp

GeneradorPDF.o: GeneradorPDF.cpp GeneradorPDF.h
	$(CXX) $(CXXFLAGS) -c GeneradorPDF.cpp

GeneradorQR.o: GeneradorQR.cpp GeneradorQR.h qrcodegen.hpp
	$(CXX) $(CXXFLAGS) -c GeneradorQR.cpp

qrcodegen.o: qrcodegen.cpp qrcodegen.hpp
	$(CXX) $(CXXFLAGS) -c qrcodegen.cpp

lodepng.o: lodepng.cpp lodepng.h
	$(CXX) $(CXXFLAGS) -c lodepng.cpp

Marquesina.o: Marquesina.cpp Marquesina.h
	$(CXX) $(CXXFLAGS) -c Marquesina.cpp

# Limpiar archivos objeto y ejecutables
clean:
	del /Q *.o $(TARGET) 2>nul || true

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Regla para compilar solo la marquesina
marquesina: marquesina_sfml.cpp
	$(CXX) $(CXXFLAGS) marquesina_sfml.cpp -o marquesina_sfml.exe $(SFML_FLAGS)

.PHONY: all clean run marquesina



