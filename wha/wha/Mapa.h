#ifndef Mapa_h
#define Mapa_h
#include <iostream>
#include <vector>
#include "Arbol.h"
#include "Habitacion.h"
#include "Rio.h"
#include "Gato.h"
#include "Camino.h"

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
		for (auto obj : objetos) {
			delete obj;
		}
		objetos.clear();
	};
	//agregar entidades al mapa
	void agregarObjeto(ObjetoMapa* nuevoObjeto) {
		objetos.push_back(nuevoObjeto);
		nuevoObjeto->dibujarEnMatriz(matriz);
	}
	//get
	int getFilas() { return this->filas; }
	int getColumnas() { return this->columnas; }
	//set
	void setFilas(int f) { this->filas = f; }
	void setColumnas(int c) { this->columnas = c; }
	//acciones
	bool esPosicionValida(int x, int y) {
		//limites del mapa
		if (x < 0 || x >= columnas || y < 0 || y >= filas) return false;

		//colision
		for (const auto& obj : objetos) {
			if (obj->colisionaCon(x, y)) {
				return false; //no pasa
			}
		}
		return true; //pasa
	}
	bool estaEnHabitacion(int px, int py) {
		for (const auto& obj : objetos) {
			if (obj->getTipo() == habitacion && obj->estaDentro(px, py)) {
				return true;
			}
		}
		return false;
	}
	void actualizarNieblaVision(int px, int py, int radio) {};
	void dibujarMapa() {};
};

#endif // !Mapa_h

