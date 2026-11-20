#ifndef Nivel2_h
#define Nivel2_h
#include "Nivel.h"
#include "MapaNivel2.h"
#include "MatricesNivel2.h"
#include "Cinematica.h"

class Nivel2 : public Nivel {
private:
    bool cuartosVisitados[10] = { false };
    bool cuartosEnemigos[9] = { false };
    int probabilidadEnemigo = 0;
public:
    Nivel2() : Nivel(2, "Laberinto del Pueblo", 100, 200) {}
    virtual ~Nivel2() {}

    void generarEnemigoEnCuarto(int idCuarto, int px, int py) {
        if (mapa == nullptr || idCuarto <= 0 || idCuarto == 5) {
            return;
        }

        // Límites (xMin, yMin, xMax, yMax) según los cuartos definidos en determinarCuarto
        int xMin = 1, xMax = mapa->getColumnas() - 2;
        int yMin = 1, yMax = mapa->getFilas() - 2;

        switch (idCuarto) {
        case 1: xMin = 10;  xMax = 50;  yMin = 5;  yMax = 16; break;
        case 2: xMin = 41;  xMax = 73;  yMin = 23; yMax = 35; break;
        case 3: xMin = 106; xMax = 137; yMin = 23; yMax = 34; break;
        case 4: xMin = 144; xMax = 163; yMin = 26; yMax = 34; break;
        case 6: xMin = 41;  xMax = 73;  yMin = 42; yMax = 55; break;
        case 7: xMin = 108; xMax = 129; yMin = 39; yMax = 53; break;
        case 8: xMin = 40;  xMax = 72;  yMin = 72; yMax = 84; break;
        case 9: xMin = 84;  xMax = 130; yMin = 68; yMax = 73; break;
        default: return;
        }

        // Tamaño del sprite de Enemigo (4 de ancho x 2 de alto)
        const int eW = 4;
        const int eH = 2;

        // Dimensiones del protagonista para evitar solapamiento
        const int pW = (protagonista != nullptr) ? protagonista->getAncho() : 5;
        const int pH = (protagonista != nullptr) ? protagonista->getAlto() : 4;

        auto posicionLibre = [&](int tx, int ty) -> bool {
            if (tx < xMin || ty < yMin || tx + eW - 1 > xMax || ty + eH - 1 > yMax) {
                return false;
            }
            if (tx < 0 || ty < 0 || tx + eW > mapa->getColumnas() || ty + eH > mapa->getFilas()) {
                return false;
            }

            // Comprueba cada celda ocupada por el enemigo en el mapa
            for (int r = 0; r < eH; r++) {
                for (int c = 0; c < eW; c++) {
                    if (!mapa->esPosicionValida(tx + c, ty + r)) {
                        return false;
                    }
                }
            }

            // Evita solaparse con el jugador (y deja un pequeño margen)
            if (protagonista != nullptr) {
                bool solapa = (tx < px + pW + 1 && tx + eW + 1 > px &&
                    ty < py + pH + 1 && ty + eH + 1 > py);
                if (solapa) {
                    return false;
                }
            }

            return true;
            };

        System::Random rng;
        int elegidoX = -1;
        int elegidoY = -1;

        // 1. Intenta posiciones aleatorias dentro del cuarto
        for (int intento = 0; intento < 60; intento++) {
            int rangoX = (xMax - eW + 1 >= xMin) ? (xMax - eW + 1 - xMin + 1) : 1;
            int rangoY = (yMax - eH + 1 >= yMin) ? (yMax - eH + 1 - yMin + 1) : 1;
            int tx = xMin + rng.Next(0, rangoX);
            int ty = yMin + rng.Next(0, rangoY);

            if (posicionLibre(tx, ty)) {
                elegidoX = tx;
                elegidoY = ty;
                break;
            }
        }

        // 2. Respaldo: busca cerca de (px, py) dentro de los límites del cuarto
        if (elegidoX == -1 || elegidoY == -1) {
            for (int dy = -4; dy <= 4 && elegidoX == -1; dy++) {
                for (int dx = -4; dx <= 4; dx++) {
                    if (posicionLibre(px + dx, py + dy)) {
                        elegidoX = px + dx;
                        elegidoY = py + dy;
                        break;
                    }
                }
            }
        }

        if (elegidoX != -1 && elegidoY != -1) {
            enemigos.push_back(new Enemigo(elegidoX, elegidoY, "Sanguijuela Magica", 2, 1, 4, idCuarto));
            mostrarMensajeTemporal("[PELIGRO] Un enemigo aparecio en el cuarto!", 75);
        }
    }

    const std::vector<Enemigo*>& getEnemigos() const override {
        return this->enemigos;
    }

    bool encuentroEnmigosPorCuarto(int idCuarto) {
        if (idCuarto == 5 - 1) { //id del cuarto banio
            return false;
        }

        //al entrar a un cuarto aumenta la probabilidad de que aparezca el enemigo, solo una vez
        if (!cuartosEnemigos[idCuarto - 1]) {
            cuartosEnemigos[idCuarto - 1] = true;
            probabilidadEnemigo++;
        }

        //probabilidad proporcional a los cuartos visitados sobre 8 (en porcentaje 0..100)
        int probabilidad = (probabilidadEnemigo * 100) / 8;
        System::Random r;
        int tirada = r.Next(1, 100);
        if (tirada <= probabilidad) return true;

        return false;
    }

    virtual int determinarCuarto(int px, int py) const override {
        if (px >= 10 && px <= 50 && py >= 5 && py <= 16) return 1;
        if (px >= 41 && px <= 73 && py >= 23 && py <= 35) return 2;
        if (px >= 106 && px <= 137 && py >= 23 && py <= 34) return 3;
        if (px >= 144 && px <= 163 && py >= 26 && py <= 34) return 4;
        if (px >= 12 && px <= 30 && py >= 33 && py <= 41) return 5;
        if (px >= 41 && px <= 73 && py >= 42 && py <= 55) return 6;
        if (px >= 108 && px <= 129 && py >= 39 && py <= 53) return 7;
        if (px >= 40 && px <= 72 && py >= 72 && py <= 84) return 8;
        if (px >= 84 && px <= 130 && py >= 68 && py <= 73) return 9;
        return 0;
    }

    virtual std::string getNombreUbicacionActual() const override {
        switch (idCuartoActual) {
        case 1: return "Hogar Abandonado";
        case 2: return "Casa del Panadero";
        case 3: return "Casa Sospechosa";
        case 4: return "Refugio de Mago";
        case 5: return "Banos del Pueblo";
        case 6: return "Cuartel Moralis";
        case 7: return "Choza del Pueblo";
        case 8: return "Taberna de Kaln";
        case 9: return "Botica del Pueblo";
        default: return "Pueblo de Kaln";
        }
    }

    bool estaEnZonaSalida(int px, int py) const {
        return ((px >= 176 && px <= 200 && py >= 9 && py <= 17) || (px >= 180 && px <= 200 && py >= 10 && py <= 17));
    }

    void activarFinNivel() {
        if (!completado && !mostrarEstadisticasFin) {
            completado = true;
            mostrarEstadisticasFin = true;
            if (tiempoFin == 0) {
                tiempoFin = clock();
            }
            if (gestorMisiones != nullptr) {
                gestorMisiones->setPuntosMisiones(gestorMisiones->getPuntosMisiones() + 100);
                gestorMisiones->setObjetivoActual("Pueblo de Kaln superado: Rumbo al Arbol de Plata");
            }
        }
    }

    virtual void actualizarCuartoActual(int px, int py) override {
        if (estaEnZonaSalida(px, py)) {
            activarFinNivel();
            return;
        }

        int nuevoCuarto = determinarCuarto(px, py);
        if (nuevoCuarto != idCuartoActual) {
            idCuartoActual = nuevoCuarto;
            switch (nuevoCuarto) {
            case 1:mostrarMensajeTemporal("[HOGAR ABANDONADO]", 60); break;
            case 2:mostrarMensajeTemporal("[CASA DEL PANADERO]", 60); break;
            case 3:mostrarMensajeTemporal("[CASA DE LA VIEJITA NADA SOSPECHOSA]", 60); break;
            case 4:mostrarMensajeTemporal("[CASA ALQUILADA USADA POR MAGO]", 60); break;
            case 5:mostrarMensajeTemporal("[BANIO DEL PUEBLO]", 60); break;
            case 6:mostrarMensajeTemporal("[CUARTEL MORALIS]", 60); break;
            case 7:mostrarMensajeTemporal("[CHOZA DEL PUEBLO]", 60); break;
            case 8:mostrarMensajeTemporal("[TABERNA DE KALN]", 60); break;
            case 9:mostrarMensajeTemporal("[BOTICA]", 60); break;
            default:
                mensajeTemporal = "";
                ticksMensajeTemporal = 0;
                break;
            }

            if (nuevoCuarto != 0) {
                if(nuevoCuarto !=5) verificarCinematicaLugar(px, py);
                if (encuentroEnmigosPorCuarto(nuevoCuarto)) {
                    generarEnemigoEnCuarto(nuevoCuarto, px, py);
                };
            }
        }
    }

    virtual bool verificarProximidadEspecial(int px, int py, int pw, int ph) override {
        (void)pw;
        (void)ph;
        if (estaEnZonaSalida(px, py)) {
            activarFinNivel();
            return true;
        }
        if (!completado && !mostrarEstadisticasFin) {
            if (px >= 170 && px <= 200 && py >= 8 && py <= 18) {
                promptFlotante = "[SALIDA] Avanzar hacia el Gran Arbol de Plata";
                return true;
            }
        }
        return false;
    }

    virtual bool procesarInteraccionEspecial(int px, int py, int pw, int ph) override {
        (void)pw;
        (void)ph;
        if (px >= 170 && px <= 200 && py >= 8 && py <= 18) {
            activarFinNivel();
            return true;
        }
        return false;
    }

    static bool esCuartoEstatico(int px, int py) {
        if (px >= 10 && px <= 50 && py >= 5 && py <= 16) return true;
        if (px >= 41 && px <= 73 && py >= 23 && py <= 35) return true;
        if (px >= 106 && px <= 137 && py >= 23 && py <= 34) return true;
        if (px >= 144 && px <= 163 && py >= 26 && py <= 34) return true;
        if (px >= 12 && px <= 30 && py >= 33 && py <= 41) return true;
        if (px >= 41 && px <= 73 && py >= 42 && py <= 55) return true;
        if (px >= 108 && px <= 129 && py >= 39 && py <= 53) return true;
        if (px >= 40 && px <= 72 && py >= 72 && py <= 84) return true;
        if (px >= 84 && px <= 130 && py >= 68 && py <= 73) return true;
        return false;
    }

    void mostrarCinematicaIntro(Pantalla& pantalla) override {
        Cinematica* introNivel2 = new Cinematica();
        introNivel2->agregarCuadro(Nivel2_Inicio01, "Tartah: Es Ininia", 3000, 10, 2, 12, 5);
        introNivel2->agregarCuadro(Nivel2_Inicio02, "Coco: Es coustas", 3000, 10, 2, 12, 5);
        introNivel2->agregarCuadro(Nivel2_Inicio03, "Coco: Oh no, se van! Tenemos que alcanzarlos rapido.", 3000, 10, 2, 12, 5);
        introNivel2->agregarCuadro(Nivel2_Inicio04, "Tartah: Espera Coco, no corras tan rapido! Voy al banio.", 3000, 10, 2, 12, 5);
        introNivel2->reproducir();
        delete introNivel2;
        pantalla.mostrarHistoriaNivel2();
    }

    void reproducirCinematicaLugar(int idCuarto) {
        if (idCuarto <= 0 || idCuarto >= 10) return;
        if (cuartosVisitados[idCuarto]) return;

        Cinematica* cine = new Cinematica();

        switch (idCuarto) {
        case 1:
            break;
        case 2:
            cine->agregarCuadro(Nivel2_Panaderia01, "Un mago de sombrero puntiagudo!", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Panaderia02, "El gato fue atrapado por los escombros!", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Panaderia03, "Magia prohibida!", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Panaderia04, "Esta ayudandolo?", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Panaderia05, "La magia se puede utilizar para curar?", 2500, 10, 2, 12, 5);
            break;
        case 3:
            cine->agregarCuadro(Nivel2_Inicio01, "CASA DE LA VIEJITA NADA SOSPECHOSA", 2500, 10, 2, 10, 7);
            break;
        case 4:
            cine->agregarCuadro(Nivel2_Inicio01, "CASA ALQUILADA USADA POR MAGO", 2500, 10, 2, 10, 7);
            break;
        case 6:
            cine->agregarCuadro(Nivel2_Moralis01, "Hagan aviso", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Moralis02, "El pueblo esta siendo invadido por una sanguijuela magica", 2500, 10, 2, 12, 5);
            break;
        case 7:
            cine->agregarCuadro(Nivel2_ChozaPueblo01, "Veras Tartah", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_ChozaPueblo02, "La razon por la que Coustas puede caminar", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_ChozaPueblo03, "Es por la semilla del arbol de plata", 2500, 10, 2, 12, 5);
            break;
        case 8:
            cine->agregarCuadro(Nivel2_Taberna01, "Tipo1: Si como te decia...", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Taberna02, "Tipo2: Corran!", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Taberna03, "Tipo2: No sé que sea esa cosa pero nos va a matar", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Taberna04, "Tipo1: ah", 2500, 10, 2, 12, 5);
            break;
        case 9:
            cine->agregarCuadro(Nivel2_Botica01, "Tipo1: Ayuda! Mi amigo esta herido", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Botica02, "Medico1: Hay mucho heridos", 2500, 10, 2, 12, 5);
            cine->agregarCuadro(Nivel2_Botica03, "Medico2: No hay suficientes medicos y siguen llegando más personas", 2500, 10, 2, 12, 5);
            break;
        default:
            delete cine;
            return;
        }

        cine->reproducir();
        delete cine;

        cuartosVisitados[idCuarto] = true;
    }

    void verificarCinematicaLugar(int px, int py) {
        int cuarto = determinarCuarto(px, py);
        if (cuarto > 0 && cuarto < 10) {
            if (!cuartosVisitados[cuarto]) {
                reproducirCinematicaLugar(cuarto);
            }
        }
    }

    virtual void iniciarNivel() override {
        this->tiempoInicio = clock();
        this->tiempoFin = 0;
        this->completado = false;
        this->modalActivo = 0;
        this->seleccionModal = 0;
        this->mostrarEstadisticasFin = false;
        this->idCuartoActual = 0;
        this->mensajeTemporal = "";
        this->ticksMensajeTemporal = 0;

        for (int i = 0; i < 10; i++) {
            cuartosVisitados[i] = false;
        }

        int cuartoInicial = determinarCuarto(16, 36);
        if (cuartoInicial > 0 && cuartoInicial < 10) {
            cuartosVisitados[cuartoInicial] = true;
        }

        if (gestorDialogos != nullptr) gestorDialogos->terminarDialogo();
        if (gestorMisiones != nullptr) {
            gestorMisiones->setPuntosMisiones(0);
            gestorMisiones->setEnModalMisiones(false);
            gestorMisiones->setEnDetalleMision(false);
            gestorMisiones->setSeleccionMision(0);
            gestorMisiones->setObjetivoActual("Atravesar el Laberinto del Pueblo");
        }

        if (mapa != nullptr) {
            delete mapa;
            mapa = nullptr;
        }
        mapa = new Mapa(100, 200);

        std::vector<std::string> matrizCargada;
        MapaNivel2::cargarMatriz(matrizCargada);
        mapa->cargarMatriz(matrizCargada);

        if (protagonista == nullptr) {
            protagonista = new Protagonista(16, 36, "Tartah", 3);
        }
        else {
            protagonista->setX(16);
            protagonista->setY(36);
            protagonista->setVida(3);
            protagonista->setVidaMaxima(3);
        }

        limpiarLetreros();
        letreros.push_back(new Letrero(18, 35, "Letrero: [BANIO DEL PUEBLO] Tartah busca a Coustas."));
        letreros.push_back(new Letrero(37, 49, "Letrero: Parece que desde la entrada se puede escuchar."));
        letreros.push_back(new Letrero(55, 87, "Letrero: Esta gente de aca tiene mucho que decir. "));
        letreros.push_back(new Letrero(66, 64, "Letrero: Aqui nada. Sigue avanzando. "));
        letreros.push_back(new Letrero(176, 25, "Letrero: Nada. Sigue avanzando. "));
        letreros.push_back(new Letrero(150, 36, "Letrero: Algunas personas hablan muy alto. "));
        letreros.push_back(new Letrero(129, 47, "Letrero: Aquellos necesitan un mejor hogar. "));
        letreros.push_back(new Letrero(91, 76, "Letrero: Vaya! Una gran falta de recursos. "));
        letreros.push_back(new Letrero(102, 29, "Letrero: Parece que aca estuvo Ininia. "));
        letreros.push_back(new Letrero(75, 29, "Letrero: No todos pueden comer pan. "));
        letreros.push_back(new Letrero(27, 11, "Letrero: Un dia todo se va. Y no queda nada. "));

        limpiarItemsSuelo();

        dibujarLetrerosEnMapa();

        if (protagonista != nullptr) {
            actualizarCuartoActual(protagonista->getX(), protagonista->getY());
        }
    }
};

#endif