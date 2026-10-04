#ifndef Cinematica_h
#define Cinematica_h

#include "MatricesNivel2.h"
#include <vector>
#include <string>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Estructura de cada fotograma gestionada dinámicamente
struct CuadroCinematica {
    std::vector<std::vector<PixelArt>> matrizPixel;
    std::string texto;
    int duracionMS;

    CuadroCinematica(const std::vector<std::vector<PixelArt>>& matriz, const std::string& txt, int ms)
        : matrizPixel(matriz), texto(txt), duracionMS(ms) {
    }
};

class Cinematica {
private:
    std::vector<CuadroCinematica*> secuencia;

public:
    Cinematica() {}

    ~Cinematica() {
        limpiarMemoria();
    }

    void limpiarMemoria() {
        for (size_t i = 0; i < secuencia.size(); i++) {
            if (secuencia[i] != nullptr) {
                delete secuencia[i];
                secuencia[i] = nullptr;
            }
        }
        secuencia.clear();
    }

    void agregarCuadro(const std::vector<std::vector<PixelArt>>& matriz, const std::string& texto, int duracionMS) {
        CuadroCinematica* nuevoCuadro = new CuadroCinematica(matriz, texto, duracionMS);
        secuencia.push_back(nuevoCuadro);
    }

    void reproducir() {
#ifdef _WIN32
        // Activar soporte de secuencias ANSI ÚNICAMENTE durante la animación
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        DWORD originalMode = 0;
        if (hOut != INVALID_HANDLE_VALUE) {
            GetConsoleMode(hOut, &originalMode);
            dwMode = originalMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
#endif

        for (size_t i = 0; i < secuencia.size(); i++) {
            CuadroCinematica* c = secuencia[i];
            if (c == nullptr) continue;

            // 1. Limpieza de pantalla
            system("cls");
            std::cout << "\n\n";

            // 2. Renderizado de la matriz PixelArt en RGB de 24 bits
            int altoMatriz = (int)c->matrizPixel.size();
            for (int r = 0; r < altoMatriz; r++) {
                std::cout << "         "; // Margen para centrar
                int anchoMatriz = (int)c->matrizPixel[r].size();

                for (int col = 0; col < anchoMatriz; col++) {
                    const PixelArt& px = c->matrizPixel[r][col];

                    if (px.transparente) {
                        std::cout << "  "; // Dos espacios para mantener proporción cuadrada
                    }
                    else {
                        // Dos bloques (char)219 seguidos para formar un pixel 1:1
                        std::cout << "\033[38;2;" << px.r << ";" << px.g << ";" << px.v << "m"
                            << (char)219 << (char)219
                            << "\033[0m";
                    }
                }
                std::cout << "\n";
            }

            // 3. Subtítulo o cuadro de texto inferior
            if (!c->texto.empty()) {
                std::cout << "\n         +---------------------------------------------------+\n";
                std::cout << "         | " << c->texto << "\n";
                std::cout << "         +---------------------------------------------------+\n";
            }

            std::cout << std::flush;

            // 4. Pausa por fotograma
#ifdef _WIN32
            Sleep(c->duracionMS);
#else
            usleep(c->duracionMS * 1000);
#endif
        }

#ifdef _WIN32
        // Restaurar la configuración previa de la consola al finalizar
        if (hOut != INVALID_HANDLE_VALUE) {
            SetConsoleMode(hOut, originalMode);
        }
#endif

        system("cls");
    }
};

#endif