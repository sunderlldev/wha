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
    int textoX;
    int textoY;
    int imagenX;
    int imagenY;

    CuadroCinematica(const std::vector<std::vector<PixelArt>>& matriz, const std::string& txt, int ms, int txtX = 10, int txtY = 2, int imgX = 10, int imgY = 6)
        : matrizPixel(matriz), texto(txt), duracionMS(ms), textoX(txtX), textoY(txtY), imagenX(imgX), imagenY(imgY) {
    }
};

class Cinematica {
private:
    std::vector<CuadroCinematica*> secuencia;

    void moverCursor(int x, int y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    void imprimirTextoAdaptable(const std::string& texto, int x, int y) {
        if (texto.empty()) return;

        int longitud = (int)texto.length();
        int anchoCaja = longitud + 4;

        moverCursor(x, y);
        std::cout << "+";
        for (int i = 0; i < anchoCaja; i++) std::cout << "-";
        std::cout << "+";

        moverCursor(x, y + 1);
        std::cout << "|  " << texto << "  |";

        moverCursor(x, y + 2);
        std::cout << "+";
        for (int i = 0; i < anchoCaja; i++) std::cout << "-";
        std::cout << "+";
    }

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

    void agregarCuadro(const std::vector<std::vector<PixelArt>>& matriz, const std::string& texto, int duracionMS, int textoX = 10, int textoY = 2, int imagenX = 10, int imagenY = 6) {
        CuadroCinematica* nuevoCuadro = new CuadroCinematica(matriz, texto, duracionMS, textoX, textoY, imagenX, imagenY);
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

            imprimirTextoAdaptable(c->texto, c->textoX, c->textoY);

            int altoMatriz = (int)c->matrizPixel.size();
            for (int r = 0; r < altoMatriz; r++) {
                moverCursor(c->imagenX, c->imagenY + r);
                int anchoMatriz = (int)c->matrizPixel[r].size();

                for (int col = 0; col < anchoMatriz; col++) {
                    const PixelArt& px = c->matrizPixel[r][col];

                    if (px.transparente) {
                        std::cout << "  ";
                    }
                    else {
                        std::cout << "\033[48;2;" << px.r << ";" << px.g << ";" << px.b << "m  \033[0m";
                    }
                }
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