#ifndef LibretaHechizoAire_h
#define LibretaHechizoAire_h

#include "ItemMagico.h"
#include "Enemigo.h"
#include <vector>
#include <string>

class LibretaHechizoAire : public ItemMagico {
private:
    int cargasRestantes;
    int cargasMaximas;
    int ultimaDirX;
    int ultimaDirY;

    void actualizarDescripcion() {
        descripcion = "Libreta con hechizos de aire de Coco (" +
            std::to_string(cargasRestantes) + "/" +
            std::to_string(cargasMaximas) +
            " cargas). Usa ESPACIO para activar hechizo. Golpea hasta 2 enemigos.";
    }

public:
    LibretaHechizoAire(int x = 0, int y = 0, int cargasIniciales = 3)
        : ItemMagico(x, y, "Libreta de Hechizos de Aire", "", "Hechizo de Aire", false),
        cargasRestantes(cargasIniciales),
        cargasMaximas(3),
        ultimaDirX(1),
        ultimaDirY(0) {
        if (cargasRestantes < 0) cargasRestantes = 0;
        if (cargasRestantes > cargasMaximas) cargasRestantes = cargasMaximas;
        actualizarDescripcion();
    }

    int getCargasRestantes() const { return cargasRestantes; }
    int getCargasMaximas() const { return cargasMaximas; }
    bool tieneCargas() const { return cargasRestantes > 0; }

    void setDireccionMirada(int dx, int dy) {
        if (dx != 0 || dy != 0) {
            ultimaDirX = dx;
            ultimaDirY = dy;
        }
    }

    // Dibuja un librito celeste/cian de 3x2 en el suelo
    char getCaracter(int r, int c) const override{
        if (r == 0) {
            if (c == 0) return '[';
            if (c == 1) return '~';
            if (c == 2) return ']';
        }
        else {
            if (c == 0) return '|';
            if (c == 1) return '*';
            if (c == 2) return '|';
        }
        return ' ';
    }

    int getColor() const override {
        return 3; // Cian brillante
    }

    int disparar(int origenX, int origenY, char dir, std::vector<Enemigo*>& enemigos) {
        if (cargasRestantes <= 0) {
            return 0;
        }

        int dx = 1;
        int dy = 0;

        switch (dir) {
        case 'W':
        case 'w':
            dx = 0;
            dy = -1;
            break;
        case 'S':
        case 's':
            dx = 0;
            dy = 1;
            break;
        case 'A':
        case 'a':
            dx = -1;
            dy = 0;
            break;
        case 'D':
        case 'd':
        default:
            dx = 1;
            dy = 0;
            break;
        }

        cargasRestantes--;
        actualizarDescripcion();

        int derrotados = 0;
        const int maxEnemigos = 2;
        const int alcance = 18;

        for (int paso = 1; paso <= alcance && derrotados < maxEnemigos; paso++) {
            int cx = origenX + dx * paso;
            int cy = origenY + dy * paso;

            for (size_t i = 0; i < enemigos.size() && derrotados < maxEnemigos; i++) {
                Enemigo* en = enemigos[i];
                if (en != nullptr && en->getActivo()) {
                    bool tocaX = (cx >= en->getX() - 1 && cx <= en->getX() + en->getAncho());
                    bool tocaY = (cy >= en->getY() - 1 && cy <= en->getY() + en->getAlto());
                    if (tocaX && tocaY) {
                        en->setActivo(false);
                        derrotados++;
                    }
                }
            }
        }

        return derrotados;
    }
};

#endif