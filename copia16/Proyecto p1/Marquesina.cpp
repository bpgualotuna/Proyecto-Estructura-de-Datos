// Marquesina.cpp
#include "Marquesina.h"
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

extern std::mutex coutMutex;

std::atomic<bool> Marquesina::paused{false};
std::mutex        Marquesina::pauseMutex;
std::condition_variable Marquesina::pauseCv;

Marquesina::Marquesina(const std::string& msg) : message(msg), running(false) {}

Marquesina::~Marquesina() {
    stop();
}

void Marquesina::start() {
    running = true;
    marqueeThread = std::thread(&Marquesina::run, this);
}

void Marquesina::stop() {  
    running = false;
    resumeAll();
    if (marqueeThread.joinable())
        marqueeThread.join();
}

void Marquesina::run() {
    std::string buffer = message + "   ";
    size_t pos = 0;

    while (running) {
        {
            std::unique_lock<std::mutex> lk(pauseMutex);
            pauseCv.wait(lk, [&]{ return !paused.load() || !running.load(); });
        }

        {
            std::lock_guard<std::mutex> lock(coutMutex);
            std::cout
                << "\033[s"          // guarda cursor
                << "\033[1;1H"       // mueve a línea 1, col 1
                << "\033[2K";        // borra toda la línea

            std::string wrap = buffer.substr(pos) + buffer.substr(0, pos);
            std::cout << wrap.substr(0, 60);

            std::cout
                << "\033[u"          // restaura cursor
                << std::flush;
        }
        pos = (pos + 1) % buffer.length();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    // Al terminar, limpiar la línea superior
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "\033[1;1H\033[2K" << std::flush;
    }
}


void Marquesina::pauseAll() {
    paused.store(true);
}


void Marquesina::resumeAll() {
    paused.store(false);
    pauseCv.notify_all();
}