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
		listaNivel.push_back(Nivel(1, "Nivel 1", 200, 600)); // Nivel 1
		listaNivel.push_back(Nivel(2, "Nivel 2", 200, 600)); // Nivel 2
		listaNivel.push_back(Nivel(3, "Nivel 3", 200, 600)); // Nivel 3
		listaNivel[nivelActual].inciarNivel();
	}
	~Juego() = default;
	// Métodos para consultar niveles
	Nivel& getNivelActualObj() {
		return listaNivel[nivelActual];
	}
	size_t getCantNiveles() const { return listaNivel.size(); }
	//get (obtener)
	bool getEjecutando() { return this->ejecutando; }
	int getNivelActual() { return this->nivelActual; }
	//set
	void setEjecutando(bool estado) { this->ejecutando = estado; }
	void setNivelActual(int nivel) { this->nivelActual = nivel; }
	//acciones
	void menuPrincipal() {};
	void cambiarNivel() {
		if (nivelActual + 1 < (int)listaNivel.size()) {
			nivelActual++;
			listaNivel[nivelActual].inciarNivel();
		}
		else {
			mostrarDesenlaceFinal();
		}
	};
	void mostrarDesenlaceFinal() {
		std::cout << "¡Vamos campeon completaste el juego, 10 horas, 6 horas!\n";
	};
	void mostrarPreguntaReflexxiva() {};
	void actualizar() {
		if (!ejecutando) return;

		getNivelActualObj().actualizar();
	}
};

#endif // !Juego_h

