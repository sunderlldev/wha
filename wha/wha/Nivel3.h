#ifndef Nivel3_h
#define Nivel3_h
#include "Nivel.h"

class Nivel3 : public Nivel {
public:
	Nivel3() : Nivel(3, "Gran Árbol de Plata", 200, 600) {}
	virtual ~Nivel3() {}

	virtual void inciarNivel() override {
		this->tiempoInicio = clock();
		this->tiempoFin = 0;
		this->completado = false;
		if (gestorMisiones != nullptr) {
			gestorMisiones->setObjetivoActual("Gran Árbol de Plata - Fase 3");
		}
	}
};

#endif
