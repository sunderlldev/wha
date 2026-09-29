#ifndef Personaje_h
#define Personaje_h
#include <iostream>

class Personaje {
protected:
    int x, y;
    std::string nombre;
    int vida;
    int vidaMaxima;
    int ancho;
    int alto;
    char frame1[4][5];
    char frame2[4][5];
    int frameActual;
    int temporizadorAnimacion;

public:
    Personaje()
        : x(0), y(0), nombre("Sin Nombre"), vida(3), vidaMaxima(3),
          ancho(2), alto(2), frameActual(0), temporizadorAnimacion(0) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 5; c++) {
                frame1[r][c] = ' ';
                frame2[r][c] = ' ';
            }
        }
        frame1[0][0] = '('; frame1[0][1] = ')';
        frame1[1][0] = '/'; frame1[1][1] = '\\';
        frame2[0][0] = '('; frame2[0][1] = ')';
        frame2[1][0] = '|'; frame2[1][1] = '|';
    }

    Personaje(int x, int y, std::string n, int v = 3)
        : x(x), y(y), nombre(n), vida(v), vidaMaxima(v),
          ancho(2), alto(2), frameActual(0), temporizadorAnimacion(0) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 5; c++) {
                frame1[r][c] = ' ';
                frame2[r][c] = ' ';
            }
        }
        frame1[0][0] = '('; frame1[0][1] = ')';
        frame1[1][0] = '/'; frame1[1][1] = '\\';
        frame2[0][0] = '('; frame2[0][1] = ')';
        frame2[1][0] = '|'; frame2[1][1] = '|';
    }

    virtual ~Personaje() {}

    int getX() const { return this->x; }
    int getY() const { return this->y; }
    std::string getNombre() const { return this->nombre; }
    int getVida() const { return this->vida; }
    int getVidaMaxima() const { return this->vidaMaxima; }
    int getAncho() const { return this->ancho; }
    int getAlto() const { return this->alto; }
    int getFrameActual() const { return this->frameActual; }
    int getAnchoSprite() const { return this->ancho; }
    int getAltoSprite() const { return this->alto; }

    char getCaracter(int fila, int col) const {
        if (fila < 0 || fila >= this->alto || col < 0 || col >= this->ancho) return ' ';
        if (frameActual == 0) return frame1[fila][col];
        return frame2[fila][col];
    }

    void actualizarAnimacion(int deltaMs) {
        temporizadorAnimacion += deltaMs;
        if (temporizadorAnimacion >= 500) {
            temporizadorAnimacion = 0;
            frameActual = (frameActual == 0) ? 1 : 0;
        }
    }

    void setX(int newX) { this->x = newX; }
    void setY(int newY) { this->y = newY; }
    void setNombre(const std::string& newNombre) { this->nombre = newNombre; }
    void setVida(int newVida) { this->vida = newVida; }
    void setVidaMaxima(int newVidaMaxima) { this->vidaMaxima = newVidaMaxima; }

    void setFrame1(char f00, char f01, char f10, char f11) {
        frame1[0][0] = f00; frame1[0][1] = f01;
        frame1[1][0] = f10; frame1[1][1] = f11;
    }

    void setFrame2(char f00, char f01, char f10, char f11) {
        frame2[0][0] = f00; frame2[0][1] = f01;
        frame2[1][0] = f10; frame2[1][1] = f11;
    }

    virtual void mover() {}
    virtual void recibirDanio(int cantidad) {
        this->vida -= cantidad;
        if (this->vida < 0) this->vida = 0;
    }
    virtual bool estaVivo() const {
        return this->vida > 0;
    }
};

#endif
