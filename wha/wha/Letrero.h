#ifndef Letrero_h
#define Letrero_h
#include "ObjetoMapa.h"
#include <string>
#include <vector>

class Letrero : public ObjetoMapa {
private:
	std::string texto;
	bool leido;

public:
	Letrero(int x, int y, const std::string& texto)
		: ObjetoMapa(x, y, 3, 1, false, TipoObjeto::letrero), texto(texto), leido(false) {}

	virtual ~Letrero() {}

	const std::string& getTexto() const { return this->texto; }
	bool getLeido() const { return this->leido; }
	void marcarLeido() { this->leido = true; }

	virtual void dibujarEnMatriz(std::vector<std::string>& matriz) {
		if (y >= 0 && y < (int)matriz.size()) {
			if (x >= 0 && x + 2 < (int)matriz[y].size()) {
				matriz[y][x]     = '[';
				matriz[y][x + 1] = '!';
				matriz[y][x + 2] = ']';
			}
		}
	}
};

#endif
