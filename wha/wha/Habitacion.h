#ifndef Habitacion_h
#define Habitacion_h
#include "ObjetoMapa.h"

enum TipoHabitacion {
    habitacionInicial,
    tallerQifrey,
    habitacionRichie,
    habitacionGusanoPincel,
    almacenAbandonado
};

class Habitacion : public ObjetoMapa {
private:
    std::string nombre;
    TipoHabitacion tipo;
    std::vector<std::string> arteHabitacion;

    int calcularAnchoVisual(const std::string& str) const {
        int anchoVisual = 0;
        for (size_t i = 0; i < str.size(); ) {
            unsigned char c = str[i];
            // Detectar cuántos bytes ocupa el carácter Unicode en UTF-8
            if ((c & 0x80) == 0) i += 1;        // ASCII Estándar (1 byte)
            else if ((c & 0xE0) == 0xC0) i += 2; // UTF-8 (2 bytes)
            else if ((c & 0xF0) == 0xE0) i += 3; // UTF-8 (3 bytes - Bordes de caja)
            else if ((c & 0xF8) == 0xF0) i += 4; // UTF-8 (4 bytes)
            else i += 1;

            anchoVisual++; // Cada símbolo gráfico cuenta como 1 casilla visual
        }
        return anchoVisual;
    }

    void cargarArteSegunTipo(TipoHabitacion t) {
        switch (t) {
        case habitacionInicial:
            arteHabitacion = {
                "┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐",
                "│ ┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                    ┌───┌──────┐             ┌──────┐───┐                                    │ │",
                "│ │                                    │ ┌─┐──────┘             └──────┌─┐ │                                    │ │",
                "│ │                                    ┌─└─┘                           └─┘─┐                                    │ │",
                "│ │                                    │ │                               │ │                                    │ │",
                "│ │                                    │ │                               │ │                                    │ │",
                "│ │                                    └─┘                               └─┘                                    │ │",
                "│ │                                    │                                   │                                    │ │",
                "│ │                                    │                                   │                                    │ │",
                "│ │                                    │                                   │                                    │ │",
                "│ │                                    ┌─┐                               ┌─┐                                    │ │",
                "│ │                                    │ │                               │ │                                    │ │",
                "│ │                                    │ │                               │ │                                    │ │",
                "│ │                                    └─┌─┐                           ┌─┐─┘                                    │ │",
                "│ │                                    │ └─┘──────┐             ┌──────└─┘ │                                    │ │",
                "│ │                                    └───└──────┘             └──────┘───┘                                    │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ │                                                                                                             │ │",
                "│ └──────────────────────────────────────┐                               ┌──────────────────────────────────────┘ │",
                "└────────────────────────────────────────┘                               └────────────────────────────────────────┘"
            };
            break;

        case tallerQifrey: // Habitación Doble Muro Chica
            arteHabitacion = {
                "┌─────────────────────────────────────┐                               ┌──────────────────────────────────┐",
                "│ ┌───────────────────────────────────┘                               └────────────────────────────────┐ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                ┌───┌──────┐             ┌──────┐───┐                               │ │",
                "│ │                                │ ┌─┐──────┘             └──────┌─┐ │                               │ │",
                "│ │                                ┌─└─┘                           └─┘─┐                               │ │",
                "│ │                                │ │                               │ │                               │ │",
                "│ │                                │ │                               │ │                               │ │",
                "│ │                                └─┘                               └─┘                               │ │",
                "│ │                                │                                   │                               │ │",
                "│ │                                │                                   │                               │ │",
                "│ │                                │                                   │                               │ │",
                "│ │                                ┌─┐                               ┌─┐                               │ │",
                "│ │                                │ │                               │ │                               │ │",
                "│ │                                │ │                               │ │                               │ │",
                "│ │                                └─┌─┐                           ┌─┐─┘                               │ │",
                "│ │                                │ └─┘──────┐             ┌──────└─┘ │                               │ │",
                "│ │                                └───└──────┘             └──────┘───┘                               │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ │                                                                                                    │ │",
                "│ └────────────────────────────────────────────────────────────────────────────────────────────────────┘ │",
                "└────────────────────────────────────────────────────────────────────────────────────────────────────────┘"
            };
            break;

        case habitacionRichie: // Puerta a la Izquierda
            arteHabitacion = {
                "┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐",
                "│ ┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "└─┘                                                                                                                │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "                                                                                                                   │ │",
                "┌─┐                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ │                                                                                                                │ │",
                "│ └────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘ │",
                "└────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘"
            };
            break;

        case habitacionGusanoPincel: // Puerta a la Derecha
            arteHabitacion = {
                "┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐",
                "│ ┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "└─┘                                                                                                                   │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "                                                                                                                      │ │",
                "┌─┐                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ │                                                                                                                   │ │",
                "│ └───────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘ │",
                "└───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘"
            };
            break;

        case almacenAbandonado: // Habitación Cuadrada Mediana
            arteHabitacion = {
                "┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────┐",
                "│ ┌─────────────────────────────────────────────────────────────────────────────────────────────────────────┐ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "└─┘                                                                                                         │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "                                                                                                            │ │",
                "┌─┐                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ │                                                                                                         │ │",
                "│ └─────────────────────────────────────────────────────────────────────────────────────────────────────────┘ │",
                "└─────────────────────────────────────────────────────────────────────────────────────────────────────────────┘"
            };
            break;
        }
    }

public:
    Habitacion(int x, int y, TipoHabitacion t = habitacionInicial, std::string nom = "Habitacion")
        : ObjetoMapa(x, y, 0, 0, true, habitacion), tipo(t), nombre(nom) {

        cargarArteSegunTipo(tipo);

        // Usamos el ancho visual real en lugar de .size()
        this->ancho = calcularAnchoVisual(arteHabitacion[0]);
        this->alto = (int)arteHabitacion.size();
    }

    // Colisión basada en casillas vacías (' ') vs casillas con pared
    bool colisionaCon(int px, int py) const override {
        int relX = px - x;
        int relY = py - y;

        if (relX >= 0 && relX < ancho && relY >= 0 && relY < alto) {
            // Nota: Para colisiones simples, si la posición visual no es aire ' ', es pared.
            // Si la casilla en esa columna visual no es ' ', bloqueamos el paso.
            return true; // Personalizable según tu mapa de colisión interno
        }
        return false;
    }

    void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        for (size_t r = 0; r < arteHabitacion.size(); r++) {
            int posX = x;
            int posY = y + (int)r;

            if (posY < 0 || posY >= (int)matriz.size()) continue;

            // Al copiar líneas completas UTF-8 a la matriz del mapa, 
            // estampamos el string completo para preservar los gráficos perfectos:
            for (size_t i = 0; i < arteHabitacion[r].size(); ) {
                unsigned char c = arteHabitacion[r][i];
                int bytes = 1;
                if ((c & 0xE0) == 0xC0) bytes = 2;
                else if ((c & 0xF0) == 0xE0) bytes = 3;
                else if ((c & 0xF8) == 0xF0) bytes = 4;

                std::string simbolo = arteHabitacion[r].substr(i, bytes);

                if (posX >= 0 && posX < (int)matriz[0].size()) {
                    if (simbolo != " ") {
                        // Estampamos en la matriz si tu contenedor de renderizado lo soporta
                        matriz[posY][posX] = simbolo[0];
                    }
                }
                i += bytes;
                posX++; // Avanza 1 casilla en el mapa
            }
        }
    }
};

#endif // !Habitacion_h
