#ifndef Juego_h
#define Juego_h
#include "Nivel.h"
#include "Nivel1.h"
#include "Nivel2.h"
#include "Nivel3.h"
#include "Pantalla.h"
#include "GestorAudio.h"
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
	GestorAudio audio;
	bool redibujarNecesario;
	int puntajeGlobal;
	int segundoPrevio;
	int tickAnimPrevio;
	int tickLluviaPrevio;
public:
	Juego() : ejecutando(true), nivelActual(0), redibujarNecesario(true), puntajeGlobal(0), segundoPrevio(-1), tickAnimPrevio(-1), tickLluviaPrevio(-1) {
		pantalla.configurarConsola();
		listaNivel.push_back(new Nivel1());
		listaNivel.push_back(new Nivel2());
		listaNivel.push_back(new Nivel3());
		listaNivel[nivelActual]->iniciarNivel();

		if (nivelActual == 1) {
			listaNivel[nivelActual]->mostrarCinematicaIntro(pantalla);
		}
	}

	~Juego() {
		ejecutando = false;
		audio.detenerMusica();
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
		while (ejecutando) {
			int opcion = pantalla.mostrarMenuPrincipal();
			if (opcion == 0) {
				int nivelSel = pantalla.mostrarSelectorNivel();
				if (nivelSel == 0) {
					nivelActual = 0;
					listaNivel[nivelActual]->iniciarNivel();
					pantalla.mostrarHistoriaIntro();
					audio.reproducirNivel(nivelActual + 1);
					redibujarNecesario = true;
					break;
				} else if (nivelSel == 1) {
					nivelActual = 1;
					listaNivel[nivelActual]->iniciarNivel();
					listaNivel[nivelActual]->mostrarCinematicaIntro(pantalla);
					audio.reproducirNivel(nivelActual + 1);
					redibujarNecesario = true;
					break;
				} else if (nivelSel == 2) {
					nivelActual = 2;
					listaNivel[nivelActual]->iniciarNivel();
					listaNivel[nivelActual]->mostrarCinematicaIntro(pantalla);
					audio.reproducirNivel(nivelActual + 1);
					redibujarNecesario = true;
					break;
				}
			} else if (opcion == 1) {
				pantalla.mostrarCreditos();
			} else if (opcion == 2) {
				pantalla.mostrarLore();
			} else if (opcion == 3) {
				ejecutando = false;
				break;
			}
		}
	}

	void cambiarNivel() {
		if (nivelActual + 1 < (int)listaNivel.size()) {
			audio.detenerMusica();
			puntajeGlobal += listaNivel[nivelActual]->getPuntajeTotalNivel();
			nivelActual++;
			pantalla.resetSonidoFin();
			redibujarNecesario = true;
			listaNivel[nivelActual]->iniciarNivel();
			listaNivel[nivelActual]->mostrarCinematicaIntro(pantalla);
			audio.reproducirNivel(nivelActual + 1);
		} else {
			audio.detenerMusica();
			mostrarDesenlaceFinal();
		}
	}

	void iniciarAudioMinijuego() {
		audio.reproducirMinijuego();
	}

	void restaurarAudioNivel() {
		audio.reproducirNivel(nivelActual + 1);
	}

	void mostrarDesenlaceFinal() {
		pantalla.mostrarEpilogoFinal(puntajeGlobal);
		ejecutando = false;
	}

	void actualizar() {
		if (!ejecutando) return;
		audio.actualizar();

		Nivel& nivel = getNivelActualObj();
		if (nivel.getSolicitaSalir()) {
			ejecutando = false;
			audio.detenerMusica();
			return;
		}
		bool huboCambio = nivel.actualizar();

		if (nivel.getTransicionBajando()) {
			nivel.setTransicionBajando(false);
			Protagonista* prota = nivel.getProtagonista();
			pantalla.animarEscaleraPozo(true, nivel.getNumeroNivel(), nivel.getNombreNivel(), prota != nullptr ? prota->getNombre() : "Coco", prota != nullptr ? prota->getVida() : 3, prota != nullptr ? prota->getVidaMaxima() : 3);
			nivel.entrarCuartoRicheh(pantalla);
			redibujarNecesario = true;
			return;
		}

		if (nivel.getTransicionSubiendo()) {
			nivel.setTransicionSubiendo(false);
			Protagonista* prota = nivel.getProtagonista();
			pantalla.animarEscaleraPozo(false, nivel.getNumeroNivel(), nivel.getNombreNivel(), prota != nullptr ? prota->getNombre() : "Coco", prota != nullptr ? prota->getVida() : 3, prota != nullptr ? prota->getVidaMaxima() : 3);
			nivel.salirCuartoRicheh();
			redibujarNecesario = true;
			return;
		}

		if (nivel.getTransicionMinijuego()) {
			nivel.setTransicionMinijuego(false);
			nivel.ejecutarMinijuego(pantalla, audio);
			redibujarNecesario = true;
			return;
		}

		if (nivel.getTransicionEasterEgg()) {
			nivel.setTransicionEasterEgg(false);
			pantalla.mostrarEasterEggTrollface();
			redibujarNecesario = true;
			return;
		}

		if (nivel.verificarObjetivo()) {
			cambiarNivel();
			return;
		}

		int segActual = nivel.getSegundosTranscurridos();
		int tickAnim = (int)((clock() * 2) / CLOCKS_PER_SEC);
		int tickLluvia = (int)((clock() * 8) / CLOCKS_PER_SEC);
		if (segActual != segundoPrevio || tickAnim != tickAnimPrevio || tickLluvia != tickLluviaPrevio) {
			segundoPrevio = segActual;
			tickAnimPrevio = tickAnim;
			tickLluviaPrevio = tickLluvia;
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

				bool (*funcCuarto)(int, int) = (nivel.getNumeroNivel() == 2) ? Nivel2::esCuartoEstatico : ((nivel.getNumeroNivel() == 3) ? Nivel3::esCuartoEstatico : Nivel1::esCuartoEstatico);
				bool (*funcCamino)(int, int) = (nivel.getNumeroNivel() == 2) ? nullptr : ((nivel.getNumeroNivel() == 3) ? Nivel3::esCaminoEstatico : Nivel1::esCaminoEstatico);
				Nivel1* n1Ref = dynamic_cast<Nivel1*>(&nivel);
				bool cuevaDesbloqueada = (n1Ref != nullptr) ? n1Ref->getParedPiedraDestruida() : true;

				pantalla.limpiarBuffer();
				pantalla.copiarViewport(mapa->getMatriz(), camX, camY, tickAnim, nivel.getEnCuartoRicheh(), funcCuarto, funcCamino, cuevaDesbloqueada);
				pantalla.aplicarLluvia(tickLluvia, camX, camY, nivel.getEnCuartoRicheh(), funcCuarto);

				if (!nivel.getEnCuartoRicheh() && nivel.getPozoEncontrado()) {
					int pox = nivel.getPozoX() - camX;
					int poy = nivel.getPozoY() - camY;
					const char* artPozo[3] = {
						"/==/",
						"|==|",
						"/==/"
					};
					for (int r = 0; r < 3; r++) {
						for (int c = 0; c < 4; c++) {
							if (pox + c >= 0 && pox + c < pantalla.getAnchoJuego() && poy + r >= 0 && poy + r < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(pox + c, poy + r, artPozo[r][c], 12);
							}
						}
					}
				}

				if (!nivel.getEnCuartoRicheh()) {
					const std::vector<ItemMagico*>& suelo = nivel.getItemsSuelo();
					for (size_t i = 0; i < suelo.size(); i++) {
						if (suelo[i] != nullptr && !suelo[i]->getRecogido()) {
							if (!cuevaDesbloqueada && suelo[i]->getX() >= 387 && suelo[i]->getY() <= 40) {
								continue;
							}
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

					const std::vector<Letrero*>& lets = nivel.getLetreros();
					int segReloj = (int)(clock() / CLOCKS_PER_SEC);
					bool parpadeoAlerta = (segReloj % 2 != 0);
					for (size_t i = 0; i < lets.size(); i++) {
						if (lets[i] != nullptr) {
							int lx = lets[i]->getX() - camX;
							int ly = lets[i]->getY() - camY;
							if (lx + 2 >= 0 && lx < pantalla.getAnchoJuego() && ly >= 0 && ly < pantalla.getAltoTotal()) {
								if (lets[i]->getLeido()) {
									pantalla.setPixelJuego(lx, ly, '[', 10);
									pantalla.setPixelJuego(lx + 1, ly, '!', 10);
									pantalla.setPixelJuego(lx + 2, ly, ']', 10);
								} else {
									if (parpadeoAlerta) {
										pantalla.setPixelJuego(lx, ly, '[', 10);
										pantalla.setPixelJuego(lx + 1, ly, '!', 7);
										pantalla.setPixelJuego(lx + 2, ly, ']', 10);
									} else {
										pantalla.setPixelJuego(lx, ly, '[', 8);
										pantalla.setPixelJuego(lx + 1, ly, '!', 4);
										pantalla.setPixelJuego(lx + 2, ly, ']', 8);
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
										int colQ = (r == 2) ? 1 : q->getColorTraje();
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
										int colA = (r == 2) ? 1 : a->getColorTraje();
										pantalla.setPixelJuego(ax + c, ay + r, ch, colA);
									}
								}
							}
						}
					}

					Nivel1* n1 = dynamic_cast<Nivel1*>(&nivel);
					if (n1 != nullptr && n1->debeDibujarMyrphon()) {
						int mx = n1->getMyrphonX() - camX;
						int my = n1->getMyrphonY() - camY;
						const char* myrFila0 = "(o> ";
						const char* myrFila1 = "/||\\";
						for (int c = 0; c < 4; c++) {
							if (mx + c >= 0 && mx + c < pantalla.getAnchoJuego() && my >= 0 && my < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(mx + c, my, myrFila0[c], 4);
							}
							if (mx + c >= 0 && mx + c < pantalla.getAnchoJuego() && my + 1 >= 0 && my + 1 < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(mx + c, my + 1, myrFila1[c], 4);
							}
						}
					}
					nivel.dibujarEntidadesExtra(pantalla, camX, camY);
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
					if (nivel.getGestorDialogos() != nullptr && nivel.getGestorDialogos()->getMyrphonRescatado()) {
						int mx = 64 - camX;
						int my = 12 - camY;
						const char* myrFila0 = "(o> ";
						const char* myrFila1 = "/||\\";
						for (int c = 0; c < 4; c++) {
							if (mx + c >= 0 && mx + c < pantalla.getAnchoJuego() && my >= 0 && my < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(mx + c, my, myrFila0[c], 4);
							}
							if (mx + c >= 0 && mx + c < pantalla.getAnchoJuego() && my + 1 >= 0 && my + 1 < pantalla.getAltoTotal()) {
								pantalla.setPixelJuego(mx + c, my + 1, myrFila1[c], 4);
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
								int colPersonaje = 6;

								if (nivel.getNumeroNivel() == 2) {
									colPersonaje = 2;
								} else if (nivel.getNumeroNivel() == 3) {
									colPersonaje = 6;
								}

								int colCoco = (r == 2) ? 1 : colPersonaje;
								if (nivel.getNumeroNivel() == 3) {
									if (r == 1) colCoco = 7;
									else if (r == 2) colCoco = 3;
									else if (r == 3) colCoco = 8;
									else colCoco = 6;
								}
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
					prota->getVidaMaxima(),
					nivel.getNombreUbicacionActual()
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
					GestorDialogos* gd = nivel.getGestorDialogos();
					bool myrphonRescatado = (gd != nullptr && gd->getMyrphonRescatado());
					bool misionMyrphonActiva = (gd != nullptr && gd->getMisionMyrphonActiva());

					std::vector<std::string> nombres;
					nombres.push_back("Coco");
					nombres.push_back("Qifrey");
					nombres.push_back("Richeh");
					nombres.push_back("Agott");
					nombres.push_back("Tetia");
					nombres.push_back("Olruggio");
					nombres.push_back("Iguin");
					nombres.push_back("Restys");
					nombres.push_back("Myrphon");

					std::vector<std::string> roles;
					roles.push_back("Aprendiz de Maga");
					roles.push_back("Maestro Hechicero");
					roles.push_back("Aprendiz");
					roles.push_back("Aprendiz");
					roles.push_back("Aprendiz");
					roles.push_back("Inspector Mágico");
					roles.push_back("Mago de Ala Ancha");
					roles.push_back("Mago de Ala Ancha");
					roles.push_back(myrphonRescatado ? "Mascota de Richeh" : "Mascota perdida de Richeh");

					std::vector<std::string> desc;
					desc.push_back("Protagonista del Nivel 1. Descubrió la verdad sobre el dibujo mágico.");
					desc.push_back("Tutor y protector del atelier. Especialista en trazos y magia de agua.");
					desc.push_back("Amiga reflexiva de Coco. Gran conocedora de runas antiguas.");
					desc.push_back("Compañera disciplinada y exigente. Aspira a la perfección del trazo.");
					desc.push_back("Compañera alegre y entusiasta. Le fascina la magia que anima vidas.");
					desc.push_back("Hechicero artesano que protege el atelier contra peligros.");
					desc.push_back("Líder misterioso de los Sombreros de Ala Ancha. Es quien causó la tragedia de Coco y orquestó el asalto en la cueva.");
					desc.push_back("Mago proscrito especializado en el uso de magia médica prohibida y en alterar cuerpos para reclutar aliados.");
					desc.push_back("Pingüino-grifo de cuatro patas propiedad de Richeh. Quedó atrapado en la oscuridad del laberinto Serpentback.");

					std::vector<int> confianzas;
					confianzas.push_back(2);
					confianzas.push_back(nivel.getQifrey() != nullptr ? nivel.getQifrey()->getConfianza() : 0);
					confianzas.push_back(nivel.getRicheh() != nullptr ? nivel.getRicheh()->getConfianza() : 0);
					confianzas.push_back(nivel.getAgott() != nullptr ? nivel.getAgott()->getConfianza() : 0);
					confianzas.push_back(0);
					confianzas.push_back(0);
					confianzas.push_back(-1);
					confianzas.push_back(-1);
					confianzas.push_back(myrphonRescatado ? 3 : -2);

					std::vector<bool> desbloqueados;
					desbloqueados.push_back(true);
					desbloqueados.push_back(true);
					desbloqueados.push_back(nivel.getRicheh() != nullptr ? nivel.getRicheh()->getYaHablo() : false);
					desbloqueados.push_back(nivel.getAgott() != nullptr ? nivel.getAgott()->getYaHablo() : false);
					desbloqueados.push_back(false);
					desbloqueados.push_back(false);
					desbloqueados.push_back(true);
					desbloqueados.push_back(true);
					desbloqueados.push_back(misionMyrphonActiva || myrphonRescatado);

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
