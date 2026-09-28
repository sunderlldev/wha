#ifndef Juego_h
#define Juego_h
#include "Nivel.h"
#include "Pantalla.h"
#include <vector>
#include <string>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class Juego {
private:
	bool ejecutando;
	int nivelActual;
	std::vector<Nivel*> listaNivel;
	Pantalla pantalla;
	bool redibujarNecesario;
	int puntajeGlobal;
	int segundoPrevio;
public:
	Juego() : ejecutando(true), nivelActual(0), redibujarNecesario(true), puntajeGlobal(0), segundoPrevio(-1) {
		pantalla.configurarConsola();
		listaNivel.push_back(new Nivel(1, "Atelier de Qifrey", 200, 600));
		listaNivel.push_back(new Nivel(2, "Tierras Prohibidas", 200, 600));
		listaNivel.push_back(new Nivel(3, "Gran Arbol de Plata", 200, 600));
		listaNivel[nivelActual]->inciarNivel();
	}

	~Juego() {
		for (size_t i = 0; i < listaNivel.size(); i++) {
			delete listaNivel[i];
		}
		listaNivel.clear();
	}

	Nivel& getNivelActualObj() {
		return *listaNivel[nivelActual];
	}

	size_t getCantNiveles() const { return listaNivel.size(); }
	bool getEjecutando() const { return this->ejecutando; }
	int getNivelActual() const { return this->nivelActual; }
	void setEjecutando(bool estado) { this->ejecutando = estado; }
	void setNivelActual(int nivel) { this->nivelActual = nivel; }
	void menuPrincipal() {}

	void cambiarNivel() {
		if (nivelActual + 1 < (int)listaNivel.size()) {
			puntajeGlobal += listaNivel[nivelActual]->getPuntajeTotalNivel();
			nivelActual++;
			redibujarNecesario = true;
			listaNivel[nivelActual]->inciarNivel();
		} else {
			mostrarDesenlaceFinal();
		}
	}

	void mostrarDesenlaceFinal() {
		std::cout << "Felicidades! Has completado el viaje del Arbol de Plata!\n";
	}

	void mostrarPreguntaReflexxiva() {}

	void actualizar() {
		if (!ejecutando) return;

		Nivel& nivel = getNivelActualObj();
		bool huboCambio = nivel.actualizar();

		if (nivel.getTransicionBajando()) {
			nivel.setTransicionBajando(false);
			pantalla.dibujarPantallaMensajeCentrado("[Pronto animacion de Coco bajando, xd]");
			pantalla.dibujar();
#ifdef _WIN32
			Sleep(3000);
#else
			usleep(3000000);
#endif
			nivel.entrarCuartoRicheh();
			redibujarNecesario = true;
			return;
		}

		if (nivel.getTransicionSubiendo()) {
			nivel.setTransicionSubiendo(false);
			pantalla.dibujarPantallaMensajeCentrado("[Pronto animacion de Coco subiendo, xd]");
			pantalla.dibujar();
#ifdef _WIN32
			Sleep(3000);
#else
			usleep(3000000);
#endif
			nivel.salirCuartoRicheh();
			redibujarNecesario = true;
			return;
		}

		if (nivel.verificarObjetivo()) {
			cambiarNivel();
			return;
		}

		int segActual = nivel.getSegundosTranscurridos();
		if (segActual != segundoPrevio) {
			segundoPrevio = segActual;
			huboCambio = true;
		}

		if (redibujarNecesario || huboCambio) {
			redibujarNecesario = false;

			Mapa* mapa = nivel.getMapa();
			Protagonista* prota = nivel.getProtagonista();

			if (mapa != nullptr && prota != nullptr) {
				int px = prota->getX();
				int py = prota->getY();

				int centroX = (pantalla.getAnchoJuego() - 2) / 2;
				int centroY = (pantalla.getAltoTotal() - 2) / 2;

				int camX = px - centroX;
				int camY = py - centroY;

				if (camX < 0) camX = 0;
				if (camY < 0) camY = 0;
				if (camX + pantalla.getAnchoJuego() > mapa->getColumnas()) {
					camX = mapa->getColumnas() - pantalla.getAnchoJuego();
				}
				if (camY + pantalla.getAltoTotal() > mapa->getFilas()) {
					camY = mapa->getFilas() - pantalla.getAltoTotal();
				}
				if (camX < 0) camX = 0;
				if (camY < 0) camY = 0;

				pantalla.limpiarBuffer();
				pantalla.copiarViewport(mapa->getMatriz(), camX, camY);

				if (!nivel.getEnCuartoRicheh() && nivel.getPozoEncontrado()) {
					int pox = nivel.getPozoX() - camX;
					int poy = nivel.getPozoY() - camY;
					for (int r = 0; r < 2; r++) {
						for (int c = 0; c < 2; c++) {
							if (pox + c >= 0 && pox + c < pantalla.getAnchoJuego() && poy + r >= 0 && poy + r < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(pox + c, poy + r, (char)190);
							}
						}
					}
				}

				if (!nivel.getEnCuartoRicheh()) {
					const std::vector<ItemMagico*>& suelo = nivel.getItemsSuelo();
					for (size_t i = 0; i < suelo.size(); i++) {
						if (suelo[i] != nullptr && !suelo[i]->getRecogido()) {
							int ix = suelo[i]->getX() - camX;
							int iy = suelo[i]->getY() - camY;
							if (ix >= 0 && ix < pantalla.getAnchoJuego() && iy >= 0 && iy < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(ix, iy, '*');
							}
						}
					}

					const std::vector<Caja*>& cajas = nivel.getCajas();
					for (size_t i = 0; i < cajas.size(); i++) {
						if (cajas[i] != nullptr) {
							int cx = cajas[i]->getX() - camX;
							int cy = cajas[i]->getY() - camY;
							for (int r = 0; r < 3; r++) {
								for (int c = 0; c < 3; c++) {
									if (cx + c >= 0 && cx + c < pantalla.getAnchoJuego() && cy + r >= 0 && cy + r < pantalla.getAltoTotal()) {
										pantalla.setPixelJuego(cx + c, cy + r, cajas[i]->getCaracter(r, c));
									}
								}
							}
						}
					}

					NPC* q = nivel.getQifrey();
					if (q != nullptr) {
						int qx = q->getX() - camX;
						int qy = q->getY() - camY;
						for (int r = 0; r < 2; r++) {
							for (int c = 0; c < 2; c++) {
								if (qx + c >= 0 && qx + c < pantalla.getAnchoJuego() && qy + r >= 0 && qy + r < pantalla.getAltoTotal()) {
									pantalla.setPixelJuego(qx + c, qy + r, q->getCaracter(r, c));
								}
							}
						}
					}

					NPC* a = nivel.getAgott();
					if (a != nullptr) {
						int ax = a->getX() - camX;
						int ay = a->getY() - camY;
						for (int r = 0; r < 2; r++) {
							for (int c = 0; c < 2; c++) {
								if (ax + c >= 0 && ax + c < pantalla.getAnchoJuego() && ay + r >= 0 && ay + r < pantalla.getAltoTotal()) {
									pantalla.setPixelJuego(ax + c, ay + r, a->getCaracter(r, c));
								}
							}
						}
					}
				} else {
					NPC* r = nivel.getRicheh();
					if (r != nullptr) {
						int rx = r->getX() - camX;
						int ry = r->getY() - camY;
						for (int row = 0; row < 2; row++) {
							for (int col = 0; col < 2; col++) {
								if (rx + col >= 0 && rx + col < pantalla.getAnchoJuego() && ry + row >= 0 && ry + row < pantalla.getAltoTotal()) {
									pantalla.setPixelJuego(rx + col, ry + row, r->getCaracter(row, col));
								}
							}
						}
					}
				}

				int pantallaX = px - camX;
				int pantallaY = py - camY;
				for (int r = 0; r < 2; r++) {
					for (int c = 0; c < 2; c++) {
						if (pantallaX + c >= 0 && pantallaX + c < pantalla.getAnchoJuego() && pantallaY + r >= 0 && pantallaY + r < pantalla.getAltoTotal()) {
							pantalla.setPixelJuego(pantallaX + c, pantallaY + r, prota->getCaracter(r, c));
						}
					}
				}

				if (!nivel.getPromptFlotante().empty() && !nivel.getEnDialogo() && !nivel.getEnModalPersonajes() && !nivel.getEnModalMisiones() && !nivel.getEnModalInventario() && !nivel.getMostrarEstadisticasFin()) {
					pantalla.dibujarPromptFlotante(nivel.getPromptFlotante());
				}

				if (nivel.getEnDialogo()) {
					std::string hablante, rol;
					int confianza = 0;
					std::vector<std::string> lineas, opciones;
					nivel.obtenerDatosDialogo(hablante, rol, confianza, lineas, opciones);
					pantalla.dibujarCuadroDialogo(hablante, rol, confianza, lineas, opciones);
				} else {
					pantalla.resetDialogoAnimado();
				}

				if (nivel.getEnModalPersonajes()) {
					std::vector<std::string> nombres;
					nombres.push_back("Coco");
					nombres.push_back("Qifrey");
					nombres.push_back("Richie");
					nombres.push_back("Agott");
					nombres.push_back("Tetia");
					nombres.push_back("Olruggio");

					std::vector<std::string> roles;
					roles.push_back("Aprendiz de Maga");
					roles.push_back("Maestro Hechicero");
					roles.push_back("Aprendiz");
					roles.push_back("Aprendiz");
					roles.push_back("Aprendiz");
					roles.push_back("Inspector Magico");

					std::vector<std::string> desc;
					desc.push_back("Protagonista del Nivel 1. Descubrio la verdad sobre el dibujo magico.");
					desc.push_back("Tutor y protector del atelier. Especialista en trazos y magia de agua.");
					desc.push_back("Amiga reflexiva de Coco. Gran conocedora de runas antiguas.");
					desc.push_back("Companera disciplinada y exigente. Aspira a la perfeccion del trazo.");
					desc.push_back("Companera alegre y entusiasta. Le fascina la magia que anima vidas.");
					desc.push_back("Hechicero artesano que protege el atelier contra peligros.");

					std::vector<int> confianzas;
					confianzas.push_back(2);
					confianzas.push_back(nivel.getQifrey() != nullptr ? nivel.getQifrey()->getConfianza() : 0);
					confianzas.push_back(nivel.getRicheh() != nullptr ? nivel.getRicheh()->getConfianza() : 0);
					confianzas.push_back(nivel.getAgott() != nullptr ? nivel.getAgott()->getConfianza() : 0);
					confianzas.push_back(0);
					confianzas.push_back(0);

					std::vector<bool> desbloqueados;
					desbloqueados.push_back(true);
					desbloqueados.push_back(true);
					desbloqueados.push_back(nivel.getRicheh() != nullptr ? nivel.getRicheh()->getYaHablo() : false);
					desbloqueados.push_back(nivel.getAgott() != nullptr ? nivel.getAgott()->getYaHablo() : false);
					desbloqueados.push_back(false);
					desbloqueados.push_back(false);

					pantalla.dibujarModalPersonajes(nivel.getSeleccionModal(), nombres, roles, desc, confianzas, desbloqueados);
				}

				if (nivel.getEnModalMisiones()) {
					std::vector<std::string> titulos;
					std::vector<std::string> descMisiones;
					std::vector<std::string> estados;
					std::vector<bool> desbloqueadas;
					nivel.obtenerDatosMisiones(titulos, descMisiones, estados, desbloqueadas);
					pantalla.dibujarModalMisiones(nivel.getSeleccionMision(), nivel.getEnDetalleMision(), titulos, descMisiones, estados, desbloqueadas);
				}

				if (nivel.getEnModalInventario()) {
					std::vector<std::string> nombres;
					std::vector<std::string> tipos;
					std::vector<std::string> descItems;
					Inventario* inv = prota->getInventario();
					if (inv != nullptr) {
						for (size_t i = 0; i < 4; i++) {
							ItemMagico* it = inv->getItem(i);
							if (it != nullptr) {
								nombres.push_back(it->getNombre());
								tipos.push_back(it->getTipoItem());
								descItems.push_back(it->getDescripcion());
							} else {
								nombres.push_back("");
								tipos.push_back("");
								descItems.push_back("");
							}
						}
					}
					pantalla.dibujarModalInventario(nivel.getSeleccionInventario(), nombres, tipos, descItems);
				}

				if (nivel.getMostrarEstadisticasFin()) {
					pantalla.dibujarEstadisticasFinNivel(
						nivel.getNumeroNivel(),
						nivel.getNombreNivel(),
						prota->getNombre(),
						segActual,
						nivel.getBonoTiempo(),
						nivel.getPuntosMisiones(),
						nivel.getQifrey() != nullptr ? nivel.getQifrey()->getConfianza() : 2
					);
				}

				pantalla.renderizarPanelLateral(
					nivel.getNumeroNivel(),
					nivel.getNombreNivel(),
					prota->getNombre(),
					prota->getVida(),
					prota->getVidaMaxima()
				);

				pantalla.dibujar();
			}
		}
#ifdef _WIN32
		Sleep(30);
#endif
	}
};

#endif
