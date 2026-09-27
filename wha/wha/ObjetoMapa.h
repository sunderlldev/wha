#ifndef ObjetoMapa_h
#define ObjetoMapa_h
#include <iostream>
#include <vector>

enum TipoObjeto {
    arbolPino,
    arbolFrondoso,
    arbolGigante,
    habitacion,
    rio,
    camino,
    gato
};

class ObjetoMapa {
protected:
    int x, y;
    int ancho, alto;
    bool esSolido;
    TipoObjeto tipo;

public:
    ObjetoMapa(int x, int y, int ancho, int alto, bool solido, TipoObjeto t)
        : x(x), y(y), ancho(ancho), alto(alto), esSolido(solido), tipo(t) {
    }

    virtual ~ObjetoMapa() {}

    virtual bool colisionaCon(int px, int py) const {
        if (!esSolido) return false;
        return (px >= x && px < x + ancho && py >= y && py < y + alto);
    }

    virtual bool estaDentro(int px, int py) const {
        return (px >= x && px < x + ancho && py >= y && py < y + alto);
    }

    virtual void dibujarEnMatriz(std::vector<std::string>& matriz) = 0;

    // Getters
    int getX() const { return x; }
    int getY() const { return y; }
    int getAncho() const { return ancho; }
    int getAlto() const { return alto; }
    bool getEsSolido() const { return esSolido; }
    TipoObjeto getTipo() const { return tipo; }
};
#endif // !ObjetoMapa_h

