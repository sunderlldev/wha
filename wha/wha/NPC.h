#ifndef NPC_h
#define NPC_h
#include "Personaje.h"
#include <iostream>

class NPC : public Personaje {
private:
	std::string rolPerspectiva;
	bool esHostil;
	std::string mensajeDialogo;
public:
	NPC(int x, int y, std::string n, char s, std::string rol, bool eH) :Personaje(x, y, n, s), rolPerspectiva(rol), esHostil(eH) {};
	~NPC() {};
	//get
	std::string getRolPerspectiva() { return this->rolPerspectiva; }
	bool getEsHostil() { return this->esHostil; }
	std::string getMensajeDialogo() { return this->mensajeDialogo; }
	//set
	void getRolPerspectiva(std::string rP) { this->rolPerspectiva = rP; }
	void getEsHostil(bool eH) { this->esHostil = eH; }
	void getMensajeDialogo(std::string mD) { this->mensajeDialogo = mD; }
	//acciones
	void moverAutomatico() {};
	std::string hablar() {};
};

#endif // !NPC_h
