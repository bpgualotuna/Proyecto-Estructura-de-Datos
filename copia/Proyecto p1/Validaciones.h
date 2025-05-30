#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include<string>
#include<iostream>
#include"FechaHora.h"
using namespace std;

int ingresar_entero(const char  *);
float ingresar_float(const char  *);
float ingresar_saldo_inicial(const char*);
string ingresar_string(const char* );
string ingresar_apellido(const char* mensaje);
string ingresar_cedula(const char* );
string ingresar_nombre_archivo(const char* mensaje);
FechaHora ingresar_fecha_validada(const char* mensaje);
FechaHora ingresar_fecha_nacimiento_validada(const char* mensaje);
#endif 
