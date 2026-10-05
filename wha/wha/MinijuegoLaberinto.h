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
		mapa.push_back(R"(################################################################################)");
		mapa.push_back(R"(##                ####                                ####                    ##)");
		mapa.push_back(R"(##                ####                                ####                    ##)");
		mapa.push_back(R"(##    ################################    ################################    ##)");
		mapa.push_back(R"(##    ################################    ################################    ##)");
		mapa.push_back(R"(##                            ####                                ####        ##)");
		mapa.push_back(R"(##                            ####                                ####        ##)");
		mapa.push_back(R"(##############    ################################    ##########################)");
		mapa.push_back(R"(##############    ################################    ##########################)");
		mapa.push_back(R"(##        ####                            ####                                ##)");
		mapa.push_back(R"(##        ####                            ####                                ##)");
		mapa.push_back(R"(##    ####################    ############################################    ##)");
		mapa.push_back(R"(##    ####################    ############################################    ##)");
		mapa.push_back(R"(##                    ####                                          ####      ##)");
		mapa.push_back(R"(##                    ####                                          ####      ##)");
		mapa.push_back(R"(##############    ####################    ####################    ##############)");
		mapa.push_back(R"(##############    ####################    ####################    ##############)");
		mapa.push_back(R"(##                              ####                                          ##)");
		mapa.push_back(R"(##                              ####                                          ##)");
		mapa.push_back(R"(##    ############################################    ####################    ##)");
		mapa.push_back(R"(##    ############################################    ####################    ##)");
		mapa.push_back(R"(##            ####                          ####                              ##)");
		mapa.push_back(R"(##            ####                          ####                              ##)");
		mapa.push_back(R"(##########################    ################################    ##############)");
		mapa.push_back(R"(##########################    ################################    ##############)");
		mapa.push_back(R"(##                              ####                                          ##)");
		mapa.push_back(R"(##                              ####                                          ##)");
		mapa.push_back(R"(##############    ####################    ################################    ##)");
		mapa.push_back(R"(##                                                                            ##)");
		mapa.push_back(R"(##                                                                            ##)");
		mapa.push_back(R"(################################################################################)");
	}

public:
	MinijuegoLaberinto() {
		cargarMapa();
		reiniciar();
	}

	~MinijuegoLaberinto() {}

	void reiniciar() {
		cocoX = 3;
		cocoY = 1;
		myrphonX = 72;
		myrphonY = 28;

		for (int i = 0; i < 4; i++) {
			bool ubicado = false;
			int intentos = 0;
			while (!ubicado && intentos < 300) {
				intentos++;
				int rx = 2 + (rand() % 73);
				int ry = 1 + (rand() % 28);
				if (colisiona(rx, ry)) continue;
				if (abs(rx - cocoX) < 18 && abs(ry - cocoY) < 6) continue;
				if (abs(rx - myrphonX) < 6 && abs(ry - myrphonY) < 3) continue;
				bool solapado = false;
				for (int j = 0; j < i; j++) {
					if (abs(rx - enemigoX[j]) < 5 && abs(ry - enemigoY[j]) < 3) {
						solapado = true;
						break;
					}
				}
				if (solapado) continue;
				enemigoX[i] = rx;
				enemigoY[i] = ry;
				ubicado = true;
			}
			if (!ubicado) {
				enemigoX[i] = (i % 2 == 0) ? 20 + i * 15 : 30 + i * 10;
				enemigoY[i] = 5 + i * 6;
			}
			if (i % 2 == 0) {
				enemigoDx[i] = (rand() % 2 == 0) ? 1 : -1;
				enemigoDy[i] = 0;
			} else {
				enemigoDx[i] = 0;
				enemigoDy[i] = (rand() % 2 == 0) ? 1 : -1;
			}
		}

		invulnerableTicks = 0;
	}

	bool esPared(int x, int y) const {
		if (y < 0 || y >= (int)mapa.size()) return true;
		if (x < 0 || x >= (int)mapa[y].size()) return true;
		return mapa[y][x] == '#';
	}

	bool colisiona(int x, int y) const {
		for (int r = 0; r < 2; r++) {
			for (int c = 0; c < 4; c++) {
				if (esPared(x + c, y + r)) return true;
			}
		}
		return false;
	}

	void moverEnemigos() {
		for (int i = 0; i < 4; i++) {
			int nx = enemigoX[i] + enemigoDx[i];
			int ny = enemigoY[i] + enemigoDy[i];
			if (colisiona(nx, ny)) {
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
		bool cercaCoco = (abs(cocoX - myrphonX) < 14 && abs(cocoY - myrphonY) < 7);
		if (cercaCoco) {
			if (cocoX < myrphonX) dx = 1;
			else if (cocoX > myrphonX) dx = -1;
			if (cocoY < myrphonY) dy = 1;
			else if (cocoY > myrphonY) dy = -1;
		} else {
			int r = rand() % 6;
			if (r == 0) dx = 1;
			else if (r == 1) dx = -1;
			else if (r == 2) dy = 1;
			else if (r == 3) dy = -1;
		}

		bool vertPrimero = (rand() % 2 == 0);
		if (vertPrimero) {
			if (dy != 0 && !colisiona(myrphonX, myrphonY + dy)) {
				myrphonY += dy;
			} else if (dx != 0 && !colisiona(myrphonX + dx, myrphonY)) {
				myrphonX += dx;
			}
		} else {
			if (dx != 0 && !colisiona(myrphonX + dx, myrphonY)) {
				myrphonX += dx;
			} else if (dy != 0 && !colisiona(myrphonX, myrphonY + dy)) {
				myrphonY += dy;
			}
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

			std::string alerta = "¡ATRAPA A MYRPHON!";
			int colAlerta = 4;
			int colBorde = 4;
			if (segsRestantes <= 5) {
				alerta = "¡¡ATRAPALOOOO!!";
				colAlerta = 7;
				colBorde = 7;
			} else if (segsRestantes <= 10) {
				alerta = "¡MYRPHON SE ESCAPA!";
				colAlerta = 7;
				colBorde = 7;
			} else if (segsRestantes <= 20) {
				alerta = "¡APURATE QUE SE ACABA EL TIEMPO!";
				colAlerta = 4;
				colBorde = 4;
			}

			pantalla.dibujarCaja(1, 0, 82, 3, colBorde);
			std::string segsStr = (segsRestantes < 10 ? "0" : "") + std::to_string(segsRestantes);
			std::string hudTexto = "TIEMPO: " + segsStr + "s | VIDAS: ";
			for (int v = 0; v < protagonista->getVidaMaxima(); v++) {
				if (v < protagonista->getVida()) hudTexto += "<3 ";
				else hudTexto += ".. ";
			}
			hudTexto += "| " + alerta;
			pantalla.setTextoJuego(3, 1, hudTexto, colAlerta);

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

			pantalla.setTextoJuego(offsetX + myrphonX, offsetY + myrphonY, "(o> ", 4);
			pantalla.setTextoJuego(offsetX + myrphonX, offsetY + myrphonY + 1, "/||\\", 4);

			for (int i = 0; i < 4; i++) {
				if (i < 2) {
					pantalla.setTextoJuego(offsetX + enemigoX[i], offsetY + enemigoY[i], " === ", 7);
					pantalla.setTextoJuego(offsetX + enemigoX[i], offsetY + enemigoY[i] + 1, "/<_o", 7);
				} else {
					pantalla.setTextoJuego(offsetX + enemigoX[i], offsetY + enemigoY[i], "==== ", 7);
					pantalla.setTextoJuego(offsetX + enemigoX[i], offsetY + enemigoY[i] + 1, "(v_v)", 7);
				}
			}

			int colCoco = (invulnerableTicks == 0 || (invulnerableTicks % 2 == 0)) ? 6 : 3;
			pantalla.setTextoJuego(offsetX + cocoX, offsetY + cocoY, " /\\ ", colCoco);
			pantalla.setTextoJuego(offsetX + cocoX, offsetY + cocoY + 1, "(°u°)", colCoco);

			pantalla.setTextoJuego(3, 37, "[W,A,S,D] Moverse a toda prisa       [ESC] Rendirse", 8);

			pantalla.renderizarPanelLateral(1, "Atelier de Qifrey", protagonista->getNombre(),
			                               protagonista->getVida(), protagonista->getVidaMaxima(),
			                               "Laberinto Serpentback");
			pantalla.dibujar();

			tickAnim++;
			if (tickAnim % 3 == 0) {
				moverEnemigos();
			}
			if (tickAnim % 4 == 0) {
				moverMyrphon();
			}

#ifdef _WIN32
			if (_kbhit()) {
				int tecla = _getch();
				if (tecla == 0 || tecla == 224) tecla = _getch();
				if (tecla == 27) {
					protagonista->setVida(protagonista->getVida() - 1);
					pantalla.animarCorazonRoto(protagonista->getVida());
					if (protagonista->getVida() <= 0) {
						pantalla.animarCaidaPozoMuerte(1, "Atelier de Qifrey", protagonista->getNombre());
						return 2;
					}
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
					if (!colisiona(nx, ny)) {
						cocoX = nx;
						cocoY = ny;
					}
				}
			}
#endif

			for (int i = 0; i < 4; i++) {
				if (abs(cocoX - enemigoX[i]) <= 3 && abs(cocoY - enemigoY[i]) <= 1) {
					if (invulnerableTicks == 0) {
						protagonista->setVida(protagonista->getVida() - 1);
						pantalla.setTextoJuego(offsetX + cocoX, offsetY + cocoY, "\\ * /", 4);
						pantalla.setTextoJuego(offsetX + cocoX, offsetY + cocoY + 1, ">!o!<", 7);
						pantalla.dibujar();
#ifdef _WIN32
						Beep(650, 40);
						Beep(400, 50);
						Beep(250, 60);
#endif
						std::this_thread::sleep_for(std::chrono::milliseconds(120));

						if (protagonista->getVida() <= 0) {
							pantalla.animarCaidaPozoMuerte(1, "Atelier de Qifrey", protagonista->getNombre());
							return 2;
						} else {
							invulnerableTicks = 25;
						}
					}
				}
			}

			if (abs(cocoX - myrphonX) <= 3 && abs(cocoY - myrphonY) <= 1) {
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
