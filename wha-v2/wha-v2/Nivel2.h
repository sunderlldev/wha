#ifndef Nivel2_h
#define Nivel2_h

#include <vector>
#include <string>
#include <gcroot.h>
#include "MapaNivel2.h"
#include "Enemigo.h"
#include "Protagonista.h"
#include "Utilidades.h"
#include "Inventario.h"
#include "Configuracion.h"

using namespace System;
using namespace System::Drawing;

class Nivel2 {
private:
	std::vector<std::string> mapa;
	Protagonista* protagonista;
	Inventario* inventario;
	std::vector<Enemigo*> enemigos;

	bool cuartosVisitados[10];
	bool cuartosEnemigos[10];
	int probabilidadEnemigo;
	int ticksPersecucion;
	int ticksCooldownDanio;
	bool bonoExploracionAplicado;

	int cargasHechizoAire;
	int ticksRafagaAire;
	int rafagaX;
	int rafagaY;
	char rafagaDir;

	int idCuartoActual;
	int camX;
	int camY;
	int ticksMensaje;
	std::string mensajeTemporal;

	gcroot<Bitmap^> bmpProtagonista;

public:
	Nivel2() {
		MapaNivel2::cargarMatriz(mapa);
		protagonista = new Protagonista(16 * 32, 36 * 32, 2);
		inventario = new Inventario();

		inventario->agregarItem("Libreta de Hechizos de Aire", "Permite lanzar rafagas de aire.", "Herramienta Magica");

		idCuartoActual = 0;
		camX = 0;
		camY = 0;
		ticksPersecucion = 0;
		ticksCooldownDanio = 0;
		cargasHechizoAire = 3;
		ticksRafagaAire = 0;
		ticksMensaje = 0;
		probabilidadEnemigo = 0;
		bonoExploracionAplicado = false;

		for (int i = 0; i < 10; i++) {
			cuartosVisitados[i] = false;
			cuartosEnemigos[i] = false;
		}
		cuartosVisitados[5] = true;

		String^ rutaProto = "..\\..\\wha-v2\\sprites/Tartah_Spritesheet.png";
		if (System::IO::File::Exists(rutaProto)) {
			bmpProtagonista = gcnew Bitmap(rutaProto);
		}
		else {
			String^ baseDir = AppDomain::CurrentDomain->BaseDirectory;
			String^ rutaCompleta = System::IO::Path::Combine(baseDir, rutaProto);
			if (System::IO::File::Exists(rutaCompleta)) bmpProtagonista = gcnew Bitmap(rutaCompleta);
		}

		mensajeTemporal = "Bienvenido al Laberinto del Pueblo";
		ticksMensaje = 120;
	}

	~Nivel2() {
		if (protagonista != nullptr) delete protagonista;
		if (inventario != nullptr) delete inventario;
		for (size_t i = 0; i < enemigos.size(); i++) {
			if (enemigos[i] != nullptr) delete enemigos[i];
		}
		enemigos.clear();
	}

	int getCantidadInventario() {
		return inventario->getCantidad();
	}

	std::string getInventarioNombre(int i) {
		return inventario->getItem(i).nombre;
	}

	std::string getInventarioDescripcion(int i) {
		return inventario->getItem(i).descripcion;
	}

	void moverJugador(int dx, int dy) {
		if (protagonista == nullptr) return;
		int nuevoX = protagonista->getX() + dx * Configuracion::VELOCIDAD_JUGADOR;
		int nuevoY = protagonista->getY() + dy * Configuracion::VELOCIDAD_JUGADOR;

		int tileSize = Configuracion::TILE_SIZE;
		int col = (nuevoX + protagonista->getAncho() / 2) / tileSize;
		int fila = (nuevoY + protagonista->getAlto() / 2) / tileSize;

		if (fila >= 0 && fila < (int)mapa.size() && col >= 0 && col < (int)mapa[fila].length()) {
			char ch = mapa[fila][col];
			if (ch == ' ' || ch == '.') {
				protagonista->setX(nuevoX);
				protagonista->setY(nuevoY);
				protagonista->moverLibre(dx, dy);
				actualizarCuartoActual();
			}
		}
	}

	void detenerJugador() {
		if (protagonista != nullptr) protagonista->detener();
	}

	void interactuar() {
		activarHechizoAire();
	}

	void procesarTick() {
		if (ticksMensaje > 0) ticksMensaje--;
		if (ticksRafagaAire > 0) ticksRafagaAire--;

		ticksPersecucion++;
		if (ticksPersecucion >= 6) {
			ticksPersecucion = 0;
			persecucionEnemigos();
		}

		if (ticksCooldownDanio > 0) {
			ticksCooldownDanio--;
		}
		else {
			colisionEnemigos();
		}

		actualizarCamara();
		protagonista->animar();
		for (size_t i = 0; i < enemigos.size(); i++) {
			enemigos[i]->animar();
		}
	}

	void dibujar(Graphics^ g) {
		dibujarMapa(g);
		dibujarEnemigos(g);
		dibujarProtagonista(g);
		dibujarHechizo(g);
		dibujarMensajes(g);
	}

	void activarHechizoAire() {
		if (protagonista == nullptr || inventario == nullptr) return;
		if (!inventario->tieneItem("Libreta de Hechizos de Aire")) {
			mensajeTemporal = "[AVISO] Aun no tienes la Libreta de Hechizos de Aire.";
			ticksMensaje = 60;
			return;
		}
		if (cargasHechizoAire <= 0) {
			mensajeTemporal = "[AVISO] Ya no te quedan hechizos de aire (0/3).";
			ticksMensaje = 60;
			return;
		}

		char dir = 'D';
		if (protagonista->getDireccion() == 0) dir = 'S';
		else if (protagonista->getDireccion() == 1) dir = 'A';
		else if (protagonista->getDireccion() == 2) dir = 'W';
		else if (protagonista->getDireccion() == 3) dir = 'D';

		int px = protagonista->getX() + protagonista->getAncho() / 2;
		int py = protagonista->getY() + protagonista->getAlto() / 2;

		cargasHechizoAire--;
		ticksRafagaAire = 8;
		rafagaX = px;
		rafagaY = py;
		rafagaDir = dir;

		int dx = 0, dy = 0;
		if (dir == 'W') dy = -1;
		else if (dir == 'S') dy = 1;
		else if (dir == 'A') dx = -1;
		else if (dir == 'D') dx = 1;

		int eliminados = 0;
		for (size_t i = 0; i < enemigos.size(); i++) {
			if (enemigos[i]->getActivo()) {
				int ex = enemigos[i]->getX();
				int ey = enemigos[i]->getY();
				int eW = enemigos[i]->getAncho();
				int eH = enemigos[i]->getAlto();

				for (int paso = 1; paso <= 14; paso++) {
					int wx = px + dx * paso * 32;
					int wy = py + dy * paso * 32;
					if (wx >= ex && wx <= ex + eW && wy >= ey && wy <= ey + eH) {
						enemigos[i]->setActivo(false);
						eliminados++;
						break;
					}
				}
			}
		}

		if (eliminados > 0) {
			mensajeTemporal = "[HECHIZO] Eliminaste a " + std::to_string(eliminados) + " enemigo(s). Cargas: " + std::to_string(cargasHechizoAire) + "/3";
			ticksMensaje = 75;
		}
		else {
			mensajeTemporal = "[HECHIZO] Lanzaste una rafaga. Cargas: " + std::to_string(cargasHechizoAire) + "/3";
			ticksMensaje = 65;
		}
	}

private:
	void dibujarMapa(Graphics^ g) {
		int tileSize = Configuracion::TILE_SIZE;
		int inicioFila = camY / tileSize;
		int finFila = inicioFila + 600 / tileSize + 2;
		int inicioCol = camX / tileSize;
		int finCol = inicioCol + 960 / tileSize + 2;

		SolidBrush^ pincelPared = gcnew SolidBrush(Color::FromArgb(80, 80, 80));
		SolidBrush^ pincelPiso = gcnew SolidBrush(Color::FromArgb(30, 30, 30));

		for (int f = inicioFila; f <= finFila && f < (int)mapa.size(); f++) {
			for (int c = inicioCol; c <= finCol && c < (int)mapa[f].length(); c++) {
				char ch = mapa[f][c];
				int px = c * tileSize - camX;
				int py = f * tileSize - camY;

				if (ch == '|' || ch == '+' || ch == '-') {
					g->FillRectangle(pincelPared, px, py, tileSize, tileSize);
				}
				else if (ch == ' ') {
					g->FillRectangle(pincelPiso, px, py, tileSize, tileSize);
				}
			}
		}
		delete pincelPared;
		delete pincelPiso;
	}

	void dibujarEnemigos(Graphics^ g) {
		SolidBrush^ pincelEne = gcnew SolidBrush(Color::Red);
		for (size_t i = 0; i < enemigos.size(); i++) {
			Enemigo* en = enemigos[i];
			if (en->getActivo()) {
				int px = en->getX() - camX;
				int py = en->getY() - camY;
				g->FillRectangle(pincelEne, px, py, en->getAncho(), en->getAlto());
			}
		}
		delete pincelEne;
	}

	void dibujarProtagonista(Graphics^ g) {
		if (protagonista == nullptr) return;
		Bitmap^ bmp = bmpProtagonista;
		if (bmp == nullptr) return;

		int frameW = bmp->Width / Configuracion::cantZeldaWidth;
		int frameH = bmp->Height / Configuracion::cantZeldaHeight;
		int px = protagonista->getX() - camX;
		int py = protagonista->getY() - camY;
		Rectangle srcRect(protagonista->getIdX() * frameW, protagonista->getIdY() * frameH, frameW, frameH);
		Rectangle destRect(px, py, frameW * 2, frameH * 2);
		g->DrawImage(bmp, destRect, srcRect, GraphicsUnit::Pixel);
	}

	void dibujarHechizo(Graphics^ g) {
		if (ticksRafagaAire <= 0) return;
		int dx = 1, dy = 0;
		if (rafagaDir == 'W') { dx = 0; dy = -1; }
		else if (rafagaDir == 'S') { dx = 0; dy = 1; }
		else if (rafagaDir == 'A') { dx = -1; dy = 0; }

		int inicioPaso = 9 - ticksRafagaAire;
		if (inicioPaso < 1) inicioPaso = 1;
		int finPaso = inicioPaso + 6;
		if (finPaso > 14) finPaso = 14;

		SolidBrush^ pincelHechizo = gcnew SolidBrush(Color::Cyan);
		for (int paso = inicioPaso; paso <= finPaso; paso++) {
			int wx = rafagaX + dx * paso * Configuracion::TILE_SIZE;
			int wy = rafagaY + dy * paso * Configuracion::TILE_SIZE;
			int sx = wx - camX;
			int sy = wy - camY;
			g->FillEllipse(pincelHechizo, sx, sy, 16, 16);
		}
		delete pincelHechizo;
	}

	void dibujarMensajes(Graphics^ g) {
		if (ticksMensaje > 0) {
			System::Drawing::Font^ fuente = gcnew System::Drawing::Font("Segoe UI", 14, FontStyle::Bold);
			SolidBrush^ fondo = gcnew SolidBrush(Color::FromArgb(150, 0, 0, 0));
			SolidBrush^ pincelTexto = gcnew SolidBrush(Color::White);
			g->FillRectangle(fondo, 20, 20, 500, 40);
			g->DrawString(ToManaged(mensajeTemporal), fuente, pincelTexto, 30, 28);
			delete fuente;
			delete fondo;
			delete pincelTexto;
		}
	}

	void actualizarCamara() {
		camX = protagonista->getX() + protagonista->getAncho() / 2 - 960 / 2;
		camY = protagonista->getY() + protagonista->getAlto() / 2 - 600 / 2;

		if (camX < 0) camX = 0;
		if (camY < 0) camY = 0;

		int maxX = (int)mapa[0].length() * 32 - 960;
		int maxY = (int)mapa.size() * 32 - 600;

		if (camX > maxX) camX = maxX;
		if (camY > maxY) camY = maxY;
	}

	void persecucionEnemigos() {
		if (protagonista == nullptr) return;
		int px = protagonista->getX();
		int py = protagonista->getY();

		for (size_t i = 0; i < enemigos.size(); i++) {
			Enemigo* en = enemigos[i];
			if (en == nullptr || !en->getActivo()) continue;

			int ex = en->getX();
			int ey = en->getY();
			int dx = 0, dy = 0;
			if (px > ex) dx = 1; else if (px < ex) dx = -1;
			if (py > ey) dy = 1; else if (py < ey) dy = -1;

			int vel = 2;
			if (dx != 0 && dy != 0) {
				en->setX(ex + dx * vel);
				en->setY(ey + dy * vel);
			}
			else if (dx != 0) {
				en->setX(ex + dx * vel);
			}
			else if (dy != 0) {
				en->setY(ey + dy * vel);
			}
		}
	}

	void colisionEnemigos() {
		if (protagonista == nullptr) return;
		int px = protagonista->getX();
		int py = protagonista->getY();
		int pw = protagonista->getAncho();
		int ph = protagonista->getAlto();

		for (size_t i = 0; i < enemigos.size(); i++) {
			Enemigo* en = enemigos[i];
			if (en == nullptr || !en->getActivo()) continue;
			int ex = en->getX();
			int ey = en->getY();
			int ew = en->getAncho();
			int eh = en->getAlto();

			if (px < ex + ew && px + pw > ex && py < ey + eh && py + ph > ey) {
				protagonista->restarVida();
				ticksCooldownDanio = 30;
				mensajeTemporal = "[DANIO] Un enemigo te ha golpeado!";
				ticksMensaje = 60;
				if (protagonista->getVidas() <= 0) {
					protagonista->setVidas(3);
					protagonista->setX(16 * 32);
					protagonista->setY(36 * 32);
				}
				break;
			}
		}
	}

	void actualizarCuartoActual() {
		int px = protagonista->getX();
		int py = protagonista->getY();
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (nuevoCuarto > 0 && nuevoCuarto < 10) {
				cuartosVisitados[nuevoCuarto] = true;
				if (nuevoCuarto != 5) {
					if (!cuartosEnemigos[nuevoCuarto]) {
						cuartosEnemigos[nuevoCuarto] = true;
						probabilidadEnemigo++;
					}
					System::Random^ r = gcnew System::Random();
					int probabilidad = (probabilidadEnemigo * 100) / 8;
					if (r->Next(1, 101) <= probabilidad) {
						generarEnemigoEnCuarto(nuevoCuarto);
					}
				}
			}
		}
	}

	int determinarCuarto(int px, int py) {
		int cx = px / 32;
		int cy = py / 32;
		if (cx >= 10 && cx <= 50 && cy >= 5 && cy <= 16) return 1;
		if (cx >= 41 && cx <= 73 && cy >= 23 && cy <= 35) return 2;
		if (cx >= 106 && cx <= 137 && cy >= 23 && cy <= 34) return 3;
		if (cx >= 144 && cx <= 163 && cy >= 26 && cy <= 34) return 4;
		if (cx >= 12 && cx <= 30 && cy >= 33 && cy <= 41) return 5;
		if (cx >= 41 && cx <= 73 && cy >= 42 && cy <= 55) return 6;
		if (cx >= 108 && cx <= 129 && cy >= 39 && cy <= 53) return 7;
		if (cx >= 40 && cx <= 72 && cy >= 72 && cy <= 84) return 8;
		if (cx >= 84 && cx <= 130 && cy >= 68 && cy <= 73) return 9;
		return 0;
	}

	void generarEnemigoEnCuarto(int idCuarto) {
		if (idCuarto <= 0 || idCuarto == 5) return;
		int xMin = 1, xMax = mapa[0].length() - 2;
		int yMin = 1, yMax = mapa.size() - 2;
		switch (idCuarto) {
		case 1: xMin = 10; xMax = 50; yMin = 5; yMax = 16; break;
		case 2: xMin = 41; xMax = 73; yMin = 23; yMax = 35; break;
		case 3: xMin = 106; xMax = 137; yMin = 23; yMax = 34; break;
		case 4: xMin = 144; xMax = 163; yMin = 26; yMax = 34; break;
		case 6: xMin = 41; xMax = 73; yMin = 42; yMax = 55; break;
		case 7: xMin = 108; xMax = 129; yMin = 39; yMax = 53; break;
		case 8: xMin = 40; xMax = 72; yMin = 72; yMax = 84; break;
		case 9: xMin = 84; xMax = 130; yMin = 68; yMax = 73; break;
		default: return;
		}
		System::Random^ rng = gcnew System::Random();
		int tx = rng->Next(xMin, xMax) * 32;
		int ty = rng->Next(yMin, yMax) * 32;
		enemigos.push_back(new Enemigo(tx, ty, "Sanguijuela", 2, 1, 4, idCuarto));
		mensajeTemporal = "[PELIGRO] Aparecio un enemigo en el cuarto!";
		ticksMensaje = 75;
	}
};

#endif