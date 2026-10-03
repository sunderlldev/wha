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
		: mapa(nullptr), richeh(nullptr), activo(false),
		  cocoPrevX(0), cocoPrevY(0),
		  transicionBajando(false), transicionSubiendo(false) {
		mapa = new Mapa(25, 82);
		std::vector<std::string> m;
		cargarMatriz(m);
		mapa->cargarMatriz(m);
		richeh = new NPC(22, 10, "Richeh", "Aprendiz de Maga");
		richeh->setConfianza(1);
		richeh->setYaHablo(false);
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
			richeh->setX(22);
			richeh->setY(10);
			richeh->setConfianza(1);
			richeh->setYaHablo(false);
		}
	}

	bool getActivo() const { return activo; }
	void setActivo(bool a) { activo = a; }

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
};

#endif