#ifndef Personaje_h
#define Personaje_h
#include <iostream>

class Personaje {
protected:
	int x, y;
	std::string nombre;
	int vida;
	int vidaMaxima;
	char simbolo;
public:
	Personaje() {};
	Personaje(int x, int y, std::string n, int v, char s) : x(x), y(y), nombre(n), vida(v), simbolo(s) {};
	~Personaje() {};
	//get obtener
	int getX() { return this->x; }
	int getY() { return this->y; }
	std::string getNombre() { return this->nombre; }
	int getVida() { return this->vida; }
	int getVidaMaxima() { return this->vidaMaxima; }
	char getSimbolo() { return this->simbolo; }
	//set
	void getX(int newX) { this->x = newX; }
	void getY(int newY) { this->y = newY; }
	void getNombre(std::string newNombre) { this->nombre = newNombre; }
	void getVida(int newVida) { this->vida = newVida; }
	void getVidaMaxima(int vidaMaxima) { this->vidaMaxima = vidaMaxima; }
	void getSimbolo(char newSimbolo) { this->simbolo = newSimbolo; }

	//Metodos de accion
	void mover() {};
	void recibirDanio() {};
	bool estaVivo() {};
};

#endif // !Personaje_h
