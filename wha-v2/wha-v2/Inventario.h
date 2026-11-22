#pragma once

#include <vector>
#include <string>

struct ItemInventario {
	std::string nombre;
	std::string descripcion;
	std::string tipo;
};

class Inventario {
private:
	std::vector<ItemInventario> items;
	int capacidad;

public:
	Inventario() {
		capacidad = 8;
	}

	int getCapacidad() { return capacidad; }
	int getCantidad() { return (int)items.size(); }
	std::vector<ItemInventario>& getItems() { return items; }

	bool tieneItem(const std::string& nombre) {
		for (size_t i = 0; i < items.size(); i++) {
			if (items[i].nombre == nombre) return true;
		}
		return false;
	}

	bool agregarItem(const std::string& nombre, const std::string& descripcion, const std::string& tipo) {
		if ((int)items.size() >= capacidad) return false;
		if (tieneItem(nombre)) return false;
		ItemInventario item;
		item.nombre = nombre;
		item.descripcion = descripcion;
		item.tipo = tipo;
		items.push_back(item);
		return true;
	}

	bool removerItem(const std::string& nombre) {
		for (size_t i = 0; i < items.size(); i++) {
			if (items[i].nombre == nombre) {
				items.erase(items.begin() + i);
				return true;
			}
		}
		return false;
	}

	ItemInventario getItem(int indice) {
		if (indice < 0 || indice >= (int)items.size()) {
			ItemInventario vacio;
			vacio.nombre = "";
			vacio.descripcion = "";
			vacio.tipo = "";
			return vacio;
		}
		return items[indice];
	}

	void limpiar() {
		items.clear();
	}
};
