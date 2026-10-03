#ifndef Nivel2_h
#define Nivel2_h
#include "Nivel.h"

class Nivel2 : public Nivel {
public:
	Nivel2() : Nivel(2, "Tierras Prohibidas", 200, 600) {}
	virtual ~Nivel2() {}

	virtual void inciarNivel() override {
		this->tiempoInicio = clock();
		this->tiempoFin = 0;
		this->completado = false;
		if (gestorMisiones != nullptr) {
			gestorMisiones->setObjetivoActual("Tierras Prohibidas - Fase 2");
		}
	}
};

#endif
