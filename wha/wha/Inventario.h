#ifndef Inventario_h
#define Inventario_h
#include "ItemMagico.h"
#include <malloc.h>
#include <iostream>

void getLongitud(ItemMagico** i) {
	std::cout << "tamano de puntero: " << sizeof(i) << "\n";
	std::cout << "Espacio reservado para un puntero: " << _msize(i) << "\n";
	std::cout << "Tamano del arreglo: " << _msize(i) / sizeof(i);
}



class Inventario {
private:
	ItemMagico** listaItem;
	int cantidadActual;
public:
	Inventario() {
		listaItem = nullptr;
	};
	~Inventario() {};
	//get
	int getCantidad() { return this->cantidadActual; }
	//set
	void getCantidad(int cantidad) { this->cantidadActual = cantidad; }
	//acciones
	//obtener longitud del arreglo inventario
	size_t cantItems() {
		if (listaItem == nullptr) return 0;
		return _msize(listaItem) / sizeof(ItemMagico*);
	};
	void agregarItem(ItemMagico* item) {
		size_t cantidadActual = cantItems();
		ItemMagico** temp = new ItemMagico*[cantidadActual + 1];
		for (size_t i = 0; i < cantidadActual; i++)
		{
			temp[i] = listaItem[i];
		}
		temp[cantidadActual] = item;
		delete[] listaItem;
		listaItem = temp;
	};
	void usarItem(int indice, Protagonista prota) {};
	void mostarInventario() {};
};

#endif // !Inventario_h

