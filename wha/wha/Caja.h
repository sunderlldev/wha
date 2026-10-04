#ifndef Caja_h
#define Caja_h
#include "ObjetoMapa.h"
#include <vector>
#include <string>

class Caja : public ObjetoMapa {
private:
	int origX;
	int origY;
	bool haSidoMovida;
public:
	Caja(int x, int y)
		: ObjetoMapa(x, y, 7, 4, true, caja), origX(x), origY(y), haSidoMovida(false) {}

	virtual ~Caja() {}

	int getOrigX() const { return origX; }
	int getOrigY() const { return origY; }
	bool getHaSidoMovida() const { return haSidoMovida; }

	void setX(int nx) { x = nx; }
	void setY(int ny) { y = ny; }
	void setHaSidoMovida(bool hsm) { haSidoMovida = hsm; }

	void mover(int dx, int dy) {
		x += dx;
		y += dy;
	}

	int getColor() const {
		return haSidoMovida ? 10 : 4;
	}

	char getCaracter(int r, int c) const {
		if (r == 0 || r == 3) {
			if (c == 0 || c == 6) return '+';
			return '-';
		}
		if (c == 0 || c == 6) return '|';
		return '#';
	}

	virtual void dibujarEnMatriz(std::vector<std::string>& matriz) {
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 7; c++) {
				int my = y + r;
				int mx = x + c;
				if (my >= 0 && my < (int)matriz.size() && mx >= 0 && mx < (int)matriz[my].size()) {
					matriz[my][mx] = getCaracter(r, c);
				}
			}
		}
	}
};

#endif
