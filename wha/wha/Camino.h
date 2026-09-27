#ifndef Camino_h
#define Camino_h

#include "ObjetoMapa.h"
#include <vector>

class Camino : public ObjetoMapa {
private:
    char patronCamino; // Carácter para representar el camino (por defecto '.')

public:
    // Constructor: recibe la posición (x, y), dimensiones y opcionalmente el carácter del piso
    // Nota: 'esColisionable' se establece en 'false' para permitir el paso
    Camino(int x, int y, int ancho, int alto, char patron = '.')
        : ObjetoMapa(x, y, ancho, alto, false, camino), patronCamino(patron) {
    }

    // Dibuja la franja o zona de camino en la matriz del mapa
    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                int destinoY = y + r;
                int destinoX = x + c;

                // Verificación de límites dentro del mapa
                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {

                    matriz[destinoY][destinoX] = patronCamino;
                }
            }
        }
    }
};

#endif // !Camino_h
