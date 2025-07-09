#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <string>

int main() {
    const std::string mensaje = "HOLA MUNDO";
    const int windowWidth = 900;
    const int windowHeight = 300;
    const int fontSize = 96;
    const sf::Color colorAzul = sf::Color(30, 144, 255); // Azul intenso

    // Cargar fuente (usa una fuente TTF que tengas, por ejemplo Arial)
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Si falla, intenta con otra fuente
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Marquesina SFML", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    std::string textoActual = "";
    size_t letraActual = 0;
    sf::Clock clock;
    float delay = 0.25f; // segundos entre letras
    bool pausa = false;
    sf::Clock pausaClock;
    float pausaDuracion = 1.0f; // segundos de pausa al final

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Añadir letras una por una
        if (!pausa && letraActual < mensaje.size() && clock.getElapsedTime().asSeconds() > delay) {
            textoActual += mensaje[letraActual++];
            clock.restart();
        }

        // Cuando termina la frase, iniciar pausa y reiniciar
        if (!pausa && letraActual == mensaje.size()) {
            pausa = true;
            pausaClock.restart();
        }
        if (pausa && pausaClock.getElapsedTime().asSeconds() > pausaDuracion) {
            textoActual = "";
            letraActual = 0;
            pausa = false;
            clock.restart();
        }

        // Preparar el texto
        sf::Text text(textoActual, font, fontSize);
        text.setFillColor(colorAzul);
        // Centrar el texto
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2, bounds.height / 2);
        text.setPosition(windowWidth / 2, windowHeight / 2 + bounds.height / 4);

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }
    return 0;
} 