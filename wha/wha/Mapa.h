#ifndef Mapa_h
#define Mapa_h
#include <iostream>

class Mapa {
private:
	int filas, columnas;
public:
	Mapa(int f, int c) : filas(f), columnas(c) {};
	~Mapa() {};
	//get
	int getFilas() { return this->filas; }
	int getColumnas() { return this->columnas; }
	//set
	void setFilas(int f) { this->filas = f; }
	void setColumnas(int c) { this->columnas = c; }
	//acciones
	bool esPosicionValida(int x, int y) {};
	void actualizarNieblaVision(int px, int py, int radio) {};
	void dibujarMapa() {};
};

#endif // !Mapa_h

