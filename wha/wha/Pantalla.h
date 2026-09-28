#ifndef Pantalla_h
#define Pantalla_h

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Animacion.h"

#ifdef _WIN32
#include <windows.h>
#endif

class Pantalla {
private:
    int anchoTotal;
    int altoTotal;
    int anchoJuego;
    int anchoPanel;
    std::vector<std::string> buffer;
    std::string ultimoDialogoHablante;
    std::string ultimoDialogoTexto;
    std::string ultimoPromptTexto;

    std::string recortarOPad(const std::string& texto, int ancho) {
        if ((int)texto.length() >= ancho) {
            return texto.substr(0, ancho);
        }
        return texto + std::string(ancho - texto.length(), ' ');
    }

    std::string encuadrarFilaPanel(const std::string& texto, int ancho) {
        std::string res = " " + texto;
        if ((int)res.length() >= ancho - 1) {
            res = res.substr(0, ancho - 1);
        } else {
            res += std::string(ancho - 1 - res.length(), ' ');
        }
        return res + "|";
    }

public:
    Pantalla() : anchoTotal(120), altoTotal(40), anchoJuego(84), anchoPanel(35),
                 ultimoDialogoHablante(""), ultimoDialogoTexto(""), ultimoPromptTexto("") {
        buffer = std::vector<std::string>(altoTotal, std::string(anchoTotal, ' '));
    }

    ~Pantalla() {}

    void resetDialogoAnimado() {
        ultimoDialogoHablante = "";
        ultimoDialogoTexto = "";
    }

    int getAnchoTotal() const { return anchoTotal; }
    int getAltoTotal() const { return altoTotal; }
    int getAnchoJuego() const { return anchoJuego; }
    int getAnchoPanel() const { return anchoPanel; }

    void configurarConsola() {
#ifdef _WIN32
        system("mode con: cols=120 lines=40");
        system("title Witch Hat Atelier - Arbol de Plata");
        system("cls");
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            CONSOLE_CURSOR_INFO cursorInfo;
            if (GetConsoleCursorInfo(hOut, &cursorInfo)) {
                cursorInfo.bVisible = FALSE;
                SetConsoleCursorInfo(hOut, &cursorInfo);
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
            buffer[f] = std::string(anchoTotal, ' ');
            buffer[f][anchoJuego] = '|';
        }
    }

    void setPixelJuego(int x, int y, char c) {
        if (x >= 0 && x < anchoJuego && y >= 0 && y < altoTotal) {
            buffer[y][x] = c;
        }
    }

    void setTextoJuego(int x, int y, const std::string& texto) {
        if (y < 0 || y >= altoTotal) return;
        for (int i = 0; i < (int)texto.length(); i++) {
            int posX = x + i;
            if (posX >= 0 && posX < anchoJuego) {
                buffer[y][posX] = texto[i];
            }
        }
    }

    void dibujarCaja(int x, int y, int ancho, int alto) {
        if (x < 0 || y < 0 || x + ancho > anchoJuego || y + alto > altoTotal) return;
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                if (r == 0 || r == alto - 1) {
                    if (c == 0 || c == ancho - 1) {
                        buffer[y + r][x + c] = '+';
                    } else {
                        buffer[y + r][x + c] = '-';
                    }
                } else if (c == 0 || c == ancho - 1) {
                    buffer[y + r][x + c] = '|';
                } else {
                    buffer[y + r][x + c] = ' ';
                }
            }
        }
    }

    void dibujarPromptFlotante(const std::string& texto) {
        int anchoCaja = (int)texto.length() + 4;
        if (anchoCaja > anchoJuego - 4) anchoCaja = anchoJuego - 4;
        int startX = (anchoJuego - anchoCaja) / 2;
        int startY = altoTotal - 4;
        dibujarCaja(startX, startY, anchoCaja, 3);

        if (texto != ultimoPromptTexto && texto.find("Coco:") != std::string::npos) {
            ultimoPromptTexto = texto;
            for (size_t i = 0; i < texto.length(); i++) {
                if (startX + 2 + (int)i < anchoJuego - 2) {
                    setPixelJuego(startX + 2 + (int)i, startY + 1, texto[i]);
                }
                dibujar();
                std::this_thread::sleep_for(std::chrono::milliseconds(18));
            }
        } else {
            setTextoJuego(startX + 2, startY + 1, texto);
            if (texto.find("Coco:") == std::string::npos) {
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
        dibujarCaja(x, y, ancho, alto);

        std::string encabezado = " " + hablante;
        if (!rol.empty()) encabezado += " - " + rol;
        setTextoJuego(x + 2, y + 1, encabezado);

        std::string confStr = "Confianza: ";
        if (confianza == 0) confStr += "[0: Sin confianza]";
        else if (confianza == 1) confStr += "[1: Neutral]";
        else confStr += "[2: Confianza plena]";
        setTextoJuego(x + ancho - (int)confStr.length() - 2, y + 1, confStr);

        for (int c = 1; c < ancho - 1; c++) {
            buffer[y + 2][x + c] = '-';
        }

        std::string textoCompleto = "";
        for (size_t i = 0; i < lineasTexto.size(); i++) {
            textoCompleto += lineasTexto[i] + "\n";
        }

        bool esNuevo = (hablante != ultimoDialogoHablante || textoCompleto != ultimoDialogoTexto);

        if (esNuevo) {
            ultimoDialogoHablante = hablante;
            ultimoDialogoTexto = textoCompleto;

            for (int r = y + 3; r <= y + 8; r++) {
                for (int c = 1; c < ancho - 1; c++) {
                    buffer[r][x + c] = ' ';
                }
            }

            int filaActual = y + 3;
            for (size_t i = 0; i < lineasTexto.size() && filaActual < y + 9; i++) {
                for (size_t c = 0; c < lineasTexto[i].length(); c++) {
                    buffer[filaActual][x + 3 + (int)c] = lineasTexto[i][c];
                    dibujar();
                    std::this_thread::sleep_for(std::chrono::milliseconds(18));
                }
                filaActual++;
            }
        } else {
            int filaActual = y + 3;
            for (size_t i = 0; i < lineasTexto.size() && filaActual < y + 9; i++) {
                setTextoJuego(x + 3, filaActual, lineasTexto[i]);
                filaActual++;
            }
        }

        int filaDivisoria = y + 9;
        for (int c = 1; c < ancho - 1; c++) {
            buffer[filaDivisoria][x + c] = '-';
        }

        int filaOpciones = y + 10;
        for (size_t i = 0; i < opciones.size() && filaOpciones < y + alto - 2; i++) {
            setTextoJuego(x + 3, filaOpciones, opciones[i]);
            filaOpciones++;
        }

        setTextoJuego(x + 3, y + alto - 2, "Elige una opcion [1, 2, 3] o pulsa ESC para salir");
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
        setTextoJuego(x + 24, y + 1, "=== GUIA DE PERSONAJES ===");
        for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = '-';

        int fila = y + 3;
        for (size_t i = 0; i < nombres.size() && fila < y + 15; i++) {
            std::string prefijo = ((int)i == seleccionado) ? "-> " : "   ";
            std::string entrada = prefijo;
            if (desbloqueados[i]) {
                entrada += "[" + std::to_string(i + 1) + "] " + nombres[i] + " (" + roles[i] + ")";
                std::string confStr = " [Confianza: " + std::to_string(confianzas[i]) + "]";
                entrada += confStr;
            } else {
                entrada += "[" + std::to_string(i + 1) + "] [ ??? ] (Bloqueado)";
            }
            setTextoJuego(x + 3, fila, entrada);
            fila += 2;
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + 16][x + c] = '-';
        setTextoJuego(x + 3, y + 17, "DETALLES:");

        if (seleccionado >= 0 && seleccionado < (int)desbloqueados.size()) {
            if (desbloqueados[seleccionado]) {
                setTextoJuego(x + 5, y + 19, "Nombre: " + nombres[seleccionado]);
                setTextoJuego(x + 5, y + 20, "Rol:    " + roles[seleccionado]);
                std::string nivelConf = "Sin confianza";
                if (confianzas[seleccionado] == 1) nivelConf = "Cautela / Neutral";
                else if (confianzas[seleccionado] >= 2) nivelConf = "Confianza plena";
                setTextoJuego(x + 5, y + 21, "Nivel de confianza: " + nivelConf);
                setTextoJuego(x + 5, y + 23, "Descripcion:");
                setTextoJuego(x + 5, y + 24, descripciones[seleccionado]);
            } else {
                setTextoJuego(x + 5, y + 19, "Personaje aun no descubierto en este nivel.");
                setTextoJuego(x + 5, y + 20, "Explora el atelier o avanza en la historia.");
            }
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = '-';
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
        for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = '-';

        int fila = y + 3;
        for (int i = 0; i < 4; i++) {
            std::string prefijo = (i == seleccionado) ? "-> " : "   ";
            std::string entrada = prefijo + "[" + std::to_string(i + 1) + "] ";
            if (i < (int)nombres.size() && !nombres[i].empty()) {
                entrada += nombres[i];
                if (i < (int)tipos.size() && !tipos[i].empty()) {
                    entrada += " (" + tipos[i] + ")";
                }
            } else {
                entrada += "(Ranura vacia)";
            }
            setTextoJuego(x + 4, fila, entrada);
            fila += 2;
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + 12][x + c] = '-';
        setTextoJuego(x + 4, y + 13, "[DETALLES DEL ITEM]");

        if (seleccionado >= 0 && seleccionado < (int)nombres.size() && !nombres[seleccionado].empty()) {
            setTextoJuego(x + 5, y + 15, "Nombre:      " + nombres[seleccionado]);
            if (seleccionado < (int)tipos.size()) {
                setTextoJuego(x + 5, y + 17, "Tipo:        " + tipos[seleccionado]);
            }
            if (seleccionado < (int)descripciones.size()) {
                setTextoJuego(x + 5, y + 19, "Descripcion: ");
                setTextoJuego(x + 5, y + 21, descripciones[seleccionado]);
            }
        } else {
            setTextoJuego(x + 5, y + 15, "Ranura de mochila sin item asignado.");
            setTextoJuego(x + 5, y + 17, "Capacidad maxima: 4 items en este nivel.");
        }

        for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = '-';
        setTextoJuego(x + 5, y + alto - 2, "[W/S] Navegar   [1, 2, 3, 4] Elegir   [I/ESC] Cerrar");
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
            for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = '-';

            int fila = y + 4;
            for (size_t i = 0; i < titulos.size() && fila < y + 22; i++) {
                std::string prefijo = ((int)i == seleccionado) ? "-> " : "   ";
                std::string entrada = prefijo + "[" + std::to_string(i + 1) + "] ";
                if (desbloqueadas[i]) {
                    entrada += titulos[i];
                    std::string est = " [" + estados[i] + "]";
                    int espacioRestante = ancho - (int)entrada.length() - (int)est.length() - 5;
                    if (espacioRestante > 0) entrada += std::string(espacioRestante, ' ');
                    entrada += est;
                } else {
                    entrada += "########";
                    std::string est = " [BLOQUEADA]";
                    int espacioRestante = ancho - (int)entrada.length() - (int)est.length() - 5;
                    if (espacioRestante > 0) entrada += std::string(espacioRestante, ' ');
                    entrada += est;
                }
                setTextoJuego(x + 4, fila, entrada);
                fila += 2;
            }

            for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = '-';
            setTextoJuego(x + 4, y + alto - 2, "[W/S] Navegar   [ENTER] Ver detalle   [M/ESC] Cerrar");
        } else {
            std::string tituloDetalle = "=== MISION " + std::to_string(seleccionado + 1) + ": ";
            if (desbloqueadas[seleccionado]) {
                tituloDetalle += titulos[seleccionado] + " ===";
            } else {
                tituloDetalle += "######## ===";
            }
            if ((int)tituloDetalle.length() > ancho - 4) {
                tituloDetalle = tituloDetalle.substr(0, ancho - 7) + "...";
            }
            setTextoJuego(x + 4, y + 1, tituloDetalle);
            for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = '-';

            if (desbloqueadas[seleccionado]) {
                setTextoJuego(x + 4, y + 5, "Mision: " + titulos[seleccionado]);
                setTextoJuego(x + 4, y + 7, "Estado: " + estados[seleccionado]);
                for (int c = 4; c < ancho - 4; c++) buffer[y + 9][x + c] = '-';
                setTextoJuego(x + 4, y + 11, "Descripcion:");
                setTextoJuego(x + 4, y + 13, descripciones[seleccionado]);
            } else {
                setTextoJuego(x + 4, y + 5, "Mision Bloqueada");
                setTextoJuego(x + 4, y + 7, "Estado: BLOQUEADA");
                for (int c = 4; c < ancho - 4; c++) buffer[y + 9][x + c] = '-';
                setTextoJuego(x + 4, y + 11, "Descripcion:");
                setTextoJuego(x + 4, y + 13, "Esta mision aun no ha sido desbloqueada.");
                setTextoJuego(x + 4, y + 15, "Completa los objetivos previos para acceder.");
            }

            for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 3][x + c] = '-';
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

        setTextoJuego(x + 18, y + 1, "=== ESTADISTICAS DE FIN DE NIVEL ===");
        for (int c = 1; c < ancho - 1; c++) buffer[y + 2][x + c] = '-';

        setTextoJuego(x + 4, y + 4, "Nivel superado:   NIVEL " + std::to_string(nivel) + " - " + nombreNivel);
        setTextoJuego(x + 4, y + 5, "Maga:             " + prota + " (Aprendiz)");
        setTextoJuego(x + 4, y + 6, "Objetivo:         Capa Magica Crafteada con Exito");

        for (int c = 1; c < ancho - 1; c++) buffer[y + 8][x + c] = '-';
        setTextoJuego(x + 4, y + 9, "[DESGLOSE DE PUNTUACION SEGUN TIEMPO Y MISIONES]");

        int min = segs / 60;
        int seg = segs % 60;
        std::string tStr = (min < 10 ? "0" : "") + std::to_string(min) + ":" + (seg < 10 ? "0" : "") + std::to_string(seg);

        setTextoJuego(x + 4, y + 11, "Tiempo empleado:                      " + tStr);
        setTextoJuego(x + 4, y + 13, "Puntos por velocidad de tiempo:       +" + std::to_string(bonoTiempo) + " pts");
        setTextoJuego(x + 4, y + 15, "Puntos por misiones y recoleccion:    +" + std::to_string(puntosMisiones) + " pts");
        for (int c = 4; c < ancho - 4; c++) buffer[y + 17][x + c] = '-';

        int total = bonoTiempo + puntosMisiones;
        setTextoJuego(x + 4, y + 19, "PUNTAJE TOTAL DEL NIVEL:              " + std::to_string(total) + " PTS");

        for (int c = 1; c < ancho - 1; c++) buffer[y + 21][x + c] = '-';
        setTextoJuego(x + 4, y + 22, "[LOGROS Y ESTADO DE LORE]");
        std::string confStr = (confianzaQifrey == 2) ? "Confianza Plena (Nivel Maximo 2/2)" : "Neutral (1/2)";
        setTextoJuego(x + 4, y + 24, "Vinculo con Maestro Qifrey:           " + confStr);
        setTextoJuego(x + 4, y + 25, "Objeto legendario desbloqueado:       Capa Magica de Vuelo");
        setTextoJuego(x + 4, y + 26, "Habilidad de vuelo:                   Activada para Coco");

        for (int c = 1; c < ancho - 1; c++) buffer[y + alto - 4][x + c] = '-';
        setTextoJuego(x + 4, y + alto - 3, "[1 / ENTER] Avanzar al Nivel 2    [C] Seguir explorando");
        setTextoJuego(x + 4, y + alto - 2, "[ESC] Salir del juego");
    }

    void renderizarPanelLateral(int nivel, const std::string& nombreNivel,
                               const std::string& protagonista, int vida, int vidaMax) {
        std::vector<std::string> lineasPanel(altoTotal, std::string(anchoPanel, ' '));

        std::string separador = std::string(anchoPanel - 1, '-') + "|";
        std::string bordeCaja = std::string(anchoPanel - 1, '-') + "+";

        lineasPanel[0]  = bordeCaja;
        lineasPanel[1]  = encuadrarFilaPanel("     WITCH HAT ATELIER", anchoPanel);
        lineasPanel[2]  = encuadrarFilaPanel("      ARBOL DE PLATA", anchoPanel);
        lineasPanel[3]  = separador;
        lineasPanel[4]  = encuadrarFilaPanel("NIVEL " + std::to_string(nivel) + ": " + nombreNivel, anchoPanel);
        lineasPanel[5]  = encuadrarFilaPanel("MAGA: " + protagonista + " (Aprendiz)", anchoPanel);

        std::string corazones = "";
        for (int i = 0; i < vidaMax; i++) {
            if (i < vida) corazones += "<3 ";
            else corazones += ".. ";
        }
        lineasPanel[6]  = encuadrarFilaPanel("VIDA: " + corazones, anchoPanel);
        lineasPanel[7]  = separador;
        lineasPanel[8]  = encuadrarFilaPanel("[CONTROLES]", anchoPanel);
        lineasPanel[9]  = separador;
        lineasPanel[10] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[11] = encuadrarFilaPanel("[W, A, S, D]  Moverse en mapa", anchoPanel);
        lineasPanel[12] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[13] = encuadrarFilaPanel("[ENTER]       Interactuar", anchoPanel);
        lineasPanel[14] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[15] = encuadrarFilaPanel("[M]           Misiones", anchoPanel);
        lineasPanel[16] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[17] = encuadrarFilaPanel("[I]           Inventario", anchoPanel);
        lineasPanel[18] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[19] = encuadrarFilaPanel("[P]           Personajes", anchoPanel);
        lineasPanel[20] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[21] = encuadrarFilaPanel("[ESC]         Salir / Pausa", anchoPanel);
        lineasPanel[22] = encuadrarFilaPanel("", anchoPanel);
        lineasPanel[23] = separador;

        for (int f = 24; f < altoTotal - 1; f++) {
            lineasPanel[f] = encuadrarFilaPanel("", anchoPanel);
        }
        lineasPanel[altoTotal - 1] = bordeCaja;

        int colInicioPanel = anchoJuego + 1;
        for (int f = 0; f < altoTotal; f++) {
            for (int c = 0; c < anchoPanel; c++) {
                buffer[f][colInicioPanel + c] = lineasPanel[f][c];
            }
        }
    }

    void dibujarPantallaMensajeCentrado(const std::string& texto) {
        limpiarBuffer();
        int fila = altoTotal / 2;
        int col = (anchoTotal - (int)texto.length()) / 2;
        if (col < 0) col = 0;
        if (fila >= 0 && fila < altoTotal) {
            for (size_t i = 0; i < texto.length() && col + (int)i < anchoTotal; i++) {
                buffer[fila][col + i] = texto[i];
            }
        }
    }

    void copiarViewport(const std::vector<std::string>& matrizMapa, int camaraX, int camaraY) {
        int filasMapa = (int)matrizMapa.size();
        if (filasMapa == 0) return;
        int columnasMapa = (int)matrizMapa[0].size();

        for (int yPantalla = 0; yPantalla < altoTotal; yPantalla++) {
            int yMundo = camaraY + yPantalla;
            for (int xPantalla = 0; xPantalla < anchoJuego; xPantalla++) {
                int xMundo = camaraX + xPantalla;
                if (yMundo >= 0 && yMundo < filasMapa && xMundo >= 0 && xMundo < columnasMapa) {
                    buffer[yPantalla][xPantalla] = matrizMapa[yMundo][xMundo];
                } else {
                    buffer[yPantalla][xPantalla] = ' ';
                }
            }
        }
    }

    void dibujar() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD pos = { 0, 0 };
        SetConsoleCursorPosition(hOut, pos);
#else
        std::cout << "\033[H";
#endif
        std::string frameCompleto = "";
        for (int f = 0; f < altoTotal; f++) {
            frameCompleto += buffer[f];
            if (f < altoTotal - 1) {
                frameCompleto += "\n";
            }
        }
        std::cout << frameCompleto;
        std::cout.flush();
    }
};

#endif
