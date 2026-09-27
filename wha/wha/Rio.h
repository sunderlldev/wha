#ifndef Rio_h
#define Rio_h
#include "ObjetoMapa.h"

class Rio : public ObjetoMapa {
private:
    char patronAgua;
public:
    Rio(int x, int y, int ancho, int alto, char patron = char(178))
        : ObjetoMapa(x, y, ancho, alto, true, rio), patronAgua(patron) {
    }

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                int destinoY = y + r;
                int destinoX = x + c;

                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {

                    matriz[destinoY][destinoX] = patronAgua;
                }
            }
        }
    };
};
#endif // !Rio_h

