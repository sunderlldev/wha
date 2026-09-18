#ifndef Nivel_h
#define Nivel_h
#include <iostream>

class Nivel {
private:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
public:
	Nivel(int numeroN, std::string nombreN) : numeroNivel(numeroN), nombreNivel(nombreN) {};
	~Nivel() {};
	//get (obtener)
	int getNumeroNivel() { return this->numeroNivel; }
	std::string getNombreNivel() { return this->nombreNivel; }
	bool getCompletado() { return this->completado; }
	//set (asignar)
	void setNumeroNivel(int numero) { this->numeroNivel = numero; }
	void setNombreNivel(std::string nombre) { this->nombreNivel = nombre; }
	void setCompletado(bool estado) { this->completado = estado; }
	//acciones
	void inciarNivel() {};
	bool verificarObjetivo() {};
	void mostrarPrologo() {};
	void mostrarMuerteArbolPlata() {};
	void actualizar() {};
};

#endif // !Nivel_h

