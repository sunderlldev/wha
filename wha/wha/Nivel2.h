#ifndef Nivel2_h
#define Nivel2_h
#include "Nivel.h"
#include "MapaNivel2.h"
#include "MatricesNivel2.h"
#include "Cinematica.h"

class Nivel2 : public Nivel {
public:
	Nivel2() : Nivel(2, "Laberinto del Pueblo", 100, 200) {}
	virtual ~Nivel2() {}

    virtual int determinarCuarto(int px, int py) const override {
        if (px >= 10 && px <= 50 && py >= 5 && py <= 16) return 1;
        if (px >= 41 && px <= 73 && py >= 23 && py <= 35) return 2;
        if (px >= 106 && px <= 137 && py >= 23 && py <= 34) return 3;
        if (px >= 144 && px <= 163 && py >= 26 && py <= 34) return 4;
        if (px >= 12 && px <= 30 && py >= 33 && py <= 41) return 5; // Zona de Inicio
        if (px >= 41 && px <= 73 && py >= 42 && py <= 55) return 6;
        if (px >= 108 && px <= 129 && py >= 39 && py <= 53) return 7;
        if (px >= 40 && px <= 72 && py >= 72 && py <= 84) return 8;
        if (px >= 84 && px <= 130 && py >= 68 && py <= 73) return 9;
        return 0;
    }

	virtual void actualizarCuartoActual(int px, int py) override {
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
			case 8:mostrarMensajeTemporal("[BAR]", 60); break;
			case 9:mostrarMensajeTemporal("[BOTICA]", 60); break;
			default:
				mensajeTemporal = "";
				ticksMensajeTemporal = 0;
				break;
			}
		}
	}

	void mostrarCinematicaIntro(Pantalla& pantalla) override {
		Cinematica* introNivel2 = new Cinematica();
		introNivel2->agregarCuadro(Nivel2_Inicio01, "Es Ininia", 3000);
		introNivel2->reproducir();
		delete introNivel2;
	}

	virtual void inciarNivel() override {
		//un moton de variables necesarias para el nivel
		this->tiempoInicio = clock();
		this->tiempoFin = 0;
		this->completado = false;
		this->modalActivo = 0;
		this->seleccionModal = 0;
		this->mostrarEstadisticasFin = false;
		this->idCuartoActual = 0;
		this->mensajeTemporal = "";
		this->ticksMensajeTemporal = 0;

		if (gestorDialogos != nullptr) gestorDialogos->terminarDialogo();
		if (gestorMisiones != nullptr) {
			gestorMisiones->setPuntosMisiones(0);
			gestorMisiones->setEnModalMisiones(false);
			gestorMisiones->setEnDetalleMision(false);
			gestorMisiones->setSeleccionMision(0);
			gestorMisiones->setObjetivoActual("Atravesar el Laberinto del Pueblo");
		}

		//Cargar la dishosa matriz
		if (mapa != nullptr) {
			delete mapa;
			mapa = nullptr;
		}
		mapa = new Mapa(100, 200);

		std::vector<std::string> matrizCargada;
		MapaNivel2::cargarMatriz(matrizCargada); //aqui estoy poniendo el MapaNivel2 en vez del 1 (cambio po :v)
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

		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr && mapa != nullptr) {
				int lx = letreros[i]->getX();
				int ly = letreros[i]->getY();
				mapa->setCaracter(lx, ly, '[');
				mapa->setCaracter(lx + 1, ly, '!');
				mapa->setCaracter(lx + 2, ly, ']');
			}
		}

		// 3. Verificación de seguridad antes de invocar métodos de 'protagonista'
		if (protagonista != nullptr) {
			actualizarCuartoActual(protagonista->getX(), protagonista->getY());
		}
	}
};

#endif
