#ifndef Juego_h
#define Juego_h
#include "Nivel.h"
#include <iostream>

class Juego {
private:
	bool ejecutando;
	int nivelActual;
	Nivel** ListaNivel[3];
public:
	Juego() {};
	~Juego() {};
	//get (obtener)
	bool getEjeecutando() { return this->ejecutando; }
	int getNivelActual() { return this->nivelActual; }
	//set
	void setEjecutando(bool estado) { this->ejecutando = estado; }
	void setNivelActual(int nivel) { this->nivelActual = nivel; }
	//acciones
	void menuPrincipal() {};
	void cambiarNivel() {};
	void mostrarDesenlaceFinal() {};
	void mostrarPreguntaReflexxiva() {};
};

#endif // !Juego_h

