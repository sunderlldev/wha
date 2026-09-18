#ifndef Inventario_h
#define Inventario_h
#include "ItemMagico.h"
#include <iostream>

class Inventario {
private:
	ItemMagico** listaItem;
	int capacidad, cantidadActual;
public:
	Inventario(int c) : capacidad(c) {};
	~Inventario() {};
	//get
	int getCapacidad() { return this->capacidad; }
	int getCantidad() { return this->cantidadActual; }
	//set
	void getCapacidad(int capacidad) { this->capacidad = capacidad; }
	void getCantidad(int cantidad) { this->cantidadActual = cantidad; }
	//acciones
	bool agregarItem(ItemMagico* item) {};
	void usarItem(int indice, Protagonista prota) {};
	void mostarInventario() {};
};

#endif // !Inventario_h

