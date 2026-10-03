#ifndef Arbol_h
#define Arbol_h
#include "ObjetoMapa.h"

class Arbol : public ObjetoMapa {
private:
    const std::vector<std::string>& arteAscii;

    static const std::vector<std::string>& obtenerDiseno(TipoObjeto tipo) {
        static const std::vector<std::string> pino = {
            "   /\\  ",
            "  //\\\\ ",
            " ///\\\\\\",
            "   ][  "
        };
        static const std::vector<std::string> frondoso = {
            "    #o#      ",
            "  ####o#     ",
            " #o# \\#|_#,# ",
            "###\\ |/   #o#",
            " # {}{      #",
            "    }{{      ",
            "   ,'  `     "
        };
        static const std::vector<std::string> gigante = {
            "        &&                ",
            "      &&&&&               ",
            "    &&&\\/& &&&            ",
            "   &&|,/  |/& &&          ",
            "    &&/   /  /_&  &&      ",
            "      \\  {  |_____/_&     ",
            "      {  / /          &&& ",
            "      `, \\{___________/_&&",
            "       } }{       \\       ",
            "       }{{         \\____& ",
            "      {}{           `&\\&& ",
            "      {{}             &&  ",
            ", -=-~{ .-^- _            ",
            "      `}                  ",
            "       {                  "
        };

        switch (tipo) {
        case arbolFrondoso:
            return frondoso;
        case arbolGigante:
            return gigante;
        case arbolPino:
        default:
            return pino;
        }
    }
public:
    Arbol(int x, int y, TipoObjeto tipo)
        : ObjetoMapa(x, y, 0, 0, true, tipo),
        arteAscii(obtenerDiseno(tipo)) {
        this->ancho = (int)arteAscii[0].size();
        this->alto = (int)arteAscii.size();
    }

    bool colisionaCon(int px, int py) const override {
        if (!esSolido) return false;
        int r = py - y;
        int c = px - x;
        if (r >= 0 && r < alto && c >= 0 && c < ancho) {
            return arteAscii[r][c] != ' ';
        }
        return false;
    }

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (int r = 0; r < (int)arteAscii.size(); r++) {
            for (int c = 0; c < (int)arteAscii[r].size(); c++) {
                int destinoY = y + r;
                int destinoX = x + c;

                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {

                    char caracter = arteAscii[r][c];
                    if (caracter != ' ') {
                        matriz[destinoY][destinoX] = caracter;
                    }
                }
            }
        }
    }
};

#endif
