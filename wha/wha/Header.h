#pragma once

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
	Personaje(int x, int y, std::string n, int v, char s): x(x), y(y), nombre(n), vida(v), simbolo(s){};
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

class NPC : public Personaje {
private:
	std::string rolPerspectiva;
	bool esHostil;
	std::string mensajeDialogo;
public:
	NPC() {};
	~NPC() {};
	//get
	std::string getRolPerspectiva() { return this->rolPerspectiva; }
	bool getEsHostil() { return this->esHostil; }
	std::string getMensajeDialogo() { return this->mensajeDialogo; }
	//set
	void getRolPerspectiva(std::string rP) {  this->rolPerspectiva=rP; }
	void getEsHostil(bool eH) {  this->esHostil=eH; }
	void getMensajeDialogo(std::string mD) {  this->mensajeDialogo=mD; }
	//acciones
	void moverAutomatico() {};
	std::string hablar() {};
};

class Protagonista: public Personaje {
private:
	int tintaPlata;
	int tintaMaxima;
	int perspectiva;
	bool tieneCapaVuelo;
	bool tieneSemillaPlata;
public:
	Protagonista(int x, int y, std::string n, int v, int p) {};
	~Protagonista() {};
	//get
	int getTintaPabla() { return this->tintaPlata; }
	int getTintaMaximo() { return this->tintaMaxima; }
	int getPerspectiva() { return this->perspectiva; }
	bool getTieneCapaVuelo() { return this->tieneCapaVuelo; }
	//set
	void getTintaPabla(int newTintaPlata) {  this->tintaPlata = newTintaPlata; }
	void getTintaMaximo(int newTintaMagica) { this->tintaMaxima = newTintaMagica; }
	void getPerspectiva(int newPersvectica) {  this->perspectiva= newPersvectica; }
	void getTieneCapaVuelo(bool tieneCapaVuelo) { this->tieneCapaVuelo = tieneCapaVuelo; }
	//acciones
	void usarHabilidadPerspectiva() {};
	void tomarDesicionEtica() {};
	void interactuar(NPC) {};
};

class Juego {

};

