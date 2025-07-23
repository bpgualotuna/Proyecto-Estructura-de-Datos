#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <mutex>
#include <condition_variable>

// Mutex global para sincronizar toda salida por consola
extern std::mutex coutMutex;

/**
 * @brief Controla y muestra un mensaje animado (marquesina) en consola usando hilos.
 */
class Marquesina {
private:
    std::atomic<bool> running;               ///< Indica si la marquesina está activa.
    static std::atomic<bool> paused;         ///< Indica si todas las marquesinas están pausadas.
    static std::mutex pauseMutex;            ///< Mutex para pausar/reanudar.
    static std::condition_variable pauseCv;  ///< Variable de condición para pausar.
    std::thread marqueeThread;               ///< Hilo de ejecución de la marquesina.
    std::string message;                     ///< Mensaje a mostrar.

public:
    /**
     * @brief Constructor. Inicializa con el mensaje a mostrar.
     * @param msg Mensaje de la marquesina.
     */
    Marquesina(const std::string& msg);

    /**
     * @brief Destructor. Detiene la marquesina si está activa.
     */
    ~Marquesina();

    /**
     * @brief Inicia la animación de la marquesina.
     */
    void start();

    /**
     * @brief Detiene la animación de la marquesina.
     */
    void stop();

    /**
     * @brief Función que ejecuta la lógica de la marquesina (para el hilo).
     */
    void run();

    /**
     * @brief Pausa todas las marquesinas activas.
     */
    static void pauseAll();

    /**
     * @brief Reanuda todas las marquesinas pausadas.
     */
    static void resumeAll();
};
