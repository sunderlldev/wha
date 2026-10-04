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

            system("cls");
            std::cout << "\n\n";

            int altoMatriz = (int)c->matrizPixel.size();
            for (int r = 0; r < altoMatriz; r++) {
                std::cout << "         ";
                int anchoMatriz = (int)c->matrizPixel[r].size();

                for (int col = 0; col < anchoMatriz; col++) {
                    const PixelArt& px = c->matrizPixel[r][col];

                    if (px.transparente) {
                        std::cout << "  ";
                    }
                    else {
                        std::cout << "\033[38;2;" << px.r << ";" << px.g << ";" << px.v << "m\xe2\x96\x88\xe2\x96\x88\033[0m";
                    }
                }
                std::cout << "\n";
            }

            if (!c->texto.empty()) {
                std::cout << "\n         +---------------------------------------------------+\n";
                std::cout << "         | " << c->texto << "\n";
                std::cout << "         +---------------------------------------------------+\n";
            }

            std::cout << std::flush;

#ifdef _WIN32
            Sleep(c->duracionMS);
#else
            usleep(c->duracionMS * 1000);
#endif
        }

#ifdef _WIN32
        if (hOut != INVALID_HANDLE_VALUE) {
            SetConsoleMode(hOut, originalMode);
        }
#endif

        system("cls");
    }
};

#endif
