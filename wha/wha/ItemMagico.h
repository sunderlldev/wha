#ifndef ItemMagico_h
#define ItemMagico_h
#include "Protagonista.h"
#include <iostream>

class ItemMagico {
protected:
	int y, x;
	std::string nombre;
	bool recogido;
public:
	ItemMagico() {};
	ItemMagico(int x, int y, std::string n, bool r) :x(x), y(y), nombre(n), recogido(r) {};
	~ItemMagico() {};
	//get
	int getX() { return this->x; }
	int getY() { return this->y; }
	std::string getNombre() { return this->nombre; }
	bool getRecogido() { return this->recogido; }
	//set
	void getX(int newX) { this->x = newX; }
	void getY(int newY) { this->y = newY; }
	void getNombre(std::string nombre) { this->nombre = nombre; }
	void getRecogido(bool estado) { this->recogido = estado; }
	//accion
	void aplicarEfecto(Protagonista prota) {};
};

#endif // !ItemMagico_h
