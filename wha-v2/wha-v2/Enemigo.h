#pragma once
#include "Entidad.h"
#include <string>

class Enemigo : public Entidad {
private:
	int danio;
	int color;
	bool activo;
	int idCuartoOrigen;
	int vidas;
	int ticksAnimacion;
	int frameActual;

public:
	Enemigo(int px, int py, std::string nombre = "Sanguijuela", int vida = 2, int danio = 1, int color = 4, int cuarto = 0)
		: Entidad(px, py, 32, 32) {
		this->vidas = vida;
		this->danio = danio;
		this->color = color;
		this->activo = true;
		this->idCuartoOrigen = cuarto;
		this->ticksAnimacion = 0;
		this->frameActual = 0;
	}

	int getDanio() const { return danio; }
	int getColor() const { return color; }
	bool getActivo() const { return activo; }
	int getIdCuartoOrigen() const { return idCuartoOrigen; }
	int getVidas() const { return vidas; }

	void setDanio(int d) { danio = d; }
	void setColor(int col) { color = col; }
	void setActivo(bool act) { activo = act; }
	void setIdCuartoOrigen(int c) { idCuartoOrigen = c; }
	void setVidas(int v) { vidas = v; }

	void animar() {
		ticksAnimacion++;
		if (ticksAnimacion % 10 == 0) {
			frameActual = (frameActual + 1) % 2;
		}
	}
};