#ifndef TorreAgott_h
#define TorreAgott_h
#include <vector>
#include <string>
#include <cstdlib>
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

		srand(12345);
		int numCajas = 18;
		std::vector<std::pair<int, int>> posicionesUsadas;

		for (int i = 0; i < numCajas; i++) {
			int bx = 0;
			int by = 0;
			bool posValida = false;
			int intentos = 0;

			while (!posValida && intentos < 100) {
				intentos++;
				bx = 472 + (rand() % (560 - 472 + 1));
				by = 76 + (rand() % (122 - 76 + 1));

				posValida = true;
				for (size_t k = 0; k < posicionesUsadas.size(); k++) {
					if (abs(bx - posicionesUsadas[k].first) < 4 && abs(by - posicionesUsadas[k].second) < 4) {
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
			indiceCajaLibro = 5 % cajas.size();
			indiceCajaPozo = 12 % cajas.size();
			if (indiceCajaPozo == indiceCajaLibro) {
				indiceCajaPozo = (indiceCajaPozo + 1) % cajas.size();
			}
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
				if (nx < bx + 3 && nx + 5 > bx && ny < by + 3 && ny + 4 > by) {
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

		if (nbx < 465 || nbx + 3 > 578 || nby < 64 || nby + 3 > 134) {
			puedeMover = false;
		}

		if (puedeMover && mapa != nullptr) {
			for (int r = 0; r < 3; r++) {
				for (int c = 0; c < 3; c++) {
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
					if (nbx < jbx + 3 && nbx + 3 > jbx && nby < jby + 3 && nby + 3 > jby) {
						puedeMover = false;
						break;
					}
				}
			}
		}

		if (puedeMover && agott != nullptr) {
			int ax = agott->getX();
			int ay = agott->getY();
			if (nbx < ax + 2 && nbx + 3 > ax && nby < ay + 2 && nby + 3 > ay) {
				puedeMover = false;
			}
		}

		if (puedeMover && pozoEncontrado) {
			if (nbx < pozoX + 3 && nbx + 3 > pozoX && nby < pozoY + 3 && nby + 3 > pozoY) {
				puedeMover = false;
			}
		}

		if (puedeMover) {
			for (size_t j = 0; j < cajas.size(); j++) {
				if ((int)j != indiceCaja && cajas[j] != nullptr) {
					int jbx = cajas[j]->getX();
					int jby = cajas[j]->getY();
					if (jugadorNx < jbx + 3 && jugadorNx + 2 > jbx && jugadorNy < jby + 3 && jugadorNy + 2 > jby) {
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
					itemsSuelo.push_back(new ItemMagico(origX + 1, origY + 1, "Libro de hechizos", "Tomo antiguo con instrucciones de trazos arcanos.", "Grimorio Magico", false));
					if (cajasMovidasContador == 1) {
						mensajeTemporal = "Coco: \\\"Pff, a la primera!\\\"";
					} else if (cajasMovidasContador == 2) {
						mensajeTemporal = "Coco: \\\"Bueno, no costo tanto encontrarlo!\\\"";
					} else {
						mensajeTemporal = "Coco: \\\"Por fin, lo encontre!\\\"";
					}
					ticksMensajeTemporal = 75;
				} else if (indiceCaja == indiceCajaPozo && !pozoEncontrado) {
					pozoEncontrado = true;
					pozoX = cajas[indiceCaja]->getOrigX();
					pozoY = cajas[indiceCaja]->getOrigY();
					mensajeTemporal = "Coco: \\\"Un pozo con escaleras?? Quien puede esconderse aqui?\\\"";
					ticksMensajeTemporal = 75;
				}
			}

			cajas[indiceCaja]->mover(dx, dy);
			return true;
		}
		return false;
	}
};

#endif
