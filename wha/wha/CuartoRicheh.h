#ifndef CuartoRicheh_h
#define CuartoRicheh_h

#include <vector>
#include <string>
#include <cstdlib>
#include "Mapa.h"
#include "NPC.h"
#include "Protagonista.h"

class CuartoRicheh {
private:
	Mapa* mapa;
	NPC* richeh;
	bool activo;
	int cocoPrevX;
	int cocoPrevY;
	bool transicionBajando;
	bool transicionSubiendo;

public:
	static void cargarMatriz(std::vector<std::string>& matriz) {
		matriz.clear();
		matriz.push_back("+--------------------------------------------------------------------------------+");
		matriz.push_back("|                                                                                |");
		matriz.push_back("|                                                                                |");
		matriz.push_back("|     +------+                              +------------------------------+     |");
		matriz.push_back("|     |      |                              |                              |     |");
		matriz.push_back("|     |  %%  |                              |                              |     |");
		matriz.push_back("|     |  %%  |                              |                              |     |");
		matriz.push_back("|     |      |                              |                              |     |");
		matriz.push_back("|     +      +------------------------------+                              |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     |                                                                    |     |");
		matriz.push_back("|     +-------------------------------------+                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           |                              |     |");
		matriz.push_back("|                                           +------------------------------+     |");
		matriz.push_back("|                                                                                |");
		matriz.push_back("|                                                                                |");
		matriz.push_back("+--------------------------------------------------------------------------------+");
	}

	CuartoRicheh()
		: mapa(nullptr), richeh(nullptr),
		  activo(false), cocoPrevX(0), cocoPrevY(0),
		  transicionBajando(false), transicionSubiendo(false) {
		mapa = new Mapa(25, 82);
		std::vector<std::string> m;
		cargarMatriz(m);
		mapa->cargarMatriz(m);
		richeh = new NPC(55, 12, "Richeh", "Aprendiz de Maga");
		richeh->setConfianza(1);
		richeh->setYaHablo(false);
		mapa->setCaracter(22, 11, '['); mapa->setCaracter(23, 11, '!'); mapa->setCaracter(24, 11, ']');
		mapa->setCaracter(40, 11, '['); mapa->setCaracter(41, 11, '!'); mapa->setCaracter(42, 11, ']');
	}

	~CuartoRicheh() {
		if (mapa != nullptr) {
			delete mapa;
			mapa = nullptr;
		}
		if (richeh != nullptr) {
			delete richeh;
			richeh = nullptr;
		}
	}

	void reiniciar() {
		activo = false;
		transicionBajando = false;
		transicionSubiendo = false;
		if (richeh != nullptr) {
			richeh->setX(55);
			richeh->setY(12);
			richeh->setConfianza(1);
			richeh->setYaHablo(false);
			richeh->setExpresion(0);
		}
		if (mapa != nullptr) {
			std::vector<std::string> m;
			cargarMatriz(m);
			mapa->cargarMatriz(m);
			mapa->setCaracter(22, 11, '['); mapa->setCaracter(23, 11, '!'); mapa->setCaracter(24, 11, ']');
			mapa->setCaracter(40, 11, '['); mapa->setCaracter(41, 11, '!'); mapa->setCaracter(42, 11, ']');
		}
	}

	void colocarMyrphon() {
		if (mapa != nullptr) {
			mapa->setCaracter(64, 12, '_');
			mapa->setCaracter(65, 12, 'v');
			mapa->setCaracter(66, 12, '_');
			mapa->setCaracter(63, 13, '(');
			mapa->setCaracter(64, 13, 'o');
			mapa->setCaracter(65, 13, ',');
			mapa->setCaracter(66, 13, 'o');
			mapa->setCaracter(67, 13, ')');
		}
	}

	bool getActivo() const { return activo; }

	Mapa* getMapa() { return mapa; }
	NPC* getRicheh() { return richeh; }

	bool getTransicionBajando() const { return transicionBajando; }
	void setTransicionBajando(bool tb) { transicionBajando = tb; }

	bool getTransicionSubiendo() const { return transicionSubiendo; }
	void setTransicionSubiendo(bool ts) { transicionSubiendo = ts; }

	void entrar(Protagonista* prota) {
		if (prota != nullptr) {
			cocoPrevX = prota->getX();
			cocoPrevY = prota->getY();
			prota->setX(8);
			prota->setY(8);
		}
		activo = true;
	}

	void salir(Protagonista* prota) {
		if (prota != nullptr) {
			prota->setX(cocoPrevX);
			prota->setY(cocoPrevY);
		}
		activo = false;
	}

	bool estaCercaDelPozo(int px, int py) const {
		return (abs(px - 9) <= 4 && abs(py - 5) <= 4);
	}

	bool estaCercaDeLetreroCuriosidades(int px, int py) const {
		return (abs(px - 22) <= 3 && abs(py - 11) <= 2);
	}

	bool estaCercaDeLetreroLore(int px, int py) const {
		return (abs(px - 40) <= 3 && abs(py - 11) <= 2);
	}
};

#endif