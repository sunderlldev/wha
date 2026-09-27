#ifndef Habitacion_h
#define Habitacion_h
#include "ObjetoMapa.h"

class Habitacion : public ObjetoMapa {
private:
    int puertaX, puertaY;
    std::string nombre;
public:
    Habitacion(int x, int y, int ancho, int alto, int px, int py, std::string nom)
        : ObjetoMapa(x, y, ancho, alto, true, habitacion), puertaX(px), puertaY(py), nombre(nom) {
    }

    //para pasar por la puerta
    bool colisionaCon(int px, int py) const override {
        if (px == puertaX && py == puertaY) return false; //puerta

        // Colisión solo en los paredes de la habitacion
        bool bordeX = (px == x || px == x + ancho - 1);
        bool bordeY = (py == y || py == y + alto - 1);
        return (bordeX && py >= y && py < y + alto) || (bordeY && px >= x && px < x + ancho);
    }

    std::string getNombre() const { return nombre; }

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                int posX = x + c;
                int posY = y + r;

                // Comprobar límites
                if (posY < 0 || posY >= (int)matriz.size() ||
                    posX < 0 || posX >= (int)matriz[0].size()) continue;

                // Dibuja las 4 esquinas
                if (r == 0 && c == 0) matriz[posY][posX] = '?';
                else if (r == 0 && c == ancho - 1) matriz[posY][posX] = '?';
                else if (r == alto - 1 && c == 0) matriz[posY][posX] = '?';
                else if (r == alto - 1 && c == ancho - 1) matriz[posY][posX] = '?';
                // Paredes horizontales y verticales
                else if (r == 0 || r == alto - 1) matriz[posY][posX] = '?';
                else if (c == 0 || c == ancho - 1) matriz[posY][posX] = '?';
            }
        }
    }
};
#endif // !Habitacion_h
