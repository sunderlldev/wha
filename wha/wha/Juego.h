#ifndef Juego_h
#define Juego_h
#include "Nivel.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Nivel3.h"
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
	int tickAnimPrevio;
public:
	Juego() : ejecutando(true), nivelActual(1), redibujarNecesario(true), puntajeGlobal(0), segundoPrevio(-1), tickAnimPrevio(-1) {
		pantalla.configurarConsola();
		listaNivel.push_back(new Nivel1());
		listaNivel.push_back(new Nivel2());
		listaNivel.push_back(new Nivel3());
		listaNivel[nivelActual]->inciarNivel();

		if (nivelActual == 1) {
			Nivel2* n2 = dynamic_cast<Nivel2*>(listaNivel[1]);
			if (n2 != nullptr) {
				n2->mostrarCinematicaIntro(pantalla);
			}
		}
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

	bool getEjecutando() const { return this->ejecutando; }
	void menuPrincipal() {
		pantalla.mostrarHistoriaIntro();
	}

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
		int tickAnim = (int)((clock() * 2) / CLOCKS_PER_SEC);
		if (segActual != segundoPrevio || tickAnim != tickAnimPrevio) {
			segundoPrevio = segActual;
			tickAnimPrevio = tickAnim;
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

				if (nivel.getEnCuartoRicheh()) {
					if (mapa->getColumnas() < pantalla.getAnchoJuego()) {
						camX = (mapa->getColumnas() - pantalla.getAnchoJuego()) / 2;
					}
					if (mapa->getFilas() < pantalla.getAltoTotal()) {
						camY = (mapa->getFilas() - pantalla.getAltoTotal()) / 2;
					}
				} else {
					if (camX < -15) camX = -15;
					if (camY < -10) camY = -10;
					if (camX > mapa->getColumnas() - pantalla.getAnchoJuego() + 15) {
						camX = mapa->getColumnas() - pantalla.getAnchoJuego() + 15;
					}
					if (camY > mapa->getFilas() - pantalla.getAltoTotal() + 10) {
						camY = mapa->getFilas() - pantalla.getAltoTotal() + 10;
					}
				}

				pantalla.limpiarBuffer();
				pantalla.copiarViewport(mapa->getMatriz(), camX, camY, tickAnim);

				if (!nivel.getEnCuartoRicheh() && nivel.getPozoEncontrado()) {
					int pox = nivel.getPozoX() - camX;
					int poy = nivel.getPozoY() - camY;
					for (int r = 0; r < 2; r++) {
						for (int c = 0; c < 2; c++) {
							if (pox + c >= 0 && pox + c < pantalla.getAnchoJuego() && poy + r >= 0 && poy + r < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(pox + c, poy + r, '%', 3);
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
							int iAncho = suelo[i]->getAncho();
							int iAlto = suelo[i]->getAlto();
							int iCol = suelo[i]->getColor();
							for (int r = 0; r < iAlto; r++) {
								for (int c = 0; c < iAncho; c++) {
									if (ix + c >= 0 && ix + c < pantalla.getAnchoJuego() && iy + r >= 0 && iy + r < pantalla.getAltoTotal()) {
										char ch = suelo[i]->getCaracter(r, c);
										if (ch != ' ') {
											pantalla.setPixelJuego(ix + c, iy + r, ch, iCol);
										}
									}
								}
							}
						}
					}

					const std::vector<Caja*>& cajas = nivel.getCajas();
					for (size_t i = 0; i < cajas.size(); i++) {
						if (cajas[i] != nullptr) {
							int cx = cajas[i]->getX() - camX;
							int cy = cajas[i]->getY() - camY;
							int cAncho = cajas[i]->getAncho();
							int cAlto = cajas[i]->getAlto();
							int cColor = cajas[i]->getColor();
							for (int r = 0; r < cAlto; r++) {
								for (int c = 0; c < cAncho; c++) {
									if (cx + c >= 0 && cx + c < pantalla.getAnchoJuego() && cy + r >= 0 && cy + r < pantalla.getAltoTotal()) {
										pantalla.setPixelJuego(cx + c, cy + r, cajas[i]->getCaracter(r, c), cColor);
									}
								}
							}
						}
					}

					NPC* q = nivel.getQifrey();
					if (q != nullptr) {
						int qx = q->getX() - camX;
						int qy = q->getY() - camY;
						for (int r = 0; r < q->getAlto(); r++) {
							for (int c = 0; c < q->getAncho(); c++) {
								if (qx + c >= 0 && qx + c < pantalla.getAnchoJuego() && qy + r >= 0 && qy + r < pantalla.getAltoTotal()) {
									char ch = q->getCaracter(r, c);
									if (ch != ' ') {
										int colQ = (r == 2) ? 1 : 3;
										pantalla.setPixelJuego(qx + c, qy + r, ch, colQ);
									}
								}
							}
						}
					}

					NPC* a = nivel.getAgott();
					if (a != nullptr) {
						int ax = a->getX() - camX;
						int ay = a->getY() - camY;
						for (int r = 0; r < a->getAlto(); r++) {
							for (int c = 0; c < a->getAncho(); c++) {
								if (ax + c >= 0 && ax + c < pantalla.getAnchoJuego() && ay + r >= 0 && ay + r < pantalla.getAltoTotal()) {
									char ch = a->getCaracter(r, c);
									if (ch != ' ') {
										int colA = (r == 2) ? 1 : 5;
										pantalla.setPixelJuego(ax + c, ay + r, ch, colA);
									}
								}
							}
						}
					}
				} else {
					NPC* r = nivel.getRicheh();
					if (r != nullptr) {
						int rx = r->getX() - camX;
						int ry = r->getY() - camY;
						for (int row = 0; row < r->getAlto(); row++) {
							for (int col = 0; col < r->getAncho(); col++) {
								if (rx + col >= 0 && rx + col < pantalla.getAnchoJuego() && ry + row >= 0 && ry + row < pantalla.getAltoTotal()) {
									char ch = r->getCaracter(row, col);
									if (ch != ' ') {
										int colR = (row == 2) ? 1 : 4;
										pantalla.setPixelJuego(rx + col, ry + row, ch, colR);
									}
								}
							}
						}
					}
				}

				int pantallaX = px - camX;
				int pantallaY = py - camY;
				for (int r = 0; r < prota->getAlto(); r++) {
					for (int c = 0; c < prota->getAncho(); c++) {
						if (pantallaX + c >= 0 && pantallaX + c < pantalla.getAnchoJuego() && pantallaY + r >= 0 && pantallaY + r < pantalla.getAltoTotal()) {
							char ch = prota->getCaracter(r, c);
							if (ch != ' ') {
								int colPersonaje = 6; //color azul

								if (nivel.getNumeroNivel() == 2) { //colocar color segun nivel
									colPersonaje = 2; //color verde
								}

								int colCoco = (r == 2) ? 1 : colPersonaje;
								pantalla.setPixelJuego(pantallaX + c, pantallaY + r, ch, colCoco);
							}
						}
					}
				}

				pantalla.renderizarPanelLateral(
					nivel.getNumeroNivel(),
					nivel.getNombreNivel(),
					prota->getNombre(),
					prota->getVida(),
					prota->getVidaMaxima()
				);

				if (!nivel.getPromptFlotante().empty() && !nivel.getEnDialogo() && nivel.getModalActivo() == 0 && !nivel.getMostrarEstadisticasFin()) {
					pantalla.dibujarPromptFlotante(nivel.getPromptFlotante());
				} else {
					pantalla.resetPromptAnimado();
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
						for (size_t i = 0; i < 6; i++) {
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

				pantalla.dibujar();
			}
		}
#ifdef _WIN32
		Sleep(30);
#endif
	}
};

#endif
