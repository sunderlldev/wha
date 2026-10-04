#ifndef Mapa_h
#define Mapa_h
#include <vector>
#include <string>
#include "ObjetoMapa.h"

class Mapa {
private:
	int filas, columnas;
	std::vector<std::string> matriz;
	std::vector<ObjetoMapa*> objetos;
public:
	Mapa(int f, int c) : filas(f), columnas(c) {
		matriz = std::vector<std::string>(filas, std::string(columnas, ' '));
	}

	~Mapa() {
		for (size_t i = 0; i < objetos.size(); i++) {
			if (objetos[i] != nullptr) {
				delete objetos[i];
			}
		}
		objetos.clear();
	}

	void agregarObjeto(ObjetoMapa* nuevoObjeto) {
		objetos.push_back(nuevoObjeto);
		nuevoObjeto->dibujarEnMatriz(matriz);
	}

	int getFilas() const { return this->filas; }
	int getColumnas() const { return this->columnas; }
	const std::vector<std::string>& getMatriz() const { return this->matriz; }

	void cargarMatriz(const std::vector<std::string>& m) {
		this->matriz = m;
		this->filas = (int)m.size();
		if (this->filas > 0) {
			this->columnas = (int)m[0].size();
		}
	}

	void setCaracter(int x, int y, char c) {
		if (y >= 0 && y < filas && x >= 0 && x < columnas) {
			matriz[y][x] = c;
		}
	}

	bool esPosicionValida(int x, int y) {
		if (x < 0 || x >= columnas || y < 0 || y >= filas) return false;

		for (size_t i = 0; i < objetos.size(); i++) {
			if (objetos[i]->getTipo() != habitacion && objetos[i]->colisionaCon(x, y)) {
				return false;
			}
		}
		if (y < (int)matriz.size() && x < (int)matriz[y].size()) {
			char c = matriz[y][x];
			if (c != ' ' && c != '.' && c != '=' && c != ':' && c != '#' && c != '*' && c != '[' && c != '!' && c != ']') {
				return false;
			}
		}
		return true;
	}
};

#endif
