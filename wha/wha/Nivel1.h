#ifndef Nivel1_h
#define Nivel1_h
#include "Nivel.h"
#include "MapaNivel1.h"
#include "Arbol.h"
#include "Gato.h"
#include "Myrphon.h"

class Nivel1 : public Nivel {
private:
	Myrphon* myrphon;
	bool monologoCruceActivado;
	bool paredPiedraDestruida;

public:
	Nivel1() : Nivel(1, "Atelier de Qifrey", 197, 524), myrphon(nullptr), monologoCruceActivado(false), paredPiedraDestruida(false) {}
	virtual ~Nivel1() {
		if (myrphon != nullptr) {
			delete myrphon;
			myrphon = nullptr;
		}
		if (cuartoRicheh != nullptr) {
			delete cuartoRicheh;
			cuartoRicheh = nullptr;
		}
		if (torreAgott != nullptr) {
			delete torreAgott;
			torreAgott = nullptr;
		}
	}

	static bool esCuartoEstatico(int px, int py) {
		if (px >= 59 && px <= 125 && py >= 19 && py <= 45) return true;
		if (px >= 382 && px <= 457 && py >= 11 && py <= 49) return true;
		if (px >= 384 && px <= 501 && py >= 62 && py <= 131) return true;
		if (px >= 382 && px <= 456 && py >= 150 && py <= 178) return true;
		if (px >= 62 && px <= 122 && py >= 148 && py <= 175) return true;
		return false;
	}

	static bool esCaminoEstatico(int px, int py) {
		if (px >= 77 && px <= 107 && py >= 45 && py <= 147) return true;
		if (px >= 108 && px <= 302 && py >= 95 && py <= 110) return true;
		if (px >= 303 && px <= 319 && py >= 27 && py <= 168) return true;
		if (px >= 320 && px <= 381 && py >= 27 && py <= 36) return true;
		if (px >= 320 && px <= 383 && py >= 99 && py <= 107) return true;
		if (px >= 320 && px <= 381 && py >= 160 && py <= 168) return true;
		return false;
	}

	virtual int determinarCuarto(int px, int py) const override {
		if (px >= 59 && px <= 125 && py >= 19 && py <= 45) return 1;
		if (px >= 382 && px <= 457 && py >= 11 && py <= 49) return 2;
		if (px >= 384 && px <= 501 && py >= 62 && py <= 131) return 3;
		if (px >= 382 && px <= 456 && py >= 150 && py <= 178) return 4;
		if (px >= 62 && px <= 122 && py >= 148 && py <= 175) return 5;
		return 0;
	}

	virtual std::string getNombreUbicacionActual() const override {
		if (getEnCuartoRicheh()) return "Sotano de Richeh";
		if (idCuartoActual == 1) return "Choza de Hechizos";
		if (idCuartoActual == 2) return "Almacen Abandonado";
		if (idCuartoActual == 3) return "Torre de Agott";
		if (idCuartoActual == 4) return "Laberinto Serpentback";
		if (idCuartoActual == 5) return "Despacho de Qifrey";
		return "Atelier de Qifrey";
	}

	virtual void actualizarCuartoActual(int px, int py) override {
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (nuevoCuarto == 1) {
				mostrarMensajeTemporal("La Choza de Hechizos", 60);
			} else if (nuevoCuarto == 2) {
				mostrarMensajeTemporal("El Almacen Abandonado", 60);
			} else if (nuevoCuarto == 3) {
				mostrarMensajeTemporal("Torre de Agott", 60);
				if (agott != nullptr && !agott->getYaHablo()) {
					agott->setYaHablo(true);
					if (gestorDialogos != nullptr) {
						gestorDialogos->iniciarDialogo("Agott", 200);
					}
				}
			} else if (nuevoCuarto == 4) {
				mostrarMensajeTemporal("Laberinto Serpentback", 60);
			} else if (nuevoCuarto == 5) {
				mostrarMensajeTemporal("Despacho de Qifrey", 60);
			} else {
				mensajeTemporal = "";
				ticksMensajeTemporal = 0;
			}
		}

		if (!monologoCruceActivado && px >= 295 && px <= 315 && py >= 96 && py <= 108) {
			monologoCruceActivado = true;
			promptFlotante = "Coco: ¡Vaya...! Tres caminos se abren ante mí... ¿Hacia dónde debería dirigirme ahora?";
			mostrarMensajeTemporal(promptFlotante, 120);
		}
	}

	virtual bool verificarProximidadEspecial(int px, int py, int pw, int ph) override {
		if (!paredPiedraDestruida && px >= 365 && px <= 385 && py >= 25 && py <= 38) {
			Inventario* inv = protagonista->getInventario();
			bool tienePluma = (inv != nullptr && inv->tieneItem("Pluma Termica"));
			if (tienePluma) {
				promptFlotante = "ENTER: Usar Pluma Termica para quebrar la pared de roca";
			} else {
				promptFlotante = "Coco: La pared de roca esta agrietada... Necesito una pluma termica para quebrarla.";
			}
			return true;
		}
		if (myrphon != nullptr && !myrphon->getRescatado()) {
			int mx = myrphon->getX();
			int my = myrphon->getY();
			int mw = myrphon->getAncho();
			int mh = myrphon->getAlto();
			int distX = (px + pw <= mx) ? (mx - (px + pw)) : ((mx + mw <= px) ? (px - (mx + mw)) : 0);
			int distY = (py + ph <= my) ? (my - (py + ph)) : ((my + mh <= py) ? (py - (my + mh)) : 0);
				if (distX <= 3 && distY <= 3) {
					if (gestorDialogos != nullptr && gestorDialogos->getMisionMyrphonActiva()) {
						promptFlotante = "ENTER: Rescatar a Myrphon";
					} else {
						promptFlotante = "Coco: Un pequeño pingüino con rasgos de grifo... Parece perdido.";
					}
					return true;
				}
		}
		return false;
	}

	virtual bool procesarInteraccionEspecial(int px, int py, int pw, int ph) override {
		if (!paredPiedraDestruida && px >= 365 && px <= 385 && py >= 25 && py <= 38) {
			Inventario* inv = protagonista->getInventario();
			if (inv != nullptr && inv->tieneItem("Pluma Termica")) {
				paredPiedraDestruida = true;
				if (mapa != nullptr) {
					for (int wy = 27; wy <= 36; wy++) {
						for (int wx = 380; wx <= 386; wx++) {
							mapa->setCaracter(wx, wy, ' ');
						}
					}
				}
				inv->removerItem("Pluma Termica");
				promptFlotante = "Punta incandescente aplicada: La pared de roca se quebro.";
				return true;
			}
		}
		if (myrphon != nullptr && !myrphon->getRescatado()) {
			int mx = myrphon->getX();
			int my = myrphon->getY();
			int mw = myrphon->getAncho();
			int mh = myrphon->getAlto();
			int distX = (px + pw <= mx) ? (mx - (px + pw)) : ((mx + mw <= px) ? (px - (mx + mw)) : 0);
			int distY = (py + ph <= my) ? (my - (py + ph)) : ((my + mh <= py) ? (py - (my + mh)) : 0);
			if (distX <= 3 && distY <= 3) {
				myrphon->setRescatado(true);
				if (mapa != nullptr) {
					for (int r = 0; r < mh; r++) {
						for (int c = 0; c < mw; c++) {
							mapa->setCaracter(mx + c, my + r, ' ');
						}
					}
				}
				if (gestorDialogos != nullptr) {
					gestorDialogos->setMyrphonRescatado(true);
				}
				if (gestorMisiones != nullptr) {
					gestorMisiones->setObjetivoActual("Llevar a Myrphon de regreso con Richeh");
				}
				promptFlotante = "¡Rescataste a Myrphon! Llévaselo a Richeh.";
				mostrarMensajeTemporal("¡Myrphon rescatado! Vuelve al sótano de Richeh", 100);
				return true;
			}
		}
		return false;
	}

	virtual void iniciarNivel() override {
		this->tiempoInicio = clock();
		this->tiempoFin = 0;
		this->completado = false;
		if (gestorDialogos != nullptr) {
			gestorDialogos->terminarDialogo();
			gestorDialogos->setMyrphonRescatado(false);
			gestorDialogos->setMisionMyrphonActiva(false);
			gestorDialogos->setRichehEnojada(false);
			gestorDialogos->setDioVaraRicheh(false);
		}
		if (gestorMisiones != nullptr) {
			gestorMisiones->setPuntosMisiones(0);
			gestorMisiones->setEnModalMisiones(false);
			gestorMisiones->setEnDetalleMision(false);
			gestorMisiones->setSeleccionMision(0);
		}
		this->modalActivo = 0;
		this->seleccionModal = 0;
		this->mostrarEstadisticasFin = false;
		this->idCuartoActual = 0;
		this->mensajeTemporal = "";
		this->ticksMensajeTemporal = 0;
		this->paredPiedraDestruida = false;

		if (cuartoRicheh == nullptr) {
			cuartoRicheh = new CuartoRicheh();
		} else {
			cuartoRicheh->reiniciar();
		}
		if (torreAgott == nullptr) {
			torreAgott = new TorreAgott();
		} else {
			torreAgott->reiniciar();
		}

		if (mapa != nullptr) {
			delete mapa;
		}
		mapa = new Mapa(197, 524);

		std::vector<std::string> matrizCargada;
		MapaNivel1::cargarMatriz(matrizCargada);
		mapa->cargarMatriz(matrizCargada);

		int pinosCoords[][2] = {
			{328, 5}, {506, 7}, {225, 8}, {358, 9}, {19, 16}, {303, 17}, {217, 32}, {330, 42},
			{230, 49}, {22, 51}, {150, 59}, {363, 67}, {229, 68}, {25, 80}, {55, 85}, {121, 85},
			{246, 86}, {342, 87}, {33, 108}, {327, 113}, {60, 114}, {361, 115}, {193, 116},
			{267, 118}, {169, 129}, {368, 132}, {126, 133}, {58, 136}, {333, 139}, {231, 144},
			{152, 146}, {360, 151}, {14, 159}, {226, 159}, {156, 165}, {240, 166}, {151, 174},
			{254, 175}, {360, 181}, {270, 182}, {159, 186}, {280, 189}, {18, 190}, {411, 190},
			{496, 191}
		};
		for (int i = 0; i < 45; i++) {
			mapa->agregarObjeto(new Arbol(pinosCoords[i][0], pinosCoords[i][1], arbolPino));
		}

		int frondososCoords[][2] = {
			{443, 3}, {303, 7}, {230, 23}, {267, 34}, {353, 40}, {505, 47}, {414, 54}, {45, 56},
			{243, 57}, {121, 62}, {5, 67}, {127, 76}, {236, 76}, {264, 84}, {506, 85}, {336, 119},
			{11, 120}, {448, 132}, {13, 143}, {277, 165}, {317, 173}, {287, 179}, {464, 187},
			{373, 188}, {435, 189}
		};
		for (int i = 0; i < 25; i++) {
			mapa->agregarObjeto(new Arbol(frondososCoords[i][0], frondososCoords[i][1], arbolFrondoso));
		}

		int gigantesCoords[][2] = {
			{251, 13}, {333, 50}, {31, 64}, {144, 73}, {25, 125}, {331, 180}
		};
		for (int i = 0; i < 6; i++) {
			mapa->agregarObjeto(new Arbol(gigantesCoords[i][0], gigantesCoords[i][1], arbolGigante));
		}

		mapa->agregarObjeto(new Gato(416, 181));

		if (myrphon != nullptr) {
			delete myrphon;
			myrphon = nullptr;
		}
		myrphon = new Myrphon(418, 168);
		mapa->agregarObjeto(myrphon);

		if (protagonista == nullptr) {
			protagonista = new Protagonista(90, 22, "Coco", 3);
		} else {
			protagonista->setX(90);
			protagonista->setY(22);
			protagonista->setVida(3);
			protagonista->setVidaMaxima(3);
		}

		if (qifrey == nullptr) {
			qifrey = new NPC(90, 160, "Qifrey", "Maestro Hechicero");
		} else {
			qifrey->setX(90);
			qifrey->setY(160);
			qifrey->setConfianza(0);
			qifrey->setYaHablo(false);
			qifrey->setDioTinta(false);
			qifrey->setCrafteoCapa(false);
		}

		if (agott == nullptr) {
			agott = new NPC(440, 66, "Agott", "Aprendiz de Maga");
		} else {
			agott->setX(440);
			agott->setY(66);
			agott->setConfianza(1);
			agott->setYaHablo(false);
		}

		limpiarItemsSuelo();
		itemsSuelo.push_back(new ItemMagico(405, 20, "Fibra de Arbol", "Fibras del Arbol de Plata para tejer el manto de aprendiz.", "Material Textil Arcano", false));
		itemsSuelo.push_back(new ItemMagico(440, 20, "Frasco de Tinta", "Frasco con tinta arcaica de plata preservada en el almacén antiguo.", "Objeto de Colección", false));
		itemsSuelo.push_back(new ItemMagico(160, 50, "Tinta de Viento", "Esencia de tinta de viento encontrada junto a la orilla del gran río.", "Objeto de Colección", false));

		monologoCruceActivado = false;
		limpiarLetreros();
		letreros.push_back(new Letrero(80, 24, "Letrero: Choza de Trazos. Dibuja runas con pasión y cuida tus pergaminos."));
		letreros.push_back(new Letrero(140, 102, "Letrero: Historia del Manga. Witch Hat Atelier fue creado por la mangaka Kamome Shirahama e inició su publicación el 22 de julio de 2016. Comenzó a lanzarse de forma mensual en la revista Morning Two de la editorial Kodansha, destacando por su magia basada en el arte del dibujo."));
		letreros.push_back(new Letrero(265, 102, "Letrero: Enciclopedia Mágica. Las tres sendas del Atelier: hacia el norte el almacén de vestigios antiguos, al este la Torre de Agott, y al sur el sendero hacia el despacho del maestro Qifrey."));
		letreros.push_back(new Letrero(380, 101, "Letrero: Torre de Agott. Prohibido el paso sin autorización de Agott."));
		letreros.push_back(new Letrero(355, 32, "Letrero: Almacén Abandonado. Peligro: Derrumbe. Usa magia ígnea."));
		letreros.push_back(new Letrero(75, 152, "Letrero: Despacho de Qifrey. Maestro del atelier y protector del agua."));

		dibujarLetrerosEnMapa();

		if (torreAgott != nullptr) {
			torreAgott->reiniciar();
		}

		if (!paredPiedraDestruida && mapa != nullptr) {
			for (int wy = 27; wy <= 36; wy++) {
				for (int wx = 380; wx <= 386; wx++) {
					mapa->setCaracter(wx, wy, 'O');
				}
			}
		}

		if (gestorMisiones != nullptr) {
			gestorMisiones->setObjetivoActual("Hablar con Maestro Qifrey");
		}

		actualizarCuartoActual(protagonista->getX(), protagonista->getY());
	}
};

#endif
