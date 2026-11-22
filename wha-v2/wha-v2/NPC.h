#pragma once

#include "Entidad.h"
#include "Configuracion.h"
#include <string>
#include <cmath>

class NPC : public Entidad {
private:
	int idPersonaje;
	std::string nombre;
	std::string rol;
	int radioInteraccion;
	int vecesHablado;

public:
	NPC(int px, int py, int id, std::string nom, std::string r)
		: Entidad(px, py, 24, 32) {
		idPersonaje = id;
		nombre = nom;
		rol = r;
		radioInteraccion = 52;
		vecesHablado = 0;
		idX = 0;
		idY = 0;
		escalaVisual = Configuracion::ESCALA_PERSONAJE;
	}

	virtual ~NPC() {}

	int getIdPersonaje() { return idPersonaje; }
	std::string getNombre() { return nombre; }
	std::string getRol() { return rol; }
	int getVecesHablado() { return vecesHablado; }
	void incrementarVecesHablado() { vecesHablado++; }

	bool estaCerca(int targetX, int targetY) {
		int centroX = x + ancho / 2;
		int centroY = y + alto / 2;
		int dx = centroX - targetX;
		int dy = centroY - targetY;
		return (dx * dx + dy * dy) <= (radioInteraccion * radioInteraccion);
	}

	Rectangle getInteractionRect() {
		return Rectangle(x - 30, y - 30, ancho + 60, alto + 60);
	}
};
