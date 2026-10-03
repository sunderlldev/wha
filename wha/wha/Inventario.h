#ifndef Inventario_h
#define Inventario_h
#include "ItemMagico.h"
#include <vector>
#include <string>
#include <iostream>

class Inventario {
private:
	std::vector<ItemMagico*> listaItem;
	int capacidadMaxima;
public:
	Inventario(int capacidad = 6) : capacidadMaxima(capacidad) {}
	~Inventario() {
		for (size_t i = 0; i < listaItem.size(); i++) {
			if (listaItem[i] != nullptr) {
				delete listaItem[i];
			}
		}
		listaItem.clear();
	}

	size_t getCantidad() const { return listaItem.size(); }
	int getCapacidadMaxima() const { return capacidadMaxima; }
	void setCapacidadMaxima(int cap) { capacidadMaxima = cap; }

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

	void vaciar() {
		for (size_t i = 0; i < listaItem.size(); i++) {
			if (listaItem[i] != nullptr) {
				delete listaItem[i];
			}
		}
		listaItem.clear();
	}

	void mostarInventario() {
		if (listaItem.empty()) {
			std::cout << "[El inventario esta vacio]\n";
			return;
		}
		std::cout << "[INVENTARIO]\n";
		for (size_t i = 0; i < listaItem.size(); i++) {
			if (listaItem[i] != nullptr) {
				std::cout << " [" << i + 1 << "] " << listaItem[i]->getNombre() << "\n";
			}
		}
	}
};

#endif
