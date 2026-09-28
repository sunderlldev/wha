#ifndef NPC_h
#define NPC_h
#include "Personaje.h"
#include <iostream>
#include <string>

class NPC : public Personaje {
private:
	std::string rolPerspectiva;
	bool esHostil;
	std::string mensajeDialogo;
	int confianza;
	bool yaHablo;
	bool dioTinta;
	bool crafteoCapa;
public:
	NPC(int x, int y, std::string n, char s, std::string rol, bool eH)
		: Personaje(x, y, n, 100), rolPerspectiva(rol), esHostil(eH), mensajeDialogo(""), confianza(0), yaHablo(false), dioTinta(false), crafteoCapa(false) {
		frame1[0][0] = '('; frame1[0][1] = s;
		frame1[1][0] = '/'; frame1[1][1] = '\\';
		frame2[0][0] = '('; frame2[0][1] = s;
		frame2[1][0] = '|'; frame2[1][1] = '|';
	}

	NPC(int x, int y, std::string n, std::string rol = "Hechicero", bool eH = false)
		: Personaje(x, y, n, 100), rolPerspectiva(rol), esHostil(eH), mensajeDialogo(""), confianza(0), yaHablo(false), dioTinta(false), crafteoCapa(false) {
		char inicial = n.empty() ? 'N' : n[0];
		frame1[0][0] = '('; frame1[0][1] = inicial;
		frame1[1][0] = '/'; frame1[1][1] = '\\';
		frame2[0][0] = '('; frame2[0][1] = inicial;
		frame2[1][0] = '|'; frame2[1][1] = '|';
	}

	virtual ~NPC() {}

	std::string getRolPerspectiva() const { return this->rolPerspectiva; }
	bool getEsHostil() const { return this->esHostil; }
	std::string getMensajeDialogo() const { return this->mensajeDialogo; }
	int getConfianza() const { return this->confianza; }
	bool getYaHablo() const { return this->yaHablo; }
	bool getDioTinta() const { return this->dioTinta; }
	bool getCrafteoCapa() const { return this->crafteoCapa; }

	void setRolPerspectiva(const std::string& rP) { this->rolPerspectiva = rP; }
	void setEsHostil(bool eH) { this->esHostil = eH; }
	void setMensajeDialogo(const std::string& mD) { this->mensajeDialogo = mD; }
	void setConfianza(int c) { this->confianza = c; }
	void setYaHablo(bool yh) { this->yaHablo = yh; }
	void setDioTinta(bool dt) { this->dioTinta = dt; }
	void setCrafteoCapa(bool cc) { this->crafteoCapa = cc; }

	std::string getDescripcionConfianza() const {
		if (confianza == 0) return "Sin confianza";
		if (confianza == 1) return "Neutral";
		return "Confianza plena";
	}

	void moverAutomatico() {}
	std::string hablar() { return this->mensajeDialogo; }
};

#endif
