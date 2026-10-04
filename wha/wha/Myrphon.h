#ifndef Myrphon_h
#define Myrphon_h
#include "ObjetoMapa.h"
#include <string>
#include <vector>

class Myrphon : public ObjetoMapa {
private:
    std::vector<std::string> arteAscii;
    bool rescatado;

public:
    Myrphon(int x, int y)
        : ObjetoMapa(x, y, 7, 2, false, myrphon), rescatado(false) {
        arteAscii = {
            " (\\)   ",
            "<(o)==<"
        };
    }

    virtual ~Myrphon() {}

    bool getRescatado() const { return rescatado; }
    void setRescatado(bool r) { rescatado = r; }

    virtual void dibujarEnMatriz(std::vector<std::string>& matriz) override {
        if (rescatado) return;
        for (int r = 0; r < (int)arteAscii.size(); r++) {
            for (int c = 0; c < (int)arteAscii[r].size(); c++) {
                int destinoY = y + r;
                int destinoX = x + c;
                if (destinoY >= 0 && destinoY < (int)matriz.size() &&
                    destinoX >= 0 && destinoX < (int)matriz[0].size()) {
                    char ch = arteAscii[r][c];
                    if (ch != ' ') {
                        matriz[destinoY][destinoX] = ch;
                    }
                }
            }
        }
    }
};

#endif
