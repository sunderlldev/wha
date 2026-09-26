#ifndef Inventario_h
#define Inventario_h
#include "ItemMagico.h"
#include <vector>
#include <iostream>

class Inventario {
private:
	std::vector<ItemMagico*> listaItem;
public:
	Inventario() = default;
	~Inventario() = default;
	//acciones
	size_t cantItems() const {return listaItem.size();}//obtener cant de Items en el inventario que seria la maxima capacidad actual
	size_t getCantidad() const {return listaItem.size();}
	void agregarItem(ItemMagico* item) {listaItem.push_back(item);}
	void usarItem(int indice, Protagonista prota) {};
	ItemMagico* getItem(size_t indice) {
		if (indice < listaItem.size()) {
			return listaItem[indice];
		}
		return nullptr;
	}
	void mostarInventario() {
		if (listaItem.empty()) {
			std::cout << "[El inventario está vacío]\n";
			return;
		}

		std::cout << "[INVENTARIO]\n";
		for (size_t i = 0; i < getCantidad(); i++) {
			std::cout << " [" << i + 1 << "] " << listaItem[i]->getNombre() << "\n";
		}
	};
};

#endif // !Inventario_h

