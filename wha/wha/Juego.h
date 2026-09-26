#ifndef Juego_h
#define Juego_h
#include "Nivel.h"
#include <vector>
#include <iostream>

class Juego {
private:
	bool ejecutando;
	int nivelActual;
	std::vector<Nivel> listaNivel;
public:
	Juego() : ejecutando(true), nivelActual(0) {
		listaNivel.push_back(Nivel(1, "nivel 1")); // Nivel 1
		listaNivel.push_back(Nivel(2, "nivel 2")); // Nivel 2
		listaNivel.push_back(Nivel(3, "nivel 3")); // Nivel 3
	}
	Juego() = default;
	~Juego() = default;
	// Métodos para consultar niveles
	Nivel& getNivelActualObj() {
		return listaNivel[nivelActual];
	}
	size_t getCantNiveles() const { return listaNivel.size(); }
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

