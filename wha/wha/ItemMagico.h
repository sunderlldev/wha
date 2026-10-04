#ifndef ItemMagico_h
#define ItemMagico_h
#include <string>

class ItemMagico {
protected:
	int x, y;
	std::string nombre;
	std::string descripcion;
	std::string tipoItem;
	bool recogido;
public:
	ItemMagico() : x(0), y(0), nombre(""), descripcion(""), tipoItem("Item Magico"), recogido(false) {}
	ItemMagico(int x, int y, std::string n, std::string desc, std::string tipo = "Item Magico", bool r = false)
		: x(x), y(y), nombre(n), descripcion(desc), tipoItem(tipo), recogido(r) {}
	virtual ~ItemMagico() {}

	int getX() const { return this->x; }
	int getY() const { return this->y; }
	std::string getNombre() const { return this->nombre; }
	std::string getDescripcion() const { return this->descripcion; }
	std::string getTipoItem() const { return this->tipoItem; }
	bool getRecogido() const { return this->recogido; }

	void setRecogido(bool estado) { this->recogido = estado; }

	virtual int getAncho() const { return 3; }
	virtual int getAlto() const { return 2; }

	virtual char getCaracter(int r, int c) const {
		if (nombre == "Libro de hechizos") {
			if (r == 0) {
				if (c == 0) return '[';
				if (c == 1) return '=';
				if (c == 2) return ']';
			} else {
				if (c == 0) return '/';
				if (c == 1) return '_';
				if (c == 2) return '\\';
			}
		} else if (nombre == "Tela") {
			if (r == 0) {
				if (c == 0) return '(';
				if (c == 1) return '~';
				if (c == 2) return ')';
			} else {
				return '~';
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

	virtual int getColor() const {
		if (nombre == "Libro de hechizos") return 3;
		if (nombre == "Tela") return 1;
		if (nombre == "Frasco de Tinta") return 5;
		if (nombre == "Tinta de Viento") return 2;
		return 4;
	}
};

#endif
