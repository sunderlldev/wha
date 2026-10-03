#ifndef Letrero_h
#define Letrero_h
#include "ObjetoMapa.h"
#include <string>
#include <vector>

class Letrero : public ObjetoMapa {
private:
	std::string texto;

public:
	Letrero(int x, int y, const std::string& texto)
		: ObjetoMapa(x, y, 3, 2, true, TipoObjeto::letrero), texto(texto) {}

	virtual ~Letrero() {}

	const std::string& getTexto() const { return this->texto; }
	void setTexto(const std::string& t) { this->texto = t; }

	virtual void dibujarEnMatriz(std::vector<std::string>& matriz) {
		if (y >= 0 && y + 1 < (int)matriz.size()) {
			if (x >= 0 && x + 2 < (int)matriz[y].size()) {
				matriz[y][x]     = '[';
				matriz[y][x + 1] = '!';
				matriz[y][x + 2] = ']';
				matriz[y + 1][x]     = ' ';
				matriz[y + 1][x + 1] = '|';
				matriz[y + 1][x + 2] = ' ';
			}
		}
	}
};

#endif
