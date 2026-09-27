#ifndef Nivel_h
#define Nivel_h
#include <iostream>
#include "Mapa.h"

class Nivel {
private:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
	Mapa* mapa;
public:
	Nivel() : numeroNivel(1), nombreNivel("Inicio"), completado(false), mapa(nullptr) {}
	Nivel(int numeroN, std::string nombreN, int filasMapa, int columnasMapa)
		: numeroNivel(numeroN), nombreNivel(nombreN), completado(false) {
		//instancia para el mapeta 1
		this->mapa = new Mapa(filasMapa, columnasMapa);
	}
	~Nivel() {
		if (this->mapa != nullptr) {
			delete this->mapa;
			this->mapa = nullptr;
		}
	};
	//get (obtener)
	int getNumeroNivel() { return this->numeroNivel; }
	std::string getNombreNivel() { return this->nombreNivel; }
	bool getCompletado() { return this->completado; }
	Mapa* getMapa() { return this->mapa; }  //retorna el puntero del mapa
	//set (asignar)
	void setNumeroNivel(int numero) { this->numeroNivel = numero; }
	void setNombreNivel(std::string nombre) { this->nombreNivel = nombre; }
	void setCompletado(bool estado) { this->completado = estado; }
	void setMapa(Mapa* nuevoMapa) {
		if (this->mapa != nullptr) delete this->mapa;
		this->mapa = nuevoMapa;
	}
	//acciones
	void inciarNivel() {};
	bool verificarObjetivo() {};
	void mostrarPrologo() {};
	void mostrarMuerteArbolPlata() {};
	void actualizar() {};
};

#endif // !Nivel_h

