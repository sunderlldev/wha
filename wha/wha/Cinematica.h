#ifndef Cinematica_h
#define Cinematica_h

#include "MatricesNivel2.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

struct CuadroCinematica {
    std::string fotogramaPrederibujado;
    int duracionMS;

    CuadroCinematica(const std::vector<std::vector<PixelArt>>& matriz, const std::string& txt, int ms, int imgX = 10, int imgY = 2, int textoX = 10, int textoY = 20)
        : duracionMS(ms)
    {
        std::ostringstream oss;

        oss << "\033[" << imgY << ";" << imgX << "H";

        int altoMatriz = (int)matriz.size();
        for (int r = 0; r < altoMatriz; r++) {
            if (r > 0) {
                oss << "\033[" << (imgY + r) << ";" << imgX << "H";
            }
            int anchoMatriz = (int)matriz[r].size();
            for (int col = 0; col < anchoMatriz; col++) {
                const PixelArt& px = matriz[r][col];
                if (px.transparente) {
                    oss << "  ";
                }
                else {
                    oss << "\033[48;2;" << (int)px.r << ";" << (int)px.g << ";" << (int)px.b << "m  \033[0m";
                }
            }
        }

        if (!txt.empty()) {
            int longitud = (int)txt.length();
            int anchoCaja = longitud + 4;

            oss << "\033[" << textoY << ";" << textoX << "H+";
            for (int i = 0; i < anchoCaja; i++) oss << "-";
            oss << "+";

            oss << "\033[" << (textoY + 1) << ";" << textoX << "H|  " << txt << "  |";

            oss << "\033[" << (textoY + 2) << ";" << textoX << "H+";
            for (int i = 0; i < anchoCaja; i++) oss << "-";
            oss << "+";
        }

        fotogramaPrederibujado = oss.str();
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

    void agregarCuadro(const std::vector<std::vector<PixelArt>>& matriz, const std::string& texto, int duracionMS, int imgX = 10, int imgY = 2, int textoX = 10, int textoY = 20) {
        CuadroCinematica* nuevoCuadro = new CuadroCinematica(matriz, texto, duracionMS, imgX, imgY, textoX, textoY);
        secuencia.push_back(nuevoCuadro);
    }

    void reproducir() {
#ifdef _WIN32
        HANDLE hOriginal = GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE hBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL
        );

        if (hBuffer != INVALID_HANDLE_VALUE) {
            SetConsoleActiveScreenBuffer(hBuffer);
            DWORD dwMode = 0;
            GetConsoleMode(hBuffer, &dwMode);
            SetConsoleMode(hBuffer, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }

        for (size_t i = 0; i < secuencia.size(); i++) {
            CuadroCinematica* c = secuencia[i];
            if (c == nullptr) continue;

            DWORD written = 0;
            std::string cuadroFinal = "\033[H" + c->fotogramaPrederibujado;

            if (hBuffer != INVALID_HANDLE_VALUE) {
                WriteConsoleA(hBuffer, cuadroFinal.c_str(), (DWORD)cuadroFinal.length(), &written, NULL);
            }
            else {
                std::cout << cuadroFinal << std::flush;
            }

            Sleep(c->duracionMS);

            if (hBuffer != INVALID_HANDLE_VALUE) {
                COORD coord = { 0, 0 };
                DWORD dummy;
                FillConsoleOutputCharacterA(hBuffer, ' ', 120 * 40, coord, &dummy);
                FillConsoleOutputAttribute(hBuffer, 0x07, 120 * 40, coord, &dummy);
            }
        }

        if (hBuffer != INVALID_HANDLE_VALUE) {
            SetConsoleActiveScreenBuffer(hOriginal);
            CloseHandle(hBuffer);
        }
#else
        std::cout << "\033[2J\033[H" << std::flush;
        for (size_t i = 0; i < secuencia.size(); i++) {
            CuadroCinematica* c = secuencia[i];
            if (c == nullptr) continue;

            std::cout << "\033[H" << c->fotogramaPrederibujado << std::flush;
            usleep(c->duracionMS * 1000);
        }
        std::cout << "\033[2J\033[H" << std::flush;
#endif
    }
};

#endif