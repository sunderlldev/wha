#ifndef Protagonista_h
#define Protagonista_h
#include "Personaje.h"
#include "Inventario.h"
#include <string>

class Protagonista : public Personaje {
private:
	Inventario* inventario;
public:
	Protagonista(int x, int y, std::string n, int v = 3)
		: Personaje(x, y, n, v) {
		inventario = new Inventario(6);
		ancho = 5;
		alto = 4;

		frame1[0][0] = ' '; frame1[0][1] = '/'; frame1[0][2] = ' '; frame1[0][3] = '\\'; frame1[0][4] = ' ';
		frame1[1][0] = '/'; frame1[1][1] = '_'; frame1[1][2] = '_'; frame1[1][3] = '_'; frame1[1][4] = '\\';
		frame1[2][0] = '('; frame1[2][1] = '*'; frame1[2][2] = 'u'; frame1[2][3] = '*'; frame1[2][4] = ')';
		frame1[3][0] = '/'; frame1[3][1] = ' '; frame1[3][2] = '|'; frame1[3][3] = ' '; frame1[3][4] = '\\';

		frame2[0][0] = ' '; frame2[0][1] = '/'; frame2[0][2] = ' '; frame2[0][3] = '\\'; frame2[0][4] = ' ';
		frame2[1][0] = '/'; frame2[1][1] = '_'; frame2[1][2] = '_'; frame2[1][3] = '_'; frame2[1][4] = '\\';
		frame2[2][0] = '('; frame2[2][1] = '*'; frame2[2][2] = 'u'; frame2[2][3] = '*'; frame2[2][4] = ')';
		frame2[3][0] = ' '; frame2[3][1] = '|'; frame2[3][2] = ' '; frame2[3][3] = '|'; frame2[3][4] = ' ';
	}

	virtual ~Protagonista() {
		if (this->inventario != nullptr) {
			delete this->inventario;
			this->inventario = nullptr;
		}
	}

	Inventario* getInventario() { return this->inventario; }
};

#endif
