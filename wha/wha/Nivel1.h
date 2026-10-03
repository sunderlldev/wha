#ifndef Nivel1_h
#define Nivel1_h
#include "Nivel.h"
#include "MapaNivel1.h"
#include "Arbol.h"
#include "Gato.h"

class Nivel1 : public Nivel {
public:
	Nivel1() : Nivel(1, "Atelier de Qifrey", 197, 524) {}
	virtual ~Nivel1() {}

	virtual int determinarCuarto(int px, int py) const override {
		if (px >= 59 && px <= 125 && py >= 19 && py <= 45) return 1;
		if (px >= 382 && px <= 457 && py >= 11 && py <= 49) return 2;
		if (px >= 384 && px <= 501 && py >= 62 && py <= 131) return 3;
		if (px >= 382 && px <= 456 && py >= 150 && py <= 178) return 4;
		if (px >= 62 && px <= 122 && py >= 148 && py <= 175) return 5;
		return 0;
	}

	virtual void actualizarCuartoActual(int px, int py) override {
		int nuevoCuarto = determinarCuarto(px, py);
		if (nuevoCuarto != idCuartoActual) {
			idCuartoActual = nuevoCuarto;
			if (nuevoCuarto == 1) {
				mostrarMensajeTemporal("[LA CHOZA DE HECHIZOS]", 60);
			} else if (nuevoCuarto == 2) {
				mostrarMensajeTemporal("[EL ALMACEN ABANDONADO]", 60);
			} else if (nuevoCuarto == 3) {
				mostrarMensajeTemporal("[TORRE DE AGOTT]", 60);
				if (agott != nullptr && !agott->getYaHablo()) {
					agott->setYaHablo(true);
					if (gestorDialogos != nullptr) {
						gestorDialogos->iniciarDialogo("Agott", 200);
					}
				}
			} else if (nuevoCuarto == 4) {
				mostrarMensajeTemporal("[BOSQUE DE PLATA]", 60);
			} else if (nuevoCuarto == 5) {
				mostrarMensajeTemporal("[DESPACHO DE QIFREY]", 60);
			} else {
				mensajeTemporal = "";
				ticksMensajeTemporal = 0;
			}
		}
	}

	virtual bool verificarProximidadEspecial(int px, int py, int pw, int ph) override {
		(void)pw;
		(void)ph;
		if (!paredPiedraDestruida && px >= 365 && px <= 385 && py >= 25 && py <= 38) {
			Inventario* inv = protagonista->getInventario();
			bool tieneVara = (inv != nullptr && inv->tieneItem("Vara magica"));
			if (tieneVara) {
				promptFlotante = "[ENTER] Usar Vara magica para derribar pared";
			} else {
				promptFlotante = "Coco: Este lugar parece estar bloqueado, puedo derribarlo pero necesito magia...";
			}
			return true;
		}
		return false;
	}

	virtual bool procesarInteraccionEspecial(int px, int py, int pw, int ph) override {
		(void)pw;
		(void)ph;
		if (!paredPiedraDestruida && px >= 365 && px <= 385 && py >= 25 && py <= 38) {
			Inventario* inv = protagonista->getInventario();
			if (inv != nullptr && inv->tieneItem("Vara magica")) {
				paredPiedraDestruida = true;
				if (mapa != nullptr) {
					for (int wy = 27; wy <= 36; wy++) {
						for (int wx = 380; wx <= 386; wx++) {
							mapa->setCaracter(wx, wy, ' ');
						}
					}
				}
				inv->removerItem("Vara magica");
				promptFlotante = "[Lanzaste bola de fuego! Pared destruida (Vara consumida)]";
				return true;
			}
		}
		return false;
	}

	virtual void inciarNivel() override {
		this->tiempoInicio = clock();
		this->tiempoFin = 0;
		this->completado = false;
		if (gestorDialogos != nullptr) gestorDialogos->terminarDialogo();
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

		if (cuartoRicheh != nullptr) {
			cuartoRicheh->reiniciar();
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

		mapa->agregarObjeto(new Gato(416, 181, "Gato"));

		if (protagonista == nullptr) {
			protagonista = new Protagonista(90, 22, "Coco", 3, 1);
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
		itemsSuelo.push_back(new ItemMagico(405, 20, "Tela", "Trozo de tela arcana resistente y ligera para confeccionar vestiduras.", "Material Magico", false));
		itemsSuelo.push_back(new ItemMagico(440, 20, "Frasco de Tinta", "Frasco con tinta arcaica de plata preservada en el almacen antiguo.", "Objeto de Coleccion", false));
		itemsSuelo.push_back(new ItemMagico(160, 50, "Tinta de Viento", "Esencia de tinta de viento encontrada junto a la orilla del gran rio.", "Objeto de Coleccion", false));

		limpiarLetreros();
		letreros.push_back(new Letrero(75, 22, "Letrero: [CHOZA DE TRAZOS] Dibuja runas con pasion y cuida tus pergaminos."));
		letreros.push_back(new Letrero(355, 30, "Letrero: ALMACEN ABANDONADO. Peligro: Derrumbe. Usa magia ignea."));
		letreros.push_back(new Letrero(400, 70, "Letrero: TORRE DE AGOTT. Prohibido el paso sin autorizacion de Agott."));
		letreros.push_back(new Letrero(75, 152, "Letrero: DESPACHO DE QIFREY. Maestro del atelier y protector del agua."));

		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr && mapa != nullptr) {
				int lx = letreros[i]->getX();
				int ly = letreros[i]->getY();
				mapa->setCaracter(lx, ly, '[');
				mapa->setCaracter(lx + 1, ly, '!');
				mapa->setCaracter(lx + 2, ly, ']');
				mapa->setCaracter(lx, ly + 1, ' ');
				mapa->setCaracter(lx + 1, ly + 1, '|');
				mapa->setCaracter(lx + 2, ly + 1, ' ');
			}
		}

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
