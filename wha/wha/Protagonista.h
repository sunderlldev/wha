#ifndef Protagonista_h
#define Protagonista_h
#include "NPC.h"
#include "Inventario.h"
#include <iostream>
#include <string>

class Protagonista : public Personaje {
private:
	int perspectiva;
	bool tieneCapaVuelo;
	bool tieneSemillaPlata;
	Inventario* inventario;
public:
	Protagonista(int x, int y, std::string n, int v = 3, int p = 1)
		: Personaje(x, y, n, v), perspectiva(p), tieneCapaVuelo(false), tieneSemillaPlata(false) {
		inventario = new Inventario(4);
		frame1[0][0] = '('; frame1[0][1] = ')';
		frame1[1][0] = '/'; frame1[1][1] = '\\';
		frame2[0][0] = '('; frame2[0][1] = ')';
		frame2[1][0] = '|'; frame2[1][1] = '|';
	}

	virtual ~Protagonista() {
		if (this->inventario != nullptr) {
			delete this->inventario;
			this->inventario = nullptr;
		}
	}

	int getPerspectiva() const { return this->perspectiva; }
	bool getTieneCapaVuelo() const { return this->tieneCapaVuelo; }
	bool getTieneSemillaPlata() const { return this->tieneSemillaPlata; }
	Inventario* getInventario() { return this->inventario; }

	void setPerspectiva(int p) { this->perspectiva = p; }
	void setTieneCapaVuelo(bool cv) { this->tieneCapaVuelo = cv; }
	void setTieneSemillaPlata(bool sp) { this->tieneSemillaPlata = sp; }

	void usarHabilidadPerspectiva() {}
	void tomarDesicionEtica() {}
	void interactuar(NPC*) {}
};

#endif
