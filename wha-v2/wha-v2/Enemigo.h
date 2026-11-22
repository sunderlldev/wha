#pragma once

#include "Entidad.h"

class Enemigo : public Entidad {
private:
	int vidas;
	int direccionPatrulla;
	int minCoord;
	int maxCoord;
	int ticksAnim;

public:
	Enemigo(int px, int py, int dir, int minC, int maxC)
		: Entidad(px, py, 24, 32) {
		vidas = 3;
		direccionPatrulla = dir;
		minCoord = minC;
		maxCoord = maxC;
		dx = 2;
		dy = 2;
		idX = 0;
		idY = 0;
		ticksAnim = 0;
	}

	virtual ~Enemigo() {}

	int getVidas() { return vidas; }

	void recibirDanio() {
		if (vidas > 0) vidas--;
		if (vidas == 0) visible = false;
	}

	void mover() {
		if (!visible) return;

		if (direccionPatrulla == 0) {
			x += dx;
			if (x >= maxCoord) {
				x = maxCoord;
				dx = -dx;
				direccion = 1;
			}
			else if (x <= minCoord) {
				x = minCoord;
				dx = -dx;
				direccion = 3;
			}
		}
		else {
			y += dy;
			if (y >= maxCoord) {
				y = maxCoord;
				dy = -dy;
				direccion = 2;
			}
			else if (y <= minCoord) {
				y = minCoord;
				dy = -dy;
				direccion = 0;
			}
		}

		ticksAnim++;
		if (ticksAnim % 4 == 0) {
			idX++;
			if (idX >= 4) idX = 0;
		}
		idY = direccion;
	}
};
