#ifndef Protagonista_h
#define Protagonista_h
#include "NPC.h"
#include <iostream>

class Protagonista : public Personaje {
private:
	int tintaPlata;
	int tintaMaxima;
	int perspectiva;
	bool tieneCapaVuelo;
	bool tieneSemillaPlata;
public:
	Protagonista(int x, int y, std::string n, int v, int p):Personaje(x, y, n, v), perspectiva(p) {};
	~Protagonista() {};
	//get
	int getTintaPabla() { return this->tintaPlata; }
	int getTintaMaximo() { return this->tintaMaxima; }
	int getPerspectiva() { return this->perspectiva; }
	bool getTieneCapaVuelo() { return this->tieneCapaVuelo; }
	//set
	void getTintaPabla(int newTintaPlata) { this->tintaPlata = newTintaPlata; }
	void getTintaMaximo(int newTintaMagica) { this->tintaMaxima = newTintaMagica; }
	void getPerspectiva(int newPersvectica) { this->perspectiva = newPersvectica; }
	void getTieneCapaVuelo(bool tieneCapaVuelo) { this->tieneCapaVuelo = tieneCapaVuelo; }
	//acciones
	void usarHabilidadPerspectiva() {};
	void tomarDesicionEtica() {};
	void interactuar(NPC) {};
};

#endif // !Protagonista_h
