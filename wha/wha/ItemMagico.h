#ifndef ItemMagico_h
#define ItemMagico_h
#include "Protagonista.h"
#include <iostream>

class ItemMagico {
private:
	int y, x;
	std::string nombre, tipoEfecto;
	int valorEfecto;
	bool recogido;
public:
	ItemMagico(int x, int y, std::string n, std::string tE, int vE, bool r) :x(x), y(y), nombre(n), tipoEfecto(tE), valorEfecto(vE), recogido(r) {};
	~ItemMagico() {};
	//get
	int getX() { return this->x; }
	int getY() { return this->y; }
	std::string getNombre() { return this->nombre; }
	std::string getTipoEfecto() { return this->tipoEfecto; }
	int getValorEfecto() { return this->valorEfecto; }
	bool getRecogido() { return this->recogido; }
	//set
	void getX(int newX) { this->x = newX; }
	void getY(int newY) { this->y = newY; }
	void getNombre(std::string nombre) { this->nombre = nombre; }
	void getTipoEfecto(std::string efecto) { this->tipoEfecto = efecto; }
	void getValorEfecto(int valor) { this->valorEfecto = valor; }
	void getRecogido(bool estado) { this->recogido = estado; }
	//accion
	void aplicarEfecto(Protagonista prota) {};
};

#endif // !ItemMagico_h
