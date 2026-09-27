#ifndef Arbol_h
#define Arbol_h
#include "ObjetoMapa.h"

class Arbol : public ObjetoMapa {
private:
    //la buena practica
    const std::vector<std::string>& arteAscii;

    //diseño segune enum
    static const std::vector<std::string>& obtenerDiseno(TipoObjeto tipo) {
        static const std::vector<std::string> pino = {
            "   /\\  ",
            "  //\\\\ ",
            " ///\\\\\\",
            "   ][  "
        };
        static const std::vector<std::string> frondoso = {
            "    #o#      "
            "  ####o#     "
            " #o# \\#|_#,# "
            "###\\ |/   #o#"
            " # {}{      #"
            "    }{{      "
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

        // Retorna el diseño correspondiente según el valor del enum
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

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (size_t r = 0; r < arteAscii.size(); r++) {
            for (size_t c = 0; c < arteAscii[r].size(); c++) {
                int destinoY = y + r;
                int destinoX = x + c;

                // Verificación de límites de la matriz general
                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {

                    char caracter = arteAscii[r][c];
                    if (caracter != ' ') { // Transparencia en espacios
                        matriz[destinoY][destinoX] = caracter;
                    }
                }
            }
        }
    }
};

#endif // !Arbol_h

