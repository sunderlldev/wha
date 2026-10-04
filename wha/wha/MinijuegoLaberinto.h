#ifndef MinijuegoLaberinto_h
#define MinijuegoLaberinto_h

#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

#include "Pantalla.h"
#include "GestorAudio.h"
#include "Protagonista.h"
#include "GestorMisiones.h"

class MinijuegoLaberinto {
private:
	std::vector<std::string> mapa;
	int cocoX;
	int cocoY;
	int myrphonX;
	int myrphonY;
	int enemigoX[4];
	int enemigoY[4];
	int enemigoDx[4];
	int enemigoDy[4];
	int invulnerableTicks;

	void cargarMapa() {
		mapa.clear();
		mapa.push_back(R"(##############################################################################)");
		mapa.push_back(R"(# #     #         #           #       #   #     #       #       #     #     ##)");
		mapa.push_back(R"(# ##### # ####  ### # ####  # ##### # #   # #  ## ###   # # ####  # ### # ####)");
		mapa.push_back(R"(#   #       # #     #                   # #   #     # #   # #     #   #     ##)");
		mapa.push_back(R"(### # # # # # ####  # ##### # #######     # # # ##### #######  #####  # ### ##)");
		mapa.push_back(R"(# # #     # #             # # #         #   #   #     #     #   #   #   #   ##)");
		mapa.push_back(R"(# # # ### #   #  #####  # # ### # # #  ## # # ###  #### ### ### # # ####  # ##)");
		mapa.push_back(R"(# # # # #   # #         # #     #       #       #         #     # #       # ##)");
		mapa.push_back(R"(# # # # #  #### ##### # # #   # # ##### # ### # ##### # # ### # # ####### # ##)");
		mapa.push_back(R"(# # #                 # #           #   # #     #   #   #   #     #   #   # ##)");
		mapa.push_back(R"(# # ### # ## #  ####### #  ######## # #     ## ####  ####  ## # #   # # # # ##)");
		mapa.push_back(R"(#     #   #             # # #     # # #           #       #       #         ##)");
		mapa.push_back(R"(#  ## #####   ##  ##  #   # # # # #    # #####  # # # ### #  # ## #  #########)");
		mapa.push_back(R"(# #   #     #     # #   # #   #   #             # # #             # #   #   ##)");
		mapa.push_back(R"(# ##  # ### # ###   # ### ##  #  ## #######  # #  ### # # #######   # # # # ##)");
		mapa.push_back(R"(#   #   #   # #     #   #     #                                 # #   #   # ##)");
		mapa.push_back(R"(##  ####  # # # ### #   ## ##  # ###### ### #######  #### ### # # ##### #   ##)");
		mapa.push_back(R"(# #         # # # # #             #     #   #   #     #       #       #     ##)");
		mapa.push_back(R"(# ####### #   # # #   ####     ## ######  # # # # # # #    ##### #    ###   ##)");
		mapa.push_back(R"(#   #     #   #   #   #   #   #   #       # #         # #         # # #     ##)");
		mapa.push_back(R"(#  ## # #  #### # ## ## # ### # # # ##  ### #   ###     # ## # #### #   ######)");
		mapa.push_back(R"(#     #       # #         #   # #         #           # #                   ##)");
		mapa.push_back(R"(# # # # #######   ## #### # ### ###  ## # ##### ####### # ### # ######### # ##)");
		mapa.push_back(R"(# #   #         # #   #       #   #   # #     #       #       # #         # ##)");
		mapa.push_back(R"(# # # ######  ##  #  ##  #### ###  ###  #   # # # ##  #   ###   ##  #  #### ##)");
		mapa.push_back(R"(# # # #           #   # #       #   #   #           # #     #               ##)");
		mapa.push_back(R"(# #   ### #  ######   # #  #  ##### # ##   #### ### # # ####  ### # ##### # ##)");
		mapa.push_back(R"(# # #     #         #   #         #   #   #                           #   # ##)");
		mapa.push_back(R"(# # ##### ##  # ###### ######## ### ### ###  #### ##  #  # #### ####    ##  ##)");
		mapa.push_back(R"(# #           #               #                 #       #               #   ##)");
		mapa.push_back(R"(##############################################################################)");
	}

public:
	MinijuegoLaberinto() {
		cargarMapa();
		reiniciar();
	}

	~MinijuegoLaberinto() {}

	void reiniciar() {
		cocoX = 3;
		cocoY = 3;
		myrphonX = 73;
		myrphonY = 27;

		enemigoX[0] = 18; enemigoY[0] = 3;  enemigoDx[0] = 0;  enemigoDy[0] = 1;
		enemigoX[1] = 36; enemigoY[1] = 13; enemigoDx[1] = 1;  enemigoDy[1] = 0;
		enemigoX[2] = 54; enemigoY[2] = 7;  enemigoDx[2] = 0;  enemigoDy[2] = 1;
		enemigoX[3] = 44; enemigoY[3] = 25; enemigoDx[3] = -1; enemigoDy[3] = 0;

		invulnerableTicks = 0;
	}

	bool esPared(int x, int y) const {
		if (y < 0 || y >= (int)mapa.size()) return true;
		if (x < 0 || x >= (int)mapa[y].size()) return true;
		return mapa[y][x] == '#';
	}

	void moverEnemigos() {
		for (int i = 0; i < 4; i++) {
			int nx = enemigoX[i] + enemigoDx[i];
			int ny = enemigoY[i] + enemigoDy[i];
			if (esPared(nx, ny)) {
				enemigoDx[i] = -enemigoDx[i];
				enemigoDy[i] = -enemigoDy[i];
			} else {
				enemigoX[i] = nx;
				enemigoY[i] = ny;
			}
		}
	}

	void moverMyrphon() {
		int dx = 0;
		int dy = 0;
		if (abs(cocoX - myrphonX) < 8 && abs(cocoY - myrphonY) < 6) {
			if (cocoX < myrphonX) dx = 1;
			else if (cocoX > myrphonX) dx = -1;
			if (cocoY < myrphonY) dy = 1;
			else if (cocoY > myrphonY) dy = -1;
		} else {
			int r = rand() % 5;
			if (r == 0) dx = 1;
			else if (r == 1) dx = -1;
			else if (r == 2) dy = 1;
			else if (r == 3) dy = -1;
		}

		if (dx != 0 && !esPared(myrphonX + dx, myrphonY)) {
			myrphonX += dx;
		} else if (dy != 0 && !esPared(myrphonX, myrphonY + dy)) {
			myrphonY += dy;
		}
	}

	int ejecutar(Pantalla& pantalla, GestorAudio& audio, Protagonista* protagonista, GestorMisiones* gestorMisiones) {
		if (protagonista == nullptr) return 0;
		audio.reproducirMinijuego();
		reiniciar();

		auto tInicio = std::chrono::steady_clock::now();
		int tickAnim = 0;

		while (true) {
			auto tAhora = std::chrono::steady_clock::now();
			int segsPasados = (int)std::chrono::duration_cast<std::chrono::seconds>(tAhora - tInicio).count();
			int segsRestantes = 30 - segsPasados;

			if (segsRestantes <= 0) {
				protagonista->setVida(protagonista->getVida() - 1);
				pantalla.animarCorazonRoto(protagonista->getVida());
				if (protagonista->getVida() <= 0) {
					pantalla.animarCaidaPozoMuerte(1, "Atelier de Qifrey", protagonista->getNombre());
					return 2;
				} else {
					reiniciar();
					tInicio = std::chrono::steady_clock::now();
					continue;
				}
			}

			pantalla.limpiarBuffer();

			pantalla.dibujarCaja(1, 0, 82, 3, 4);
			std::string segsStr = (segsRestantes < 10 ? "0" : "") + std::to_string(segsRestantes);
			std::string hudTexto = "TIEMPO: " + segsStr + "s | VIDAS: ";
			for (int v = 0; v < protagonista->getVidaMaxima(); v++) {
				if (v < protagonista->getVida()) hudTexto += "<3 ";
				else hudTexto += ".. ";
			}
			hudTexto += "| ¡ATRAPA A MYRPHON!";
			pantalla.setTextoJuego(3, 1, hudTexto, 4);

			int offsetY = 4;
			int offsetX = 2;

			for (int r = 0; r < (int)mapa.size() && offsetY + r < 36; r++) {
				for (int c = 0; c < (int)mapa[r].size() && offsetX + c < 82; c++) {
					char ch = mapa[r][c];
					if (ch == '#') {
						pantalla.setPixelJuego(offsetX + c, offsetY + r, '#', 8);
					}
				}
			}

			pantalla.setPixelJuego(offsetX + myrphonX, offsetY + myrphonY, 'm', 4);
			pantalla.setPixelJuego(offsetX + myrphonX + 1, offsetY + myrphonY, '!', 4);

			for (int i = 0; i < 4; i++) {
				pantalla.setPixelJuego(offsetX + enemigoX[i], offsetY + enemigoY[i], 'X', 7);
			}

			if (invulnerableTicks == 0 || (invulnerableTicks % 2 == 0)) {
				pantalla.setPixelJuego(offsetX + cocoX, offsetY + cocoY, '(', 6);
				pantalla.setPixelJuego(offsetX + cocoX + 1, offsetY + cocoY, '*', 6);
				pantalla.setPixelJuego(offsetX + cocoX + 2, offsetY + cocoY, ')', 6);
			} else {
				pantalla.setPixelJuego(offsetX + cocoX, offsetY + cocoY, '(', 3);
				pantalla.setPixelJuego(offsetX + cocoX + 1, offsetY + cocoY, '*', 3);
				pantalla.setPixelJuego(offsetX + cocoX + 2, offsetY + cocoY, ')', 3);
			}

			pantalla.setTextoJuego(3, 37, "[W,A,S,D] Moverse a toda prisa       [ESC] Rendirse", 8);

			pantalla.renderizarPanelLateral(1, "Atelier de Qifrey", protagonista->getNombre(),
			                               protagonista->getVida(), protagonista->getVidaMaxima(),
			                               "Laberinto Serpentback");
			pantalla.dibujar();

			tickAnim++;
			if (tickAnim % 3 == 0) {
				moverEnemigos();
			}
			if (tickAnim % 5 == 0) {
				moverMyrphon();
			}

#ifdef _WIN32
			if (_kbhit()) {
				int tecla = _getch();
				if (tecla == 0 || tecla == 224) tecla = _getch();
				if (tecla == 27) {
					return 0;
				}
				int dx = 0;
				int dy = 0;
				if (tecla == 'w' || tecla == 'W') dy = -1;
				else if (tecla == 's' || tecla == 'S') dy = 1;
				else if (tecla == 'a' || tecla == 'A') dx = -1;
				else if (tecla == 'd' || tecla == 'D') dx = 1;

				if (dx != 0 || dy != 0) {
					int nx = cocoX + dx;
					int ny = cocoY + dy;
					if (!esPared(nx, ny) && !esPared(nx + 2, ny)) {
						cocoX = nx;
						cocoY = ny;
					}
				}
			}
#endif

			for (int i = 0; i < 4; i++) {
				if (abs(cocoX - enemigoX[i]) <= 1 && abs(cocoY - enemigoY[i]) <= 1) {
					if (invulnerableTicks == 0) {
						protagonista->setVida(protagonista->getVida() - 1);
						invulnerableTicks = 20;
#ifdef _WIN32
						Beep(220, 80);
#endif
						if (protagonista->getVida() <= 0) {
							pantalla.animarCaidaPozoMuerte(1, "Atelier de Qifrey", protagonista->getNombre());
							return 2;
						}
					}
				}
			}

			if (abs(cocoX - myrphonX) <= 2 && abs(cocoY - myrphonY) <= 1) {
				pantalla.dibujarModalVictoriaMyrphon();
				if (gestorMisiones != nullptr) {
					gestorMisiones->sumarPuntosMision(50);
				}
				return 1;
			}

			if (invulnerableTicks > 0) {
				invulnerableTicks--;
			}

#ifdef _WIN32
			Sleep(40);
#else
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
#endif
		}
		return 0;
	}
};

#endif
