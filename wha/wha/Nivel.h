#ifndef Nivel_h
#define Nivel_h
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include "Mapa.h"
#include "Protagonista.h"
#include "NPC.h"
#include "ItemMagico.h"
#include "Inventario.h"
#include "Caja.h"
#include "CuartoRicheh.h"
#include "GestorDialogos.h"
#include "GestorMisiones.h"
#include "TorreAgott.h"
#include "Letrero.h"
#include "Pantalla.h"
class GestorAudio;
#ifdef _WIN32
#include <conio.h>
#endif

class Nivel {
protected:
	int numeroNivel;
	std::string nombreNivel;
	bool completado;
	bool mostrarEstadisticasFin;
	Mapa* mapa;
	Protagonista* protagonista;
	NPC* qifrey;
	NPC* agott;
	std::vector<ItemMagico*> itemsSuelo;
	std::vector<Letrero*> letreros;
	int idCuartoActual;
	std::string promptFlotante;
	std::string mensajeTemporal;
	int ticksMensajeTemporal;
	clock_t tiempoInicio;
	clock_t tiempoFin;
	int modalActivo;
	int seleccionModal;
	bool solicitaSalir;
	CuartoRicheh* cuartoRicheh;
	TorreAgott* torreAgott;
	GestorDialogos* gestorDialogos;
	GestorMisiones* gestorMisiones;
	bool transicionMinijuego;
	bool animacionMyrphonHecha;
	bool transicionEasterEgg;
	bool transicionDanioCoustas;

	bool estaCerca(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2, int maxDist) const {
		int distX = (x1 + w1 <= x2) ? (x2 - (x1 + w1)) : ((x2 + w2 <= x1) ? (x1 - (x2 + w2)) : 0);
		int distY = (y1 + h1 <= y2) ? (y2 - (y1 + h1)) : ((y2 + h2 <= y1) ? (y1 - (y2 + h2)) : 0);
		return (distX <= maxDist && distY <= maxDist);
	}

	bool colisionaConNPC(int nx, int ny, int pw, int ph, NPC* npc) const {
		if (npc == nullptr) return false;
		return (nx < npc->getX() + npc->getAncho() && nx + pw > npc->getX() &&
		        ny < npc->getY() + npc->getAlto() && ny + ph > npc->getY());
	}

	bool interactuarLetrero(int px, int py, int pw, int ph) {
		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr && estaCerca(px, py, pw, ph, letreros[i]->getX(), letreros[i]->getY(), letreros[i]->getAncho(), letreros[i]->getAlto(), 3)) {
				letreros[i]->marcarLeido();
				mensajeTemporal = letreros[i]->getTexto();
				ticksMensajeTemporal = 100;
				promptFlotante = mensajeTemporal;
				actualizarProximidad();
				return true;
			}
		}
		return false;
	}

	void limpiarItemsSuelo() {
		for (size_t i = 0; i < itemsSuelo.size(); i++) {
			if (itemsSuelo[i] != nullptr) {
				delete itemsSuelo[i];
			}
		}
		itemsSuelo.clear();
	}

	void limpiarLetreros() {
		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr) {
				delete letreros[i];
			}
		}
		letreros.clear();
	}

	void dibujarLetrerosEnMapa() {
		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr && mapa != nullptr) {
				mapa->dibujarObjeto(letreros[i]);
			}
		}
	}

public:
	Nivel(int numeroN, std::string nombreN, int filasMapa, int columnasMapa)
		: numeroNivel(numeroN), nombreNivel(nombreN), completado(false),
		  mostrarEstadisticasFin(false), mapa(nullptr), protagonista(nullptr),
		  qifrey(nullptr), agott(nullptr),
		  idCuartoActual(0), promptFlotante(""), mensajeTemporal(""),
		  ticksMensajeTemporal(0), tiempoInicio(0), tiempoFin(0),
		  modalActivo(0), seleccionModal(0), solicitaSalir(false),
		  cuartoRicheh(nullptr), torreAgott(nullptr), transicionMinijuego(false),
		  animacionMyrphonHecha(false), transicionEasterEgg(false), transicionDanioCoustas(false) {
		this->mapa = new Mapa(filasMapa, columnasMapa);
		this->gestorDialogos = new GestorDialogos();
		this->gestorMisiones = new GestorMisiones();
	}

	virtual ~Nivel() {
		if (this->mapa != nullptr) {
			delete this->mapa;
			this->mapa = nullptr;
		}
		if (this->protagonista != nullptr) {
			delete this->protagonista;
			this->protagonista = nullptr;
		}
		if (this->qifrey != nullptr) {
			delete this->qifrey;
			this->qifrey = nullptr;
		}
		if (this->agott != nullptr) {
			delete this->agott;
			this->agott = nullptr;
		}
		if (this->cuartoRicheh != nullptr) {
			delete this->cuartoRicheh;
			this->cuartoRicheh = nullptr;
		}
		if (this->torreAgott != nullptr) {
			delete this->torreAgott;
			this->torreAgott = nullptr;
		}
		if (this->gestorDialogos != nullptr) {
			delete this->gestorDialogos;
			this->gestorDialogos = nullptr;
		}
		if (this->gestorMisiones != nullptr) {
			delete this->gestorMisiones;
			this->gestorMisiones = nullptr;
		}
		limpiarItemsSuelo();
		limpiarLetreros();
	}

	virtual void mostrarCinematicaIntro(Pantalla&) {
	}

	void mostrarMensajeTemporal(const std::string& msg, int ticks) {
		this->mensajeTemporal = msg;
		this->ticksMensajeTemporal = ticks;
	}

	void entrarCuartoRicheh(Pantalla& pantalla) {
		if (cuartoRicheh != nullptr) {
			cuartoRicheh->entrar(protagonista);
			if (gestorDialogos != nullptr && gestorDialogos->getDioVaraRicheh()) {
				cuartoRicheh->colocarMyrphon();
			}
		}
		promptFlotante = "";
		mostrarMensajeTemporal("[SÓTANO SECRETO DE RICHEH]", 60);
		if (gestorDialogos != nullptr && gestorDialogos->getMyrphonRescatado() && !animacionMyrphonHecha) {
			animacionMyrphonHecha = true;
			if (cuartoRicheh != nullptr) {
				int camX = 0;
				int camY = (cuartoRicheh->getMapa()->getFilas() - pantalla.getAltoTotal()) / 2;
				pantalla.animarMyrphonRegresaARicheh(cuartoRicheh->getMapa()->getMatriz(), camX, camY,
				                                     numeroNivel, nombreNivel,
				                                     protagonista != nullptr ? protagonista->getNombre() : "Coco",
				                                     protagonista != nullptr ? protagonista->getVida() : 3,
				                                     protagonista != nullptr ? protagonista->getVidaMaxima() : 3);
				cuartoRicheh->colocarMyrphon();
			}
			NPC* richeh = getRicheh();
			if (richeh != nullptr) richeh->setExpresion(1);
			gestorDialogos->iniciarDialogo("Richeh", 350);
		}
	}

	void entrarCuartoRicheh() {
		if (cuartoRicheh != nullptr) {
			cuartoRicheh->entrar(protagonista);
			if (gestorDialogos != nullptr && gestorDialogos->getDioVaraRicheh()) {
				cuartoRicheh->colocarMyrphon();
			}
		}
		promptFlotante = "";
		mostrarMensajeTemporal("[SÓTANO SECRETO DE RICHEH]", 60);
		if (gestorDialogos != nullptr && gestorDialogos->getMyrphonRescatado() && !gestorDialogos->getDioVaraRicheh()) {
			NPC* richeh = getRicheh();
			if (richeh != nullptr) richeh->setExpresion(1);
			gestorDialogos->iniciarDialogo("Richeh", 350);
		}
	}

	void salirCuartoRicheh() {
		if (cuartoRicheh != nullptr) {
			cuartoRicheh->salir(protagonista);
		}
		promptFlotante = "";
		mostrarMensajeTemporal("[TORRE DE AGOTT]", 60);
	}

	bool getEnCuartoRicheh() const {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getActivo() : false;
	}
	NPC* getRicheh() {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getRicheh() : nullptr;
	}
	bool getTransicionBajando() const {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getTransicionBajando() : false;
	}
	bool getTransicionSubiendo() const {
		return (cuartoRicheh != nullptr) ? cuartoRicheh->getTransicionSubiendo() : false;
	}
	void setTransicionBajando(bool tb) {
		if (cuartoRicheh != nullptr) cuartoRicheh->setTransicionBajando(tb);
	}
	void setTransicionSubiendo(bool ts) {
		if (cuartoRicheh != nullptr) cuartoRicheh->setTransicionSubiendo(ts);
	}
	bool getTransicionMinijuego() const { return this->transicionMinijuego; }
	void setTransicionMinijuego(bool tm) { this->transicionMinijuego = tm; }
	bool getTransicionEasterEgg() const { return this->transicionEasterEgg; }
	void setTransicionEasterEgg(bool te) { this->transicionEasterEgg = te; }
	bool getTransicionDanioCoustas() const { return this->transicionDanioCoustas; }
	void setTransicionDanioCoustas(bool td) { this->transicionDanioCoustas = td; }
	virtual void ejecutarMinijuego(Pantalla&, GestorAudio&) {}
	virtual void dibujarEntidadesExtra(Pantalla&, int, int) const {}

	Protagonista* getProtagonista() { return this->protagonista; }
	NPC* getQifrey() { return this->qifrey; }
	NPC* getAgott() { return this->agott; }

	bool getLibroEncontrado() const { return (torreAgott != nullptr) ? torreAgott->getLibroEncontrado() : false; }
	bool getPozoEncontrado() const { return (torreAgott != nullptr) ? torreAgott->getPozoEncontrado() : false; }
	int getPozoX() const { return (torreAgott != nullptr) ? torreAgott->getPozoX() : -1; }
	int getPozoY() const { return (torreAgott != nullptr) ? torreAgott->getPozoY() : -1; }
	int getCajasMovidasContador() const { return (torreAgott != nullptr) ? torreAgott->getCajasMovidasContador() : 0; }
	const std::vector<Caja*>& getCajas() const {
		static const std::vector<Caja*> vacio;
		return (torreAgott != nullptr) ? torreAgott->getCajas() : vacio;
	}

	int getNumeroNivel() const { return this->numeroNivel; }
	std::string getNombreNivel() const { return this->nombreNivel; }
	Mapa* getMapa() {
		if (getEnCuartoRicheh() && cuartoRicheh != nullptr) {
			return cuartoRicheh->getMapa();
		}
		return this->mapa;
	}
	const std::vector<ItemMagico*>& getItemsSuelo() const { return this->itemsSuelo; }
	const std::vector<Letrero*>& getLetreros() const { return this->letreros; }
	const std::string& getPromptFlotante() const { return this->promptFlotante; }
	virtual std::string getNombreUbicacionActual() const { return this->nombreNivel; }
	bool getSolicitaSalir() const { return this->solicitaSalir; }
	void setSolicitaSalir(bool ss) { this->solicitaSalir = ss; }

	bool getEnDialogo() const { return (gestorDialogos != nullptr) ? gestorDialogos->getEnDialogo() : false; }
	int getEstadoDialogo() const { return (gestorDialogos != nullptr) ? gestorDialogos->getEstadoDialogo() : 0; }
	std::string getNpcDialogoActual() const { return (gestorDialogos != nullptr) ? gestorDialogos->getNpcDialogoActual() : ""; }
	GestorDialogos* getGestorDialogos() { return this->gestorDialogos; }
	int getModalActivo() const { return this->modalActivo; }
	bool getEnModalPersonajes() const { return this->modalActivo == 1; }
	bool getEnModalInventario() const { return this->modalActivo == 2; }
	bool getEnModalMisiones() const { return this->modalActivo == 3; }
	int getSeleccionModal() const { return this->seleccionModal; }
	bool getEnDetalleMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getEnDetalleMision() : false; }
	int getSeleccionMision() const { return (gestorMisiones != nullptr) ? gestorMisiones->getSeleccionMision() : 0; }
	int getSeleccionInventario() const { return this->seleccionModal; }
	bool getMostrarEstadisticasFin() const { return this->mostrarEstadisticasFin; }

	virtual int determinarCuarto(int px, int py) const {
		(void)px;
		(void)py;
		return 0;
	}

	virtual void actualizarCuartoActual(int px, int py) {
		(void)px;
		(void)py;
	}

	virtual bool verificarProximidadEspecial(int px, int py, int pw, int ph) {
		(void)px;
		(void)py;
		(void)pw;
		(void)ph;
		return false;
	}

	virtual bool procesarInteraccionEspecial(int px, int py, int pw, int ph) {
		(void)px;
		(void)py;
		(void)pw;
		(void)ph;
		return false;
	}

	virtual void iniciarNivel() = 0;

	int getSegundosTranscurridos() const {
		if (tiempoInicio == 0) return 0;
		clock_t tActual = (tiempoFin > 0) ? tiempoFin : clock();
		return (int)((tActual - tiempoInicio) / CLOCKS_PER_SEC);
	}

	int getBonoTiempo() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getBonoTiempo(getSegundosTranscurridos()) : 0;
	}

	int getPuntajeTotalNivel() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getPuntajeTotalNivel(getSegundosTranscurridos()) : 0;
	}

	void sumarPuntosMision(int p) {
		if (gestorMisiones != nullptr) gestorMisiones->sumarPuntosMision(p);
	}

	int getPuntosMisiones() const {
		return (gestorMisiones != nullptr) ? gestorMisiones->getPuntosMisiones() : 0;
	}

	bool verificarObjetivo() {
		return this->completado && !this->mostrarEstadisticasFin;
	}

	void obtenerDatosMisiones(std::vector<std::string>& titulos,
	                          std::vector<std::string>& descripciones,
	                          std::vector<std::string>& estados,
	                          std::vector<bool>& desbloqueadas) {
		if (gestorMisiones != nullptr) {
			int ed = (gestorDialogos != nullptr) ? gestorDialogos->getEstadoDialogo() : 0;
			bool ma = (gestorDialogos != nullptr && gestorDialogos->getMisionMyrphonActiva());
			bool mr = (gestorDialogos != nullptr && gestorDialogos->getMyrphonRescatado());
			bool dv = (gestorDialogos != nullptr && gestorDialogos->getDioVaraRicheh());
			gestorMisiones->obtenerDatosMisiones(numeroNivel, protagonista, qifrey, ed, titulos, descripciones, estados, desbloqueadas, ma, mr, dv);
		}
	}

	void obtenerDatosDialogo(std::string& hablante, std::string& rol, int& confianza,
	                         std::vector<std::string>& lineas,
	                         std::vector<std::string>& opciones) {
		if (gestorDialogos != nullptr) {
			NPC* richeh = getRicheh();
			bool dioVara = (richeh != nullptr && richeh->getYaHablo());
			bool libroEnc = (torreAgott != nullptr && torreAgott->getLibroEncontrado());
			gestorDialogos->obtenerDatosDialogo(protagonista, qifrey, agott, richeh, dioVara, libroEnc,
			                                    hablante, rol, confianza, lineas, opciones);
		}
	}

	void procesarOpcionDialogo(int opcion) {
		if (gestorDialogos != nullptr && gestorMisiones != nullptr) {
			std::string obj = gestorMisiones->getObjetivoActual();
			int pts = gestorMisiones->getPuntosMisiones();
			NPC* richeh = getRicheh();
			bool dioVara = (richeh != nullptr && richeh->getYaHablo());
			bool libroEnc = (torreAgott != nullptr && torreAgott->getLibroEncontrado());
			gestorDialogos->procesarOpcionDialogo(opcion, protagonista, qifrey, agott, richeh,
			                                      dioVara, libroEnc, promptFlotante, obj, pts,
			                                      completado, mostrarEstadisticasFin);
			if (mostrarEstadisticasFin && tiempoFin == 0) {
				tiempoFin = clock();
			}
			gestorMisiones->setObjetivoActual(obj);
			gestorMisiones->setPuntosMisiones(pts);
		}
	}

	void actualizarProximidad() {
		if (protagonista == nullptr) return;
		if ((mostrarEstadisticasFin || completado) && tiempoFin == 0) {
			tiempoFin = clock();
		}
		bool ed = (gestorDialogos != nullptr && gestorDialogos->getEnDialogo());
		if (modalActivo != 0 || ed || mostrarEstadisticasFin) {
			promptFlotante = "";
			return;
		}

		int px = protagonista->getX();
		int py = protagonista->getY();
		int pw = protagonista->getAncho();
		int ph = protagonista->getAlto();

		if (ticksMensajeTemporal > 0 && !mensajeTemporal.empty()) {
			promptFlotante = mensajeTemporal;
			return;
		}

		promptFlotante = "";

		if (getEnCuartoRicheh()) {
			NPC* richeh = getRicheh();
			if (gestorDialogos != nullptr && gestorDialogos->getMyrphonRescatado() && !gestorDialogos->getDioVaraRicheh()) {
				if (!gestorDialogos->getEnDialogo()) {
					if (richeh != nullptr) richeh->setExpresion(1);
					gestorDialogos->iniciarDialogo("Richeh", 350);
					promptFlotante = "";
					return;
				}
			}
			if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDelPozo(px, py)) {
				promptFlotante = "[E] Subir a la torre";
				return;
			}
			if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDeLetreroCuriosidades(px, py)) {
				promptFlotante = "[E / ENTER] Leer: Curiosidades de Richeh";
				return;
			}
			if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDeLetreroLore(px, py)) {
				promptFlotante = "[E / ENTER] Leer: Diario del Atelier";
				return;
			}
			if (richeh != nullptr && estaCerca(px, py, pw, ph, richeh->getX(), richeh->getY(), richeh->getAncho(), richeh->getAlto(), 2)) {
				promptFlotante = "[E / ENTER] Hablar con Richeh";
				return;
			}
			return;
		}

		if (verificarProximidadEspecial(px, py, pw, ph)) {
			return;
		}

		if (qifrey != nullptr && estaCerca(px, py, pw, ph, qifrey->getX(), qifrey->getY(), qifrey->getAncho(), qifrey->getAlto(), 2)) {
			promptFlotante = "[E / ENTER] Interactuar con Qifrey";
			return;
		}

		if (agott != nullptr && estaCerca(px, py, pw, ph, agott->getX(), agott->getY(), agott->getAncho(), agott->getAlto(), 2)) {
			promptFlotante = "[E / ENTER] Hablar con Agott";
			return;
		}

		if (torreAgott != nullptr && torreAgott->estaCercaDelPozo(px, py)) {
			promptFlotante = "[E] Bajar al pozo";
			return;
		}

		for (size_t i = 0; i < itemsSuelo.size(); i++) {
			if (itemsSuelo[i] != nullptr && !itemsSuelo[i]->getRecogido()) {
				if (estaCerca(px, py, pw, ph, itemsSuelo[i]->getX(), itemsSuelo[i]->getY(), itemsSuelo[i]->getAncho(), itemsSuelo[i]->getAlto(), 2)) {
					promptFlotante = "[E / ENTER] Recoger: " + itemsSuelo[i]->getNombre();
					return;
				}
			}
		}

		for (size_t i = 0; i < letreros.size(); i++) {
			if (letreros[i] != nullptr && estaCerca(px, py, pw, ph, letreros[i]->getX(), letreros[i]->getY(), letreros[i]->getAncho(), letreros[i]->getAlto(), 3)) {
				promptFlotante = "[E / ENTER] Leer letrero";
				return;
			}
		}
	}

	virtual bool actualizar() {
		bool huboCambio = false;

		if (this->protagonista != nullptr) {
			int framePrevio = this->protagonista->getFrameActual();
			this->protagonista->actualizarAnimacion(30);
			if (this->qifrey != nullptr) {
				this->qifrey->actualizarAnimacion(30);
			}
			if (this->agott != nullptr) {
				this->agott->actualizarAnimacion(30);
			}
			NPC* richeh = getRicheh();
			if (richeh != nullptr) {
				richeh->actualizarAnimacion(30);
			}
			if (this->protagonista->getFrameActual() != framePrevio) {
				huboCambio = true;
			}
		}

		if (ticksMensajeTemporal > 0) {
			ticksMensajeTemporal--;
			if (ticksMensajeTemporal == 0) {
				mensajeTemporal = "";
				actualizarProximidad();
				huboCambio = true;
			}
		}

#ifdef _WIN32
		if (_kbhit()) {
			int tecla = _getch();
			if (tecla == 0 || tecla == 224) {
				tecla = _getch();
			}

			if (mostrarEstadisticasFin) {
				if (tecla == '1' || tecla == 13) {
					mostrarEstadisticasFin = false;
					completado = true;
					huboCambio = true;
				} else if (tecla == 'c' || tecla == 'C' || tecla == 27) {
					mostrarEstadisticasFin = false;
					completado = false;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (modalActivo == 1) {
				if (tecla == 'w' || tecla == 'W') {
					if (seleccionModal > 0) {
						seleccionModal--;
						huboCambio = true;
					}
				} else if (tecla == 's' || tecla == 'S') {
					if (seleccionModal < 8) {
						seleccionModal++;
						huboCambio = true;
					}
				} else if (tecla >= '1' && tecla <= '9') {
					seleccionModal = tecla - '1';
					huboCambio = true;
				} else if (tecla == 'p' || tecla == 'P' || tecla == 27 || tecla == 13) {
					modalActivo = 0;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (modalActivo == 3) {
				if (gestorMisiones != nullptr) {
					if (!gestorMisiones->getEnDetalleMision()) {
						int sm = gestorMisiones->getSeleccionMision();
						if (tecla == 'w' || tecla == 'W') {
							if (sm > 0) {
								gestorMisiones->setSeleccionMision(sm - 1);
								huboCambio = true;
							}
						} else if (tecla == 's' || tecla == 'S') {
							if (sm < 4) {
								gestorMisiones->setSeleccionMision(sm + 1);
								huboCambio = true;
							}
						} else if (tecla == '1') {
							gestorMisiones->setSeleccionMision(0);
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == '2') {
							gestorMisiones->setSeleccionMision(1);
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == '3') {
							gestorMisiones->setSeleccionMision(2);
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == '4') {
							gestorMisiones->setSeleccionMision(3);
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == '5') {
							gestorMisiones->setSeleccionMision(4);
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == 13) {
							gestorMisiones->setEnDetalleMision(true);
							huboCambio = true;
						} else if (tecla == 'm' || tecla == 'M' || tecla == 27) {
							modalActivo = 0;
							gestorMisiones->setEnModalMisiones(false);
							gestorMisiones->setEnDetalleMision(false);
							huboCambio = true;
						}
					} else {
						if (tecla == 13 || tecla == 27 || tecla == 'm' || tecla == 'M') {
							gestorMisiones->setEnDetalleMision(false);
							huboCambio = true;
						}
					}
				}
				return huboCambio;
			}

			if (modalActivo == 2) {
				if (tecla == 'w' || tecla == 'W') {
					if (seleccionModal > 0) {
						seleccionModal--;
						huboCambio = true;
					}
				} else if (tecla == 's' || tecla == 'S') {
					if (seleccionModal < 7) {
						seleccionModal++;
						huboCambio = true;
					}
				} else if (tecla >= '1' && tecla <= '8') {
					seleccionModal = tecla - '1';
					huboCambio = true;
				} else if (tecla == 'i' || tecla == 'I' || tecla == 27 || tecla == 13) {
					modalActivo = 0;
					huboCambio = true;
				}
				return huboCambio;
			}

			if (gestorDialogos != nullptr && gestorDialogos->getEnDialogo()) {
				if (tecla == '1') {
					procesarOpcionDialogo(1);
					huboCambio = true;
				} else if (tecla == '2') {
					procesarOpcionDialogo(2);
					huboCambio = true;
				} else if (tecla == '3') {
					procesarOpcionDialogo(3);
					huboCambio = true;
				} else if (tecla == '4') {
					procesarOpcionDialogo(4);
					huboCambio = true;
				} else if (tecla == 13) {
					procesarOpcionDialogo(1);
					huboCambio = true;
				} else if (tecla == 27) {
					gestorDialogos->terminarDialogo();
					huboCambio = true;
				}
				actualizarProximidad();
				return huboCambio;
			}

			if (tecla == 27) {
				solicitaSalir = true;
				huboCambio = true;
				return huboCambio;
			}

			if (tecla == 'p' || tecla == 'P') {
				modalActivo = 1;
				seleccionModal = 0;
				huboCambio = true;
				return huboCambio;
			}

			if (tecla == 'm' || tecla == 'M') {
				modalActivo = 3;
				if (gestorMisiones != nullptr) {
					gestorMisiones->setEnModalMisiones(true);
					gestorMisiones->setEnDetalleMision(false);
					gestorMisiones->setSeleccionMision(0);
				}
				huboCambio = true;
				return huboCambio;
			}

			if (tecla == 'i' || tecla == 'I') {
				modalActivo = 2;
				seleccionModal = 0;
				huboCambio = true;
				return huboCambio;
			}

			if (getEnCuartoRicheh()) {
				if (tecla == 13 || tecla == 'e' || tecla == 'E') {
					int px = protagonista->getX();
					int py = protagonista->getY();
					int pw = protagonista->getAncho();
					int ph = protagonista->getAlto();
					if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDelPozo(px, py)) {
						setTransicionSubiendo(true);
						huboCambio = true;
						return huboCambio;
					}
					if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDeLetreroCuriosidades(px, py)) {
						if (gestorDialogos != nullptr) {
							int randFact = rand() % 3;
							int st = 400;
							if (randFact == 1) st = 402;
							else if (randFact == 2) st = 404;
							gestorDialogos->iniciarDialogo("Letrero", st);
						}
						huboCambio = true;
						return huboCambio;
					}
					if (cuartoRicheh != nullptr && cuartoRicheh->estaCercaDeLetreroLore(px, py)) {
						if (gestorDialogos != nullptr) {
							gestorDialogos->iniciarDialogo("Letrero", 410);
						}
						huboCambio = true;
						return huboCambio;
					}
					NPC* richeh = getRicheh();
					if (richeh != nullptr) {
						int rx = richeh->getX();
						int ry = richeh->getY();
						int rw = richeh->getAncho();
						int rh = richeh->getAlto();
						int distX = (px + pw <= rx) ? (rx - (px + pw)) : ((rx + rw <= px) ? (px - (rx + rw)) : 0);
						int distY = (py + ph <= ry) ? (ry - (py + ph)) : ((ry + rh <= py) ? (py - (ry + rh)) : 0);
						if (distX <= 2 && distY <= 2) {
							if (gestorDialogos != nullptr) {
								if (gestorDialogos->getDioVaraRicheh()) {
									gestorDialogos->iniciarDialogo("Richeh", 360);
								} else if (gestorDialogos->getMyrphonRescatado()) {
									richeh->setExpresion(1);
									gestorDialogos->iniciarDialogo("Richeh", 350);
								} else if (gestorDialogos->getMisionMyrphonActiva()) {
									gestorDialogos->iniciarDialogo("Richeh", 330);
								} else if (gestorDialogos->getRichehEnojada()) {
									gestorDialogos->iniciarDialogo("Richeh", 320);
								} else {
									gestorDialogos->iniciarDialogo("Richeh", 300);
								}
							}
							huboCambio = true;
							return huboCambio;
						}
					}
				}

				int dx = 0;
				int dy = 0;
				if (tecla == 'w' || tecla == 'W') dy = -1;
				else if (tecla == 's' || tecla == 'S') dy = 1;
				else if (tecla == 'a' || tecla == 'A') dx = -1;
				else if (tecla == 'd' || tecla == 'D') dx = 1;

				if (dx != 0 || dy != 0) {
					for (int paso = 0; paso < 2; paso++) {
						int nx = protagonista->getX() + dx;
						int ny = protagonista->getY() + dy;
						Mapa* mapaR = (cuartoRicheh != nullptr) ? cuartoRicheh->getMapa() : nullptr;
						if (mapaR != nullptr) {
							bool colision = false;
							for (int r = 0; r < protagonista->getAlto(); r++) {
								for (int c = 0; c < protagonista->getAncho(); c++) {
									if (!mapaR->esPosicionValida(nx + c, ny + r)) {
										colision = true;
										break;
									}
								}
								if (colision) break;
							}
							if (!colision && colisionaConNPC(nx, ny, protagonista->getAncho(), protagonista->getAlto(), getRicheh())) {
								colision = true;
							}
							if (!colision) {
								protagonista->setX(nx);
								protagonista->setY(ny);
								huboCambio = true;
							} else {
								break;
							}
						}
					}
				}
				actualizarProximidad();
				return huboCambio;
			}

			if (tecla == 13 || tecla == 'e' || tecla == 'E') {
				int px = protagonista->getX();
				int py = protagonista->getY();
				int pw = protagonista->getAncho();
				int ph = protagonista->getAlto();

				if (torreAgott != nullptr && torreAgott->estaCercaDelPozo(px, py)) {
					setTransicionBajando(true);
					huboCambio = true;
					return huboCambio;
				}

				if (procesarInteraccionEspecial(px, py, pw, ph)) {
					huboCambio = true;
					return huboCambio;
				}

				if (qifrey != nullptr && estaCerca(px, py, pw, ph, qifrey->getX(), qifrey->getY(), qifrey->getAncho(), qifrey->getAlto(), 2)) {
					if (gestorDialogos != nullptr) {
						gestorDialogos->iniciarDialogo("Qifrey", 1);
					}
					huboCambio = true;
					return huboCambio;
				}

				if (agott != nullptr && estaCerca(px, py, pw, ph, agott->getX(), agott->getY(), agott->getAncho(), agott->getAlto(), 2)) {
					if (gestorDialogos != nullptr) {
						gestorDialogos->iniciarDialogoAgott();
					}
					huboCambio = true;
					return huboCambio;
				}

				for (size_t i = 0; i < itemsSuelo.size(); i++) {
					if (itemsSuelo[i] != nullptr && !itemsSuelo[i]->getRecogido()) {
						if (estaCerca(px, py, pw, ph, itemsSuelo[i]->getX(), itemsSuelo[i]->getY(), itemsSuelo[i]->getAncho(), itemsSuelo[i]->getAlto(), 2)) {
							Inventario* inv = protagonista->getInventario();
							if (inv != nullptr) {
								if (inv->agregarItem(new ItemMagico(0, 0, itemsSuelo[i]->getNombre(), itemsSuelo[i]->getDescripcion(), itemsSuelo[i]->getTipoItem(), true))) {
									itemsSuelo[i]->setRecogido(true);
									sumarPuntosMision(25);
									mostrarMensajeTemporal("[Recogiste: " + itemsSuelo[i]->getNombre() + "]", 60);
									huboCambio = true;
									return huboCambio;
								} else {
									mostrarMensajeTemporal("[Mochila llena: ¡Máx. 8 ítems!]", 60);
									huboCambio = true;
									return huboCambio;
								}
							}
						}
					}
				}

				if (interactuarLetrero(px, py, pw, ph)) {
					huboCambio = true;
					return huboCambio;
				}
			}

			int dx = 0;
			int dy = 0;
			if (numeroNivel == 3) {
				if (tecla == 'w' || tecla == 'W') dy = 1;
				else if (tecla == 's' || tecla == 'S') dy = -1;
				else if (tecla == 'a' || tecla == 'A') dx = 1;
				else if (tecla == 'd' || tecla == 'D') dx = -1;
			} else {
				if (tecla == 'w' || tecla == 'W') dy = -1;
				else if (tecla == 's' || tecla == 'S') dy = 1;
				else if (tecla == 'a' || tecla == 'A') dx = -1;
				else if (tecla == 'd' || tecla == 'D') dx = 1;
			}

			if (dx != 0 || dy != 0) {
				for (int paso = 0; paso < 2; paso++) {
					int nx = protagonista->getX() + dx;
					int ny = protagonista->getY() + dy;

					int cajaEmpujada = (torreAgott != nullptr) ? torreAgott->detectarColisionCaja(nx, ny) : -1;

					if (cajaEmpujada != -1 && torreAgott != nullptr) {
						if (torreAgott->intentarEmpujarCaja(cajaEmpujada, dx, dy, nx, ny, mapa, agott, itemsSuelo, mensajeTemporal, ticksMensajeTemporal)) {
							protagonista->setX(nx);
							protagonista->setY(ny);
							actualizarCuartoActual(nx, ny);
							huboCambio = true;
						} else {
							break;
						}
					} else if (mapa != nullptr) {
						bool colision = false;
						for (int r = 0; r < protagonista->getAlto(); r++) {
							for (int c = 0; c < protagonista->getAncho(); c++) {
								if (!mapa->esPosicionValida(nx + c, ny + r)) {
									colision = true;
									break;
								}
							}
							if (colision) break;
						}

						if (!colision && colisionaConNPC(nx, ny, protagonista->getAncho(), protagonista->getAlto(), qifrey)) {
							colision = true;
						}

						if (!colision && colisionaConNPC(nx, ny, protagonista->getAncho(), protagonista->getAlto(), agott)) {
							colision = true;
						}

						if (!colision) {
							if (nx != protagonista->getX() || ny != protagonista->getY()) {
								protagonista->setX(nx);
								protagonista->setY(ny);
								actualizarCuartoActual(nx, ny);
								huboCambio = true;
							}
						} else {
							break;
						}
					}
				}
			}
			actualizarProximidad();
		}
#endif
		return huboCambio;
	}
};

#endif
