#include "BusquedaAVL.h"
#include <iostream>
#include <conio.h> // Required for _getch()
#include <windows.h> // Required for system("cls")
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits> // Required for std::numeric_limits
#include <string>
#include <vector>
#include <queue>
#include <cmath>

void construirYGraficarAVLPorNombre() {
    auto lista = leerCuentasDesdeArchivo("cuentas_01_000.txt");
    AVLTree<CuentaSimple, CompararPorNombre> arbol;
    int count = 0;
    lista.recorrer([&arbol, &count](const CuentaSimple& c) { arbol.insert(c); count++; });
    if (count == 0) {
        std::cout << "\n[ADVERTENCIA] No se insertaron nodos. Verifica el archivo cuentas_01_000.txt\n";
    } else {
        std::cout << "\nÁrbol AVL por Nombre: (" << count << " nodos)\n";
        arbol.printTree();
    }
}
void construirYGraficarAVLPorApellido() {
    auto lista = leerCuentasDesdeArchivo("cuentas_01_000.txt");
    AVLTree<CuentaSimple, CompararPorApellido> arbol;
    int count = 0;
    lista.recorrer([&arbol, &count](const CuentaSimple& c) { arbol.insert(c); count++; });
    if (count == 0) {
        std::cout << "\n[ADVERTENCIA] No se insertaron nodos. Verifica el archivo cuentas_01_000.txt\n";
    } else {
        std::cout << "\nÁrbol AVL por Apellido: (" << count << " nodos)\n";
        arbol.printTree();
    }
}
void construirYGraficarAVLPorEdad() {
    auto lista = leerCuentasDesdeArchivo("cuentas_01_000.txt");
    AVLTree<CuentaSimple, CompararPorEdad> arbol;
    int count = 0;
    lista.recorrer([&arbol, &count](const CuentaSimple& c) { arbol.insert(c); count++; });
    if (count == 0) {
        std::cout << "\n[ADVERTENCIA] No se insertaron nodos. Verifica el archivo cuentas_01_000.txt\n";
    } else {
        std::cout << "\nÁrbol AVL por Edad: (" << count << " nodos)\n";
        arbol.printTree();
    }
}
void construirYGraficarAVLPorFecha() {
    auto lista = leerCuentasDesdeArchivo("cuentas_01_000.txt");
    AVLTree<CuentaSimple, CompararPorFecha> arbol;
    int count = 0;
    lista.recorrer([&arbol, &count](const CuentaSimple& c) { arbol.insert(c); count++; });
    if (count == 0) {
        std::cout << "\n[ADVERTENCIA] No se insertaron nodos. Verifica el archivo cuentas_01_000.txt\n";
    } else {
        std::cout << "\nÁrbol AVL por Fecha de Creación: (" << count << " nodos)\n";
        arbol.printTree();
    }
}

void buscarEnAVLPorCampo() {
    int cursor = 0;
    const char* campos[] = {
        "Nombre",
        "Apellido",
        "Edad",
        "Fecha de Creación",
        "Volver"
    };
    int totalCampos = 5;
    bool salir = false;
    while (!salir) {
        system("cls");
        std::cout << "=== Buscar valor específico en AVL ===\n\n";
        for (int i = 0; i < totalCampos; i++) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << campos[i] << std::endl;
        }
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--;
            if (tecla == 80 && cursor < totalCampos - 1) cursor++;
        } else if (tecla == 13) {
            if (cursor == 4) { salir = true; continue; }
            std::string valor;
            int valorEdad = 0;
            std::cout << "Ingrese el valor a buscar: ";
            if (cursor == 2) {
                std::cin >> valorEdad;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            } else {
                std::getline(std::cin, valor);
            }
            auto lista = leerCuentasDesdeArchivo("cuentas_01_000.txt");
            bool encontrado = false;
            if (cursor == 0) { // Nombre
                AVLTree<CuentaSimple, CompararPorNombre> arbol;
                lista.recorrer([&arbol](const CuentaSimple& c) { arbol.insert(c); });
                CuentaSimple buscado; buscado.nombre = valor;
                auto nodo = arbol.find(buscado);
                if (nodo) {
                    std::cout << "\nEncontrado: " << nodo->data.nombre << " " << nodo->data.apellido << ", Edad: " << nodo->data.edad << ", Fecha: " << nodo->data.fechaCreacion << ", Saldo: " << nodo->data.saldo << std::endl;
                    encontrado = true;
                }
            } else if (cursor == 1) { // Apellido
                AVLTree<CuentaSimple, CompararPorApellido> arbol;
                lista.recorrer([&arbol](const CuentaSimple& c) { arbol.insert(c); });
                CuentaSimple buscado; buscado.apellido = valor;
                auto nodo = arbol.find(buscado);
                if (nodo) {
                    std::cout << "\nEncontrado: " << nodo->data.nombre << " " << nodo->data.apellido << ", Edad: " << nodo->data.edad << ", Fecha: " << nodo->data.fechaCreacion << ", Saldo: " << nodo->data.saldo << std::endl;
                    encontrado = true;
                }
            } else if (cursor == 2) { // Edad
                AVLTree<CuentaSimple, CompararPorEdad> arbol;
                lista.recorrer([&arbol](const CuentaSimple& c) { arbol.insert(c); });
                CuentaSimple buscado; buscado.edad = valorEdad;
                auto nodo = arbol.find(buscado);
                if (nodo) {
                    std::cout << "\nEncontrado: " << nodo->data.nombre << " " << nodo->data.apellido << ", Edad: " << nodo->data.edad << ", Fecha: " << nodo->data.fechaCreacion << ", Saldo: " << nodo->data.saldo << std::endl;
                    encontrado = true;
                }
            } else if (cursor == 3) { // Fecha
                AVLTree<CuentaSimple, CompararPorFecha> arbol;
                lista.recorrer([&arbol](const CuentaSimple& c) { arbol.insert(c); });
                CuentaSimple buscado; buscado.fechaCreacion = valor;
                auto nodo = arbol.find(buscado);
                if (nodo) {
                    std::cout << "\nEncontrado: " << nodo->data.nombre << " " << nodo->data.apellido << ", Edad: " << nodo->data.edad << ", Fecha: " << nodo->data.fechaCreacion << ", Saldo: " << nodo->data.saldo << std::endl;
                    encontrado = true;
                }
            }
            if (!encontrado) {
                std::cout << "\nNo se encontró ningún registro con ese valor.\n";
            }
            std::cout << "Presione cualquier tecla para continuar...";
            _getch();
        }
    }
}

void BusquedaAVL::mostrarMenu() {
    int cursor = 0;
    const char* opciones[] = {
        "Visualizar AVL por Nombre",
        "Visualizar AVL por Apellido",
        "Visualizar AVL por Edad",
        "Visualizar AVL por Fecha de Creación",
        "Buscar valor específico en AVL",
        "Volver al menú principal"
    };
    int conteoOpciones = 6;
    bool salir = false;
    while (!salir) {
        system("cls");
        std::cout << "=== MENÚ AVL DE CUENTAS ===\n\n";
        for (int i = 0; i < conteoOpciones; i++) {
            std::cout << (i == cursor ? "> " : "  ");
            std::cout << opciones[i] << std::endl;
        }
        int tecla = _getch();
        if (tecla == 224) {
            tecla = _getch();
            if (tecla == 72 && cursor > 0) cursor--;
            if (tecla == 80 && cursor < conteoOpciones - 1) cursor++;
        } else if (tecla == 13) {
            system("cls");
            switch (cursor) {
                case 0:
                    construirYGraficarAVLPorNombre();
                    std::cout << "Presione cualquier tecla para continuar...";
                    _getch();
                    break;
                case 1:
                    construirYGraficarAVLPorApellido();
                    std::cout << "Presione cualquier tecla para continuar...";
                    _getch();
                    break;
                case 2:
                    construirYGraficarAVLPorEdad();
                    std::cout << "Presione cualquier tecla para continuar...";
                    _getch();
                    break;
                case 3:
                    construirYGraficarAVLPorFecha();
                    std::cout << "Presione cualquier tecla para continuar...";
                    _getch();
                    break;
                case 4:
                    buscarEnAVLPorCampo();
                    break;
                case 5:
                    salir = true;
                    break;
            }
        }
    }
}

// AVLNode y AVLTree templates
// Insertar, buscar y graficar (solo base, sin balanceo aún)
template <typename T, typename Compare>
AVLNode<T, Compare>* AVLTree<T, Compare>::insertRec(AVLNode<T, Compare>* node, const T& value) {
    if (!node) return new AVLNode<T, Compare>(value);
    if (cmp(value, node->data))
        node->left = insertRec(node->left, value);
    else
        node->right = insertRec(node->right, value);
    // (Balanceo pendiente)
    return node;
}

template <typename T, typename Compare>
AVLNode<T, Compare>* AVLTree<T, Compare>::find(const T& value) {
    AVLNode<T, Compare>* current = root;
    while (current) {
        if (cmp(value, current->data))
            current = current->left;
        else if (cmp(current->data, value))
            current = current->right;
        else
            return current;
    }
    return nullptr;
}

// Función auxiliar para obtener el valor a imprimir en el nodo
std::string obtenerValorNodo(const CuentaSimple& cuenta, int tipo) {
    switch (tipo) {
        case 0: return cuenta.nombre;
        case 1: return cuenta.apellido;
        case 2: return std::to_string(cuenta.edad);
        case 3: return cuenta.fechaCreacion;
        default: return "?";
    }
}

template <typename T, typename Compare>
std::string truncarValor(const std::string& val) {
    if (val.length() > 4) return val.substr(0, 4);
    return val;
}

template <typename T, typename Compare>
int altura(AVLNode<T, Compare>* node) {
    if (!node) return 0;
    int l = altura<T, Compare>(node->left);
    int r = altura<T, Compare>(node->right);
    return 1 + (l > r ? l : r);
}

// Impresión adornada de árbol binario en consola
// Usa líneas y ramas para que el árbol sea compacto y visual

template <typename T, typename Compare>
void printTreeAdornado(AVLNode<T, Compare>* node, std::string prefix = "", bool isLeft = true, int tipoCampo = 0) {
    if (!node) return;
    // Elige el campo a mostrar
    std::string val;
    if (tipoCampo == 0) val = node->data.nombre;
    else if (tipoCampo == 1) val = node->data.apellido;
    else if (tipoCampo == 2) val = std::to_string(node->data.edad);
    else if (tipoCampo == 3) val = node->data.fechaCreacion;
    else val = "?";
    if (val.length() > 6) val = val.substr(0, 6);

    std::cout << prefix;
    std::cout << (isLeft ? "|--" : "`--" );
    std::cout << val << std::endl;

    // Si ambos hijos existen, imprime el izquierdo con '|  ' y el derecho con '   '
    if (node->left || node->right) {
        if (node->left)
            printTreeAdornado<T, Compare>(node->left, prefix + (isLeft ? "|  " : "   "), true, tipoCampo);
        if (node->right)
            printTreeAdornado<T, Compare>(node->right, prefix + (isLeft ? "|  " : "   "), false, tipoCampo);
    }
}

template <typename T, typename Compare>
void AVLTree<T, Compare>::printTree() {
    int tipoCampo = 0;
    if constexpr (std::is_same<Compare, CompararPorNombre>::value) tipoCampo = 0;
    else if constexpr (std::is_same<Compare, CompararPorApellido>::value) tipoCampo = 1;
    else if constexpr (std::is_same<Compare, CompararPorEdad>::value) tipoCampo = 2;
    else if constexpr (std::is_same<Compare, CompararPorFecha>::value) tipoCampo = 3;
    printTreeAdornado<T, Compare>(root, "", false, tipoCampo);
}

// --- ARREGLAR LECTURA DE ARCHIVO ---
ListaDobleCircular<CuentaSimple> leerCuentasDesdeArchivo(const std::string& nombreArchivo) {
    ListaDobleCircular<CuentaSimple> lista;
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        CuentaSimple cuenta;
        std::string saldoStr, edadStr;
        // Leer todos los campos y verificar que existan
        if (!std::getline(ss, cuenta.id, ',')) continue;           // 1. ID cuenta
        std::string cedula; // 2. Cedula (no se usa)
        if (!std::getline(ss, cedula, ',')) continue;
        if (!std::getline(ss, cuenta.nombre, ',')) continue;       // 3. Nombre
        if (!std::getline(ss, cuenta.apellido, ',')) continue;     // 4. Apellido
        if (!std::getline(ss, cuenta.tipo, ',')) continue;         // 5. Tipo
        if (!std::getline(ss, saldoStr, ',')) continue;            // 6. Saldo
        if (!std::getline(ss, edadStr, ',')) continue;             // 7. Edad
        if (!std::getline(ss, cuenta.fechaCreacion, ',')) continue;// 8. Fecha
        try {
            cuenta.saldo = saldoStr.empty() ? 0 : std::stod(saldoStr);
            cuenta.edad = edadStr.empty() ? 0 : std::stoi(edadStr);
        } catch (...) {
            continue; // Saltar líneas con datos numéricos inválidos
        }
        lista.insertar(cuenta);
    }
    return lista;
} 