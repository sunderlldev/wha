#pragma once

#include "Entidad.h"
#include "Configuracion.h"

class Protagonista : public Entidad {
private:
	int vidas;
	int tinta;
	bool enMovimiento;
	int nivel;
	int ticksAnimacion;

	static int filaDireccion(int dir) {
		switch (dir) {
		case 0: return 0;
		case 1: return 1;
		case 2: return 2;
		case 3: return 3;
		default: return 0;
		}
	}

public:
	Protagonista(int px, int py, int numNivel) : Entidad(px, py, 24, 32) {
		vidas = 3;
		tinta = 60;
		enMovimiento = false;
		nivel = numNivel;
		dx = 4;
		dy = 4;
		direccion = 0;
		idX = 0;
		idY = 0;
		ticksAnimacion = 0;
		escalaVisual = Configuracion::ESCALA_PERSONAJE;
	}

	virtual ~Protagonista() {}

	int getIdX() { return idX; }
	int getIdY() { return idY; }
	int getVidas() { return vidas; }
	int getTinta() { return tinta; }
	int getNivel() { return nivel; }
	bool getEnMovimiento() { return enMovimiento; }
	int getDireccion() { return direccion; }

	void setVidas(int v) { vidas = v; }
	void setTinta(int t) { tinta = t; }
	void setNivel(int n) { nivel = n; }
	void setEnMovimiento(bool valor) { enMovimiento = valor; }

	void restarVida() {
		if (vidas > 0) vidas--;
	}

	void sumarVida() {
		if (vidas < 3) vidas++;
	}

	void sumarTinta(int cant) {
		tinta += cant;
		if (tinta > 100) tinta = 100;
	}

	void gastarTinta(int cant) {
		tinta -= cant;
		if (tinta < 0) tinta = 0;
	}

	void moverLibre(int desplazamientoX, int desplazamientoY) {
		if (desplazamientoX != 0 || desplazamientoY != 0) {
			enMovimiento = true;
			if (desplazamientoX < 0) direccion = 1;
			else if (desplazamientoX > 0) direccion = 3;
			else if (desplazamientoY < 0) direccion = 2;
			else if (desplazamientoY > 0) direccion = 0;
		}
	}

	void detener() {
		enMovimiento = false;
		idX = 0;
		idY = filaDireccion(direccion);
	}

	void animar(bool corriendo = false) {
		ticksAnimacion++;
		if (enMovimiento) {
			int divisor = corriendo ? 1 : Configuracion::FPSZeldaSprtChange;
			if (ticksAnimacion % divisor == 0) {
				idX++;
				if (idX >= Configuracion::cantZeldaWalking) idX = 0;
			}
			idY = 4 + filaDireccion(direccion);
		}
		else {
			int ciclo = ticksAnimacion % 90;
			if (ciclo >= 82) idX = 2;
			else if (ciclo >= 78 || ciclo <= 4) idX = 1;
			else idX = 0;
			if (idX >= Configuracion::cantZeldaQuiet) idX = 0;
			idY = filaDireccion(direccion);
		}
	}
};
