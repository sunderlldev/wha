#ifndef ObjetoMapa_h
#define ObjetoMapa_h
#include <iostream>
#include <vector>

enum TipoObjeto {
    arbolPino,
    arbolFrondoso,
    habitacion,
    rio,
    camino,
    gato
};

class ObjetoMapa {
protected:
    int x, y;
    int ancho, alto;
    bool esSolido; // Indica si bloquea el paso (colisión)
    TipoObjeto tipo;

public:
    ObjetoMapa(int x, int y, int ancho, int alto, bool solido, TipoObjeto t)
        : x(x), y(y), ancho(ancho), alto(alto), esSolido(solido), tipo(t) {
    }

    virtual ~ObjetoMapa() {}

    // Evalúa si el jugador intenta entrar a un área sólida
    virtual bool colisionaCon(int px, int py) const {
        if (!esSolido) return false;
        return (px >= x && px < x + ancho && py >= y && py < y + alto);
    }

    // Evalúa si el jugador está completamente dentro del área rectangular
    virtual bool estaDentro(int px, int py) const {
        return (px >= x && px < x + ancho && py >= y && py < y + alto);
    }

    // Método virtual puro para que cada objeto dibuje sus caracteres específicos
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

