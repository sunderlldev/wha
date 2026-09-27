#ifndef Personaje_h
#define Personaje_h
#include <iostream>
#include "Sprite.h"

class Personaje {
protected:
    int x, y;
    std::string nombre;
    int vida;
    int vidaMaxima;

    // Matriz de píxeles del personaje (o un objeto Sprite)
    std::vector<std::vector<PixelArt>> sprite;

public:
    // Constructor por defecto
    Personaje()
        : x(0), y(0), nombre("Sin Nombre"), vida(100), vidaMaxima(100) {
    }

    // Constructor parametrizado
    Personaje(int x, int y, std::string n, int v = 100)
        : x(x), y(y), nombre(n), vida(v), vidaMaxima(v) {
    }

    // Destructor VIRTUAL
    virtual ~Personaje() {}

    // Getters
    int getX() const { return this->x; }
    int getY() const { return this->y; }
    std::string getNombre() const { return this->nombre; }
    int getVida() const { return this->vida; }
    int getVidaMaxima() const { return this->vidaMaxima; }

    int getAnchoSprite() const { return sprite.empty() ? 0 : (int)sprite[0].size(); }
    int getAltoSprite() const { return (int)sprite.size(); }

    // Setters
    void setX(int newX) { this->x = newX; }
    void setY(int newY) { this->y = newY; }
    void setNombre(const std::string& newNombre) { this->nombre = newNombre; }
    void setVida(int newVida) { this->vida = newVida; }
    void setVidaMaxima(int newVidaMaxima) { this->vidaMaxima = newVidaMaxima; }

    // Asignar la matriz del Pixel Art
    void setSprite(const std::vector<std::vector<PixelArt>>& nuevoSprite) {
        this->sprite = nuevoSprite;
    }

    // Métodos de acción
    virtual void mover() {}
    virtual void recibirDanio(int cantidad) {
        this->vida -= cantidad;
        if (this->vida < 0) this->vida = 0;
    }
    virtual bool estaVivo() const {
        return this->vida > 0;
    }
};

#endif // !Personaje_h
