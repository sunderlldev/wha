#ifndef Inventario_h
#define Inventario_h
#include "ItemMagico.h"
#include <vector>
#include <string>

class Inventario {
private:
	std::vector<ItemMagico*> listaItem;
	int capacidadMaxima;
public:
	Inventario(int capacidad = 8) : capacidadMaxima(capacidad) {}
	~Inventario() {
		for (size_t i = 0; i < listaItem.size(); i++) {
			if (listaItem[i] != nullptr) {
				delete listaItem[i];
			}
		}
		listaItem.clear();
	}

	bool agregarItem(ItemMagico* item) {
		if ((int)listaItem.size() >= capacidadMaxima) {
			return false;
		}
		listaItem.push_back(item);
		return true;
	}

	bool tieneItem(const std::string& nombre) const {
		for (size_t i = 0; i < listaItem.size(); i++) {
			if (listaItem[i] != nullptr && listaItem[i]->getNombre() == nombre) {
				return true;
			}
		}
		return false;
	}

	bool removerItem(const std::string& nombre) {
		for (size_t i = 0; i < listaItem.size(); i++) {
			if (listaItem[i] != nullptr && listaItem[i]->getNombre() == nombre) {
				delete listaItem[i];
				listaItem.erase(listaItem.begin() + i);
				return true;
			}
		}
		return false;
	}

	ItemMagico* getItem(size_t indice) {
		if (indice < listaItem.size()) {
			return listaItem[indice];
		}
		return nullptr;
	}
};

#endif
