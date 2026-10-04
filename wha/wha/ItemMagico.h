#ifndef ItemMagico_h
#define ItemMagico_h
#include <string>

class ItemMagico {
private:
	int x, y;
	std::string nombre;
	std::string descripcion;
	std::string tipoItem;
	bool recogido;
public:
	ItemMagico() : x(0), y(0), nombre(""), descripcion(""), tipoItem("Ítem Mágico"), recogido(false) {}
	ItemMagico(int x, int y, std::string n, std::string desc, std::string tipo = "Ítem Mágico", bool r = false)
		: x(x), y(y), nombre(n), descripcion(desc), tipoItem(tipo), recogido(r) {}
	~ItemMagico() {}

	int getX() const { return this->x; }
	int getY() const { return this->y; }
	std::string getNombre() const { return this->nombre; }
	std::string getDescripcion() const { return this->descripcion; }
	std::string getTipoItem() const { return this->tipoItem; }
	bool getRecogido() const { return this->recogido; }

	void setRecogido(bool estado) { this->recogido = estado; }

	int getAncho() const { return 3; }
	int getAlto() const { return 2; }

	char getCaracter(int r, int c) const {
		if (nombre == "Grimorio de Trazos") {
			if (r == 0) {
				if (c == 0) return '[';
				if (c == 1) return '=';
				if (c == 2) return ']';
			} else {
				if (c == 0) return '/';
				if (c == 1) return '_';
				if (c == 2) return '\\';
			}
		} else if (nombre == "Fibra de Arbol") {
			if (r == 0) {
				if (c == 0) return '(';
				if (c == 1) return '~';
				if (c == 2) return ')';
			} else {
				return '~';
			}
		} else if (nombre == "Pluma Termica") {
			if (r == 0) {
				if (c == 1) return '^';
				return ' ';
			} else {
				if (c == 0) return '/';
				if (c == 1) return '|';
				if (c == 2) return '\\';
			}
		} else if (nombre == "Frasco de Tinta") {
			if (r == 0) {
				if (c == 1) return 'o';
				return ' ';
			} else {
				if (c == 0) return '(';
				if (c == 1) return '_';
				if (c == 2) return ')';
			}
		} else if (nombre == "Tinta de Viento") {
			if (r == 0) {
				if (c == 1) return '*';
				return ' ';
			} else {
				if (c == 0) return '(';
				if (c == 1) return '~';
				if (c == 2) return ')';
			}
		} else {
			if (r == 0) {
				if (c == 0) return '/';
				if (c == 2) return '\\';
				return ' ';
			} else {
				if (c == 0) return '\\';
				if (c == 2) return '/';
				return ' ';
			}
		}
		return ' ';
	}

	int getColor() const {
		if (nombre == "Grimorio de Trazos") return 3;
		if (nombre == "Fibra de Arbol") return 1;
		if (nombre == "Pluma Termica") return 7;
		if (nombre == "Frasco de Tinta") return 5;
		if (nombre == "Tinta de Viento") return 2;
		return 4;
	}
};

#endif
