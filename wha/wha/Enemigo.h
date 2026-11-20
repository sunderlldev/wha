#ifndef Enemigo_h
#define Enemigo_h
#include "Personaje.h"
#include <string>

class Enemigo : public Personaje {
private:
    int danio;
    int color;
    bool activo;
    int idCuartoOrigen;

public:
    Enemigo(int x, int y, std::string nombre = "Sanguijuela", int vida = 2, int danio = 1, int color = 4, int cuarto = 0)
        : Personaje(x, y, nombre, vida), danio(danio), color(color), activo(true), idCuartoOrigen(cuarto) {
        ancho = 4;
        alto = 2;

        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 7; c++) {
                frame1[r][c] = ' ';
                frame2[r][c] = ' ';
            }
        }

        // Frame 1
        frame1[0][0] = '('; frame1[0][1] = 'o'; frame1[0][2] = 'o'; frame1[0][3] = ')';
        frame1[1][0] = '/'; frame1[1][1] = '_'; frame1[1][2] = '_'; frame1[1][3] = '\\';

        // Frame 2
        frame2[0][0] = '('; frame2[0][1] = '>'; frame2[0][2] = '<'; frame2[0][3] = ')';
        frame2[1][0] = '\\'; frame2[1][1] = '-'; frame2[1][2] = '-'; frame2[1][3] = '/';
    }

    int getDanio() const { return this->danio; }
    int getColor() const { return this->color; }
    bool getActivo() const { return this->activo; }
    int getIdCuartoOrigen() const { return this->idCuartoOrigen; }

    void setDanio(int d) { this->danio = d; }
    void setColor(int col) { this->color = col; }
    void setActivo(bool act) { this->activo = act; }
    void setIdCuartoOrigen(int c) { this->idCuartoOrigen = c; }
};

#endif