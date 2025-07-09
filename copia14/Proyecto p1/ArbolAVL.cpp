#include "ArbolAVL.h"
#include <iostream>

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

void ArbolAVL::agregarDistancia(int distancia) {
    raiz = insertar(raiz, distancia);
}

NodoAVLInt* ArbolAVL::insertar(NodoAVLInt* nodo, int distancia) {
    if (nodo == nullptr) {
        return new NodoAVLInt(distancia);
    }
    if (distancia < nodo->distancia) {
        nodo->izquierdo = insertar(nodo->izquierdo, distancia);
    } else if (distancia > nodo->distancia) {
        nodo->derecho = insertar(nodo->derecho, distancia);
    } else {
        return nodo;
    }
    nodo->altura = 1 + std::max(obtenerAltura(nodo->izquierdo), obtenerAltura(nodo->derecho));
    int balance = obtenerFactorBalance(nodo);
    if (balance > 1 && distancia < nodo->izquierdo->distancia) {
        return rotarDerecha(nodo);
    }
    if (balance < -1 && distancia > nodo->derecho->distancia) {
        return rotarIzquierda(nodo);
    }
    if (balance > 1 && distancia > nodo->izquierdo->distancia) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }
    if (balance < -1 && distancia < nodo->derecho->distancia) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

int ArbolAVL::obtenerAltura(NodoAVLInt* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return nodo->altura;
}

int ArbolAVL::obtenerFactorBalance(NodoAVLInt* nodo) {
    if (nodo == nullptr) {
        return 0;
    }
    return obtenerAltura(nodo->izquierdo) - obtenerAltura(nodo->derecho);
}

NodoAVLInt* ArbolAVL::rotarDerecha(NodoAVLInt* y) {
    NodoAVLInt* x = y->izquierdo;
    NodoAVLInt* T2 = x->derecho;
    x->derecho = y;
    y->izquierdo = T2;
    y->altura = std::max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
    x->altura = std::max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
    return x;
}

NodoAVLInt* ArbolAVL::rotarIzquierda(NodoAVLInt* x) {
    NodoAVLInt* y = x->derecho;
    NodoAVLInt* T2 = y->izquierdo;
    y->izquierdo = x;
    x->derecho = T2;
    x->altura = std::max(obtenerAltura(x->izquierdo), obtenerAltura(x->derecho)) + 1;
    y->altura = std::max(obtenerAltura(y->izquierdo), obtenerAltura(y->derecho)) + 1;
    return y;
}

void ArbolAVL::recorridoInorden(NodoAVLInt* nodo) {
    if (nodo != nullptr) {
        recorridoInorden(nodo->izquierdo);
        std::cout << nodo->distancia << " ";
        recorridoInorden(nodo->derecho);
    }
}

void ArbolAVL::mostrarDistancias() {
    recorridoInorden(raiz);
    std::cout << std::endl;
}

void ArbolAVL::mostrarArbol(sf::RenderWindow &ventana, sf::Font &fuente) {
    NodoAVLInt* raiz = obtenerRaiz();
    if (raiz != nullptr) {
        float desplazamientoY = 50;
        dibujarNodo(raiz, ventana, fuente, 400, 50 + desplazamientoY, 150);
    }
}

void ArbolAVL::dibujarNodo(NodoAVLInt* nodo, sf::RenderWindow &ventana, sf::Font &fuente, float x, float y, float offset) {
    if (nodo == nullptr) return;
    if (nodo->izquierdo != nullptr) {
        sf::VertexArray linea(sf::Lines, 2);
        linea[0].position = sf::Vector2f(x, y);
        linea[1].position = sf::Vector2f(x - offset, y + 100);
        linea[0].color = sf::Color::White;
        linea[1].color = sf::Color::White;
        ventana.draw(linea);
    }
    if (nodo->derecho != nullptr) {
        sf::VertexArray linea(sf::Lines, 2);
        linea[0].position = sf::Vector2f(x, y);
        linea[1].position = sf::Vector2f(x + offset, y + 100);
        linea[0].color = sf::Color::White;
        linea[1].color = sf::Color::White;
        ventana.draw(linea);
    }
    sf::CircleShape circulo(30);
    circulo.setFillColor(sf::Color::Black);
    circulo.setPosition(x - circulo.getRadius(), y - circulo.getRadius());
    ventana.draw(circulo);
    sf::Text texto(std::to_string(nodo->distancia), fuente, 20);
    texto.setFillColor(sf::Color::White);
    texto.setPosition(x - texto.getLocalBounds().width / 2, y - texto.getLocalBounds().height / 2);
    ventana.draw(texto);
    if (nodo->izquierdo != nullptr) {
        dibujarNodo(nodo->izquierdo, ventana, fuente, x - offset, y + 100, offset / 2);
    }
    if (nodo->derecho != nullptr) {
        dibujarNodo(nodo->derecho, ventana, fuente, x + offset, y + 100, offset / 2);
    }
}
NodoAVLInt* ArbolAVL::obtenerRaiz() {
    return raiz;
} 

void ArbolAVL::printTree() {
    if (raiz == nullptr) {
        std::cout << "El árbol AVL está vacío" << std::endl;
        return;
    }
    printTree(raiz, "", false);
}

void ArbolAVL::printTree(NodoAVLInt* nodo, std::string prefix, bool isLeft) {
    if (nodo != nullptr) {
        // Imprimir el nodo actual
        std::cout << prefix << (isLeft ? "├── " : "└── ") 
                  << "Distancia: " << nodo->distancia << " (Altura: " << nodo->altura << ")" << std::endl;

        // Imprimir subárboles
        printTree(nodo->izquierdo, prefix + (isLeft ? "│   " : "    "), true);
        printTree(nodo->derecho, prefix + (isLeft ? "│   " : "    "), false);
    }
}

NodoAVLInt* ArbolAVL::buscar(int distancia) {
    std::cout << "Árbol AVL antes de la búsqueda:" << std::endl;
    printTree();
    NodoAVLInt* nodo = raiz;
    while (nodo != nullptr) {
        if (distancia == nodo->distancia) {
            std::cout << "Distancia " << distancia << " encontrada en el árbol." << std::endl;
            return nodo;
        }
        if (distancia < nodo->distancia) {
            nodo = nodo->izquierdo;
        } else {
            nodo = nodo->derecho;
        }
    }
    std::cout << "Distancia " << distancia << " no encontrada en el árbol." << std::endl;
    return nullptr;
}