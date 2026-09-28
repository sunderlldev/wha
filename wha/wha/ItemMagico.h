#ifndef ItemMagico_h
#define ItemMagico_h
#include <iostream>
#include <string>

class Protagonista;

class ItemMagico {
protected:
	int x, y;
	std::string nombre;
	std::string descripcion;
	std::string tipoItem;
	bool recogido;
public:
	ItemMagico() : x(0), y(0), nombre(""), descripcion(""), tipoItem("Item Magico"), recogido(false) {}
	ItemMagico(int x, int y, std::string n, bool r = false)
		: x(x), y(y), nombre(n), descripcion(""), tipoItem("Item Magico"), recogido(r) {}
	ItemMagico(int x, int y, std::string n, std::string desc, std::string tipo = "Item Magico", bool r = false)
		: x(x), y(y), nombre(n), descripcion(desc), tipoItem(tipo), recogido(r) {}
	virtual ~ItemMagico() {}

	int getX() const { return this->x; }
	int getY() const { return this->y; }
	std::string getNombre() const { return this->nombre; }
	std::string getDescripcion() const { return this->descripcion; }
	std::string getTipoItem() const { return this->tipoItem; }
	bool getRecogido() const { return this->recogido; }

	void setX(int newX) { this->x = newX; }
	void setY(int newY) { this->y = newY; }
	void setNombre(const std::string& n) { this->nombre = n; }
	void setDescripcion(const std::string& d) { this->descripcion = d; }
	void setTipoItem(const std::string& t) { this->tipoItem = t; }
	void setRecogido(bool estado) { this->recogido = estado; }

	virtual void aplicarEfecto(Protagonista*) {}
};

#endif
