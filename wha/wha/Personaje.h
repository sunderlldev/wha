#ifndef Personaje_h
#define Personaje_h
#include <string>

class Personaje {
protected:
    int x, y;
    std::string nombre;
    int vida;
    int vidaMaxima;
    int ancho;
    int alto;
    char frame1[4][7];
    char frame2[4][7];
    int frameActual;
    int temporizadorAnimacion;
    
public:
    Personaje()
        : x(0), y(0), nombre("Sin Nombre"), vida(3), vidaMaxima(3),
          ancho(2), alto(2), frameActual(0), temporizadorAnimacion(0) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 7; c++) {
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
            for (int c = 0; c < 7; c++) {
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
    void setVida(int newVida) { this->vida = newVida; }
    void setVidaMaxima(int newVidaMaxima) { this->vidaMaxima = newVidaMaxima; }
};

#endif
