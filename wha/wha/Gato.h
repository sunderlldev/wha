#ifndef Gato_h
#define Gato_h
#include "ObjetoMapa.h"
#include <string>
#include <vector>

class Gato : public ObjetoMapa {
private:
    std::string nombre;
    std::vector<std::string> arteAscii;
public:
    Gato(int x, int y, std::string nom = "Ok")
        : ObjetoMapa(x, y, 24, 4, false, gato), nombre(nom) {
        arteAscii = {
            " _._     _,-'\"\"`-._    ",
            "(,-.`._,'(       |\\`-/|",
            "    `-. - ' \\ )-`( , o o)",
            "          `-    \\`_`\"'-"
        };
    }

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (int r = 0; r < (int)arteAscii.size(); r++) {
            for (int c = 0; c < (int)arteAscii[r].size(); c++) {
                int destinoY = y + r;
                int destinoX = x + c;

                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {

                    char caracterActual = arteAscii[r][c];
                    if (caracterActual != ' ') {
                        matriz[destinoY][destinoX] = caracterActual;
                    }
                }
            }
        }
    }
};

#endif
