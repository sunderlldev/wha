#ifndef NPC_h
#define NPC_h
#include "Personaje.h"
#include <iostream>
#include <string>

class NPC : public Personaje {
private:
	std::string rolPerspectiva;
	int confianza;
	bool yaHablo;
	bool dioTinta;
	bool crafteoCapa;
public:
	NPC(int x, int y, std::string n, std::string rol = "Hechicero")
		: Personaje(x, y, n, 100), rolPerspectiva(rol), confianza(0), yaHablo(false), dioTinta(false), crafteoCapa(false) {
		ancho = 7;
		alto = 4;
		for (int r = 0; r < 4; r++) {
			for (int c = 0; c < 7; c++) {
				frame1[r][c] = ' ';
				frame2[r][c] = ' ';
			}
		}

		if (n == "Agott") {
			const char* f1[4] = {
				"  /  \\ ",
				" /____\\",
				"( -_- )",
				" | || |"
			};
			const char* f2[4] = {
				"  /  \\ ",
				" /____\\",
				"( -.- )",
				" | || |"
			};
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 7; c++) {
					frame1[r][c] = f1[r][c];
					frame2[r][c] = f2[r][c];
				}
			}
		} else if (n == "Qifrey") {
			const char* f1[4] = {
				"  /  \\ ",
				" /____\\",
				"( 0_. )",
				" / || \\"
			};
			const char* f2[4] = {
				"  /  \\ ",
				" /____\\",
				"( -_. )",
				" / || \\"
			};
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 7; c++) {
					frame1[r][c] = f1[r][c];
					frame2[r][c] = f2[r][c];
				}
			}
		} else if (n == "Richeh") {
			const char* f1[4] = {
				"  /  \\ ",
				" /____\\",
				"( ._. )",
				" / || \\"
			};
			const char* f2[4] = {
				"  /  \\ ",
				" /____\\",
				"( -_- )",
				" / || \\"
			};
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 7; c++) {
					frame1[r][c] = f1[r][c];
					frame2[r][c] = f2[r][c];
				}
			}
		} else {
			char inicial = n.empty() ? 'N' : n[0];
			const char* f1[4] = {
				"  /  \\ ",
				" /____\\",
				"(     )",
				" / || \\"
			};
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 7; c++) {
					frame1[r][c] = f1[r][c];
					frame2[r][c] = f1[r][c];
				}
			}
			frame1[2][3] = inicial;
			frame2[2][3] = inicial;
		}
	}

	virtual ~NPC() {}

	std::string getRolPerspectiva() const { return this->rolPerspectiva; }
	int getConfianza() const { return this->confianza; }
	bool getYaHablo() const { return this->yaHablo; }
	bool getDioTinta() const { return this->dioTinta; }
	bool getCrafteoCapa() const { return this->crafteoCapa; }

	void setRolPerspectiva(const std::string& rP) { this->rolPerspectiva = rP; }
	void setConfianza(int c) { this->confianza = c; }
	void setYaHablo(bool yh) { this->yaHablo = yh; }
	void setDioTinta(bool dt) { this->dioTinta = dt; }
	void setCrafteoCapa(bool cc) { this->crafteoCapa = cc; }

	std::string getDescripcionConfianza() const {
		if (confianza == 0) return "Sin confianza";
		if (confianza == 1) return "Neutral";
		return "Amigos";
	}

	void setExpresion(int exp) {
		if (this->nombre == "Richeh") {
			if (exp == 1) {
				frame1[2][2] = 'O';
				frame1[2][3] = '_';
				frame1[2][4] = 'O';
				frame2[2][2] = 'O';
				frame2[2][3] = '_';
				frame2[2][4] = 'O';
			} else if (exp == 2) {
				frame1[2][2] = '^';
				frame1[2][3] = 'u';
				frame1[2][4] = '^';
				frame2[2][2] = '^';
				frame2[2][3] = 'u';
				frame2[2][4] = '^';
			} else {
				frame1[2][2] = '.';
				frame1[2][3] = '_';
				frame1[2][4] = '.';
				frame2[2][2] = '-';
				frame2[2][3] = '_';
				frame2[2][4] = '-';
			}
		}
	}
};

#endif
