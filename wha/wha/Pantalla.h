#ifndef Pantalla_h
#define Pantalla_h

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <clocale>
#include "Animacion.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BRIGHT_BLACK "\033[90m"
#define BRIGHT_RED "\033[91m"
#define BRIGHT_GREEN "\033[92m"
#define BRIGHT_YELLOW "\033[93m"
#define BRIGHT_BLUE "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN "\033[96m"
#define BRIGHT_WHITE "\033[97m"
#define ORANGE "\033[38;5;208m"
#define DARK_GREEN "\033[38;5;28m"
#define BROWN "\033[38;5;130m"

class Pantalla {
private:
    int anchoTotal;
    int altoTotal;
    int anchoJuego;
    int anchoPanel;
    std::vector<std::wstring> buffer;
    std::vector<std::vector<int>> bufferColor;
    std::string ultimoDialogoHablante;
    std::string ultimoDialogoTexto;
    std::string ultimoPromptTexto;

    int longitudVisible(const std::string& str) const {
        int len = 0;
        for (size_t i = 0; i < str.length(); i++) {
            if (((unsigned char)str[i] & 0xC0) != 0x80) {
                len++;
            }
        }
        return len;
    }

    std::vector<wchar_t> aWideString(const std::string& str) const {
        std::vector<wchar_t> res;
        size_t i = 0;
        while (i < str.length()) {
            unsigned char c0 = (unsigned char)str[i];
            if (c0 == 219) {
                res.push_back(L'█');
                i++;
            } else if ((c0 & 0x80) == 0) {
                res.push_back((wchar_t)c0);
                i++;
            } else if ((c0 & 0xE0) == 0xC0 && i + 1 < str.length()) {
                wchar_t wc = (wchar_t)(((c0 & 0x1F) << 6) | ((unsigned char)str[i + 1] & 0x3F));
                res.push_back(wc);
                i += 2;
            } else if ((c0 & 0xF0) == 0xE0 && i + 2 < str.length()) {
                wchar_t wc = (wchar_t)(((c0 & 0x0F) << 12) | (((unsigned char)str[i + 1] & 0x3F) << 6) | ((unsigned char)str[i + 2] & 0x3F));
                res.push_back(wc);
                i += 3;
            } else {
                res.push_back((wchar_t)c0);
                i++;
            }
        }
        return res;
    }

    std::wstring encuadrarFilaPanelW(const std::string& texto, int ancho) const {
        std::vector<wchar_t> wchars = aWideString(" " + texto);
        std::wstring res;
        for (size_t i = 0; i < wchars.size() && (int)res.length() < ancho - 1; i++) {
            res += wchars[i];
        }
        while ((int)res.length() < ancho - 1) {
            res += L' ';
        }
        res += L'|';
        return res;
    }

    const wchar_t* obtenerCodigoColorW(int color) const {
        switch (color) {
            case 1: return L"\033[97m";
            case 2: return L"\033[92m";
            case 3: return L"\033[96m";
            case 4: return L"\033[93m";
            case 5: return L"\033[95m";
            case 6: return L"\033[94m";
            case 7: return L"\033[91m";
            case 8: return L"\033[90m";
            case 9: return L"\033[32m";
            case 10: return L"\033[38;5;208m";
            case 11: return L"\033[38;5;28m";
            case 12: return L"\033[38;5;130m";
            case 13: return L"\033[34m";
            default: return L"\033[0m";
        }
    }

    const char* obtenerCodigoColor(int color) const {
        switch (color) {
            case 1: return BRIGHT_WHITE;
            case 2: return BRIGHT_GREEN;
            case 3: return BRIGHT_CYAN;
            case 4: return BRIGHT_YELLOW;
            case 5: return BRIGHT_MAGENTA;
            case 6: return BRIGHT_BLUE;
            case 7: return BRIGHT_RED;
            case 8: return BRIGHT_BLACK;
            case 9: return GREEN;
            case 10: return ORANGE;
            case 11: return DARK_GREEN;
            case 12: return BROWN;
            case 13: return BLUE;
            default: return RESET;
        }
    }

public:
    Pantalla() : anchoTotal(120), altoTotal(40), anchoJuego(84), anchoPanel(35),
                 ultimoDialogoHablante(""), ultimoDialogoTexto(""), ultimoPromptTexto("") {
        buffer = std::vector<std::wstring>(altoTotal, std::wstring(anchoTotal, L' '));
        bufferColor = std::vector<std::vector<int>>(altoTotal, std::vector<int>(anchoTotal, 0));
    }

    ~Pantalla() {}

    void resetDialogoAnimado() {
        ultimoDialogoHablante = "";
        ultimoDialogoTexto = "";
    }

    void resetPromptAnimado() {
        ultimoPromptTexto = "";
    }

    int getAltoTotal() const { return altoTotal; }
    int getAnchoJuego() const { return anchoJuego; }

    void configurarConsola() {
        setlocale(LC_ALL, "");
#ifdef _WIN32
        system("mode con: cols=120 lines=40");
        system("title Witch Hat Atelier - Árbol de Plata");
        system("cls");
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            CONSOLE_CURSOR_INFO cursorInfo;
            if (GetConsoleCursorInfo(hOut, &cursorInfo)) {
                cursorInfo.bVisible = FALSE;
                SetConsoleCursorInfo(hOut, &cursorInfo);
            }
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= 0x0004;
                SetConsoleMode(hOut, dwMode);
            }
        }
#else
        std::cout << "\033[?25l";
        std::cout << "\033[8;40;120t";
        std::cout << "\033[2J\033[H";
#endif
    }

    void limpiarBuffer() {
        for (int f = 0; f < altoTotal; f++) {
            buffer[f] = std::wstring(anchoTotal, L' ');
            buffer[f][anchoJuego] = L'|';
            for (int c = 0; c < anchoTotal; c++) {
                if (c == anchoJuego) {
                    bufferColor[f][c] = 8;
                } else if (c < anchoJuego) {
                    int r = std::abs(c * 7 + f * 13) % 3;
                    buffer[f][c] = (r == 0) ? L'"' : ((r == 1) ? L'\'' : L',');
                    bufferColor[f][c] = 11;
                } else {
                    bufferColor[f][c] = 0;
                }
            }
        }
    }

    void setPixelJuego(int x, int y, char c, int color = 0) {
        if (x >= 0 && x < anchoJuego && y >= 0 && y < altoTotal) {
            buffer[y][x] = ((unsigned char)c == 219) ? L'█' : (wchar_t)(unsigned char)c;
            bufferColor[y][x] = color;
        }
    }

    void setTextoJuego(int x, int y, const std::string& texto, int color = 0) {
        if (y < 0 || y >= altoTotal) return;
        std::vector<wchar_t> wchars = aWideString(texto);
        for (size_t i = 0; i < wchars.size(); i++) {
            int posX = x + (int)i;
            if (posX >= 0 && posX < anchoJuego) {
                buffer[y][posX] = wchars[i];
                bufferColor[y][posX] = color;
            }
        }
    }

    void dibujarCaja(int x, int y, int ancho, int alto, int colorBorde = 8) {
        if (x < 0 || y < 0 || x + ancho > anchoJuego || y + alto > altoTotal) return;
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                if (r == 0 || r == alto - 1) {
                    if (c == 0 || c == ancho - 1) {
                        buffer[y + r][x + c] = L'+';
                    } else {
                        buffer[y + r][x + c] = L'-';
                    }
                    bufferColor[y + r][x + c] = colorBorde;
                } else if (c == 0 || c == ancho - 1) {
                    buffer[y + r][x + c] = L'|';
                    bufferColor[y + r][x + c] = colorBorde;
                } else {
                    buffer[y + r][x + c] = L' ';
                    bufferColor[y + r][x + c] = 0;
                }
            }
        }
    }

    void dibujarPromptFlotante(const std::string& texto) {
        int anchoCaja = longitudVisible(texto) + 4;
        if (anchoCaja > anchoJuego - 4) anchoCaja = anchoJuego - 4;
        int startX = (anchoJuego - anchoCaja) / 2;
        int startY = altoTotal - 4;
        dibujarCaja(startX, startY, anchoCaja, 3, 4);

        if (texto != ultimoPromptTexto && (texto.find("Coco:") != std::string::npos || texto.find("Letrero:") != std::string::npos)) {
            ultimoPromptTexto = texto;
            std::vector<wchar_t> wchars = aWideString(texto);
            for (size_t i = 0; i < wchars.size(); i++) {
                if (startX + 2 + (int)i < anchoJuego - 2) {
                    buffer[startY + 1][startX + 2 + (int)i] = wchars[i];
                    bufferColor[startY + 1][startX + 2 + (int)i] = 1;
                }
                dibujar();
#ifdef _WIN32
                if (wchars[i] != L' ' && i % 3 == 0) {
                    int freq = (texto.find("Coco:") != std::string::npos) ? 720 : 500;
                    Beep(freq, 10);
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(12));
                }
#else
                std::this_thread::sleep_for(std::chrono::milliseconds(12));
#endif
            }
        } else {
            setTextoJuego(startX + 2, startY + 1, texto, 1);
            if (texto.find("Coco:") == std::string::npos && texto.find("Letrero:") == std::string::npos) {
                ultimoPromptTexto = texto;
            }
        }
    }

    void animarTexto(const std::string& texto, int velocidadMs = 25) {
        Animacion::animarTexto(texto, velocidadMs);
    }

    void animarDialogo(const std::string& hablante, const std::string& texto, int velocidadMs = 25) {
        Animacion::animarDialogo(hablante, texto, velocidadMs);
    }

    void dibujarCuadroDialogo(const std::string& hablante, const std::string& rol, int confianza,
                              const std::vector<std::string>& lineasTexto,
                              const std::vector<std::string>& opciones) {
        int x = 2;
        int y = 21;
        int ancho = 80;
        int alto = 18;
        dibujarCaja(x, y, ancho, alto, 8);

        std::string encabezado = " " + hablante;
        if (!rol.empty()) encabezado += " - " + rol;
        setTextoJuego(x + 2, y + 1, encabezado, 4);

        std::string confStr = "Confianza: ";
        if (confianza <= 0) confStr += "Sin confianza";
        else if (confianza == 1) confStr += "Neutral";
        else confStr += "Amigos";
        setTextoJuego(x + ancho - longitudVisible(confStr) - 2, y + 1, confStr, 3);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 2][x + c] = L'-';
            bufferColor[y + 2][x + c] = 8;
        }

        std::vector<std::string> lineasProcesadas;
        for (size_t i = 0; i < lineasTexto.size(); i++) {
            std::string actual = "";
            for (size_t j = 0; j < lineasTexto[i].length(); j++) {
                if (lineasTexto[i][j] == '\n') {
                    lineasProcesadas.push_back(actual);
                    actual = "";
                } else {
                    actual += lineasTexto[i][j];
                }
            }
            lineasProcesadas.push_back(actual);
        }

        std::string textoCompleto = "";
        for (size_t i = 0; i < lineasProcesadas.size(); i++) {
            textoCompleto += lineasProcesadas[i] + "\n";
        }

        bool esNuevo = (hablante != ultimoDialogoHablante || textoCompleto != ultimoDialogoTexto);

        if (esNuevo) {
            ultimoDialogoHablante = hablante;
            ultimoDialogoTexto = textoCompleto;

            for (int r = y + 3; r <= y + 8; r++) {
                for (int c = 1; c < ancho - 1; c++) {
                    buffer[r][x + c] = L' ';
                    bufferColor[r][x + c] = 0;
                }
            }

#ifdef _WIN32
            int freqHablante = 480;
            if (hablante == "Coco") freqHablante = 720;
            else if (hablante == "Qifrey") freqHablante = 280;
            else if (hablante == "Agott") freqHablante = 540;
            else if (hablante == "Richeh") freqHablante = 380;
#endif

            int filaActual = y + 3;
            for (size_t i = 0; i < lineasProcesadas.size() && filaActual < y + 9; i++) {
                std::vector<wchar_t> wchars = aWideString(lineasProcesadas[i]);
                for (size_t c = 0; c < wchars.size(); c++) {
                    if (x + 3 + (int)c < x + ancho - 3) {
                        buffer[filaActual][x + 3 + (int)c] = wchars[c];
                        bufferColor[filaActual][x + 3 + (int)c] = 1;
                    }
                    dibujar();
#ifdef _WIN32
                    if (wchars[c] != L' ' && c % 3 == 0) {
                        Beep(freqHablante, 10);
                    } else {
                        std::this_thread::sleep_for(std::chrono::milliseconds(12));
                    }
#else
                    std::this_thread::sleep_for(std::chrono::milliseconds(12));
#endif
                }
                filaActual++;
            }
        } else {
            int filaActual = y + 3;
            for (size_t i = 0; i < lineasProcesadas.size() && filaActual < y + 9; i++) {
                setTextoJuego(x + 3, filaActual, lineasProcesadas[i], 1);
                filaActual++;
            }
        }

        int filaDivisoria = y + 9;
        for (int c = 1; c < ancho - 1; c++) {
            buffer[filaDivisoria][x + c] = L'-';
            bufferColor[filaDivisoria][x + c] = 8;
        }

        int filaOpciones = y + 10;
        for (size_t i = 0; i < opciones.size() && filaOpciones < y + alto - 2; i++) {
            setTextoJuego(x + 3, filaOpciones, opciones[i], 3);
            filaOpciones++;
        }

        if (opciones.empty()) {
            setTextoJuego(x + 3, y + alto - 2, "Pulsa ESC o ENTER para continuar", 4);
        } else if (opciones.size() == 1) {
            setTextoJuego(x + 3, y + alto - 2, "Elige una opción [1] o pulsa ESC para salir", 4);
        } else {
            setTextoJuego(x + 3, y + alto - 2, "Elige una opción [1-" + std::to_string(opciones.size()) + "] o pulsa ESC para salir", 4);
        }
        if (esNuevo) {
            dibujar();
        }
    }

    void dibujarModalPersonajes(int seleccionado, const std::vector<std::string>& nombres,
                                const std::vector<std::string>& roles,
                                const std::vector<std::string>& descripciones,
                                const std::vector<int>& confianzas,
                                const std::vector<bool>& desbloqueados) {
        int x = 4;
        int y = 4;
        int ancho = 76;
        int alto = 32;
        dibujarCaja(x, y, ancho, alto);
        setTextoJuego(x + 24, y + 1, "=== GUÍA DE PERSONAJES ===");
        for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = L'-';

        int fila = y + 3;
        for (size_t i = 0; i < nombres.size() && fila < y + 15; i++) {
            std::string prefijo = ((int)i == seleccionado) ? "-> " : "   ";
            std::string entrada = prefijo;
            if (desbloqueados[i]) {
                entrada += "[" + std::to_string(i + 1) + "] " + nombres[i] + " (" + roles[i] + ")";
                std::string textoConf = "Sin confianza";
                if (confianzas[i] == 1) textoConf = "Neutral";
                else if (confianzas[i] >= 2) textoConf = "Amigos";
                entrada += " [" + textoConf + "]";
            } else {
                entrada += "[" + std::to_string(i + 1) + "] [ ??? ] (Bloqueado)";
            }
            setTextoJuego(x + 3, fila, entrada);
            fila += 2;
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + 16][x + c] = L'-';
        setTextoJuego(x + 3, y + 17, "DETALLES:");

        if (seleccionado >= 0 && seleccionado < (int)desbloqueados.size()) {
            if (desbloqueados[seleccionado]) {
                setTextoJuego(x + 5, y + 19, "Nombre: " + nombres[seleccionado]);
                setTextoJuego(x + 5, y + 20, "Rol:    " + roles[seleccionado]);
                std::string nivelConf = "Sin confianza";
                if (confianzas[seleccionado] == 1) nivelConf = "Neutral";
                else if (confianzas[seleccionado] >= 2) nivelConf = "Amigos";
                setTextoJuego(x + 5, y + 21, "Nivel de confianza: " + nivelConf);
                setTextoJuego(x + 5, y + 23, "Descripción:");
                setTextoJuego(x + 5, y + 24, descripciones[seleccionado]);
            } else {
                setTextoJuego(x + 5, y + 19, "Personaje aún no descubierto en este nivel.");
                setTextoJuego(x + 5, y + 20, "Explora el atelier o avanza en la historia.");
            }
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = L'-';
        setTextoJuego(x + 5, y + alto - 2, "[W/S] Navegar   [P/ESC] Cerrar panel");
    }

    void dibujarModalInventario(int seleccionado, const std::vector<std::string>& nombres,
                                const std::vector<std::string>& tipos,
                                const std::vector<std::string>& descripciones) {
        int x = 6;
        int y = 4;
        int ancho = 72;
        int alto = 32;
        dibujarCaja(x, y, ancho, alto);
        setTextoJuego(x + 22, y + 1, "=== MOCHILA / INVENTARIO ===");
        for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = L'-';

        int fila = y + 3;
        for (int i = 0; i < 6; i++) {
            std::string prefijo = (i == seleccionado) ? "-> " : "   ";
            std::string entrada = prefijo + "[" + std::to_string(i + 1) + "] ";
            if (i < (int)nombres.size() && !nombres[i].empty()) {
                entrada += nombres[i];
                if (i < (int)tipos.size() && !tipos[i].empty()) {
                    entrada += " (" + tipos[i] + ")";
                }
            } else {
                entrada += "(Ranura vacía)";
            }
            setTextoJuego(x + 4, fila, entrada);
            fila += 1;
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + 10][x + c] = L'-';
        setTextoJuego(x + 4, y + 11, "[DETALLES DEL ÍTEM]");

        if (seleccionado >= 0 && seleccionado < (int)nombres.size() && !nombres[seleccionado].empty()) {
            setTextoJuego(x + 5, y + 13, "Nombre:      " + nombres[seleccionado]);
            if (seleccionado < (int)tipos.size()) {
                setTextoJuego(x + 5, y + 15, "Tipo:        " + tipos[seleccionado]);
            }
            if (seleccionado < (int)descripciones.size()) {
                setTextoJuego(x + 5, y + 17, "Descripción: ");
                setTextoJuego(x + 5, y + 19, descripciones[seleccionado]);
            }
        } else {
            setTextoJuego(x + 5, y + 13, "Ranura de mochila sin ítem asignado.");
            setTextoJuego(x + 5, y + 15, "Capacidad máxima: 6 ítems en este nivel.");
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = L'-';
        setTextoJuego(x + 5, y + alto - 2, "[W/S] Navegar   [1-6] Elegir   [I/ESC] Cerrar");
    }

    void dibujarModalMisiones(int seleccionado, bool verDetalle,
                              const std::vector<std::string>& titulos,
                              const std::vector<std::string>& descripciones,
                              const std::vector<std::string>& estados,
                              const std::vector<bool>& desbloqueadas) {
        int x = 6;
        int y = 5;
        int ancho = 72;
        int alto = 30;
        dibujarCaja(x, y, ancho, alto);

        if (!verDetalle) {
            setTextoJuego(x + 25, y + 1, "=== MISIONES ===");
            for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = L'-';

            int fila = y + 4;
            for (size_t i = 0; i < titulos.size() && fila < y + 22; i++) {
                std::string prefijo = ((int)i == seleccionado) ? "-> " : "   ";
                std::string entrada = prefijo + "[" + std::to_string(i + 1) + "] ";
                if (desbloqueadas[i]) {
                    entrada += titulos[i];
                    std::string est = " [" + estados[i] + "]";
                    int espacioRestante = ancho - longitudVisible(entrada) - longitudVisible(est) - 5;
                    if (espacioRestante > 0) entrada += std::string(espacioRestante, ' ');
                    entrada += est;
                } else {
                    entrada += "########";
                    std::string est = " [BLOQUEADA]";
                    int espacioRestante = ancho - longitudVisible(entrada) - longitudVisible(est) - 5;
                    if (espacioRestante > 0) entrada += std::string(espacioRestante, ' ');
                    entrada += est;
                }
                setTextoJuego(x + 4, fila, entrada);
                fila += 2;
            }

            for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = L'-';
            setTextoJuego(x + 4, y + alto - 2, "[W/S] Navegar   [ENTER] Ver detalle   [M/ESC] Cerrar");
        } else {
            std::string tituloDetalle = "=== MISIÓN " + std::to_string(seleccionado + 1) + ": ";
            if (desbloqueadas[seleccionado]) {
                tituloDetalle += titulos[seleccionado] + " ===";
            } else {
                tituloDetalle += "######## ===";
            }
            if (longitudVisible(tituloDetalle) > ancho - 4) {
                tituloDetalle = tituloDetalle.substr(0, ancho - 7) + "...";
            }
            setTextoJuego(x + 4, y + 1, tituloDetalle);
            for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = L'-';

            if (desbloqueadas[seleccionado]) {
                setTextoJuego(x + 4, y + 5, "Misión: " + titulos[seleccionado]);
                setTextoJuego(x + 4, y + 7, "Estado: " + estados[seleccionado]);
                for (int c = 4; c < ancho - 4; c++) buffer[y + 9][x + c] = L'-';
                setTextoJuego(x + 4, y + 11, "Descripción:");
                std::string desc = descripciones[seleccionado];
                std::vector<std::string> lineasDesc;
                std::string lineaActual = "";
                for (size_t i = 0; i < desc.length(); i++) {
                    if (desc[i] == '\n') {
                        lineasDesc.push_back(lineaActual);
                        lineaActual = "";
                    } else {
                        lineaActual += desc[i];
                        if (longitudVisible(lineaActual) >= ancho - 10) {
                            lineasDesc.push_back(lineaActual);
                            lineaActual = "";
                        }
                    }
                }
                if (!lineaActual.empty()) {
                    lineasDesc.push_back(lineaActual);
                }
                int filaDesc = y + 13;
                for (size_t i = 0; i < lineasDesc.size() && filaDesc < y + alto - 4; i++) {
                    setTextoJuego(x + 4, filaDesc, lineasDesc[i]);
                    filaDesc += 2;
                }
            } else {
                setTextoJuego(x + 4, y + 5, "Misión Bloqueada");
                setTextoJuego(x + 4, y + 7, "Estado: BLOQUEADA");
                for (int c = 4; c < ancho - 4; c++) buffer[y + 9][x + c] = L'-';
                setTextoJuego(x + 4, y + 11, "Descripción:");
                setTextoJuego(x + 4, y + 13, "Esta misión aún no ha sido desbloqueada.");
                setTextoJuego(x + 4, y + 15, "Completa los objetivos previos para acceder.");
            }

            for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = L'-';
            setTextoJuego(x + 4, y + alto - 2, "[ENTER / ESC] Volver a la lista de misiones");
        }
    }

    void dibujarEstadisticasFinNivel(int nivel, const std::string& nombreNivel,
                                     const std::string& prota, int segs,
                                     int bonoTiempo, int puntosMisiones,
                                     int confianzaQifrey) {
        int x = 4;
        int y = 3;
        int ancho = 76;
        int alto = 34;
        dibujarCaja(x, y, ancho, alto);

        setTextoJuego(x + 18, y + 1, "=== ESTADÍSTICAS DE FIN DE NIVEL ===");
        for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = L'-';

        setTextoJuego(x + 4, y + 4, "Nivel superado:   NIVEL " + std::to_string(nivel) + " - " + nombreNivel);
        setTextoJuego(x + 4, y + 5, "Maga:             " + prota + " (Aprendiz)");
        setTextoJuego(x + 4, y + 6, "Objetivo:         Capa Mágica Crafteada con Éxito");

        for (int c = 1; c < ancho - 1; c++) buffer[y + 8][x + c] = L'-';
        setTextoJuego(x + 4, y + 9, "[DESGLOSE DE PUNTUACIÓN SEGÚN TIEMPO Y MISIONES]");

        int min = segs / 60;
        int seg = segs % 60;
        std::string tStr = (min < 10 ? "0" : "") + std::to_string(min) + ":" + (seg < 10 ? "0" : "") + std::to_string(seg);

        setTextoJuego(x + 4, y + 11, "Tiempo empleado:                      " + tStr);
        setTextoJuego(x + 4, y + 13, "Puntos por velocidad de tiempo:       +" + std::to_string(bonoTiempo) + " pts");
        setTextoJuego(x + 4, y + 15, "Puntos por misiones y recolección:    +" + std::to_string(puntosMisiones) + " pts");
        for (int c = 4; c < ancho - 4; c++) buffer[y + 17][x + c] = L'-';

        int total = bonoTiempo + puntosMisiones;
        setTextoJuego(x + 4, y + 19, "PUNTAJE TOTAL DEL NIVEL:              " + std::to_string(total) + " PTS");

        for (int c = 1; c < ancho - 1; c++) buffer[y + 21][x + c] = L'-';
        setTextoJuego(x + 4, y + 22, "[LOGROS Y ESTADO DE LORE]");
        std::string confStr = "Sin confianza";
        if (confianzaQifrey == 1) confStr = "Neutral";
        else if (confianzaQifrey >= 2) confStr = "Amigos";
        setTextoJuego(x + 4, y + 24, "Vínculo con Maestro Qifrey:           " + confStr);
        setTextoJuego(x + 4, y + 25, "Objeto legendario desbloqueado:       Capa Mágica de Vuelo");
        setTextoJuego(x + 4, y + 26, "Habilidad de vuelo:                   Activada para Coco");

        for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 4][x + c] = L'-';
        setTextoJuego(x + 4, y + alto - 3, "[1 / ENTER] Avanzar al Nivel 2    [C] Seguir explorando");
        setTextoJuego(x + 4, y + alto - 2, "[ESC] Salir del juego");
    }

    void renderizarPanelLateral(int nivel, const std::string& nombreNivel,
                               const std::string& protagonista, int vida, int vidaMax) {
        std::vector<std::wstring> lineasPanel(altoTotal, std::wstring(anchoPanel, L' '));

        std::wstring separador = std::wstring(anchoPanel - 1, L'-') + L"|";
        std::wstring bordeCaja = std::wstring(anchoPanel - 1, L'-') + L"+";

        lineasPanel[0]  = bordeCaja;
        lineasPanel[1]  = encuadrarFilaPanelW("     WITCH HAT ATELIER", anchoPanel);
        lineasPanel[2]  = encuadrarFilaPanelW("      ÁRBOL DE PLATA", anchoPanel);
        lineasPanel[3]  = separador;
        lineasPanel[4]  = encuadrarFilaPanelW("NIVEL " + std::to_string(nivel) + ": " + nombreNivel, anchoPanel);
        lineasPanel[5]  = encuadrarFilaPanelW("MAGA: " + protagonista + " (Aprendiz)", anchoPanel);

        std::string corazones = "";
        for (int i = 0; i < vidaMax; i++) {
            if (i < vida) corazones += "<3 ";
            else corazones += ".. ";
        }
        lineasPanel[6]  = encuadrarFilaPanelW("VIDA: " + corazones, anchoPanel);
        lineasPanel[7]  = separador;
        lineasPanel[8]  = encuadrarFilaPanelW("[CONTROLES]", anchoPanel);
        lineasPanel[9]  = separador;
        lineasPanel[10] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[11] = encuadrarFilaPanelW("[W, A, S, D]  Moverse en mapa", anchoPanel);
        lineasPanel[12] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[13] = encuadrarFilaPanelW("[ENTER]       Interactuar", anchoPanel);
        lineasPanel[14] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[15] = encuadrarFilaPanelW("[M]           Misiones", anchoPanel);
        lineasPanel[16] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[17] = encuadrarFilaPanelW("[I]           Inventario", anchoPanel);
        lineasPanel[18] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[19] = encuadrarFilaPanelW("[P]           Personajes", anchoPanel);
        lineasPanel[20] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[21] = encuadrarFilaPanelW("[ESC]         Salir / Pausa", anchoPanel);
        lineasPanel[22] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[23] = separador;

        for (int f = 24; f < altoTotal - 1; f++) {
            lineasPanel[f] = encuadrarFilaPanelW("", anchoPanel);
        }
        lineasPanel[altoTotal - 1] = bordeCaja;

        int colInicioPanel = anchoJuego + 1;
        for (int f = 0; f < altoTotal; f++) {
            for (int c = 0; c < anchoPanel; c++) {
                buffer[f][colInicioPanel + c] = lineasPanel[f][c];
                if (f == 0 || f == 3 || f == 7 || f == 9 || f == 23 || f == altoTotal - 1 || c == anchoPanel - 1) {
                    bufferColor[f][colInicioPanel + c] = 8;
                } else if (f == 1) {
                    bufferColor[f][colInicioPanel + c] = 3;
                } else if (f == 2) {
                    bufferColor[f][colInicioPanel + c] = 4;
                } else if (f == 6) {
                    bufferColor[f][colInicioPanel + c] = 7;
                } else if (f == 8) {
                    bufferColor[f][colInicioPanel + c] = 4;
                } else {
                    bufferColor[f][colInicioPanel + c] = 1;
                }
            }
        }
    }

    void dibujarPantallaMensajeCentrado(const std::string& texto) {
        limpiarBuffer();
        int fila = altoTotal / 2;
        int col = (anchoTotal - longitudVisible(texto)) / 2;
        if (col < 0) col = 0;
        if (fila >= 0 && fila < altoTotal) {
            setTextoPantallaCompleta(col, fila, texto, 4);
        }
    }

    void limpiarBufferCompleto() {
        for (int f = 0; f < altoTotal; f++) {
            buffer[f] = std::wstring(anchoTotal, L' ');
            for (int c = 0; c < anchoTotal; c++) {
                bufferColor[f][c] = 0;
            }
        }
    }

    void setTextoPantallaCompleta(int x, int y, const std::string& texto, int color = 0) {
        if (y < 0 || y >= altoTotal) return;
        std::vector<wchar_t> wchars = aWideString(texto);
        for (size_t i = 0; i < wchars.size(); i++) {
            int posX = x + (int)i;
            if (posX >= 0 && posX < anchoTotal) {
                buffer[y][posX] = wchars[i];
                bufferColor[y][posX] = color;
            }
        }
    }

    void dibujarCajaPantallaCompleta(int x, int y, int ancho, int alto, int colorBorde = 8) {
        if (x < 0 || y < 0 || x + ancho > anchoTotal || y + alto > altoTotal) return;
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                if (r == 0 || r == alto - 1) {
                    if (c == 0 || c == ancho - 1) {
                        buffer[y + r][x + c] = L'+';
                    } else {
                        buffer[y + r][x + c] = L'-';
                    }
                    bufferColor[y + r][x + c] = colorBorde;
                } else if (c == 0 || c == ancho - 1) {
                    buffer[y + r][x + c] = L'|';
                    bufferColor[y + r][x + c] = colorBorde;
                } else {
                    buffer[y + r][x + c] = L' ';
                    bufferColor[y + r][x + c] = 0;
                }
            }
        }
    }

    void mostrarHistoriaIntro() {
        limpiarBufferCompleto();
        int x = 7;
        int y = 2;
        int ancho = 106;
        int alto = 36;
        dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

        std::string tit1 = "W I T C H   H A T   A T E L I E R";
        std::string tit2 = "E L   Á R B O L   D E   P L A T A";
        int cx1 = x + (ancho - longitudVisible(tit1)) / 2;
        int cx2 = x + (ancho - longitudVisible(tit2)) / 2;
        setTextoPantallaCompleta(cx1, y + 2, tit1, 4);
        setTextoPantallaCompleta(cx2, y + 3, tit2, 3);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 5][x + c] = L'=';
            bufferColor[y + 5][x + c] = 8;
        }

        std::string sub1 = "=== PRÓLOGO: EL SECRETO DE LA MAGIA ===";
        int cxSub1 = x + (ancho - longitudVisible(sub1)) / 2;
        setTextoPantallaCompleta(cxSub1, y + 7, sub1, 4);

        setTextoPantallaCompleta(x + 5, y + 9,  "En un mundo donde la hechicería parece un don reservado para unos pocos elegidos,", 1);
        setTextoPantallaCompleta(x + 5, y + 10, "la verdad prohibida es que cualquier ser humano es capaz de hacer magia: solo se", 1);
        setTextoPantallaCompleta(x + 5, y + 11, "necesita tinta mágica y trazar con suma precisión los sellos y círculos arcanos.", 1);

        setTextoPantallaCompleta(x + 5, y + 13, "Coco, una humilde joven fascinada por los magos, recibió un día un libro prohibido", 1);
        setTextoPantallaCompleta(x + 5, y + 14, "de un misterioso hechicero con sombrero de ala ancha. Al intentar recrear los trazos", 1);
        setTextoPantallaCompleta(x + 5, y + 15, "a escondidas en su habitación, desató un hechizo oscuro que petrificó a su madre.", 1);

        setTextoPantallaCompleta(x + 5, y + 17, "Rescatada por el hechicero Qifrey, Coco fue acogida en su atelier como aprendiz.", 1);
        setTextoPantallaCompleta(x + 5, y + 18, "Para descubrir el contrahechizo capaz de salvar a su madre, Coco deberá dominar", 1);
        setTextoPantallaCompleta(x + 5, y + 19, "el arte del dibujo mágico y superar las rigurosas pruebas de los hechiceros.", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 21][x + c] = L'-';
            bufferColor[y + 21][x + c] = 8;
        }

        std::string sub2 = "=== OBJETIVOS DEL NIVEL 1: EL ATELIER DE QIFREY ===";
        int cxSub2 = x + (ancho - longitudVisible(sub2)) / 2;
        setTextoPantallaCompleta(cxSub2, y + 23, sub2, 2);

        setTextoPantallaCompleta(x + 5, y + 25, "* Explora el atelier, la choza de trazos y la misteriosa torre de Agott.", 1);
        setTextoPantallaCompleta(x + 5, y + 26, "* Recolecta los materiales para tu Capa Mágica: Tela, Tinta mágica y Libro.", 1);
        setTextoPantallaCompleta(x + 5, y + 27, "* Conversa con Maestro Qifrey, interactúa con Agott y descubre el sótano de Richeh.", 1);
        setTextoPantallaCompleta(x + 5, y + 28, "* Busca frascos de tinta arcaica perdidos para ganarte la plena confianza del maestro.", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 30][x + c] = L'=';
            bufferColor[y + 30][x + c] = 8;
        }

        std::string pie = "[ Presiona ENTER para iniciar el viaje ]";
        int cxPie = x + (ancho - longitudVisible(pie)) / 2;
        setTextoPantallaCompleta(cxPie, y + 32, pie, 4);

        dibujar();

#ifdef _WIN32
        while (true) {
            if (_kbhit()) {
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) {
                    tecla = _getch();
                }
                if (tecla == 13 || tecla == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    void copiarViewport(const std::vector<std::string>& matrizMapa, int camaraX, int camaraY, int tickAnim = 0, bool enSubMapa = false, bool (*esCuartoFunc)(int, int) = nullptr, bool (*esCaminoFunc)(int, int) = nullptr) {
        int filasMapa = (int)matrizMapa.size();
        if (filasMapa == 0) return;
        int columnasMapa = (int)matrizMapa[0].size();

        for (int yPantalla = 0; yPantalla < altoTotal; yPantalla++) {
            int yMundo = camaraY + yPantalla;
            for (int xPantalla = 0; xPantalla < anchoJuego; xPantalla++) {
                int xMundo = camaraX + xPantalla;
                if (yMundo >= 0 && yMundo < filasMapa && xMundo >= 0 && xMundo < columnasMapa) {
                    char ch = matrizMapa[yMundo][xMundo];
                    if (ch == '~') {
                        int fase = (xMundo + yMundo + tickAnim) % 3;
                        wchar_t charAgua = (fase == 0) ? L'~' : ((fase == 1) ? L'-' : L'.');
                        buffer[yPantalla][xPantalla] = charAgua;
                        bufferColor[yPantalla][xPantalla] = 3;
                    } else if (enSubMapa && (ch == '/' || ch == '=')) {
                        buffer[yPantalla][xPantalla] = (wchar_t)(unsigned char)ch;
                        bufferColor[yPantalla][xPantalla] = 12;
                    } else if (ch == ' ' && !enSubMapa) {
                        if (esCuartoFunc != nullptr && esCuartoFunc(xMundo, yMundo)) {
                            buffer[yPantalla][xPantalla] = L' ';
                            bufferColor[yPantalla][xPantalla] = 0;
                        } else if (esCaminoFunc != nullptr && esCaminoFunc(xMundo, yMundo)) {
                            unsigned int h = (unsigned int)(xMundo * 374761393u + yMundo * 668265263u);
                            h = (h ^ (h >> 13)) * 1274126177u;
                            unsigned int r = h % 10u;
                            wchar_t charCamino = (r < 3) ? L' ' : ((r < 7) ? L'.' : ((r < 9) ? L':' : L','));
                            buffer[yPantalla][xPantalla] = charCamino;
                            bufferColor[yPantalla][xPantalla] = 8;
                        } else {
                            int r = std::abs(xMundo * 7 + yMundo * 13) % 3;
                            buffer[yPantalla][xPantalla] = (r == 0) ? L'"' : ((r == 1) ? L'\'' : L',');
                            bufferColor[yPantalla][xPantalla] = 11;
                        }
                    } else {
                        buffer[yPantalla][xPantalla] = (wchar_t)(unsigned char)ch;
                        if (ch == '&' || ch == '#' || ch == '/' || ch == '\\') {
                            bufferColor[yPantalla][xPantalla] = 2;
                        } else if (ch == '.' || ch == ':' || ch == '=') {
                            bufferColor[yPantalla][xPantalla] = 4;
                        } else if (ch == '+' || ch == '-' || ch == '|') {
                            bufferColor[yPantalla][xPantalla] = 8;
                        } else if (ch == 'O') {
                            bufferColor[yPantalla][xPantalla] = 7;
                        } else if (ch == '<') {
                            bufferColor[yPantalla][xPantalla] = 4;
                        } else if (ch == '*') {
                            bufferColor[yPantalla][xPantalla] = 4;
                        } else if (ch == '!') {
                            bufferColor[yPantalla][xPantalla] = 4;
                        } else if (ch == '[' || ch == ']') {
                            bufferColor[yPantalla][xPantalla] = 8;
                        } else {
                            bufferColor[yPantalla][xPantalla] = 0;
                        }
                    }
                } else {
                    buffer[yPantalla][xPantalla] = L' ';
                    bufferColor[yPantalla][xPantalla] = 0;
                }
            }
        }
    }

    void animarEscaleraPozo(bool bajando, int nivelNum, const std::string& nivelNom,
                            const std::string& protaNom, int vida, int vidaMax) {
        wchar_t block = L'█';
        int totalFrames = 8;
        int railIzquierda = 36;
        int railDerecha = 47;
        int anchoCoco = 10;
        int altoCoco = 8;

        for (int frame = 0; frame < totalFrames; frame++) {
            for (int f = 0; f < altoTotal; f++) {
                for (int c = 0; c < anchoJuego; c++) {
                    if (c < 24 || c > 59) {
                        buffer[f][c] = (f % 2 == 0 ? (c % 8 == 0 ? L'|' : L'-') : (c % 8 == 4 ? L'|' : L'-'));
                        bufferColor[f][c] = 8;
                    } else {
                        buffer[f][c] = L' ';
                        bufferColor[f][c] = 0;
                    }
                }
            }

            for (int f = 0; f < altoTotal; f++) {
                buffer[f][railIzquierda] = block;
                buffer[f][railIzquierda + 1] = block;
                bufferColor[f][railIzquierda] = 12;
                bufferColor[f][railIzquierda + 1] = 12;

                buffer[f][railDerecha] = block;
                buffer[f][railDerecha + 1] = block;
                bufferColor[f][railDerecha] = 12;
                bufferColor[f][railDerecha + 1] = 12;

                if (f % 3 == 0) {
                    for (int c = railIzquierda + 2; c < railDerecha; c++) {
                        buffer[f][c] = block;
                        bufferColor[f][c] = 12;
                    }
                }
            }

            int yCoco = bajando ? (3 + frame * 3) : (25 - frame * 3);
            if (yCoco < 1) yCoco = 1;
            if (yCoco > altoTotal - altoCoco - 2) yCoco = altoTotal - altoCoco - 2;
            int xCoco = 37;

            int pose = frame % 2;

            int patronCoco[8][10] = {
                {0, 0, 0, 0, 13, 13, 0, 0, 0, 0},
                {0, 0, 13, 13, 13, 13, 13, 13, 0, 0},
                {0, 0, 0, 4, 4, 4, 4, 0, 0, 0},
                {0, 13, 13, 13, 13, 13, 13, 13, 13, 0},
                {0, 0, 0, 13, 13, 13, 13, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                {0, 0, 13, 13, 0, 0, 13, 13, 0, 0},
                {0, 12, 12, 0, 0, 0, 0, 12, 12, 0}
            };

            if (pose == 0) {
                patronCoco[4][0] = 12;
                patronCoco[4][1] = 12;
                patronCoco[6][2] = 13;
                patronCoco[6][3] = 13;
                patronCoco[7][1] = 12;
                patronCoco[7][2] = 12;
            } else {
                patronCoco[4][8] = 12;
                patronCoco[4][9] = 12;
                patronCoco[6][6] = 13;
                patronCoco[6][7] = 13;
                patronCoco[7][7] = 12;
                patronCoco[7][8] = 12;
            }

            for (int r = 0; r < altoCoco; r++) {
                for (int c = 0; c < anchoCoco; c++) {
                    int colPx = patronCoco[r][c];
                    if (colPx != 0 && yCoco + r < altoTotal && xCoco + c < anchoJuego) {
                        buffer[yCoco + r][xCoco + c] = block;
                        bufferColor[yCoco + r][xCoco + c] = colPx;
                    }
                }
            }

            std::string msg = bajando ? "[ Bajando al sótano de Richeh... ]" : "[ Subiendo a la torre de Agott... ]";
            int cxMsg = (anchoJuego - longitudVisible(msg)) / 2;
            setTextoJuego(cxMsg, altoTotal - 3, msg, 4);

            renderizarPanelLateral(nivelNum, nivelNom, protaNom, vida, vidaMax);
            dibujar();

#ifdef _WIN32
            Sleep(180);
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(180));
#endif
        }
    }

    void dibujar() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
            COORD pos = { 0, 0 };
            SetConsoleCursorPosition(hOut, pos);

            std::wstring frameCompleto;
            frameCompleto.reserve(altoTotal * (anchoTotal + 20));
            int colorActual = -1;

            for (int f = 0; f < altoTotal; f++) {
                for (int c = 0; c < anchoTotal; c++) {
                    int col = bufferColor[f][c];
                    if (col != colorActual) {
                        frameCompleto += obtenerCodigoColorW(col);
                        colorActual = col;
                    }
                    frameCompleto += buffer[f][c];
                }
                if (f < altoTotal - 1) {
                    frameCompleto += L"\n";
                }
            }
            frameCompleto += L"\033[0m";

            DWORD written = 0;
            WriteConsoleW(hOut, frameCompleto.c_str(), (DWORD)frameCompleto.length(), &written, NULL);
            return;
        }
#endif
        std::cout << "\033[H";
        std::string frameCompleto = "";
        int colorActual = -1;

        for (int f = 0; f < altoTotal; f++) {
            for (int c = 0; c < anchoTotal; c++) {
                int col = bufferColor[f][c];
                if (col != colorActual) {
                    frameCompleto += obtenerCodigoColor(col);
                    colorActual = col;
                }
                wchar_t wc = buffer[f][c];
                if (wc == L'█') {
                    frameCompleto += "\xE2\x96\x88";
                } else if (wc < 128) {
                    frameCompleto += (char)wc;
                } else if (wc < 0x800) {
                    frameCompleto += (char)(0xC0 | ((wc >> 6) & 0x1F));
                    frameCompleto += (char)(0x80 | (wc & 0x3F));
                } else {
                    frameCompleto += (char)(0xE0 | ((wc >> 12) & 0x0F));
                    frameCompleto += (char)(0x80 | ((wc >> 6) & 0x3F));
                    frameCompleto += (char)(0x80 | (wc & 0x3F));
                }
            }
            if (f < altoTotal - 1) {
                frameCompleto += "\n";
            }
        }
        frameCompleto += RESET;
        std::cout << frameCompleto << std::flush;
    }
};

#endif
