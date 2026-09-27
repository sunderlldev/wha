#ifndef Gato_h
#define Gato_h
#include "ObjetoMapa.h"

class Gato : public ObjetoMapa {
private:
    std::string nombre;
    std::vector<std::string> arteAscii;
public:
    Gato(int x, int y, std::string nom = "Ok")
        : ObjetoMapa(x, y, 24, 4, false, gato), nombre(nom) {
        arteAscii = {
            " _._     _,-'\"\"`-._    "
            "(,-.`._,'(       |\\`-/|"
            "    `-. - ' \\ )-`( , o o)"
            "          `-    \\`_`\"'-"
        };
    }

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (size_t r = 0; r < arteAscii.size(); r++) {
            for (size_t c = 0; c < arteAscii[r].size(); c++) {

                int destinoY = y + r;
                int destinoX = x + c;

                //limites
                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {

                    char caracterActual = arteAscii[r][c];
                    if (caracterActual != ' ') {
                        matriz[destinoY][destinoX] = caracterActual;
                    };
                };
            };
        };
    }
};

#endif // !Gato_h

