#ifndef Pantalla_h
#define Pantalla_h

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <clocale>

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

    bool sonidoFinReproducido;
    bool nivelesDesbloqueadosCheat;

public:
    Pantalla() : anchoTotal(120), altoTotal(40), anchoJuego(84), anchoPanel(35),
                 ultimoDialogoHablante(""), ultimoDialogoTexto(""), ultimoPromptTexto(""),
                 sonidoFinReproducido(false), nivelesDesbloqueadosCheat(false) {
        buffer = std::vector<std::wstring>(altoTotal, std::wstring(anchoTotal, L' '));
        bufferColor = std::vector<std::vector<int>>(altoTotal, std::vector<int>(anchoTotal, 0));
    }

    ~Pantalla() {}

    void resetSonidoFin() {
        sonidoFinReproducido = false;
    }

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
                if (wchars[i] >= 32) {
                    buffer[y][posX] = wchars[i];
                    bufferColor[y][posX] = color;
                }
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

    std::vector<std::string> dividirEnLineas(const std::string& texto, int maxLongitud) const {
        std::vector<std::string> lineas;
        if (texto.empty()) return lineas;
        std::vector<std::string> parrafos;
        std::string parrafo = "";
        for (size_t k = 0; k < texto.length(); k++) {
            if (texto[k] == '\n' || texto[k] == '\r') {
                parrafos.push_back(parrafo);
                parrafo = "";
            } else {
                parrafo += texto[k];
            }
        }
        parrafos.push_back(parrafo);

        for (size_t p = 0; p < parrafos.size(); p++) {
            const std::string& sec = parrafos[p];
            if (sec.empty()) continue;
            std::string actual = "";
            size_t i = 0;
            while (i < sec.length()) {
                size_t proxEspacio = sec.find(' ', i);
                std::string palabra = (proxEspacio == std::string::npos) ? sec.substr(i) : sec.substr(i, proxEspacio - i);
                if (actual.empty()) {
                    actual = palabra;
                } else if (longitudVisible(actual) + 1 + longitudVisible(palabra) <= maxLongitud) {
                    actual += " " + palabra;
                } else {
                    lineas.push_back(actual);
                    actual = palabra;
                }
                if (proxEspacio == std::string::npos) break;
                i = proxEspacio + 1;
            }
            if (!actual.empty()) {
                lineas.push_back(actual);
            }
        }
        return lineas;
    }

    void dibujarPromptFlotante(const std::string& texto) {
        if (texto.empty()) return;
        int maxTexto = anchoJuego - 8;
        std::vector<std::string> lineas = dividirEnLineas(texto, maxTexto);
        if (lineas.empty()) return;

        int maxVis = 0;
        for (size_t i = 0; i < lineas.size(); i++) {
            int lv = longitudVisible(lineas[i]);
            if (lv > maxVis) maxVis = lv;
        }
        int anchoCaja = maxVis + 4;
        if (anchoCaja > anchoJuego - 4) anchoCaja = anchoJuego - 4;
        if (anchoCaja < 30) anchoCaja = 30;
        int altoCaja = (int)lineas.size() + 2;
        int startX = (anchoJuego - anchoCaja) / 2;
        int startY = altoTotal - altoCaja - 1;
        if (startY < 0) startY = 0;

        dibujarCaja(startX, startY, anchoCaja, altoCaja, 4);

        int colTexto = 1;
        if (texto.find("Coco:") != std::string::npos) colTexto = 6;
        else if (texto.find("Letrero:") != std::string::npos) colTexto = 4;

        if (texto != ultimoPromptTexto && (texto.find("Coco:") != std::string::npos || texto.find("Letrero:") != std::string::npos)) {
            ultimoPromptTexto = texto;
            int freq = (texto.find("Coco:") != std::string::npos) ? 1250 : 520;
            for (size_t i = 0; i < lineas.size(); i++) {
                std::vector<wchar_t> wchars = aWideString(lineas[i]);
                for (size_t c = 0; c < wchars.size(); c++) {
                    if (startX + 2 + (int)c < startX + anchoCaja - 2) {
                        buffer[startY + 1 + (int)i][startX + 2 + (int)c] = wchars[c];
                        bufferColor[startY + 1 + (int)i][startX + 2 + (int)c] = colTexto;
                    }
                    dibujar();
#ifdef _WIN32
                    if (wchars[c] != L' ' && c % 4 == 0) {
                        Beep(freq, 8);
                    } else {
                        std::this_thread::sleep_for(std::chrono::milliseconds(8));
                    }
#else
                    std::this_thread::sleep_for(std::chrono::milliseconds(8));
#endif
                }
            }
        } else {
            for (size_t i = 0; i < lineas.size(); i++) {
                setTextoJuego(startX + 2, startY + 1 + (int)i, lineas[i], colTexto);
            }
            if (texto.find("Coco:") == std::string::npos && texto.find("Letrero:") == std::string::npos) {
                ultimoPromptTexto = texto;
            }
        }
    }

    void animarTexto(const std::string& texto, int velocidadMs = 25) {
        for (size_t i = 0; i < texto.length(); i++) {
            std::cout << texto[i] << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(velocidadMs));
        }
        std::cout << std::endl;
    }

    void animarDialogo(const std::string& hablante, const std::string& texto, int velocidadMs = 25) {
        std::cout << "[" << hablante << "]: ";
        std::cout.flush();
        for (size_t i = 0; i < texto.length(); i++) {
            std::cout << texto[i] << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(velocidadMs));
        }
        std::cout << std::endl;
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

        if (hablante != "Letrero") {
            std::string confStr = "Confianza: ";
            if (confianza <= 0) confStr += "Sin confianza";
            else if (confianza == 1) confStr += "Neutral";
            else confStr += "Amigos";
            setTextoJuego(x + ancho - longitudVisible(confStr) - 2, y + 1, confStr, 3);
        }

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

        int colTextoDialogo = 1;
        if (hablante == "Letrero") colTextoDialogo = 4;
        else if (hablante == "Coco") colTextoDialogo = 6;

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
            int freqHablante = 520;
            if (hablante == "Coco") freqHablante = 1250;
            else if (hablante == "Richeh") freqHablante = 1600;
            else if (hablante == "Agott") freqHablante = 900;
            else if (hablante == "Qifrey") freqHablante = 320;
#endif

            int filaActual = y + 3;
            for (size_t i = 0; i < lineasProcesadas.size() && filaActual < y + 9; i++) {
                std::vector<wchar_t> wchars = aWideString(lineasProcesadas[i]);
                for (size_t c = 0; c < wchars.size(); c++) {
                    if (x + 3 + (int)c < x + ancho - 3) {
                        buffer[filaActual][x + 3 + (int)c] = wchars[c];
                        bufferColor[filaActual][x + 3 + (int)c] = colTextoDialogo;
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
                setTextoJuego(x + 3, filaActual, lineasProcesadas[i], colTextoDialogo);
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
        int y = 3;
        int ancho = 76;
        int alto = 34;
        dibujarCaja(x, y, ancho, alto, 3);
        setTextoJuego(x + 24, y + 1, "=== GUÍA DE PERSONAJES ===", 4);
        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 2][x + c] = L'-';
            bufferColor[y + 2][x + c] = 3;
        }

        int fila = y + 3;
        for (size_t i = 0; i < nombres.size() && fila < y + 13; i++) {
            bool sel = ((int)i == seleccionado);
            std::string prefijo = sel ? "-> " : "   ";
            std::string entrada = prefijo;
            int colItem = sel ? 4 : (desbloqueados[i] ? 1 : 8);
            if (desbloqueados[i]) {
                entrada += "[" + std::to_string(i + 1) + "] " + nombres[i] + " (" + roles[i] + ")";
                std::string textoConf = "Sin confianza";
                if (confianzas[i] == 1) textoConf = "Neutral";
                else if (confianzas[i] == 2) textoConf = "Amigos";
                else if (confianzas[i] == -1) textoConf = "Hostil";
                else if (confianzas[i] == -2) textoConf = "Asustado";
                else if (confianzas[i] == 3) textoConf = "Agradecido";
                entrada += " [" + textoConf + "]";
            } else {
                entrada += "[" + std::to_string(i + 1) + "] [ ??? ] (Bloqueado)";
            }
            setTextoJuego(x + 3, fila, entrada, colItem);
            fila += 1;
        }

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 13][x + c] = L'-';
            bufferColor[y + 13][x + c] = 3;
        }
        setTextoJuego(x + 3, y + 14, "DETALLES DEL PERSONAJE:", 3);

        if (seleccionado >= 0 && seleccionado < (int)desbloqueados.size()) {
            if (desbloqueados[seleccionado]) {
                setTextoJuego(x + 5, y + 16, "Nombre: " + nombres[seleccionado], 4);
                setTextoJuego(x + 5, y + 17, "Rol:    " + roles[seleccionado], 1);
                std::string nivelConf = "Sin confianza";
                int colConf = 8;
                if (confianzas[seleccionado] == 1) { nivelConf = "Neutral"; colConf = 4; }
                else if (confianzas[seleccionado] == 2) { nivelConf = "Amigos"; colConf = 2; }
                else if (confianzas[seleccionado] == -1) { nivelConf = "Hostil"; colConf = 7; }
                else if (confianzas[seleccionado] == -2) { nivelConf = "Asustado"; colConf = 4; }
                else if (confianzas[seleccionado] == 3) { nivelConf = "Agradecido"; colConf = 2; }
                setTextoJuego(x + 5, y + 18, "Confianza: " + nivelConf, colConf);
                setTextoJuego(x + 5, y + 20, "Descripcion:", 3);

                std::vector<std::string> ldesc = dividirEnLineas(descripciones[seleccionado], 46);
                for (size_t li = 0; li < ldesc.size() && y + 21 + (int)li < y + alto - 3; li++) {
                    setTextoJuego(x + 5, y + 21 + (int)li, ldesc[li], 1);
                }

                int artX = x + 56;
                int artY = y + 16;
                if (nombres[seleccionado] == "Coco") {
                    setTextoJuego(artX, artY,     "   /\\   ", 6);
                    setTextoJuego(artX, artY + 1, "  (°u°)  ", 6);
                } else if (nombres[seleccionado] == "Qifrey") {
                    setTextoJuego(artX, artY,     "   /     ", 3);
                    setTextoJuego(artX, artY + 1, " ( -_-)  ", 1);
                    setTextoJuego(artX, artY + 2, "   \\    ", 3);
                } else if (nombres[seleccionado] == "Richeh") {
                    setTextoJuego(artX, artY,     "  /--\\   ", 4);
                    setTextoJuego(artX, artY + 1, " ( •~•)  ", 6);
                } else if (nombres[seleccionado] == "Agott") {
                    setTextoJuego(artX, artY,     "  /==\\   ", 5);
                    setTextoJuego(artX, artY + 1, " ( '_´)  ", 1);
                } else if (nombres[seleccionado] == "Tetia") {
                    setTextoJuego(artX, artY,     "  /\\/\\   ", 2);
                    setTextoJuego(artX, artY + 1, " (^o^)   ", 4);
                } else if (nombres[seleccionado] == "Olruggio") {
                    setTextoJuego(artX, artY,     "  ===    ", 7);
                    setTextoJuego(artX, artY + 1, " ( -_-)  ", 1);
                } else if (nombres[seleccionado] == "Iguin") {
                    setTextoJuego(artX, artY,     "  ===    ", 7);
                    setTextoJuego(artX, artY + 1, " /<_o    ", 7);
                } else if (nombres[seleccionado] == "Restys") {
                    setTextoJuego(artX, artY,     " ====    ", 7);
                    setTextoJuego(artX, artY + 1, " (v_v)   ", 7);
                } else if (nombres[seleccionado] == "Myrphon") {
                    setTextoJuego(artX, artY,     "  (o>    ", 4);
                    setTextoJuego(artX, artY + 1, " /||\\   ", 4);
                } else if (nombres[seleccionado] == "Coustas") {
                    setTextoJuego(artX, artY,     "  _/_\\   ", 6);
                    setTextoJuego(artX, artY + 1, " ('v')   ", 1);
                    setTextoJuego(artX, artY + 2, " [===]O  ", 8);
                } else if (nombres[seleccionado] == "Dagda") {
                    setTextoJuego(artX, artY,     "  /--\\   ", 6);
                    setTextoJuego(artX, artY + 1, " (o_o)   ", 1);
                } else if (nombres[seleccionado] == "Ininia") {
                    setTextoJuego(artX, artY,     "  /==\\   ", 5);
                    setTextoJuego(artX, artY + 1, " (~_~)   ", 5);
                }
            } else {
                setTextoJuego(x + 5, y + 17, "Personaje aun no descubierto en este nivel.", 8);
                setTextoJuego(x + 5, y + 18, "Explora el atelier o avanza en la historia.", 8);
            }
        }

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + alto - 3][x + c] = L'-';
            bufferColor[y + alto - 3][x + c] = 3;
        }
        setTextoJuego(x + 5, y + alto - 2, "[W/S] Navegar   [1-9] Elegir   [P/ESC] Cerrar", 3);
    }

    void dibujarModalInventario(int seleccionado, const std::vector<std::string>& nombres,
                                const std::vector<std::string>& tipos,
                                const std::vector<std::string>& descripciones) {
        int x = 6;
        int y = 4;
        int ancho = 72;
        int alto = 32;
        dibujarCaja(x, y, ancho, alto, 4);
        setTextoJuego(x + 22, y + 1, "=== MOCHILA / INVENTARIO ===", 4);
        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 2][x + c] = L'-';
            bufferColor[y + 2][x + c] = 4;
        }

        int fila = y + 3;
        for (int i = 0; i < 8; i++) {
            bool sel = (i == seleccionado);
            std::string prefijo = sel ? "-> " : "   ";
            std::string entrada = prefijo + "[" + std::to_string(i + 1) + "] ";
            int colRanura = sel ? 4 : 1;
            if (i < (int)nombres.size() && !nombres[i].empty()) {
                entrada += nombres[i];
                if (i < (int)tipos.size() && !tipos[i].empty()) {
                    entrada += " (" + tipos[i] + ")";
                }
            } else {
                entrada += "(Ranura vacía)";
                if (!sel) colRanura = 8;
            }
            setTextoJuego(x + 4, fila, entrada, colRanura);
            fila += 1;
        }

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 12][x + c] = L'-';
            bufferColor[y + 12][x + c] = 4;
        }
        setTextoJuego(x + 4, y + 13, "[DETALLES DEL ÍTEM]", 4);

        if (seleccionado >= 0 && seleccionado < (int)nombres.size() && !nombres[seleccionado].empty()) {
            setTextoJuego(x + 5, y + 15, "Nombre:      " + nombres[seleccionado], 2);
            if (seleccionado < (int)tipos.size()) {
                setTextoJuego(x + 5, y + 17, "Tipo:        " + tipos[seleccionado], 3);
            }
            if (seleccionado < (int)descripciones.size()) {
                setTextoJuego(x + 5, y + 19, "Descripción: ", 4);
                std::vector<std::string> ldesc = dividirEnLineas(descripciones[seleccionado], 58);
                for (size_t li = 0; li < ldesc.size() && y + 21 + (int)li < y + alto - 3; li++) {
                    setTextoJuego(x + 5, y + 21 + (int)li, ldesc[li], 1);
                }
            }
        } else {
            setTextoJuego(x + 5, y + 15, "Ranura de mochila sin ítem asignado.", 8);
            setTextoJuego(x + 5, y + 17, "Capacidad máxima: 8 ítems en este nivel.", 8);
        }

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + alto - 3][x + c] = L'-';
            bufferColor[y + alto - 3][x + c] = 4;
        }
        setTextoJuego(x + 5, y + alto - 2, "[W/S] Navegar   [1-8] Elegir   [I/ESC] Cerrar", 4);
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
        dibujarCaja(x, y, ancho, alto, 3);

        if (!verDetalle) {
            setTextoJuego(x + 25, y + 1, "=== MISIONES ===", 4);
            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 2][x + c] = L'-';
                bufferColor[y + 2][x + c] = 3;
            }

            int fila = y + 4;
            for (size_t i = 0; i < titulos.size() && fila < y + 22; i++) {
                bool sel = ((int)i == seleccionado);
                std::string prefijo = sel ? "-> " : "   ";
                std::string entrada = prefijo + "[" + std::to_string(i + 1) + "] ";
                int colFila = sel ? 4 : 1;
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
                    if (!sel) colFila = 8;
                }
                setTextoJuego(x + 4, fila, entrada, colFila);
                fila += 2;
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + alto - 3][x + c] = L'-';
                bufferColor[y + alto - 3][x + c] = 3;
            }
            setTextoJuego(x + 4, y + alto - 2, "[W/S] Navegar   [ENTER] Ver detalle   [M/ESC] Cerrar", 3);
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
            setTextoJuego(x + 4, y + 1, tituloDetalle, 4);
            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 2][x + c] = L'-';
                bufferColor[y + 2][x + c] = 3;
            }

            if (desbloqueadas[seleccionado]) {
                setTextoJuego(x + 4, y + 5, "Misión: " + titulos[seleccionado], 4);
                int colEst = (estados[seleccionado] == "COMPLETADA") ? 2 : 4;
                setTextoJuego(x + 4, y + 7, "Estado: " + estados[seleccionado], colEst);
                for (int c = 4; c < ancho - 4; c++) {
                    buffer[y + 9][x + c] = L'-';
                    bufferColor[y + 9][x + c] = 3;
                }
                setTextoJuego(x + 4, y + 11, "Descripción:", 3);
                std::vector<std::string> lineasDesc = dividirEnLineas(descripciones[seleccionado], ancho - 10);
                int filaDesc = y + 13;
                for (size_t i = 0; i < lineasDesc.size() && filaDesc < y + alto - 4; i++) {
                    setTextoJuego(x + 4, filaDesc, lineasDesc[i], 1);
                    filaDesc += 2;
                }
            } else {
                setTextoJuego(x + 4, y + 5, "Misión Bloqueada", 8);
                setTextoJuego(x + 4, y + 7, "Estado: BLOQUEADA", 8);
                for (int c = 4; c < ancho - 4; c++) {
                    buffer[y + 9][x + c] = L'-';
                    bufferColor[y + 9][x + c] = 8;
                }
                setTextoJuego(x + 4, y + 11, "Descripción:", 8);
                setTextoJuego(x + 4, y + 13, "Esta misión aún no ha sido desbloqueada.", 8);
                setTextoJuego(x + 4, y + 15, "Completa los objetivos previos para acceder.", 8);
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + alto - 3][x + c] = L'-';
                bufferColor[y + alto - 3][x + c] = 3;
            }
            setTextoJuego(x + 4, y + alto - 2, "[ENTER / ESC] Volver a la lista de misiones", 3);
        }
    }

    void dibujarEstadisticasFinNivel(int nivel, const std::string& nombreNivel,
                                     const std::string& prota, int segs,
                                     int bonoTiempo, int puntosMisiones,
                                     int confianzaQifrey) {
        if (!sonidoFinReproducido) {
            sonidoFinReproducido = true;
#ifdef _WIN32
            Beep(523, 120);
            Beep(659, 120);
            Beep(784, 140);
            Beep(1046, 320);
#endif
        }

        int x = 4;
        int y = 3;
        int ancho = 76;
        int alto = 34;
        dibujarCaja(x, y, ancho, alto, 3);

        setTextoJuego(x + 18, y + 1, "=== ESTADÍSTICAS DE FIN DE NIVEL ===", 4);
        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 2][x + c] = L'-';
            bufferColor[y + 2][x + c] = 3;
        }

        setTextoJuego(x + 4, y + 4, "Nivel superado:   NIVEL " + std::to_string(nivel) + " - " + nombreNivel, 2);
        if (nivel == 3) {
            setTextoJuego(x + 4, y + 5, "Personaje:        " + prota + " (Trovador)", 6);
            setTextoJuego(x + 4, y + 6, "Destino:          El Destino de la Noche de Plata", 2);
        } else if (nivel == 2) {
            setTextoJuego(x + 4, y + 5, "Personaje:        " + prota + " (Orfebre)", 6);
            setTextoJuego(x + 4, y + 6, "Objetivo:         Rastro de Ininia Descubierto", 2);
        } else {
            setTextoJuego(x + 4, y + 5, "Maga:             " + prota + " (Aprendiz)", 6);
            setTextoJuego(x + 4, y + 6, "Objetivo:         Capa Mágica Crafteada con Éxito", 2);
        }

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 8][x + c] = L'-';
            bufferColor[y + 8][x + c] = 3;
        }
        setTextoJuego(x + 4, y + 9, "[DESGLOSE DE PUNTUACIÓN SEGÚN TIEMPO Y MISIONES]", 3);

        int min = segs / 60;
        int seg = segs % 60;
        std::string tStr = (min < 10 ? "0" : "") + std::to_string(min) + ":" + (seg < 10 ? "0" : "") + std::to_string(seg);

        setTextoJuego(x + 4, y + 11, "Tiempo empleado:                      " + tStr, 1);
        setTextoJuego(x + 4, y + 13, "Puntos por velocidad de tiempo:       +" + std::to_string(bonoTiempo) + " pts", 2);
        setTextoJuego(x + 4, y + 15, "Puntos por misiones y recolección:    +" + std::to_string(puntosMisiones) + " pts", 2);
        for (int c = 4; c < ancho - 4; c++) {
            buffer[y + 17][x + c] = L'-';
            bufferColor[y + 17][x + c] = 4;
        }

        int total = bonoTiempo + puntosMisiones;
        setTextoJuego(x + 4, y + 19, "PUNTAJE TOTAL DEL NIVEL:              " + std::to_string(total) + " PTS", 4);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 21][x + c] = L'-';
            bufferColor[y + 21][x + c] = 3;
        }
        setTextoJuego(x + 4, y + 22, "[LOGROS Y ESTADO DE LORE]", 3);
        if (nivel == 3) {
            setTextoJuego(x + 4, y + 24, "Ya se completo el juego, gracias por jugar.", 2);
            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + alto - 4][x + c] = L'-';
                bufferColor[y + alto - 4][x + c] = 3;
            }
            setTextoJuego(x + 4, y + alto - 3, "[1 / ENTER] Finalizar aventura y ver epilogo", 4);
            setTextoJuego(x + 4, y + alto - 2, "[ESC] Salir del juego", 8);
        } else if (nivel == 2) {
            setTextoJuego(x + 4, y + 24, "Pistas de Kaln reunidas:              Ininia y Coustas localizados", 2);
            setTextoJuego(x + 4, y + 25, "Ruta de huida descubierta:            Sendero hacia el Arbol de Plata", 5);
            setTextoJuego(x + 4, y + 26, "Hermandad con Coustas:                Determinacion inquebrantable", 6);

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + alto - 4][x + c] = L'-';
                bufferColor[y + alto - 4][x + c] = 3;
            }
            setTextoJuego(x + 4, y + alto - 3, "[1 / ENTER] Avanzar al Nivel 3    [C] Seguir explorando", 4);
            setTextoJuego(x + 4, y + alto - 2, "[ESC] Salir del juego", 8);
        } else {
            std::string confStr = "Sin confianza";
            int colConf = 8;
            if (confianzaQifrey == 1) { confStr = "Neutral"; colConf = 4; }
            else if (confianzaQifrey >= 2) { confStr = "Amigos"; colConf = 2; }
            setTextoJuego(x + 4, y + 24, "Vínculo con Maestro Qifrey:           " + confStr, colConf);
            setTextoJuego(x + 4, y + 25, "Objeto legendario desbloqueado:       Capa Mágica de Vuelo", 5);
            setTextoJuego(x + 4, y + 26, "Habilidad de vuelo:                   Activada para Coco", 6);

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + alto - 4][x + c] = L'-';
                bufferColor[y + alto - 4][x + c] = 3;
            }
            setTextoJuego(x + 4, y + alto - 3, "[1 / ENTER] Avanzar al Nivel 2    [C] Seguir explorando", 4);
            setTextoJuego(x + 4, y + alto - 2, "[ESC] Salir del juego", 8);
        }
    }

    void renderizarPanelLateral(int nivel, const std::string& nombreNivel,
                               const std::string& protagonista, int vida, int vidaMax,
                               const std::string& ubicacion = "") {
        std::vector<std::wstring> lineasPanel(altoTotal, std::wstring(anchoPanel, L' '));

        std::wstring separador = std::wstring(anchoPanel - 1, L'-') + L"|";
        std::wstring bordeCaja = std::wstring(anchoPanel - 1, L'-') + L"+";

        lineasPanel[0]  = bordeCaja;
        lineasPanel[1]  = encuadrarFilaPanelW("     WITCH HAT ATELIER", anchoPanel);
        lineasPanel[2]  = encuadrarFilaPanelW("      ÁRBOL DE PLATA", anchoPanel);
        lineasPanel[3]  = separador;
        lineasPanel[4]  = encuadrarFilaPanelW("NIVEL " + std::to_string(nivel) + ": " + nombreNivel, anchoPanel);
        if (nivel == 3) {
            lineasPanel[5]  = encuadrarFilaPanelW("TROVADOR: " + protagonista, anchoPanel);
        } else if (nivel == 2) {
            lineasPanel[5]  = encuadrarFilaPanelW("ORFEBRE: " + protagonista, anchoPanel);
        } else {
            lineasPanel[5]  = encuadrarFilaPanelW("MAGA: " + protagonista + " (Aprendiz)", anchoPanel);
        }

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

        std::string ubiTexto = ubicacion.empty() ? nombreNivel : ubicacion;
        lineasPanel[24] = encuadrarFilaPanelW("[UBICACION]", anchoPanel);
        lineasPanel[25] = separador;
        lineasPanel[26] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[27] = encuadrarFilaPanelW("Lugar:", anchoPanel);
        lineasPanel[28] = encuadrarFilaPanelW(" " + ubiTexto, anchoPanel);
        lineasPanel[29] = encuadrarFilaPanelW("", anchoPanel);
        lineasPanel[30] = separador;

        for (int f = 31; f < altoTotal - 1; f++) {
            lineasPanel[f] = encuadrarFilaPanelW("", anchoPanel);
        }
        lineasPanel[altoTotal - 1] = bordeCaja;

        int colInicioPanel = anchoJuego + 1;
        for (int f = 0; f < altoTotal; f++) {
            for (int c = 0; c < anchoPanel; c++) {
                buffer[f][colInicioPanel + c] = lineasPanel[f][c];
                if (f == 0 || f == 3 || f == 7 || f == 9 || f == 23 || f == 25 || f == 30 || f == altoTotal - 1 || c == anchoPanel - 1) {
                    bufferColor[f][colInicioPanel + c] = 8;
                } else if (f == 1) {
                    bufferColor[f][colInicioPanel + c] = 3;
                } else if (f == 2) {
                    bufferColor[f][colInicioPanel + c] = 4;
                } else if (f == 6) {
                    bufferColor[f][colInicioPanel + c] = 7;
                } else if (f == 8 || f == 24) {
                    bufferColor[f][colInicioPanel + c] = 4;
                } else if (f == 28) {
                    bufferColor[f][colInicioPanel + c] = 2;
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
                if (wchars[i] >= 32) {
                    buffer[y][posX] = wchars[i];
                    bufferColor[y][posX] = color;
                }
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

    void mostrarEasterEggTrollface() {
        limpiarBufferCompleto();
        std::vector<std::string> troll = {
            u8"⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⡤⢤⣤⣤⣤⣤⣤⣄⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀",
            u8"⠀⠀⠀⠀⠀⣠⣿⡿⣟⠯⡒⢯⣽⣓⣒⢾⣯⣭⣿⣿⠿⠭⠭⣯⣷⣦⡀⠀⠀⠀",
            u8"⠀⠀⠀⠀⣰⣿⣯⣞⣕⣽⠾⠿⠿⠿⢿⣏⣿⣿⣿⡗⣽⣿⣿⣷⡝⣿⣿⡆⠀⠀",
            u8"⠀⠀⠀⣀⣛⠛⢿⣛⢝⢁⣀⣀⣀⠓⠶⠈⣿⣿⡿⠗⠉⠁⢀⣀⣹⣛⣛⣳⢄⠀",
            u8"⠀⡔⡾⢁⣴⡆⢦⣬⣙⣛⣋⣤⣿⣿⣷⣾⣿⣿⣿⡆⢿⣿⡟⠻⠛⡉⣍⣲⢱⠁",
            u8"⠀⣇⣇⢸⣉⡀⢦⣌⡙⠻⠿⣯⣭⣥⠡⡤⠿⢿⣿⣿⡆⠉⡻⢿⣿⠇⢻⣟⠼⠀",
            u8"⠀⠈⠪⣴⣿⣧⡀⢉⠛⠘⢶⣦⣬⠉⣀⠓⠿⠿⠯⢉⣴⠿⠿⠓⡁⡄⠀⣿⠃⠀",
            u8"⠀⠀⠀⠙⣿⣿⣷⣌⠻⢠⣤⣀⠉⠐⠛⠿⠿⠰⠶⠦⠰⠶⠇⠘⠃⠁⠀⣿⠀⠀",
            u8"⠀⠀⠀⠀⠘⢿⣿⣿⣷⣌⠻⢿⠇⣼⣶⣶⣶⣶⣶⣶⣶⣶⣦⡀⠀⢀⣼⠇⠀⠀",
            u8"⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣷⣮⣑⠪⠭⠭⠭⠭⠭⠭⠭⠿⢻⣵⣾⠟⠁⠀⠀⠀",
            u8"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠁⠀⠀⠀⠀⠀"
        };
        int yStart = 10;
        for (size_t r = 0; r < troll.size(); r++) {
            std::vector<wchar_t> wchars = aWideString(troll[r]);
            int xStart = (anchoTotal - (int)wchars.size()) / 2;
            for (size_t c = 0; c < wchars.size(); c++) {
                int px = xStart + (int)c;
                int py = yStart + (int)r;
                if (px >= 0 && px < anchoTotal && py >= 0 && py < altoTotal) {
                    buffer[py][px] = wchars[c];
                    bufferColor[py][px] = 1;
                }
            }
        }
        std::string textoSub = "... ¿Esperabas un secreto magico ancestral aqui? ...";
        int cxSub = (anchoTotal - longitudVisible(textoSub)) / 2;
        setTextoPantallaCompleta(cxSub, yStart + 14, textoSub, 8);

        std::string btn = "[ Presiona ENTER o E para volver al taller ]";
        int cxBtn = (anchoTotal - longitudVisible(btn)) / 2;
        setTextoPantallaCompleta(cxBtn, yStart + 17, btn, 4);

        dibujar();

#ifdef _WIN32
        Beep(500, 80);
        Beep(700, 80);
        Beep(450, 120);
        while (true) {
            if (_kbhit()) {
                int t = _getch();
                if (t == 0 || t == 224) t = _getch();
                if (t == 13 || t == 'e' || t == 'E' || t == 27 || t == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    int mostrarMenuPrincipal() {
        int seleccion = 0;
        int totalOpciones = 4;

        while (true) {
            limpiarBufferCompleto();
            int x = 6;
            int y = 2;
            int ancho = 108;
            int alto = 36;
            dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

            std::string t1 = "==========================================================================";
            std::string t2 = "              W I T C H   H A T   A T E L I E R               ";
            std::string t3 = "                    E L   A R B O L   D E   P L A T A                     ";
            int cxT1 = x + (ancho - longitudVisible(t1)) / 2;
            int cxT2 = x + (ancho - longitudVisible(t2)) / 2;
            int cxT3 = x + (ancho - longitudVisible(t3)) / 2;
            setTextoPantallaCompleta(cxT1, y + 2, t1, 8);
            setTextoPantallaCompleta(cxT2, y + 3, t2, 4);
            setTextoPantallaCompleta(cxT3, y + 4, t3, 3);
            setTextoPantallaCompleta(cxT1, y + 5, t1, 8);

            std::vector<std::string> arteGorro = {
                R"(                     / \                     )",
                R"(                    /   \                    )",
                R"(                   /  _  \                   )",
                R"(                  /  / \  \                  )",
                R"(                 /  | * |  \                 )",
                R"(             .--'    \_/    '--.             )",
                R"(            (___________________)            )"
            };
            for (size_t r = 0; r < arteGorro.size(); r++) {
                int cxGorro = x + (ancho - longitudVisible(arteGorro[r])) / 2;
                setTextoPantallaCompleta(cxGorro, y + 7 + (int)r, arteGorro[r], 6);
            }

            std::string cita = "\"La verdadera magia no reside en el linaje, sino en el trazo del corazon.\"";
            int cxCita = x + (ancho - longitudVisible(cita)) / 2;
            setTextoPantallaCompleta(cxCita, y + 15, cita, 2);

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 17][x + c] = L'-';
                bufferColor[y + 17][x + c] = 8;
            }

            std::string opcTexto[4] = {
                "1. INICIAR JUEGO",
                "2. CREDITOS",
                "3. LORE DE LA OBRA",
                "4. SALIR"
            };

            for (int i = 0; i < totalOpciones; i++) {
                int yOpc = y + 19 + i * 2;
                std::string lineaOpc;
                int colOpc;
                if (seleccion == i) {
                    lineaOpc = "==>  [ " + opcTexto[i] + " ]  <==";
                    colOpc = 4;
                } else {
                    lineaOpc = "       " + opcTexto[i] + "       ";
                    colOpc = 1;
                }
                int cxOpc = x + (ancho - longitudVisible(lineaOpc)) / 2;
                setTextoPantallaCompleta(cxOpc, yOpc, lineaOpc, colOpc);
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 28][x + c] = L'-';
                bufferColor[y + 28][x + c] = 8;
            }

            std::string controles = "[W / S] Mover seleccion   |   [ENTER / E] Confirmar   |   [1..4] Acceso rapido";
            int cxCtrl = x + (ancho - longitudVisible(controles)) / 2;
            setTextoPantallaCompleta(cxCtrl, y + 30, controles, 3);

            std::string subPie = "Algoritmos (1ACC0265) - UPC 2026-2  |  Grupo 4";
            int cxSub = x + (ancho - longitudVisible(subPie)) / 2;
            setTextoPantallaCompleta(cxSub, y + 32, subPie, 8);

            dibujar();

#ifdef _WIN32
            while (true) {
                if (_kbhit()) {
                    int tecla = _getch();
                    if (tecla == 0 || tecla == 224) {
                        tecla = _getch();
                    }
                    if (tecla == 'w' || tecla == 'W') {
                        seleccion = (seleccion - 1 + totalOpciones) % totalOpciones;
                        Beep(850, 18);
                        break;
                    } else if (tecla == 's' || tecla == 'S') {
                        seleccion = (seleccion + 1) % totalOpciones;
                        Beep(850, 18);
                        break;
                    } else if (tecla == '1') {
                        seleccion = 0;
                        Beep(1200, 35);
                        return 0;
                    } else if (tecla == '2') {
                        seleccion = 1;
                        Beep(1200, 35);
                        return 1;
                    } else if (tecla == '3') {
                        seleccion = 2;
                        Beep(1200, 35);
                        return 2;
                    } else if (tecla == '4') {
                        seleccion = 3;
                        Beep(1200, 35);
                        return 3;
                    } else if (tecla == 13 || tecla == 'e' || tecla == 'E') {
                        Beep(1200, 35);
                        return seleccion;
                    }
                }
                Sleep(20);
            }
#else
            char c;
            std::cin >> c;
            if (c == 'w') seleccion = (seleccion - 1 + totalOpciones) % totalOpciones;
            else if (c == 's') seleccion = (seleccion + 1) % totalOpciones;
            else if (c == '1') return 0;
            else if (c == '2') return 1;
            else if (c == '3') return 2;
            else if (c == '4') return 3;
            else return seleccion;
#endif
        }
    }

    int mostrarSelectorNivel() {
        int seleccion = 0;
        int totalOpciones = 4;
        std::string mensajeAlerta = "";
        int colorAlerta = 7;

        while (true) {
            limpiarBufferCompleto();
            int x = 6;
            int y = 2;
            int ancho = 108;
            int alto = 36;
            dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

            std::string t1 = "==========================================================================";
            std::string t2 = "              S E L E C C I O N   D E   N I V E L              ";
            int cxT1 = x + (ancho - longitudVisible(t1)) / 2;
            int cxT2 = x + (ancho - longitudVisible(t2)) / 2;
            setTextoPantallaCompleta(cxT1, y + 2, t1, 8);
            setTextoPantallaCompleta(cxT2, y + 3, t2, 4);
            setTextoPantallaCompleta(cxT1, y + 4, t1, 8);

            std::string desc = "Selecciona el nivel de la aventura que deseas jugar:";
            int cxDesc = x + (ancho - longitudVisible(desc)) / 2;
            setTextoPantallaCompleta(cxDesc, y + 6, desc, 1);

            std::string itemsNivel[4];
            if (nivelesDesbloqueadosCheat) {
                itemsNivel[0] = "[1] Nivel 1: Coco   - El Atelier de Qifrey      [DESBLOQUEADO]";
                itemsNivel[1] = "[2] Nivel 2: Tartah - El Laberinto de Kaln      [DESBLOQUEADO]";
                itemsNivel[2] = "[3] Nivel 3: Qifrey - El Gran Arbol de Plata    [DESBLOQUEADO]";
                itemsNivel[3] = "[4] Volver al Menu Principal";
            } else {
                itemsNivel[0] = "[1] Nivel 1: Coco   - El Atelier de Qifrey      [DESBLOQUEADO]";
                itemsNivel[1] = "[2] Nivel 2: Tartah - El Laberinto de Kaln      [BLOQUEADO]";
                itemsNivel[2] = "[3] Nivel 3: Qifrey - El Gran Arbol de Plata    [BLOQUEADO]";
                itemsNivel[3] = "[4] Volver al Menu Principal";
            }

            for (int i = 0; i < totalOpciones; i++) {
                int yOpc = y + 10 + i * 3;
                std::string lineaOpc;
                int colOpc;
                bool desbloqueado = (i == 0 || (nivelesDesbloqueadosCheat && (i == 1 || i == 2)));
                if (seleccion == i) {
                    lineaOpc = "==>  " + itemsNivel[i] + "  <==";
                    colOpc = desbloqueado ? 2 : ((i == 3) ? 3 : 7);
                } else {
                    lineaOpc = "       " + itemsNivel[i] + "       ";
                    colOpc = desbloqueado ? 1 : ((i == 3) ? 8 : 8);
                }
                int cxOpc = x + (ancho - longitudVisible(lineaOpc)) / 2;
                setTextoPantallaCompleta(cxOpc, yOpc, lineaOpc, colOpc);
            }

            if (!mensajeAlerta.empty()) {
                int cxAlert = x + (ancho - longitudVisible(mensajeAlerta)) / 2;
                setTextoPantallaCompleta(cxAlert, y + 24, mensajeAlerta, colorAlerta);
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 27][x + c] = L'-';
                bufferColor[y + 27][x + c] = 8;
            }

            std::string controles = "[W / S] Mover   |   [ENTER / E] Seleccionar   |   [1..4] Opciones   |   [ESC] Volver";
            int cxCtrl = x + (ancho - longitudVisible(controles)) / 2;
            setTextoPantallaCompleta(cxCtrl, y + 29, controles, 3);

            dibujar();

#ifdef _WIN32
            while (true) {
                if (_kbhit()) {
                    int tecla = _getch();
                    if (tecla == 0 || tecla == 224) tecla = _getch();

                    if (tecla == 'w' || tecla == 'W') {
                        seleccion = (seleccion - 1 + totalOpciones) % totalOpciones;
                        mensajeAlerta = "";
                        Beep(850, 18);
                        break;
                    } else if (tecla == 's' || tecla == 'S') {
                        seleccion = (seleccion + 1) % totalOpciones;
                        mensajeAlerta = "";
                        Beep(850, 18);
                        break;
                    } else if (tecla == '0') {
                        nivelesDesbloqueadosCheat = !nivelesDesbloqueadosCheat;
                        mensajeAlerta = "";
                        if (nivelesDesbloqueadosCheat) {
                            Beep(600, 50);
                            Beep(800, 50);
                            Beep(1200, 100);
                        } else {
                            Beep(400, 80);
                        }
                        break;
                    } else if (tecla == '1') {
                        Beep(1200, 35);
                        return 0;
                    } else if (tecla == '2') {
                        if (nivelesDesbloqueadosCheat) {
                            Beep(1200, 35);
                            return 1;
                        } else {
                            Beep(300, 100);
                            mensajeAlerta = "¡Nivel Bloqueado! Debes completar el nivel previo para acceder.";
                            colorAlerta = 7;
                            break;
                        }
                    } else if (tecla == '3') {
                        if (nivelesDesbloqueadosCheat) {
                            Beep(1200, 35);
                            return 2;
                        } else {
                            Beep(300, 100);
                            mensajeAlerta = "¡Nivel Bloqueado! Debes completar el nivel previo para acceder.";
                            colorAlerta = 7;
                            break;
                        }
                    } else if (tecla == '4' || tecla == 27) {
                        Beep(800, 25);
                        return -1;
                    } else if (tecla == 13 || tecla == 'e' || tecla == 'E') {
                        if (seleccion == 0) {
                            Beep(1200, 35);
                            return 0;
                        } else if (seleccion == 1) {
                            if (nivelesDesbloqueadosCheat) {
                                Beep(1200, 35);
                                return 1;
                            } else {
                                Beep(300, 100);
                                mensajeAlerta = "¡Nivel Bloqueado! Debes completar el nivel previo para acceder.";
                                colorAlerta = 7;
                                break;
                            }
                        } else if (seleccion == 2) {
                            if (nivelesDesbloqueadosCheat) {
                                Beep(1200, 35);
                                return 2;
                            } else {
                                Beep(300, 100);
                                mensajeAlerta = "¡Nivel Bloqueado! Debes completar el nivel previo para acceder.";
                                colorAlerta = 7;
                                break;
                            }
                        } else {
                            Beep(800, 25);
                            return -1;
                        }
                    }
                }
                Sleep(20);
            }
#else
            return 0;
#endif
        }
    }

    void mostrarCreditos() {
        limpiarBufferCompleto();
        int x = 6;
        int y = 2;
        int ancho = 108;
        int alto = 36;
        dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

        std::string t1 = "==========================================================================";
        std::string t2 = "                      C R E D I T O S   D E L   J U E G O                 ";
        int cxT1 = x + (ancho - longitudVisible(t1)) / 2;
        int cxT2 = x + (ancho - longitudVisible(t2)) / 2;
        setTextoPantallaCompleta(cxT1, y + 2, t1, 8);
        setTextoPantallaCompleta(cxT2, y + 3, t2, 4);
        setTextoPantallaCompleta(cxT1, y + 4, t1, 8);

        std::string infoUniv = "UNIVERSIDAD PERUANA DE CIENCIAS APLICADAS (UPC)";
        std::string infoCurso = "CURSO: Algoritmos (1ACC0265) - Ciclo 2026-2";
        std::string infoDoc = "DOCENTE: Luis Alberto Raymundo Chacaltana";
        std::string infoObra = "OBRA ORIGINAL: Witch Hat Atelier (Kamome Shirahama, 2016)";
        setTextoPantallaCompleta(x + (ancho - longitudVisible(infoUniv)) / 2, y + 6, infoUniv, 2);
        setTextoPantallaCompleta(x + (ancho - longitudVisible(infoCurso)) / 2, y + 7, infoCurso, 3);
        setTextoPantallaCompleta(x + (ancho - longitudVisible(infoDoc)) / 2, y + 8, infoDoc, 1);
        setTextoPantallaCompleta(x + (ancho - longitudVisible(infoObra)) / 2, y + 9, infoObra, 8);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 11][x + c] = L'=';
            bufferColor[y + 11][x + c] = 8;
        }

        std::string eq = "EQUIPO DE DESARROLLO - GRUPO 4";
        setTextoPantallaCompleta(x + (ancho - longitudVisible(eq)) / 2, y + 13, eq, 4);

        std::vector<std::string> arteJuan = {
            R"(       /\       )",
            R"(      /  \      )",
            R"(     /____\     )",
            R"(    ( * . * )   )",
            R"(    / |===| \   )",
            R"(   /  |   |  \  )",
            R"(      d   b     )"
        };

        std::vector<std::string> arteKiana = {
            R"(      .---.     )",
            R"(     / ~ ~ \    )",
            R"(    ( * u * )   )",
            R"(    <| === |>   )",
            R"(    / |   | \   )",
            R"(   /  |   |  \  )",
            R"(      d   b     )"
        };

        int xJuan = x + 20;
        int xKiana = x + 64;
        int yArte = y + 15;

        for (size_t r = 0; r < arteJuan.size(); r++) {
            setTextoPantallaCompleta(xJuan, yArte + (int)r, arteJuan[r], 6);
        }
        for (size_t r = 0; r < arteKiana.size(); r++) {
            setTextoPantallaCompleta(xKiana, yArte + (int)r, arteKiana[r], 5);
        }

        std::string nomJuan = "Juan Blas";
        std::string codJuan = "Codigo: U20261B980";
        std::string rolJuan = "Diseno de Sistema y Mecanicas";
        setTextoPantallaCompleta(xJuan + (17 - longitudVisible(nomJuan)) / 2, yArte + 8, nomJuan, 4);
        setTextoPantallaCompleta(xJuan + (17 - longitudVisible(codJuan)) / 2, yArte + 9, codJuan, 3);
        setTextoPantallaCompleta(xJuan + (17 - longitudVisible(rolJuan)) / 2, yArte + 10, rolJuan, 8);

        std::string nomKiana = "Kiana Arancibia";
        std::string codKiana = "Codigo: U202619105";
        std::string rolKiana = "Entornos y Arquitectura Visual";
        setTextoPantallaCompleta(xKiana + (17 - longitudVisible(nomKiana)) / 2, yArte + 8, nomKiana, 4);
        setTextoPantallaCompleta(xKiana + (17 - longitudVisible(codKiana)) / 2, yArte + 9, codKiana, 3);
        setTextoPantallaCompleta(xKiana + (17 - longitudVisible(rolKiana)) / 2, yArte + 10, rolKiana, 8);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 29][x + c] = L'-';
            bufferColor[y + 29][x + c] = 8;
        }

        std::string pie = "[ Presiona ENTER o ESC para volver al menu principal ]";
        int cxPie = x + (ancho - longitudVisible(pie)) / 2;
        setTextoPantallaCompleta(cxPie, y + 31, pie, 4);

        dibujar();

#ifdef _WIN32
        while (true) {
            if (_kbhit()) {
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) tecla = _getch();
                if (tecla == 13 || tecla == 27 || tecla == 'e' || tecla == 'E' || tecla == 32) {
                    Beep(900, 25);
                    break;
                }
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    void mostrarLore() {
        int pagina = 0;
        int totalPaginas = 4;

        while (true) {
            limpiarBufferCompleto();
            int x = 6;
            int y = 2;
            int ancho = 108;
            int alto = 36;
            dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

            std::string t1 = "==========================================================================";
            std::string t2 = "     W I T C H   H A T   A T E L I E R :   E L   A R B O L   D E   P L A T A     ";
            std::string t3 = "                       --- SINOPSIS Y LORE ---                            ";
            int cxT1 = x + (ancho - longitudVisible(t1)) / 2;
            int cxT2 = x + (ancho - longitudVisible(t2)) / 2;
            int cxT3 = x + (ancho - longitudVisible(t3)) / 2;
            setTextoPantallaCompleta(cxT1, y + 1, t1, 8);
            setTextoPantallaCompleta(cxT2, y + 2, t2, 4);
            setTextoPantallaCompleta(cxT3, y + 3, t3, 3);
            setTextoPantallaCompleta(cxT1, y + 4, t1, 8);

            if (pagina == 0) {
                std::string tPag = "PAGINA 1 / 4: SINOPSIS & EL GRAN ENGANO DE LA MAGIA";
                setTextoPantallaCompleta(x + (ancho - longitudVisible(tPag)) / 2, y + 6, tPag, 4);

                setTextoPantallaCompleta(x + 5, y + 8, "En este mundo la magia no es un don innato: se dibuja en secreto con sellos y tinta.", 1);
                setTextoPantallaCompleta(x + 5, y + 9, "Tres personajes viviran el mismo conflicto desde perspectivas totalmente opuestas:", 3);

                setTextoPantallaCompleta(x + 5, y + 11, "=== EL GRAN ENGANO DE LA MAGIA ===", 4);
                setTextoPantallaCompleta(x + 5, y + 12, "La verdad es mucho mas simple y peligrosa: la magia se dibuja. Cualquier persona", 1);
                setTextoPantallaCompleta(x + 5, y + 13, "que sostenga un compas de punta de plata, papel arcano y tinta magica puede invocar", 1);
                setTextoPantallaCompleta(x + 5, y + 14, "el poder del viento, del fuego o del agua simplemente trazando circulos, flechas", 1);
                setTextoPantallaCompleta(x + 5, y + 15, "y sellos runicos. Precisamente porque cualquiera podria hacerlo, en el pasado remoto", 1);
                setTextoPantallaCompleta(x + 5, y + 16, "los hombres abusaron de los conjuros, alterando cuerpos humanos y desatando guerras.", 1);

                setTextoPantallaCompleta(x + 5, y + 18, "Para evitar la aniquilacion, la asamblea magica prohibio cualquier hechizo aplicado", 1);
                setTextoPantallaCompleta(x + 5, y + 19, "sobre el cuerpo humano, borro la memoria de la humanidad mediante magia de olvido y", 1);
                setTextoPantallaCompleta(x + 5, y + 20, "fundo la orden de los Caballeros Moralis (la policia inquisitorial de los brujos).", 1);
                setTextoPantallaCompleta(x + 5, y + 21, "Los hechiceros ocultaron sus rostros tras sombreros puntiagudos de ala estrecha,", 1);
                setTextoPantallaCompleta(x + 5, y + 22, "jurando que nadie fuera del circulo veria jamas el acto de dibujar un sello.", 1);

                setTextoPantallaCompleta(x + 5, y + 24, "Sin embargo, en las sombras aguardan los Sombreros de Ala Ancha (Brimmed Caps),", 6);
                setTextoPantallaCompleta(x + 5, y + 25, "una faccion proscrita que busca destruir esta mentira, reivindicando que la magia", 6);
                setTextoPantallaCompleta(x + 5, y + 26, "prohibida pertenece a todos y sembrando el caos para hacer caer el orden establecido.", 6);
            } else if (pagina == 1) {
                std::string tPag = "PAGINA 2 / 4: NIVEL 1 - COCO (EL ATELIER DE QIFREY)";
                setTextoPantallaCompleta(x + (ancho - longitudVisible(tPag)) / 2, y + 6, tPag, 2);

                setTextoPantallaCompleta(x + 5, y + 8, "=== QUIEN ES ===", 4);
                setTextoPantallaCompleta(x + 5, y + 9, "Una humilde aprendiz que petrifico accidentalmente a su madre tras copiar un hechizo", 1);
                setTextoPantallaCompleta(x + 5, y + 10, "prohibido. Su maestro Qifrey la acoge para que aprenda el contrahechizo que la salve.", 1);

                setTextoPantallaCompleta(x + 5, y + 12, "=== QUE HACE PARA GANAR ===", 2);
                setTextoPantallaCompleta(x + 5, y + 13, "Debe confeccionar su Manto de Aprendiz (Capa Magica):", 3);
                setTextoPantallaCompleta(x + 7, y + 15, "* Recibe la Tinta magica de Qifrey en su despacho al sur.", 1);
                setTextoPantallaCompleta(x + 7, y + 17, "* Mueve las cajas en la Torre de Agott para hallar el Grimorio de Trazos y el pozo.", 1);
                setTextoPantallaCompleta(x + 7, y + 19, "* Rescata al pinguino Myrphon en el laberinto de Serpentback para recibir", 1);
                setTextoPantallaCompleta(x + 9, y + 20, "la Pluma Termica de Richeh.", 1);
                setTextoPantallaCompleta(x + 7, y + 22, "* Quiebra la pared de roca de la cueva con la pluma y recoge la Fibra de Plata.", 1);
                setTextoPantallaCompleta(x + 7, y + 24, "* Entrega los 3 materiales a Qifrey para tejer la capa y consagrarse como bruja.", 1);
            } else if (pagina == 2) {
                std::string tPag = "PAGINA 3 / 4: NIVEL 2 - TARTAH (EL LABERINTO DE KALN)";
                setTextoPantallaCompleta(x + (ancho - longitudVisible(tPag)) / 2, y + 6, tPag, 6);

                setTextoPantallaCompleta(x + 5, y + 8, "=== QUIEN ES ===", 4);
                setTextoPantallaCompleta(x + 5, y + 9, "Un timido orfebre que sufre de 'vision de plata' (daltonismo magico). Busca", 1);
                setTextoPantallaCompleta(x + 5, y + 10, "desesperadamente a su amigo Coustas, un chico invalido tentado por las artes oscuras.", 1);

                setTextoPantallaCompleta(x + 5, y + 12, "=== QUE HACE PARA GANAR ===", 6);
                setTextoPantallaCompleta(x + 5, y + 13, "Debe encontrar el rastro de su amigo en el pueblo bajo la lluvia:", 3);
                setTextoPantallaCompleta(x + 7, y + 15, "* Recorre las calles empedradas esquivando a las patrullas inquisitoriales", 1);
                setTextoPantallaCompleta(x + 9, y + 16, "de los Caballeros Moralis.", 1);
                setTextoPantallaCompleta(x + 7, y + 18, "* Interroga a los aldeanos en la panaderia, botica y taberna reuniendo pistas", 1);
                setTextoPantallaCompleta(x + 9, y + 19, "y piezas ortopedicas de la silla de ruedas.", 1);
                setTextoPantallaCompleta(x + 7, y + 21, "* Descubre el escondite de la hechicera Ininia antes de que ejecuten a Coustas", 1);
                setTextoPantallaCompleta(x + 9, y + 22, "por el uso de magia prohibida.", 1);
            } else if (pagina == 3) {
                std::string tPag = "PAGINA 4 / 4: NIVEL 3 - QIFREY (EL GRAN ARBOL DE PLATA)";
                setTextoPantallaCompleta(x + (ancho - longitudVisible(tPag)) / 2, y + 6, tPag, 5);

                setTextoPantallaCompleta(x + 5, y + 8, "=== QUIEN ES ===", 4);
                setTextoPantallaCompleta(x + 5, y + 9, "El maestro sabio del atelier que oculta un oscuro trauma: de nino le arrancaron", 1);
                setTextoPantallaCompleta(x + 5, y + 10, "el ojo derecho con un ritual prohibido y vive obsesionado con cazar a los", 1);
                setTextoPantallaCompleta(x + 5, y + 11, "Sombreros de Ala Ancha para cobrar venganza.", 1);

                setTextoPantallaCompleta(x + 5, y + 13, "=== QUE HACE PARA GANAR ===", 5);
                setTextoPantallaCompleta(x + 5, y + 14, "Desata su poder en el santuario milenario del Arbol de Plata:", 3);
                setTextoPantallaCompleta(x + 7, y + 16, "* Utiliza magia de agua profunda y sellos de combate para abrirse paso", 1);
                setTextoPantallaCompleta(x + 9, y + 17, "entre las raices teluricas.", 1);
                setTextoPantallaCompleta(x + 7, y + 19, "* Enfrenta a los cabecillas de la conspiracion que orquestaron la desgracia", 1);
                setTextoPantallaCompleta(x + 9, y + 20, "de Coco y la tentacion de Coustas.", 1);
                setTextoPantallaCompleta(x + 7, y + 22, "* Decision final: Renuncia a su venganza ciega para romper el ciclo de odio", 1);
                setTextoPantallaCompleta(x + 9, y + 23, "y sellar el arbol, protegiendo el futuro de sus jovenes aprendices.", 1);
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 27][x + c] = L'=';
                bufferColor[y + 27][x + c] = 8;
            }

            std::string frase1 = "\"Un mismo trazo magico puede ser una esperanza (Coco), una barrera injusta (Tartah)";
            std::string frase2 = " o un arma de venganza (Qifrey): todo depende de la mirada de quien empuna la pluma.\"";
            int cxFrase1 = x + (ancho - longitudVisible(frase1)) / 2;
            int cxFrase2 = x + (ancho - longitudVisible(frase2)) / 2;
            setTextoPantallaCompleta(cxFrase1, y + 28, frase1, 4);
            setTextoPantallaCompleta(cxFrase2, y + 29, frase2, 4);

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 30][x + c] = L'=';
                bufferColor[y + 30][x + c] = 8;
            }

            std::string nav = "[A] Pagina Anterior   |   [D] Pagina Siguiente   |   [1..4] Ir a Pagina   |   [ESC / ENTER] Volver";
            int cxNav = x + (ancho - longitudVisible(nav)) / 2;
            setTextoPantallaCompleta(cxNav, y + 32, nav, 3);

            dibujar();

#ifdef _WIN32
            while (true) {
                if (_kbhit()) {
                    int tecla = _getch();
                    if (tecla == 0 || tecla == 224) tecla = _getch();
                    if (tecla == 'a' || tecla == 'A') {
                        pagina = (pagina - 1 + totalPaginas) % totalPaginas;
                        Beep(900, 20);
                        break;
                    } else if (tecla == 'd' || tecla == 'D') {
                        pagina = (pagina + 1) % totalPaginas;
                        Beep(900, 20);
                        break;
                    } else if (tecla >= '1' && tecla <= '4') {
                        pagina = tecla - '1';
                        Beep(900, 20);
                        break;
                    } else if (tecla == 13 || tecla == 27 || tecla == 'e' || tecla == 'E') {
                        Beep(800, 25);
                        return;
                    }
                }
                Sleep(20);
            }
#else
            std::cin.get();
            return;
#endif
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
        std::string tit2 = "E L   A R B O L   D E   P L A T A";
        int cx1 = x + (ancho - longitudVisible(tit1)) / 2;
        int cx2 = x + (ancho - longitudVisible(tit2)) / 2;
        setTextoPantallaCompleta(cx1, y + 1, tit1, 4);
        setTextoPantallaCompleta(cx2, y + 2, tit2, 3);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 3][x + c] = L'=';
            bufferColor[y + 3][x + c] = 8;
        }

        std::vector<std::string> arteAtelier = {
            R"(         /\                        .------.                        /\         )",
            R"(        /  \          /\          /  _()_  \          /\          /  \        )",
            R"(       /____\        /  \        |  (____)  |        /  \        /____\       )",
            R"(      |  __  |      /____\     .--\________/--.     /____\      |  __  |      )",
            R"(    .-| |  | |     |  __  |    |  [ ATELIER ] |    |  __  |     | |  | |-.    )",
            R"(   /  | |__| |     | |  | |    |  .---..---.  |    | |  | |     | |__| |  \   )",
            R"(  | ()|______|     | |__| |    |  |===|| * |  |    | |__| |     |______|() |  )",
            R"( _|___|______|_____|______|____|__|___||___|__|____|______|_____|______|___|__)",
            R"(/  / |   ||   / \      ||       |              |       ||      / \   ||   | \ \ )",
            R"(|  /  |   ||  /   \     ||       |  _        _  |       ||     /   \  ||   |  \ |)",
            R"(~^~^~^~^~^~^~/_____\~^~^~^~^~^~^~| [_]  []  [_] |~^~^~^~^~^~^~/_____\~^~^~^~^~)"
        };

        for (size_t r = 0; r < arteAtelier.size(); r++) {
            int cxArt = x + (ancho - longitudVisible(arteAtelier[r])) / 2;
            setTextoPantallaCompleta(cxArt, y + 4 + (int)r, arteAtelier[r], 2);
        }

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 15][x + c] = L'-';
            bufferColor[y + 15][x + c] = 8;
        }

        setTextoPantallaCompleta(x + 4, y + 16, "=== PROLOGO: EL SECRETO DE LA MAGIA ===", 4);
        setTextoPantallaCompleta(x + 4, y + 17, "En este mundo la magia no nace contigo: se traza con tinta de plata y sellos.", 1);
        setTextoPantallaCompleta(x + 4, y + 18, "Eres Coco, acogida en el atelier del sabio Qifrey tras una tragedia familiar.", 6);
        setTextoPantallaCompleta(x + 4, y + 19, "Tu mision: encontrar el contrahechizo prohibido para salvar a tu madre.", 3);
        setTextoPantallaCompleta(x + 4, y + 20, "Domina el arte del dibujo magico y confecciona tu propio Manto de Aprendiz.", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 21][x + c] = L'-';
            bufferColor[y + 21][x + c] = 8;
        }

        setTextoPantallaCompleta(x + 4, y + 22, "=== OBJETIVOS DEL NIVEL 1: EL ATELIER DE QIFREY ===", 2);
        setTextoPantallaCompleta(x + 4, y + 23, "* Habla con Maestro Qifrey al sur para recibir tu primera leccion arcana.", 1);
        setTextoPantallaCompleta(x + 4, y + 24, "* Reune los 3 componentes: Fibra de Plata, Tinta Arcana y Grimorio de Trazos.", 1);
        setTextoPantallaCompleta(x + 4, y + 25, "* Visita la Torre de Agott y ayuda a Richeh a rescatar a Myrphon.", 1);
        setTextoPantallaCompleta(x + 4, y + 26, "* Recolecta frascos de tinta perdidos para ganar la confianza de tu maestro.", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 28][x + c] = L'=';
            bufferColor[y + 28][x + c] = 8;
        }

        std::string pie = "[ Presiona ENTER o ESPACIO para iniciar la aventura con Coco ]";
        int cxPie = x + (ancho - longitudVisible(pie)) / 2;
        setTextoPantallaCompleta(cxPie, y + 31, pie, 4);

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

    void mostrarHistoriaNivel2() {
        limpiarBufferCompleto();
        int x = 7;
        int y = 2;
        int ancho = 106;
        int alto = 36;
        dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

        std::string tit1 = "W I T C H   H A T   A T E L I E R";
        std::string tit2 = "A C T O   I I :   E L   L A B E R I N T O   D E L   P U E B L O";
        int cx1 = x + (ancho - longitudVisible(tit1)) / 2;
        int cx2 = x + (ancho - longitudVisible(tit2)) / 2;
        setTextoPantallaCompleta(cx1, y + 2, tit1, 4);
        setTextoPantallaCompleta(cx2, y + 3, tit2, 2);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 5][x + c] = L'=';
            bufferColor[y + 5][x + c] = 8;
        }

        std::string sub1 = "=== LA TRAVESÍA DE TARTAH EN EL PUEBLO DE KALN ===";
        int cxSub1 = x + (ancho - longitudVisible(sub1)) / 2;
        setTextoPantallaCompleta(cxSub1, y + 7, sub1, 4);

        setTextoPantallaCompleta(x + 5, y + 9,  "Lejos del atelier de Qifrey, en el bullicioso y laberíntico pueblo de Kaln,", 1);
        setTextoPantallaCompleta(x + 5, y + 10, "Tartah, el joven orfebre y aprendiz de hechicero, busca con angustia", 1);
        setTextoPantallaCompleta(x + 5, y + 11, "a su amigo Coustas, quien ha desaparecido tras el asedio de sombras arcanas.", 1);

        setTextoPantallaCompleta(x + 5, y + 13, "Las calles empedradas están custodiadas por patrullas de los Caballeros Moralis,", 1);
        setTextoPantallaCompleta(x + 5, y + 14, "mientras misteriosos susurros señalan que la hechicera Ininia estuvo aquí.", 1);
        setTextoPantallaCompleta(x + 5, y + 15, "Tartah deberá sortear callejones, casas y cuarteles en busca de la verdad.", 1);

        setTextoPantallaCompleta(x + 5, y + 17, "El cielo se ha cerrado en una densa llovizna constante sobre los tejados del pueblo.", 1);
        setTextoPantallaCompleta(x + 5, y + 18, "Cada rincón oculta una pista vital y cada habitante guarda secretos.", 1);
        setTextoPantallaCompleta(x + 5, y + 19, "¡El destino de Coustas y los misterios del Árbol de Plata aguardan!", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 21][x + c] = L'-';
            bufferColor[y + 21][x + c] = 8;
        }

        std::string sub2 = "=== OBJETIVOS DEL NIVEL 2: EL LABERINTO DEL PUEBLO ===";
        int cxSub2 = x + (ancho - longitudVisible(sub2)) / 2;
        setTextoPantallaCompleta(cxSub2, y + 23, sub2, 2);

        setTextoPantallaCompleta(x + 5, y + 25, "* Recorre el laberinto de calles, chozas, panadería y botica de Kaln.", 1);
        setTextoPantallaCompleta(x + 5, y + 26, "* Investiga los letreros y testimonios dejados por los aldeanos.", 1);
        setTextoPantallaCompleta(x + 5, y + 27, "* Sigue los rastros de Ininia y busca pistas sobre el paradero de Coustas.", 1);
        setTextoPantallaCompleta(x + 5, y + 28, "* Encuentra el camino para cruzar hacia el Árbol de Plata.", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 30][x + c] = L'=';
            bufferColor[y + 30][x + c] = 8;
        }

        std::string pie = "[ Presiona ENTER para iniciar el Nivel 2 ]";
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

    void mostrarHistoriaNivel3() {
        limpiarBufferCompleto();
        int x = 7;
        int y = 2;
        int ancho = 106;
        int alto = 36;
        dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

        std::string tit1 = "W I T C H   H A T   A T E L I E R";
        std::string tit2 = "A C T O   I I I :   L A   E M B O S C A D A";
        int cx1 = x + (ancho - longitudVisible(tit1)) / 2;
        int cx2 = x + (ancho - longitudVisible(tit2)) / 2;
        setTextoPantallaCompleta(cx1, y + 2, tit1, 4);
        setTextoPantallaCompleta(cx2, y + 3, tit2, 2);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 5][x + c] = L'=';
            bufferColor[y + 5][x + c] = 8;
        }

        std::string sub1 = "=== LA TRAGEDIA DE COUSTAS Y EL GRAN ARBOL DE PLATA ===";
        int cxSub1 = x + (ancho - longitudVisible(sub1)) / 2;
        setTextoPantallaCompleta(cxSub1, y + 7, sub1, 4);

        setTextoPantallaCompleta(x + 5, y + 9,  "Tras recibir la Capa de Vuelo tejida por Coco y Tartah, Coustas emprende", 1);
        setTextoPantallaCompleta(x + 5, y + 10, "el peligroso viaje hacia las ruinas del Gran Arbol de Plata con Dagda.", 1);
        setTextoPantallaCompleta(x + 5, y + 11, "Pero una patrulla de hechiceros Sombreros de Ala Ancha los ha emboscado en el desfiladero.", 1);

        setTextoPantallaCompleta(x + 5, y + 13, "Debido a su parálisis corporal y su silla de ruedas, Coustas no puede luchar.", 1);
        setTextoPantallaCompleta(x + 5, y + 14, "Dagda se lanza al frente para resistir el asedio mientras Coustas debe esquivar", 1);
        setTextoPantallaCompleta(x + 5, y + 15, "las patrullas enemigas y avanzar entre los escombros rocosos de la senda.", 1);

        setTextoPantallaCompleta(x + 5, y + 17, "El destino pende de un hilo: Dagda resiste herido al final del camino,", 1);
        setTextoPantallaCompleta(x + 5, y + 18, "mientras la misteriosa hechicera Ininia acecha en la sombra con una oferta prohibida...", 1);
        setTextoPantallaCompleta(x + 5, y + 19, "¡Alcanza a tu protector antes de que sea demasiado tarde!", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 21][x + c] = L'-';
            bufferColor[y + 21][x + c] = 8;
        }

        std::string sub2 = "=== OBJETIVOS DEL NIVEL 3: LA EMBOSCADA ===";
        int cxSub2 = x + (ancho - longitudVisible(sub2)) / 2;
        setTextoPantallaCompleta(cxSub2, y + 23, sub2, 2);

        setTextoPantallaCompleta(x + 5, y + 25, "* Desplazate con Coustas a traves de las ruinas rocosas del desfiladero.", 1);
        setTextoPantallaCompleta(x + 5, y + 26, "* Esquiva a los bandidos patrulleros (evita el contacto directo).", 1);
        setTextoPantallaCompleta(x + 5, y + 27, "* Llega hasta el claro donde Dagda defiende el Gran Arbol de Plata.", 1);
        setTextoPantallaCompleta(x + 5, y + 28, "* Descubre la propuesta de Ininia y decide el destino de la Semilla Prohibida.", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 30][x + c] = L'=';
            bufferColor[y + 30][x + c] = 8;
        }

        std::string pie = "[ Presiona ENTER para iniciar el Nivel 3 ]";
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

    void mostrarEpilogoFinal(int puntajeTotal) {
        limpiarBufferCompleto();
        int x = 7;
        int y = 2;
        int ancho = 106;
        int alto = 36;
        dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

        std::string tit1 = "W I T C H   H A T   A T E L I E R";
        std::string tit2 = "E P I L O G O :   E L   A R B O L   D E   P L A T A";
        int cx1 = x + (ancho - longitudVisible(tit1)) / 2;
        int cx2 = x + (ancho - longitudVisible(tit2)) / 2;
        setTextoPantallaCompleta(cx1, y + 2, tit1, 4);
        setTextoPantallaCompleta(cx2, y + 3, tit2, 3);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 5][x + c] = L'=';
            bufferColor[y + 5][x + c] = 8;
        }

        std::string sub1 = "=== TRES PERSPECTIVAS, UN MISMO DESTINO ===";
        int cxSub1 = x + (ancho - longitudVisible(sub1)) / 2;
        setTextoPantallaCompleta(cxSub1, y + 7, sub1, 4);

        setTextoPantallaCompleta(x + 5, y + 9,  "* COCO (LA ESPERANZA):", 6);
        setTextoPantallaCompleta(x + 5, y + 10, "  Descubrio que la magia no es privilegio divino, sino amor y trazo constante.", 1);
        setTextoPantallaCompleta(x + 5, y + 11, "  Su manto de vuelo busco abrir los cielos a quien habia sido olvidado.", 1);

        setTextoPantallaCompleta(x + 5, y + 13, "* TARTAH (LA DUDA Y LA LEALTAD):", 2);
        setTextoPantallaCompleta(x + 5, y + 14, "  Cruzo las calles lluviosas de Kaln descubriendo la herida abierta del mundo.", 1);
        setTextoPantallaCompleta(x + 5, y + 15, "  Entendio que las leyes que protegen a unos, dejan desamparados a otros.", 1);

        setTextoPantallaCompleta(x + 5, y + 17, "* COUSTAS (LA TRAGEDIA Y EL PACTO):", 5);
        setTextoPantallaCompleta(x + 5, y + 18, "  En su desesperacion por salvar a Dagda y romper las cadenas de su cuerpo,", 1);
        setTextoPantallaCompleta(x + 5, y + 19, "  recibio la Semilla Prohibida del Arbol de Plata. Su historia apenas comienza...", 1);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 21][x + c] = L'-';
            bufferColor[y + 21][x + c] = 8;
        }

        std::string ptsStr = "PUNTAJE GLOBAL OBTENIDO: " + std::to_string(puntajeTotal) + " PUNTOS";
        int cxPts = x + (ancho - longitudVisible(ptsStr)) / 2;
        setTextoPantallaCompleta(cxPts, y + 23, ptsStr, 2);

        std::string finMsg = "¡Felicidades por completar la travesia del Arbol de Plata!";
        int cxFin = x + (ancho - longitudVisible(finMsg)) / 2;
        setTextoPantallaCompleta(cxFin, y + 25, finMsg, 1);

        std::string finMsg2 = "Gracias a las decisiones tomadas, el secreto fue protegido y los aprendices estan a salvo.";
        int cxFin2 = x + (ancho - longitudVisible(finMsg2)) / 2;
        setTextoPantallaCompleta(cxFin2, y + 26, finMsg2, 3);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 28][x + c] = L'=';
            bufferColor[y + 28][x + c] = 8;
        }

        std::string pie = "[ Presiona ENTER para finalizar la partida ]";
        int cxPie = x + (ancho - longitudVisible(pie)) / 2;
        setTextoPantallaCompleta(cxPie, y + 31, pie, 4);

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

    void aplicarLluvia(int tickLluvia, int camX, int camY, bool enSubMapa, bool (*esCuartoFunc)(int, int) = nullptr) {
        if (enSubMapa) return;
        for (int y = 0; y < altoTotal; y++) {
            int ym = camY + y;
            for (int x = 0; x < anchoJuego; x++) {
                int xm = camX + x;
                if (esCuartoFunc != nullptr && esCuartoFunc(xm, ym)) continue;
                wchar_t actual = buffer[y][x];
                if (actual == L'+' || actual == L'-' || actual == L'|' || actual == L'[' || actual == L']' || actual == L'!' || actual == L'O') continue;
                unsigned int u = (unsigned int)(x + tickLluvia) & 0xFFFFu;
                unsigned int v = (unsigned int)(y - tickLluvia * 2) & 0xFFFFu;
                unsigned int h = ((u * 2246822519u) ^ (v * 3266489917u));
                h = (h ^ (h >> 15)) * 2654435761u;
                if ((h % 100u) < 2u) {
                    if (actual == L'~' || actual == L'-') {
                        buffer[y][x] = L'.';
                        bufferColor[y][x] = 3;
                    } else {
                        buffer[y][x] = (h % 2u == 0) ? L'/' : L'\'';
                        bufferColor[y][x] = 3;
                    }
                }
            }
        }
    }

    void copiarViewport(const std::vector<std::string>& matrizMapa, int camaraX, int camaraY, int tickAnim = 0, bool enSubMapa = false, bool (*esCuartoFunc)(int, int) = nullptr, bool (*esCaminoFunc)(int, int) = nullptr, bool cuevaDesbloqueada = true) {
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
                    } else if (enSubMapa && ch == ' ') {
                        bool dentroRicheh = ((xMundo >= 6 && xMundo <= 14 && yMundo >= 3 && yMundo <= 7) ||
                                             (xMundo >= 6 && xMundo <= 74 && yMundo >= 8 && yMundo <= 15) ||
                                             (xMundo >= 44 && xMundo <= 74 && yMundo >= 5 && yMundo <= 18));
                        if (dentroRicheh) {
                            int r = (xMundo % 4 == 0 && yMundo % 2 == 0);
                            buffer[yPantalla][xPantalla] = r ? L'.' : L' ';
                            bufferColor[yPantalla][xPantalla] = 8;
                        } else {
                            int r = std::abs(xMundo * 7 + yMundo * 11) % 4;
                            buffer[yPantalla][xPantalla] = (r == 0) ? L'░' : ((r == 1) ? L'▒' : ((r == 2) ? L':' : L'.'));
                            bufferColor[yPantalla][xPantalla] = 8;
                        }
                    } else if (ch == ' ' && !enSubMapa) {
                        if (esCuartoFunc != nullptr && esCuartoFunc(xMundo, yMundo)) {
                            if (xMundo >= 63 && xMundo <= 122 && yMundo >= 21 && yMundo <= 43) {
                                int r = (xMundo * 11 + yMundo * 17) % 7;
                                buffer[yPantalla][xPantalla] = (r == 0) ? L'.' : ((r == 1) ? L',' : L' ');
                                bufferColor[yPantalla][xPantalla] = 8;
                            } else if (xMundo >= 385 && xMundo <= 463 && yMundo >= 83 && yMundo <= 123) {
                                if ((xMundo / 3 + yMundo / 2) % 2 == 0) {
                                    buffer[yPantalla][xPantalla] = L'.';
                                    bufferColor[yPantalla][xPantalla] = 3;
                                } else {
                                    buffer[yPantalla][xPantalla] = L' ';
                                    bufferColor[yPantalla][xPantalla] = 0;
                                }
                            } else if (xMundo >= 63 && xMundo <= 123 && yMundo >= 148 && yMundo <= 175) {
                                int r = (xMundo % 4 == 0 && yMundo % 2 == 0);
                                buffer[yPantalla][xPantalla] = r ? L'+' : L' ';
                                bufferColor[yPantalla][xPantalla] = 8;
                            } else if (xMundo >= 383 && xMundo <= 415 && yMundo >= 25 && yMundo <= 38) {
                                if (!cuevaDesbloqueada) {
                                    buffer[yPantalla][xPantalla] = L' ';
                                    bufferColor[yPantalla][xPantalla] = 0;
                                } else {
                                    int r = (xMundo * 11 + yMundo * 17) % 7;
                                    buffer[yPantalla][xPantalla] = (r == 0) ? L'.' : ((r == 1) ? L',' : L' ');
                                    bufferColor[yPantalla][xPantalla] = 8;
                                }
                            } else {
                                buffer[yPantalla][xPantalla] = L' ';
                                bufferColor[yPantalla][xPantalla] = 0;
                            }
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
                        if (!cuevaDesbloqueada && xMundo >= 387 && xMundo <= 462 && yMundo >= 20 && yMundo <= 45) {
                            buffer[yPantalla][xPantalla] = L' ';
                            bufferColor[yPantalla][xPantalla] = 0;
                        } else {
                            buffer[yPantalla][xPantalla] = (wchar_t)(unsigned char)ch;
                            if (ch == '/') {
                                if (xMundo >= 388 && xMundo <= 396 && yMundo >= 158 && yMundo <= 170) {
                                    if (xMundo == 391) buffer[yPantalla][xPantalla] = L'░';
                                    else if (xMundo == 392) buffer[yPantalla][xPantalla] = L'▒';
                                    else buffer[yPantalla][xPantalla] = L'█';
                                    bufferColor[yPantalla][xPantalla] = 8;
                                } else {
                                    bufferColor[yPantalla][xPantalla] = 2;
                                }
                            } else if (ch == '#') {
                                if (xMundo >= 190 && xMundo <= 255 && yMundo >= 94 && yMundo <= 112) {
                                    bufferColor[yPantalla][xPantalla] = 12;
                                } else {
                                    bufferColor[yPantalla][xPantalla] = 2;
                                }
                            } else if (ch == '&' || ch == '\\') {
                                bufferColor[yPantalla][xPantalla] = 2;
                            } else if (ch == '{' || ch == '}' || ch == '[' || ch == ']') {
                                bufferColor[yPantalla][xPantalla] = 12;
                            } else if (ch == '.' || ch == ':' || ch == '=') {
                                bufferColor[yPantalla][xPantalla] = 4;
                            } else if (ch == '+' || ch == '-' || ch == '|') {
                                bufferColor[yPantalla][xPantalla] = 8;
                            } else if (ch == 'O') {
                                bufferColor[yPantalla][xPantalla] = 8;
                            } else if (ch == 'T') {
                                bufferColor[yPantalla][xPantalla] = 3;
                            } else if (ch == '*') {
                                bufferColor[yPantalla][xPantalla] = 7;
                            } else if (ch == '<' || ch == '!') {
                                bufferColor[yPantalla][xPantalla] = 4;
                            } else {
                                bufferColor[yPantalla][xPantalla] = 0;
                            }
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

    void animarMyrphonRegresaARicheh(const std::vector<std::string>& matrizCuarto, int camX, int camY,
                                     int nivelNum, const std::string& nivelNom,
                                     const std::string& protaNom, int vida, int vidaMax) {
        int startX = 14;
        int endX = 64;
        int myrY = 12;

        int totalPasos = endX - startX;
        for (int p = 0; p <= totalPasos; p += 2) {
            int mx = startX + p;
            limpiarBuffer();
            copiarViewport(matrizCuarto, camX, camY, 0, true);

            int px = 8 - camX;
            int py = 8 - camY;
            setTextoJuego(px, py, " /\\ ", 6);
            setTextoJuego(px, py + 1, "/___\\", 6);
            setTextoJuego(px, py + 2, "(*u*)", 1);
            setTextoJuego(px, py + 3, "/ | \\", 6);

            int rx = 55 - camX;
            int ry = 12 - camY;
            int exprR = (mx >= 45) ? 1 : 0;
            if (exprR == 1) {
                setTextoJuego(rx + 2, ry - 1, "!", 7);
                setTextoJuego(rx, ry, "  /  \\ ", 4);
                setTextoJuego(rx, ry + 1, " /____\\", 4);
                setTextoJuego(rx, ry + 2, "( •o• )", 1);
                setTextoJuego(rx, ry + 3, " / || \\", 4);
            } else {
                setTextoJuego(rx, ry, "  /  \\ ", 4);
                setTextoJuego(rx, ry + 1, " /____\\", 4);
                setTextoJuego(rx, ry + 2, "( ._. )", 1);
                setTextoJuego(rx, ry + 3, " / || \\", 4);
            }

            int screenMx = mx - camX;
            int screenMy = myrY - camY;
            bool pasoAlterno = ((p / 2) % 2 == 0);
            setTextoJuego(screenMx, screenMy, "(o> ", 4);
            setTextoJuego(screenMx, screenMy + 1, pasoAlterno ? "/||\\" : "\\||/", 4);

            if (mx >= 52) {
                setTextoJuego(rx + 2, ry - 2, "<3", 5);
            }

            std::string msg = (mx < 50) ? "[ Myrphon corre emocionado hacia Richeh... ]" : "[ ¡Myrphon ha regresado con Richeh! ]";
            int cxMsg = (anchoJuego - longitudVisible(msg)) / 2;
            setTextoJuego(cxMsg, altoTotal - 3, msg, 7);

            renderizarPanelLateral(nivelNum, nivelNom, protaNom, vida, vidaMax, "Sotano de Richeh");
            dibujar();

#ifdef _WIN32
            if (p % 4 == 0) {
                Beep(1400 + (p * 8), 25);
            }
            Sleep(50);
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
#endif
        }

#ifdef _WIN32
        Beep(1200, 70);
        Beep(1500, 70);
        Beep(1800, 130);
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    void animarCaidaPozoMuerte(int nivelNum, const std::string& nivelNom,
                              const std::string& protaNom) {
        wchar_t block = L'█';
        int totalFrames = 10;
        int centroX = anchoJuego / 2;
        int centroY = altoTotal / 2 - 2;

        for (int frame = 0; frame < totalFrames; frame++) {
            for (int f = 0; f < altoTotal; f++) {
                for (int c = 0; c < anchoJuego; c++) {
                    int dx = (c - centroX);
                    int dy = (f - centroY) * 2;
                    int dist2 = dx * dx + dy * dy;
                    if (dist2 > 300) {
                        buffer[f][c] = block;
                        int r = std::abs(c * 7 + f * 13) % 3;
                        bufferColor[f][c] = (r == 0) ? 2 : ((r == 1) ? 9 : 11);
                    } else if (dist2 > 150) {
                        buffer[f][c] = block;
                        bufferColor[f][c] = 8;
                    } else {
                        buffer[f][c] = L' ';
                        bufferColor[f][c] = 0;
                    }
                }
            }

            int tamCoco = 8 - frame;
            if (tamCoco < 1) tamCoco = 1;
            int altoCoco = (tamCoco > 2) ? (tamCoco * 3 / 4) : 1;

            if (frame < 8) {
                int startX = centroX - tamCoco / 2;
                int startY = centroY - altoCoco / 2;
                for (int r = 0; r < altoCoco; r++) {
                    for (int c = 0; c < tamCoco; c++) {
                        if (startY + r >= 0 && startY + r < altoTotal && startX + c >= 0 && startX + c < anchoJuego) {
                            buffer[startY + r][startX + c] = block;
                            int col = (r == 0) ? 13 : ((r == 1) ? 4 : 6);
                            bufferColor[startY + r][startX + c] = col;
                        }
                    }
                }
            }

            std::string msg = "[ ¡Coco cae en el abismo del pozo! ]";
            int cxMsg = (anchoJuego - longitudVisible(msg)) / 2;
            setTextoJuego(cxMsg, altoTotal - 3, msg, 7);

            renderizarPanelLateral(nivelNum, nivelNom, protaNom, 0, 3, "Abismo Serpentback");
            dibujar();

#ifdef _WIN32
            Beep(520 - frame * 40, 60);
            Sleep(100);
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
#endif
        }

        int xModal = 18;
        int yModal = 12;
        int anchoM = 48;
        int altoM = 14;
        dibujarCaja(xModal, yModal, anchoM, altoM, 7);
        setTextoJuego(xModal + 16, yModal + 2, "P E R D I S T E", 7);
        for (int c = 1; c < anchoM - 1; c++) {
            buffer[yModal + 4][xModal + c] = L'-';
            bufferColor[yModal + 4][xModal + c] = 7;
        }
        setTextoJuego(xModal + 5, yModal + 6, "Coco ha caido en las profundidades", 1);
        setTextoJuego(xModal + 9, yModal + 7, "del laberinto Serpentback.", 1);
        setTextoJuego(xModal + 8, yModal + 10, "[ENTER] Intentar de nuevo", 4);
        dibujar();

#ifdef _WIN32
        while (true) {
            if (_kbhit()) {
                int t = _getch();
                if (t == 0 || t == 224) t = _getch();
                if (t == 13 || t == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    void animarCorazonRoto(int vidasRestantes) {
        int xModal = 20;
        int yModal = 10;
        int anchoM = 44;
        int altoM = 18;

        for (int frame = 0; frame < 4; frame++) {
            dibujarCaja(xModal, yModal, anchoM, altoM, (frame >= 2) ? 8 : 7);

            if (frame == 0) {
                setTextoJuego(xModal + 17, yModal + 2, "/\\_/\\", 7);
                setTextoJuego(xModal + 16, yModal + 3, "(     )", 7);
                setTextoJuego(xModal + 16, yModal + 4, " \\   / ", 7);
                setTextoJuego(xModal + 17, yModal + 5, " \\ / ", 7);
                setTextoJuego(xModal + 18, yModal + 6, "  v  ", 7);
            } else if (frame == 1) {
                setTextoJuego(xModal + 17, yModal + 2, "/\\|/\\", 7);
                setTextoJuego(xModal + 16, yModal + 3, "( / \\ )", 7);
                setTextoJuego(xModal + 16, yModal + 4, " \\/ \\/ ", 7);
                setTextoJuego(xModal + 17, yModal + 5, " | | ", 7);
                setTextoJuego(xModal + 18, yModal + 6, "  v  ", 7);
            } else if (frame == 2) {
                setTextoJuego(xModal + 14, yModal + 2, "/\\_     _/\\", 10);
                setTextoJuego(xModal + 13, yModal + 3, "( /       \\ )", 10);
                setTextoJuego(xModal + 14, yModal + 4, "\\/         \\/", 10);
                setTextoJuego(xModal + 15, yModal + 5, "/           \\", 10);
                setTextoJuego(xModal + 16, yModal + 6, "'             '", 10);
            } else {
                setTextoJuego(xModal + 12, yModal + 3, "/\\         /\\", 8);
                setTextoJuego(xModal + 11, yModal + 4, "(/           \\)", 8);
                setTextoJuego(xModal + 12, yModal + 5, "\\/           \\/", 8);
                setTextoJuego(xModal + 14, yModal + 6, ".             .", 8);
                setTextoJuego(xModal + 15, yModal + 7, "'             '", 8);
            }

            for (int c = 1; c < anchoM - 1; c++) {
                buffer[yModal + 8][xModal + c] = L'-';
                bufferColor[yModal + 8][xModal + c] = 8;
            }

            setTextoJuego(xModal + 13, yModal + 10, "¡TIEMPO AGOTADO!", 7);
            setTextoJuego(xModal + 6, yModal + 12, "Myrphon escapo entre las sombras...", 1);
            std::string vidStr = "-1 Vida  (Vidas restantes: " + std::to_string(vidasRestantes > 0 ? vidasRestantes : 0) + ")";
            setTextoJuego(xModal + 8, yModal + 13, vidStr, (vidasRestantes > 0) ? 4 : 7);
            setTextoJuego(xModal + 10, yModal + 15, "[ENTER] Continuar", 3);

            dibujar();
#ifdef _WIN32
            if (frame == 1) Beep(320, 70);
            else if (frame == 2) Beep(240, 90);
            else if (frame == 3) Beep(180, 110);
            Sleep(250);
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
#endif
        }

#ifdef _WIN32
        while (true) {
            if (_kbhit()) {
                int t = _getch();
                if (t == 0 || t == 224) t = _getch();
                if (t == 13 || t == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    void dibujarModalVictoriaMyrphon() {
        int xModal = 18;
        int yModal = 11;
        int anchoM = 48;
        int altoM = 16;

        dibujarCaja(xModal, yModal, anchoM, altoM, 2);

        setTextoJuego(xModal + 14, yModal + 2, "¡¡LO CONSEGUISTE!!", 2);
        setTextoJuego(xModal + 11, yModal + 3, "¡ATRAPASTE A MYRPHON!", 4);

        for (int c = 1; c < anchoM - 1; c++) {
            buffer[yModal + 5][xModal + c] = L'-';
            bufferColor[yModal + 5][xModal + c] = 2;
        }

        setTextoJuego(xModal + 17, yModal + 7, "( •v• )  *¡piii!*", 6);
        setTextoJuego(xModal + 5, yModal + 9, "Myrphon ahora esta a salvo contigo.", 1);
        setTextoJuego(xModal + 6, yModal + 10, "Llevaselo de regreso a Richeh.", 1);

        setTextoJuego(xModal + 13, yModal + 12, "[ +50 PUNTOS DE MISION ]", 4);
        setTextoJuego(xModal + 14, yModal + 14, "[ENTER] Continuar", 3);

        dibujar();

#ifdef _WIN32
        Beep(523, 100);
        Beep(659, 100);
        Beep(784, 150);
        while (true) {
            if (_kbhit()) {
                int t = _getch();
                if (t == 0 || t == 224) t = _getch();
                if (t == 13 || t == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    void animarDanioCoustas(int vidasRestantes) {
        int xModal = 18;
        int yModal = 10;
        int anchoM = 48;
        int altoM = 19;

        for (int frame = 0; frame < 4; frame++) {
            dibujarCaja(xModal, yModal, anchoM, altoM, (frame >= 2) ? 8 : 6);

            if (frame == 0) {
                setTextoJuego(xModal + 20, yModal + 2, " _/\\_ ", 6);
                setTextoJuego(xModal + 19, yModal + 3, "( 'v' )", 1);
                setTextoJuego(xModal + 19, yModal + 4, "[=====]", 6);
                setTextoJuego(xModal + 20, yModal + 5, "O   O", 8);
            } else if (frame == 1) {
                setTextoJuego(xModal + 19, yModal + 2, " _/\\_   *", 7);
                setTextoJuego(xModal + 18, yModal + 3, "( >_< ) /", 1);
                setTextoJuego(xModal + 17, yModal + 4, "/[=====]", 6);
                setTextoJuego(xModal + 17, yModal + 5, "O  .  O", 8);
            } else if (frame == 2) {
                setTextoJuego(xModal + 18, yModal + 2, "  _/\\_  ..", 8);
                setTextoJuego(xModal + 17, yModal + 3, " ( ;_; ) </3", 7);
                setTextoJuego(xModal + 16, yModal + 4, "//[====] ", 8);
                setTextoJuego(xModal + 16, yModal + 5, "o   .  o", 8);
            } else {
                setTextoJuego(xModal + 18, yModal + 2, "  _/\\_   .", 8);
                setTextoJuego(xModal + 17, yModal + 3, " ( u_u ) .", 8);
                setTextoJuego(xModal + 16, yModal + 4, "--[====]--", 8);
                setTextoJuego(xModal + 16, yModal + 5, "o       o", 8);
            }

            for (int c = 1; c < anchoM - 1; c++) {
                buffer[yModal + 7][xModal + c] = L'-';
                bufferColor[yModal + 7][xModal + c] = 8;
            }

            setTextoJuego(xModal + 11, yModal + 9, "¡COUSTAS PIERDE EL EQUILIBRIO!", 7);
            setTextoJuego(xModal + 4, yModal + 11, "La emboscada sacude la fragil silla de ruedas...", 1);
            setTextoJuego(xModal + 5, yModal + 12, "\"Mis piernas no responden... Debo resistir...\"", 6);
            std::string vidStr = "-1 Vida  (Vidas restantes: " + std::to_string(vidasRestantes > 0 ? vidasRestantes : 0) + ")";
            setTextoJuego(xModal + 10, yModal + 14, vidStr, (vidasRestantes > 0) ? 4 : 7);
            if (vidasRestantes <= 0) {
                setTextoJuego(xModal + 4, yModal + 15, "Coustas no puede continuar... Reiniciando senda.", 7);
            }
            setTextoJuego(xModal + 13, yModal + 17, "[ENTER / ESPACIO] Continuar", 3);

            dibujar();
#ifdef _WIN32
            if (frame == 0) Beep(330, 90);
            else if (frame == 1) Beep(262, 110);
            else if (frame == 2) Beep(220, 140);
            else if (frame == 3) Beep(165, 200);
            Sleep(260);
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(260));
#endif
        }

#ifdef _WIN32
        while (true) {
            if (_kbhit()) {
                int t = _getch();
                if (t == 0 || t == 224) t = _getch();
                if (t == 13 || t == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
    }

    void animarDespedidaCoustas() {
        int x = 12;
        int y = 4;
        int ancho = 96;
        int alto = 32;

        for (int frame = 0; frame < 5; frame++) {
            limpiarBufferCompleto();
            dibujarCajaPantallaCompleta(x, y, ancho, alto, 3);

            std::string tit1 = "W I T C H   H A T   A T E L I E R";
            std::string tit2 = "D E S P E D I D A :   E L   C A M I N O   D E   C O U S T A S";
            int cx1 = x + (ancho - longitudVisible(tit1)) / 2;
            int cx2 = x + (ancho - longitudVisible(tit2)) / 2;
            setTextoPantallaCompleta(cx1, y + 2, tit1, 4);
            setTextoPantallaCompleta(cx2, y + 3, tit2, 3);

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 5][x + c] = L'=';
                bufferColor[y + 5][x + c] = 8;
            }

            int treeX = x + 72;
            int treeY = y + 7;
            setTextoPantallaCompleta(treeX + 3, treeY,     "  .---.   ", 2);
            setTextoPantallaCompleta(treeX + 1, treeY + 1, " / *** \\  ", 2);
            setTextoPantallaCompleta(treeX,     treeY + 2, "|***T***| ", 2);
            setTextoPantallaCompleta(treeX + 1, treeY + 3, " \\ *** /  ", 2);
            setTextoPantallaCompleta(treeX + 3, treeY + 4, "  '---'   ", 2);
            setTextoPantallaCompleta(treeX + 4, treeY + 5, "   | |    ", 12);
            setTextoPantallaCompleta(treeX + 4, treeY + 6, "   | |    ", 12);
            setTextoPantallaCompleta(treeX + 3, treeY + 7, "  /   \\   ", 12);

            for (int r = y + 8; r <= y + 17; r++) {
                for (int c = x + 4; c < x + ancho - 4; c++) {
                    if ((r * 7 + c * 13 + frame * 5) % 17 == 0) {
                        buffer[r][c] = L'*';
                        bufferColor[r][c] = (frame % 2 == 0) ? 1 : 3;
                    }
                }
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 18][x + c] = L'_';
                bufferColor[y + 18][x + c] = 8;
            }

            int posX = x + 16 + frame * 10;
            int posY = y + 13;

            if (frame < 4) {
                setTextoPantallaCompleta(posX + 2, posY,     " _/\\_ ", 6);
                setTextoPantallaCompleta(posX + 1, posY + 1, "('v') \\", 1);
                setTextoPantallaCompleta(posX,     posY + 2, "=[====]~", 6);
                setTextoPantallaCompleta(posX + 1, posY + 3, (frame % 2 == 0 ? " O   O " : " o   o "), 8);
            } else {
                setTextoPantallaCompleta(posX + 2, posY + 1, " _/_ ", 8);
                setTextoPantallaCompleta(posX + 1, posY + 2, "('v')~", 6);
                setTextoPantallaCompleta(posX,     posY + 3, "=[=]o ", 8);
            }

            for (int c = 1; c < ancho - 1; c++) {
                buffer[y + 20][x + c] = L'-';
                bufferColor[y + 20][x + c] = 8;
            }

            if (frame == 0) {
                setTextoPantallaCompleta(x + 6, y + 22, "Coustas contempla por ultima vez las ruinas ancestrales y la sombra de Dagda.", 1);
                setTextoPantallaCompleta(x + 6, y + 24, "\"Gracias, Dagda... tu sacrificio no habra sido en vano.\"", 6);
            } else if (frame == 1) {
                setTextoPantallaCompleta(x + 6, y + 22, "Gira las ruedas de su silla hacia el sendero abierto... El viento sacude su manto.", 1);
                setTextoPantallaCompleta(x + 6, y + 24, "\"Mis piernas quiza nunca caminen, pero mi alma ya no estara atada.\"", 4);
            } else if (frame == 2) {
                setTextoPantallaCompleta(x + 6, y + 22, "La Semilla Prohibida late en su pecho como una promesa y una advertencia.", 1);
                setTextoPantallaCompleta(x + 6, y + 24, "\"Coco... Tartah... un dia comprenderan por que elegi este sendero.\"", 5);
            } else if (frame == 3) {
                setTextoPantallaCompleta(x + 6, y + 22, "Las hojas de plata caen en silencio mientras la figura del joven se aleja.", 1);
                setTextoPantallaCompleta(x + 6, y + 24, "\"No soy un heroe ni un monstruo. Solo soy Coustas, buscando su propia verdad.\"", 3);
            } else {
                setTextoPantallaCompleta(x + 6, y + 22, "Coustas desaparece en el horizonte crepuscular del Arbol de Plata...", 2);
                setTextoPantallaCompleta(x + 6, y + 24, "El silencio abraza las ruinas. Una nueva era de magia y desafio ha nacido.", 1);
                setTextoPantallaCompleta(x + 22, y + 28, "[ Presiona ENTER o ESPACIO para ver el desenlace ]", 4);
            }

            dibujar();

#ifdef _WIN32
            if (frame == 0) { Beep(440, 150); Beep(523, 180); }
            else if (frame == 1) { Beep(587, 160); Beep(659, 200); }
            else if (frame == 2) { Beep(698, 170); Beep(784, 220); }
            else if (frame == 3) { Beep(880, 200); Beep(784, 250); }
            else if (frame == 4) { Beep(659, 220); Beep(523, 350); }
            Sleep(frame == 4 ? 300 : 700);
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(frame == 4 ? 300 : 700));
#endif
        }

#ifdef _WIN32
        while (true) {
            if (_kbhit()) {
                int tecla = _getch();
                if (tecla == 0 || tecla == 224) tecla = _getch();
                if (tecla == 13 || tecla == 32) break;
            }
            Sleep(20);
        }
#else
        std::cin.get();
#endif
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
