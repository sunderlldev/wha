#ifndef TorreAgott_h
#define TorreAgott_h
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Caja.h"
#include "ItemMagico.h"
#include "NPC.h"
#include "Mapa.h"

class TorreAgott {
private:
	std::vector<Caja*> cajas;
	int indiceCajaLibro;
	int indiceCajaPozo;
	bool libroEncontrado;
	bool pozoEncontrado;
	int cajasMovidasContador;
	int pozoX;
	int pozoY;

public:
	TorreAgott()
		: indiceCajaLibro(-1), indiceCajaPozo(-1), libroEncontrado(false),
		  pozoEncontrado(false), cajasMovidasContador(0), pozoX(-1), pozoY(-1) {}

	~TorreAgott() {
		for (size_t i = 0; i < cajas.size(); i++) {
			if (cajas[i] != nullptr) {
				delete cajas[i];
			}
		}
		cajas.clear();
	}

	bool getLibroEncontrado() const { return libroEncontrado; }
	bool getPozoEncontrado() const { return pozoEncontrado; }
	int getPozoX() const { return pozoX; }
	int getPozoY() const { return pozoY; }
	int getCajasMovidasContador() const { return cajasMovidasContador; }
	const std::vector<Caja*>& getCajas() const { return cajas; }

	void reiniciar() {
		for (size_t i = 0; i < cajas.size(); i++) {
			if (cajas[i] != nullptr) delete cajas[i];
		}
		cajas.clear();

		libroEncontrado = false;
		pozoEncontrado = false;
		cajasMovidasContador = 0;
		pozoX = -1;
		pozoY = -1;

		srand(time(0));
		int numCajas = 8;
		std::vector<std::pair<int, int>> posicionesUsadas;

		for (int i = 0; i < numCajas; i++) {
			int bx = 0;
			int by = 0;
			bool posValida = false;
			int intentos = 0;

			while (!posValida && intentos < 100) {
				intentos++;
				bx = 405 + (rand() % (480 - 405 + 1));
				by = 75 + (rand() % (120 - 75 + 1));

				posValida = true;
				if (abs(bx - 440) < 10 && abs(by - 66) < 6) {
					posValida = false;
				}
				if (bx < 405 && by >= 96 && by <= 110) {
					posValida = false;
				}
				for (size_t k = 0; k < posicionesUsadas.size(); k++) {
					if (abs(bx - posicionesUsadas[k].first) < 9 && abs(by - posicionesUsadas[k].second) < 5) {
						posValida = false;
						break;
					}
				}
			}

			if (posValida) {
				posicionesUsadas.push_back(std::make_pair(bx, by));
				cajas.push_back(new Caja(bx, by));
			}
		}

		if (cajas.size() >= 2) {
			indiceCajaLibro = rand() % cajas.size();
			indiceCajaPozo = (indiceCajaLibro + 1 + (rand() % (cajas.size() - 1))) % cajas.size();
		}
	}

	bool estaCercaDelPozo(int px, int py) const {
		if (!pozoEncontrado) return false;
		return (abs(px - pozoX) <= 4 && abs(py - pozoY) <= 4);
	}

	int detectarColisionCaja(int nx, int ny) const {
		for (size_t i = 0; i < cajas.size(); i++) {
			if (cajas[i] != nullptr) {
				int bx = cajas[i]->getX();
				int by = cajas[i]->getY();
				if (nx < bx + 7 && nx + 5 > bx && ny < by + 4 && ny + 4 > by) {
					return (int)i;
				}
			}
		}
		return -1;
	}

	bool intentarEmpujarCaja(int indiceCaja, int dx, int dy, int jugadorNx, int jugadorNy,
	                         Mapa* mapa, NPC* agott, std::vector<ItemMagico*>& itemsSuelo,
	                         std::string& mensajeTemporal, int& ticksMensajeTemporal) {
		if (indiceCaja < 0 || indiceCaja >= (int)cajas.size() || cajas[indiceCaja] == nullptr) return false;

		int nbx = cajas[indiceCaja]->getX() + dx;
		int nby = cajas[indiceCaja]->getY() + dy;
		bool puedeMover = true;

		if (nbx < 388 || nbx + 7 > 497 || nby < 65 || nby + 4 > 129) {
			puedeMover = false;
		}

		if (puedeMover && mapa != nullptr) {
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 7; c++) {
					if (!mapa->esPosicionValida(nbx + c, nby + r)) {
						puedeMover = false;
						break;
					}
				}
				if (!puedeMover) break;
			}
		}

		if (puedeMover) {
			for (size_t j = 0; j < cajas.size(); j++) {
				if ((int)j != indiceCaja && cajas[j] != nullptr) {
					int jbx = cajas[j]->getX();
					int jby = cajas[j]->getY();
					if (nbx < jbx + 7 && nbx + 7 > jbx && nby < jby + 4 && nby + 4 > jby) {
						puedeMover = false;
						break;
					}
				}
			}
		}

		if (puedeMover && agott != nullptr) {
			int ax = agott->getX();
			int ay = agott->getY();
			if (nbx < ax + agott->getAncho() && nbx + 7 > ax && nby < ay + agott->getAlto() && nby + 4 > ay) {
				puedeMover = false;
			}
		}

		if (puedeMover && pozoEncontrado) {
			bool solapadoPrevio = (cajas[indiceCaja]->getX() < pozoX + 3 &&
			                       cajas[indiceCaja]->getX() + 7 > pozoX &&
			                       cajas[indiceCaja]->getY() < pozoY + 2 &&
			                       cajas[indiceCaja]->getY() + 4 > pozoY);
			bool solapadoNuevo = (nbx < pozoX + 3 && nbx + 7 > pozoX &&
			                      nby < pozoY + 2 && nby + 4 > pozoY);
			if (!solapadoPrevio && solapadoNuevo) {
				puedeMover = false;
			}
		}

		if (puedeMover) {
			for (size_t j = 0; j < cajas.size(); j++) {
				if ((int)j != indiceCaja && cajas[j] != nullptr) {
					int jbx = cajas[j]->getX();
					int jby = cajas[j]->getY();
					if (jugadorNx < jbx + 7 && jugadorNx + 5 > jbx && jugadorNy < jby + 4 && jugadorNy + 4 > jby) {
						puedeMover = false;
						break;
					}
				}
			}
		}

		if (puedeMover) {
			if (!cajas[indiceCaja]->getHaSidoMovida()) {
				cajas[indiceCaja]->setHaSidoMovida(true);
				cajasMovidasContador++;

				if (indiceCaja == indiceCajaLibro && !libroEncontrado) {
					libroEncontrado = true;
					int origX = cajas[indiceCaja]->getOrigX();
					int origY = cajas[indiceCaja]->getOrigY();
					itemsSuelo.push_back(new ItemMagico(origX + 2, origY + 1, "Libro de hechizos", "Tomo antiguo con instrucciones de trazos arcanos.", "Grimorio Magico", false));
					if (cajasMovidasContador == 1) {
						mensajeTemporal = "Coco: Pff, a la primera!";
					} else if (cajasMovidasContador == 2) {
						mensajeTemporal = "Coco: Bueno, no costo tanto encontrarlo!";
					} else {
						mensajeTemporal = "Coco: Por fin, lo encontre!";
					}
					ticksMensajeTemporal = 100;
				} else if (indiceCaja == indiceCajaPozo && !pozoEncontrado) {
					pozoEncontrado = true;
					pozoX = cajas[indiceCaja]->getOrigX() + 2;
					pozoY = cajas[indiceCaja]->getOrigY() + 1;
					mensajeTemporal = "Coco: Un pozo con escaleras?? Quien puede esconderse aqui?";
					ticksMensajeTemporal = 100;
				}
			}

			cajas[indiceCaja]->mover(dx, dy);
			return true;
		}
		return false;
	}
};

#endif
