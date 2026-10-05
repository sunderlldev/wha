#ifndef Nivel3_h
#define Nivel3_h
#include "Nivel.h"
#include "MapaNivel3.h"

class Nivel3 : public Nivel {
private:
	int enemigoX[4];
	int enemigoY[4];
	int enemigoDx[4];
	int enemigoDy[4];
	int enemigoMin[4];
	int enemigoMax[4];
	clock_t tiempoUltimoPasoEnemigo;
	int cooldownDanio;

public:
	Nivel3() : Nivel(3, "La Emboscada", 32, 100), tiempoUltimoPasoEnemigo(0), cooldownDanio(0) {
		enemigoX[0] = 20; enemigoY[0] = 11; enemigoDx[0] = 0; enemigoDy[0] = 1; enemigoMin[0] = 10; enemigoMax[0] = 22;
		enemigoX[1] = 38; enemigoY[1] = 12; enemigoDx[1] = 1; enemigoDy[1] = 0; enemigoMin[1] = 38; enemigoMax[1] = 64;
		enemigoX[2] = 62; enemigoY[2] = 22; enemigoDx[2] = -1; enemigoDy[2] = 0; enemigoMin[2] = 38; enemigoMax[2] = 64;
		enemigoX[3] = 72; enemigoY[3] = 20; enemigoDx[3] = 0; enemigoDy[3] = -1; enemigoMin[3] = 12; enemigoMax[3] = 21;
	}

	virtual ~Nivel3() {}

	static bool esCuartoEstatico(int, int) {
		return false;
	}

	static bool esCaminoEstatico(int px, int py) {
		if (px >= 2 && px <= 98 && py >= 5 && py <= 26) return true;
		return false;
	}

	virtual std::string getNombreUbicacionActual() const override {
		if (protagonista != nullptr && protagonista->getX() >= 80) {
			return "Santuario del Arbol de Plata";
		}
		return "Desfiladero de las Ruinas";
	}

	void mostrarCinematicaIntro(Pantalla& pantalla) override {
		pantalla.mostrarHistoriaNivel3();
	}

	virtual void dibujarEntidadesExtra(Pantalla& pantalla, int camX, int camY) const override {
		const char* f0 = " /\\ ";
		const char* f1 = "(xx)";
		for (int i = 0; i < 4; i++) {
			int ex = enemigoX[i] - camX;
			int ey = enemigoY[i] - camY;
			for (int c = 0; c < 4; c++) {
				if (ex + c >= 0 && ex + c < pantalla.getAnchoJuego() && ey >= 0 && ey < pantalla.getAltoTotal()) {
					pantalla.setPixelJuego(ex + c, ey, f0[c], 4);
				}
				if (ex + c >= 0 && ex + c < pantalla.getAnchoJuego() && ey + 1 >= 0 && ey + 1 < pantalla.getAltoTotal()) {
					pantalla.setPixelJuego(ex + c, ey + 1, f1[c], 4);
				}
			}
		}
	}

	virtual bool verificarProximidadEspecial(int px, int py, int pw, int ph) override {
		if (qifrey != nullptr && estaCerca(px, py, pw, ph, qifrey->getX(), qifrey->getY(), qifrey->getAncho(), qifrey->getAlto(), 3)) {
			promptFlotante = "[E / ENTER] Auxiliar a Dagda";
			return true;
		}
		if (agott != nullptr && estaCerca(px, py, pw, ph, agott->getX(), agott->getY(), agott->getAncho(), agott->getAlto(), 3)) {
			promptFlotante = "[E / ENTER] Hablar con Ininia";
			return true;
		}
		return false;
	}

	virtual bool procesarInteraccionEspecial(int px, int py, int pw, int ph) override {
		if (qifrey != nullptr && estaCerca(px, py, pw, ph, qifrey->getX(), qifrey->getY(), qifrey->getAncho(), qifrey->getAlto(), 3)) {
			if (gestorDialogos != nullptr) {
				gestorDialogos->iniciarDialogo("Dagda", 500);
			}
			return true;
		}
		if (agott != nullptr && estaCerca(px, py, pw, ph, agott->getX(), agott->getY(), agott->getAncho(), agott->getAlto(), 3)) {
			if (gestorDialogos != nullptr) {
				gestorDialogos->iniciarDialogo("Ininia", 600);
			}
			return true;
		}
		return false;
	}

	bool actualizarPatrullas() {
		bool huboMovimiento = false;
		clock_t ahora = clock();
		if ((double)(ahora - tiempoUltimoPasoEnemigo) / CLOCKS_PER_SEC >= 0.20) {
			tiempoUltimoPasoEnemigo = ahora;
			huboMovimiento = true;
			for (int i = 0; i < 4; i++) {
				if (enemigoDx[i] != 0) {
					int nx = enemigoX[i] + enemigoDx[i];
					if (nx < enemigoMin[i] || nx > enemigoMax[i]) {
						enemigoDx[i] = -enemigoDx[i];
					} else {
						enemigoX[i] = nx;
					}
				} else if (enemigoDy[i] != 0) {
					int ny = enemigoY[i] + enemigoDy[i];
					if (ny < enemigoMin[i] || ny > enemigoMax[i]) {
						enemigoDy[i] = -enemigoDy[i];
					} else {
						enemigoY[i] = ny;
					}
				}
			}
		}

		if (cooldownDanio > 0) {
			cooldownDanio--;
		}

		if (protagonista != nullptr && cooldownDanio == 0) {
			int px = protagonista->getX();
			int py = protagonista->getY();
			int pw = protagonista->getAncho();
			int ph = protagonista->getAlto();

			for (int i = 0; i < 4; i++) {
				if (px < enemigoX[i] + 4 && px + pw > enemigoX[i] &&
				    py < enemigoY[i] + 2 && py + ph > enemigoY[i]) {
					protagonista->setVida(protagonista->getVida() - 1);
					cooldownDanio = 30;
					huboMovimiento = true;
					this->transicionDanioCoustas = true;
					if (px > 6) protagonista->setX(px - 3);
					if (protagonista->getVida() <= 0) {
						iniciarNivel();
					}
					break;
				}
			}
		}
		return huboMovimiento;
	}

	virtual bool actualizar() override {
		bool movio = actualizarPatrullas();
		bool base = Nivel::actualizar();
		return movio || base;
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
		this->cooldownDanio = 0;
		this->tiempoUltimoPasoEnemigo = clock();

		enemigoX[0] = 20; enemigoY[0] = 11; enemigoDx[0] = 0; enemigoDy[0] = 1; enemigoMin[0] = 10; enemigoMax[0] = 22;
		enemigoX[1] = 38; enemigoY[1] = 12; enemigoDx[1] = 1; enemigoDy[1] = 0; enemigoMin[1] = 38; enemigoMax[1] = 64;
		enemigoX[2] = 62; enemigoY[2] = 22; enemigoDx[2] = -1; enemigoDy[2] = 0; enemigoMin[2] = 38; enemigoMax[2] = 64;
		enemigoX[3] = 72; enemigoY[3] = 20; enemigoDx[3] = 0; enemigoDy[3] = -1; enemigoMin[3] = 12; enemigoMax[3] = 21;

		if (gestorDialogos != nullptr) {
			gestorDialogos->terminarDialogo();
		}
		if (gestorMisiones != nullptr) {
			gestorMisiones->setPuntosMisiones(50);
			gestorMisiones->setEnModalMisiones(false);
			gestorMisiones->setEnDetalleMision(false);
			gestorMisiones->setSeleccionMision(0);
			gestorMisiones->setObjetivoActual("Esquivar bandidos y auxiliar a Dagda");
		}

		if (mapa != nullptr) {
			delete mapa;
			mapa = nullptr;
		}
		mapa = new Mapa(32, 100);
		std::vector<std::string> matrizCargada;
		MapaNivel3::cargarMatriz(matrizCargada);
		mapa->cargarMatriz(matrizCargada);

		if (protagonista != nullptr) {
			delete protagonista;
			protagonista = nullptr;
		}
		protagonista = new Protagonista(4, 15, "Coustas", 3);

		if (qifrey != nullptr) {
			delete qifrey;
			qifrey = nullptr;
		}
		qifrey = new NPC(76, 14, "Dagda", "Protector de Coustas");
		qifrey->setConfianza(2);

		if (agott != nullptr) {
			delete agott;
			agott = nullptr;
		}
		agott = new NPC(89, 18, "Ininia", "Sombrero de Ala Ancha");
		agott->setConfianza(0);

		limpiarLetreros();
		letreros.push_back(new Letrero(6, 11, "Letrero: [RUINAS DEL DESFILADERO] Paso asediado por bandidos. Avanza con cautela."));
		letreros.push_back(new Letrero(46, 17, "Letrero: [CARAVANA VOLCADA] Restos del combate. Dagda resiste mas adelante."));
		letreros.push_back(new Letrero(80, 11, "Letrero: [SANTUARIO ANCESTRAL] El Gran Arbol de Plata irradia un brillo prohibido."));
		dibujarLetrerosEnMapa();

		limpiarItemsSuelo();
	}
};

#endif
